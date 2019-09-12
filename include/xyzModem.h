#ifndef _XYZMODEM_H_
#define _XYZMODEM_H_
#include "serial.h"

#define xyzModem_xmodem 1
#define xyzModem_ymodem 2
/* Don't define this until the protocol support is in place */
/*#define xyzModem_zmodem 3 */

#define xyzModem_access   -1
#define xyzModem_noZmodem -2
#define xyzModem_timeout  -3
#define xyzModem_eof      -4
#define xyzModem_cancel   -5
#define xyzModem_frame    -6
#define xyzModem_cksum    -7
#define xyzModem_sequence -8

#define xyzModem_close 1
#define xyzModem_abort 2

#define xyzModem_rcv_packet_ok 		0
#define xyzModem_reack_blknum_invliad 	1
#define xyzModem_retry_send_c		2
#define xyzModem_copy_data_ok   	3

#define CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT
#define CYGACC_CALL_IF_SET_CONSOLE_COMM(x)

#define diag_vprintf vprintf
#define diag_printf printf
#define diag_vsprintf vsprintf

#define CYGACC_CALL_IF_DELAY_US(x) udelay(x)

int   getcxmodem(void);
void  xyzModem_init(struct tty *tty);
int   xyzModem_stream_open(int *err);
void  xyzModem_stream_close(int *err);
void  xyzModem_stream_terminate(int method, int (*getc)(void));
int   xyzModem_stream_read(char *buf, int size, int *err);
char *xyzModem_error(int err);

int rom_cmd_loop(void);
int load_xmodem(unsigned int load_addr, int load_size);
#endif /* _XYZMODEM_H_ */
