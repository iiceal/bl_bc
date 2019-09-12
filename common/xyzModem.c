#include "common.h"

/* Assumption - run xyzModem protocol over the console port */

/* Values magic to the protocol */
#define SOH 0x01
#define STX 0x02
#define EOT 0x04
#define ACK 0x06
#define BSP 0x08
#define NAK 0x15
#define CAN 0x18
#define EOF 0x1A		/* ^Z for DOS officionados */

#define USE_YMODEM_LENGTH

/* to re-use pdl buffer with the xyz pkt[] */
#define PKT_MAX_DATA_SIZE (4*1024)
//unsigned char pkt_cmd_buf[PKT_MAX_DATA_SIZE] __attribute__((__aligned__(32)));
#define MALLOC_SIZE 1024
unsigned char malloc_buf[MALLOC_SIZE];
unsigned char * pkt_cmd_buf = malloc_buf;
u32 xyzModem_err_stat = 0;
/* Data & state local to the protocol */
static struct
{
  int *__chan;
  unsigned char *pkt, *bufp;
  unsigned char blk, cblk, crc1, crc2;
  unsigned char next_blk;	/* Expected block */
  int len, mode, total_retries;
  int total_SOH, total_STX, total_CAN;
  bool crc_mode, at_eof, tx_ack;
#ifdef USE_YMODEM_LENGTH
  unsigned long file_length, read_length;
#endif
} xyz;

#define xyzModem_CHAR_TIMEOUT            2000	/* 2 seconds */
#define xyzModem_MAX_RETRIES             200
#define xyzModem_MAX_RETRIES_WITH_CRC    10
#define xyzModem_CAN_COUNT                3	/* Wait for 3 CAN before quitting */

static struct tty xyzModem_tty = {0, NULL, NULL, NULL, NULL};

#define xyzModem_tstc xyzModem_tty.tstc
#define xyzModem_putc xyzModem_tty.putc
#define xyzModem_getc xyzModem_tty.getc
#define ZM_DEBUG(_x_)

int
CYGACC_COMM_IF_GETC_TIMEOUT (char chan, char *c)
{
#define DELAY 1000
  unsigned long counter = 0;
  while (!xyzModem_tstc () && (counter < xyzModem_CHAR_TIMEOUT))
    {
      udelay (DELAY);
      counter++;
    }
  if (xyzModem_tstc ())
    {
      *c = xyzModem_getc ();
      return 1;
    }
  return 0;
}

int
CYGACC_COMM_IF_TRY_GETC (char chan, char *c)
{
  if (xyzModem_tstc ())
    {
      *c = xyzModem_getc ();
      return 1;
    }
  return 0;
}

void
CYGACC_COMM_IF_PUTC (char x, char y)
{
  xyzModem_putc (y);
}

/* Validate a hex character */
__inline__ static bool
_is_hex (char c)
{
  return (((c >= '0') && (c <= '9')) ||
	  ((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')));
}

/* Convert a single hex nibble */
__inline__ static int
_from_hex (char c)
{
  int ret = 0;

  if ((c >= '0') && (c <= '9'))
    {
      ret = (c - '0');
    }
  else if ((c >= 'a') && (c <= 'f'))
    {
      ret = (c - 'a' + 0x0a);
    }
  else if ((c >= 'A') && (c <= 'F'))
    {
      ret = (c - 'A' + 0x0A);
    }
  return ret;
}

/* Convert a character to lower case */
__inline__ static char
_tolower (char c)
{
  if ((c >= 'A') && (c <= 'Z'))
    {
      c = (c - 'A') + 'a';
    }
  return c;
}

/* Parse (scan) a number */
bool
parse_num (char *s, unsigned long *val, char **es, char *delim)
{
  bool first = true;
  int radix = 10;
  char c;
  unsigned long result = 0;
  int digit;

  while (*s == ' ')
    s++;
  while (*s)
    {
      if (first && (s[0] == '0') && (_tolower (s[1]) == 'x'))
	{
	  radix = 16;
	  s += 2;
	}
      first = false;
      c = *s++;
      if (_is_hex (c) && ((digit = _from_hex (c)) < radix))
	{
	  /* Valid digit */
#ifdef CYGPKG_HAL_MIPS
	  /* FIXME: tx49 compiler generates 0x2539018 for MUL which */
	  /* isn't any good. */
	  if (16 == radix)
	    result = result << 4;
	  else
	    result = 10 * result;
	  result += digit;
#else
	  result = (result * radix) + digit;
#endif
	}
      else
	{
	  if (delim != (char *) 0)
	    {
	      /* See if this character is one of the delimiters */
	      char *dp = delim;
	      while (*dp && (c != *dp))
		dp++;
	      if (*dp)
		break;		/* Found a good delimiter */
	    }
	  return false;		/* Malformatted number */
	}
    }
  *val = result;
  if (es != (char **) 0)
    {
      *es = s;
    }
  return true;
}

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#ifndef CRC16_USE_LOOKUP_TABLE

// The non-lookup-table based loop below uses as few
// instructions as the table based version following but 
// avoid the expensive memory read.
uint16_t
cyg_crc16(unsigned char *buf, int len)
{
    int i;
    uint16_t cksum;
    uint8_t  s, t;
    uint32_t r;

    cksum = 0;
    for (i = 0;  i < len;  i++) {
        s = *buf++ ^ (cksum >> 8);
        t = s ^ (s >> 4);
        r = (cksum << 8) ^
            t       ^
            (t << 5) ^
            (t << 12);
        cksum = r;
    }
    return cksum;
}

#else

/* Table of CRC constants - implements x^16+x^12+x^5+1 */
static const uint16_t crc16_tab[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};
    
uint16_t
cyg_crc16(unsigned char *buf, int len)
{
    int i;
    uint16_t cksum;

    cksum = 0;
    for (i = 0;  i < len;  i++) {
	cksum = crc16_tab[((cksum>>8) ^ *buf++) & 0xFF] ^ (cksum << 8);
    }
    return cksum;
}
#endif

int
getcxmodem(void)
{
    if (serial_tstc())
        return (serial_getc());
    return -1;
} 

/* Wait for the line to go idle */
static void
xyzModem_flush (void)
{
  int res;
  char c;
  while (true)
    {
      res = CYGACC_COMM_IF_TRY_GETC(*xyz.__chan, &c);
      if (!res)
	return;
    }
}

static int
xyzModem_get_hdr (void)
{
  char c;
  int res;
  bool hdr_found = false;
  int i, can_total, hdr_chars;
  unsigned short cksum;

  ZM_DEBUG (zm_new ());
  /* Find the start of a header */
  can_total = 0;
  hdr_chars = 0;

  if (xyz.tx_ack)
    {
      CYGACC_COMM_IF_PUTC (*xyz.__chan, ACK);
      xyz.tx_ack = false;
    }
  while (!hdr_found)
    {
      res = CYGACC_COMM_IF_GETC_TIMEOUT (*xyz.__chan, &c);
      ZM_DEBUG (zm_save (c));
      if (res)
	{
	  hdr_chars++;
	  switch (c)
	    {
	    case SOH:
	      xyz.total_SOH++;
	    case STX:
	      if (c == STX)
		xyz.total_STX++;
	      hdr_found = true;
	      break;
	    case CAN:
	      xyz.total_CAN++;
	      ZM_DEBUG (zm_dump (__LINE__));
	      if (++can_total == xyzModem_CAN_COUNT)
		{
		  return xyzModem_cancel;
		}
	      else
		{
		  /* Wait for multiple CAN to avoid early quits */
		  break;
		}
	    case EOT:
	      /* EOT only supported if no noise */
	      if (hdr_chars == 1)
		{
		  CYGACC_COMM_IF_PUTC (*xyz.__chan, ACK);
		  ZM_DEBUG (zm_dprintf ("ACK on EOT #%d\n", __LINE__));
		  ZM_DEBUG (zm_dump (__LINE__));
		  return xyzModem_eof;
		}
	    default:
	      /* Ignore, waiting for start of header */
	      ;
	    }
	}
      else
	{
	  /* Data stream timed out */
	  xyzModem_flush ();	/* Toss any current input */
	  ZM_DEBUG (zm_dump (__LINE__));
	  CYGACC_CALL_IF_DELAY_US (250000);
	  return xyzModem_timeout;
	}
    }

  /* Header found, now read the data */
  res = CYGACC_COMM_IF_GETC_TIMEOUT (*xyz.__chan, (char *) &xyz.blk);
  ZM_DEBUG (zm_save (xyz.blk));
  if (!res)
    {
      ZM_DEBUG (zm_dump (__LINE__));
      return xyzModem_timeout;
    }
  res = CYGACC_COMM_IF_GETC_TIMEOUT (*xyz.__chan, (char *) &xyz.cblk);
  ZM_DEBUG (zm_save (xyz.cblk));
  if (!res)
    {
      ZM_DEBUG (zm_dump (__LINE__));
      return xyzModem_timeout;
    }
  xyz.len = (c == SOH) ? 128 : 1024;
  xyz.bufp = xyz.pkt;
  for (i = 0; i < xyz.len; i++)
    {
      res = CYGACC_COMM_IF_GETC_TIMEOUT (*xyz.__chan, &c);
      ZM_DEBUG (zm_save (c));
      if (res)
	{
	  xyz.pkt[i] = c;
	}
      else
	{
	  ZM_DEBUG (zm_dump (__LINE__));
	  return xyzModem_timeout;
	}
    }
  res = CYGACC_COMM_IF_GETC_TIMEOUT (*xyz.__chan, (char *) &xyz.crc1);
  ZM_DEBUG (zm_save (xyz.crc1));
  if (!res)
    {
      ZM_DEBUG (zm_dump (__LINE__));
      return xyzModem_timeout;
    }
  if (xyz.crc_mode)
    {
      res = CYGACC_COMM_IF_GETC_TIMEOUT (*xyz.__chan, (char *) &xyz.crc2);
      ZM_DEBUG (zm_save (xyz.crc2));
      if (!res)
	{
	  ZM_DEBUG (zm_dump (__LINE__));
	  return xyzModem_timeout;
	}
    }
  ZM_DEBUG (zm_dump (__LINE__));
  /* Validate the message */
  if ((xyz.blk ^ xyz.cblk) != (unsigned char) 0xFF)
    {
      ZM_DEBUG (zm_dprintf
		("Framing error - blk: %x/%x/%x\n", xyz.blk, xyz.cblk,
		 (xyz.blk ^ xyz.cblk)));
      ZM_DEBUG (zm_dump_buf (xyz.pkt, xyz.len));
      xyzModem_flush ();
      xyzModem_err_stat ++;
      return xyzModem_frame;
    }
  /* Verify checksum/CRC */
  if (xyz.crc_mode)
    {
      cksum = cyg_crc16 (xyz.pkt, xyz.len);
      if (cksum != ((xyz.crc1 << 8) | xyz.crc2))
	{
          xyzModem_err_stat ++;
	  ZM_DEBUG (zm_dprintf ("CRC error - recvd: %02x%02x, computed: %x\n",
				xyz.crc1, xyz.crc2, cksum & 0xFFFF));
	  return xyzModem_cksum;
	}
    }
  else
    {
      cksum = 0;
      for (i = 0; i < xyz.len; i++)
	{
	  cksum += xyz.pkt[i];
	}
      if (xyz.crc1 != (cksum & 0xFF))
	{
      	  xyzModem_err_stat ++;
	  ZM_DEBUG (zm_dprintf
		    ("Checksum error - recvd: %x, computed: %x\n", xyz.crc1,
		     cksum & 0xFF));
	  return xyzModem_cksum;
	}
    }
  /* If we get here, the message passes [structural] muster */
  return 0;
}

void
xyzModem_init(struct tty *ptty)
{
  xyzModem_tty.tstc = ptty->tstc;
  xyzModem_tty.putc = ptty->putc;
  xyzModem_tty.puts = ptty->puts;
  xyzModem_tty.getc = ptty->getc;
  xyzModem_tty.initialized = 1;
  xyzModem_err_stat = 0;
}

int
xyzModem_stream_open (int *err)
{
#ifdef REDBOOT
  int console_chan;
#endif
  int stat = 0;
  int retries = xyzModem_MAX_RETRIES;
  int crc_retries = xyzModem_MAX_RETRIES_WITH_CRC;

  if (xyzModem_tty.initialized == 0)
    {
      *err = xyzModem_access;
      return -1;
    }

/*    ZM_DEBUG(zm_out = zm_out_start); */
#ifdef xyzModem_zmodem
  if (info->mode == xyzModem_zmodem)
    {
      *err = xyzModem_noZmodem;
      return -1;
    }
#endif

#ifdef REDBOOT
  /* Set up the I/O channel.  Note: this allows for using a different port in the future */
  console_chan =
    CYGACC_CALL_IF_SET_CONSOLE_COMM
    (CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT);
  if (info->chan >= 0)
    {
      CYGACC_CALL_IF_SET_CONSOLE_COMM (info->chan);
    }
  else
    {
      CYGACC_CALL_IF_SET_CONSOLE_COMM (console_chan);
    }
  xyz.__chan = CYGACC_CALL_IF_CONSOLE_PROCS ();

  CYGACC_CALL_IF_SET_CONSOLE_COMM (console_chan);
  CYGACC_COMM_IF_CONTROL (*xyz.__chan, __COMMCTL_SET_TIMEOUT,
			  xyzModem_CHAR_TIMEOUT);
#else
/* TODO: CHECK ! */
  int dummy = 0;
  xyz.__chan = &dummy;
#endif
  /* re-use pdl buffer as xyz.pkt */
  xyz.pkt = pkt_cmd_buf;
  xyz.len = 0;
  xyz.crc_mode = true;
  xyz.at_eof = false;
  xyz.tx_ack = false;
  xyz.mode = xyzModem_xmodem;
  xyz.total_retries = 0;
  xyz.total_SOH = 0;
  xyz.total_STX = 0;
  xyz.total_CAN = 0;
#ifdef USE_YMODEM_LENGTH
  xyz.read_length = 0;
  xyz.file_length = 0;
#endif

  CYGACC_COMM_IF_PUTC (*xyz.__chan, (xyz.crc_mode ? 'C' : NAK));

  if (xyz.mode == xyzModem_xmodem)
    {
      /* X-modem doesn't have an information header - exit here */
      xyz.next_blk = 1;
      return 0;
    }

  while (retries-- > 0)
    {
      stat = xyzModem_get_hdr ();
      if (stat == 0)
	{
	  /* Y-modem file information header */
	  if (xyz.blk == 0)
	    {
#ifdef USE_YMODEM_LENGTH
	      /* skip filename */
	      while (*xyz.bufp++);
	      /* get the length */
	      parse_num ((char *) xyz.bufp, &xyz.file_length, NULL, " ");
#endif
	      /* The rest of the file name data block quietly discarded */
	      xyz.tx_ack = true;
	    }
	  xyz.next_blk = 1;
	  xyz.len = 0;
	  return 0;
	}
      else if (stat == xyzModem_timeout)
	{
	  if (--crc_retries <= 0)
	    xyz.crc_mode = false;
	  CYGACC_CALL_IF_DELAY_US (5 * 100000);	/* Extra delay for startup */
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, (xyz.crc_mode ? 'C' : NAK));
	  xyz.total_retries++;
	  ZM_DEBUG (zm_dprintf ("NAK (%d)\n", __LINE__));
	}
      if (stat == xyzModem_cancel)
	{
	  break;
	}
    }
  *err = stat;
  ZM_DEBUG (zm_flush ());
  return -1;
}

int
xyzModem_stream_read (char *buf, int size, int *err)
{
  int stat, total, len;
  int retries;

  total = 0;
  stat = xyzModem_cancel;
  /* Try and get 'size' bytes into the buffer */
  while (!xyz.at_eof && (size >= 0))
    {
      if (xyz.len == 0)
	{
	  retries = xyzModem_MAX_RETRIES;
	  while (retries-- > 0)
	    {
	      stat = xyzModem_get_hdr ();
	      if (stat == 0)
		{
		  if (xyz.blk == xyz.next_blk)
		    {
		      xyz.tx_ack = true;
		      ZM_DEBUG (zm_dprintf
				("ACK block %d (%d)\n", xyz.blk, __LINE__));
		      xyz.next_blk = (xyz.next_blk + 1) & 0xFF;

		      if (1)
			{
			  /* Data blocks can be padded with ^Z (EOF) characters */
			  /* This code tries to detect and remove them */
			  if ((xyz.bufp[xyz.len - 1] == EOF) &&
			      (xyz.bufp[xyz.len - 2] == EOF) &&
			      (xyz.bufp[xyz.len - 3] == EOF))
			    {
			      while (xyz.len
				     && (xyz.bufp[xyz.len - 1] == EOF))
				{
				  xyz.len--;
				}
			    }
			}

#ifdef USE_YMODEM_LENGTH
		      /*
		       * See if accumulated length exceeds that of the file.
		       * If so, reduce size (i.e., cut out pad bytes)
		       * Only do this for Y-modem (and Z-modem should it ever
		       * be supported since it can fall back to Y-modem mode).
		       */
		      if (xyz.mode != xyzModem_xmodem && 0 != xyz.file_length)
			{
			  xyz.read_length += xyz.len;
			  if (xyz.read_length > xyz.file_length)
			    {
			      xyz.len -= (xyz.read_length - xyz.file_length);
			    }
			}
#endif
		      break;
		    }
		  else if (xyz.blk == ((xyz.next_blk - 1) & 0xFF))
		    {
		      /* Just re-ACK this so sender will get on with it */
		      CYGACC_COMM_IF_PUTC (*xyz.__chan, ACK);
		      continue;	/* Need new header */
		    }
		  else
		    {
		      stat = xyzModem_sequence;
		    }
		}
	      if (stat == xyzModem_cancel)
		{
		  break;
		}
	      if (stat == xyzModem_eof)
		{
		  CYGACC_COMM_IF_PUTC (*xyz.__chan, ACK);
		  ZM_DEBUG (zm_dprintf ("ACK (%d)\n", __LINE__));
		  if (xyz.mode == xyzModem_ymodem)
		    {
		      CYGACC_COMM_IF_PUTC (*xyz.__chan,
					   (xyz.crc_mode ? 'C' : NAK));
		      xyz.total_retries++;
		      ZM_DEBUG (zm_dprintf ("Reading Final Header\n"));
		      stat = xyzModem_get_hdr ();
		      CYGACC_COMM_IF_PUTC (*xyz.__chan, ACK);
		      ZM_DEBUG (zm_dprintf ("FINAL ACK (%d)\n", __LINE__));
		    }
		  xyz.at_eof = true;
		  break;
		}
		if((stat == xyzModem_cksum) || (stat == xyzModem_frame))
		{
	      		CYGACC_COMM_IF_PUTC (*xyz.__chan, NAK);
	      		xyz.total_retries++;
			continue;
		}	 
	      CYGACC_COMM_IF_PUTC (*xyz.__chan, (xyz.crc_mode ? 'C' : NAK));
	      xyz.total_retries++;
	      ZM_DEBUG (zm_dprintf ("NAK (%d)\n", __LINE__));
	    }
	  if (stat < 0)
	    {
	      *err = stat;
	      xyz.len = -1;
	      return total;
	    }
	}
      /* Don't "read" data from the EOF protocol package */
      if (!xyz.at_eof)
	{
	  len = xyz.len;
	  if (size < len)
	    len = size;
	  memcpy ((unsigned char *)buf, xyz.bufp, len);
	  size -= len;
	  buf += len;
	  total += len;
	  xyz.len -= len;
	  xyz.bufp += len;
	}
    }
  return total;
}

void
xyzModem_stream_close (int *err)
{
  //diag_printf
  //  ("xyzModem - %s mode, %d(SOH)/%d(STX)/%d(CAN) packets, %d retries\n",
  //   xyz.crc_mode ? "CRC" : "Cksum", xyz.total_SOH, xyz.total_STX,
  //   xyz.total_CAN, xyz.total_retries);
  //ZM_DEBUG (zm_flush ());
}

/* Need to be able to clean out the input buffer, so have to take the */
/* getc */
void
xyzModem_stream_terminate (int abort, int (*getc) (void))
{
  int c;

  if (abort)
    {
      ZM_DEBUG (zm_dprintf ("!!!! TRANSFER ABORT !!!!\n"));
      switch (xyz.mode)
	{
	case xyzModem_xmodem:
	case xyzModem_ymodem:
	  /* The X/YMODEM Spec seems to suggest that multiple CAN followed by an equal */
	  /* number of Backspaces is a friendly way to get the other end to abort. */
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, CAN);
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, CAN);
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, CAN);
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, CAN);
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, BSP);
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, BSP);
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, BSP);
	  CYGACC_COMM_IF_PUTC (*xyz.__chan, BSP);
	  /* Now consume the rest of what's waiting on the line. */
	  ZM_DEBUG (zm_dprintf ("Flushing serial line.\n"));
	  xyzModem_flush ();
	  xyz.at_eof = true;
	  break;
#ifdef xyzModem_zmodem
	case xyzModem_zmodem:
	  /* Might support it some day I suppose. */
#endif
	  break;
	}
    }
  else
    {
      ZM_DEBUG (zm_dprintf ("Engaging cleanup mode...\n"));
      /*
       * Consume any trailing crap left in the inbuffer from
       * previous recieved blocks. Since very few files are an exact multiple
       * of the transfer block size, there will almost always be some gunk here.
       * If we don't eat it now, RedBoot will think the user typed it.
       */
      ZM_DEBUG (zm_dprintf ("Trailing gunk:\n"));
      while ((c = (*getc) ()) > -1);
      ZM_DEBUG (zm_dprintf ("\n"));
      /*
       * Make a small delay to give terminal programs like minicom
       * time to get control again after their file transfer program
       * exits.
       */
      CYGACC_CALL_IF_DELAY_US (250000);
    }
    printf("xmodem error number: %d\n", xyzModem_err_stat);
}

#ifdef DEADCODE
char *
xyzModem_error (int err)
{
  switch (err)
    {
    case xyzModem_access:
      return "Can't access file";
      break;
    case xyzModem_noZmodem:
      return "Sorry, zModem not available yet";
      break;
    case xyzModem_timeout:
      return "Timed out";
      break;
    case xyzModem_eof:
      return "End of file";
      break;
    case xyzModem_cancel:
      return "Cancelled";
      break;
    case xyzModem_frame:
      return "Invalid framing";
      break;
    case xyzModem_cksum:
      return "CRC/checksum error";
      break;
    case xyzModem_sequence:
      return "Block sequence error";
      break;
    default:
      return "Unknown error";
      break;
    }
}
#endif /* DEADCODE */

