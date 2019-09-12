#include "common.h"
#include "serial.h"
#include "regs/uart.h"

#define LCRVAL LCR_8N1					/* 8 data, 1 stop, no parity */
#define FCRVAL (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR)	/* enable FIFOs, reset rx/tx fifo */

#define REG_READ( _r_ )		(*(volatile unsigned int*)(_r_))
#define REG_WRITE( _r_, _v_) 	((*(volatile unsigned int*)(_r_)) = (unsigned int)(_v_))

#ifdef SERIA_DEBUG
#define serial_debug(fmt, args...)  printf("Serial: " fmt "\n", ##args);
#else
#define serial_debug(fmt, args...)
#endif


void _serial_init(HWP_UART_T *hw_uart, int baudrate)
{
	unsigned int baud_divisor;
    unsigned int regv;
	
	baud_divisor = CONFIG_APB_CLOCK / 16 / baudrate;	

	if(CONFIG_APB_CLOCK > (baud_divisor * 16 * baudrate))
		baud_divisor +=((CONFIG_APB_CLOCK/16 - baud_divisor * baudrate)*2 > baudrate) ? 1 : 0;

    //hard code for uart6 7 921600 baudrate setting

    if(hw_uart == hwp_apUart6 || hw_uart == hwp_apUart7)
    {
        regv = REG_READ(0xC01D000C);
        if(hw_uart == hwp_apUart6)
        {
            regv = regv & 0xFC0FFFFF;
            regv = regv | (unsigned int)(0x00000007 << 20);
        }
        if(hw_uart == hwp_apUart7)
        {
            regv = regv & 0x3FFFFFF;
            regv = regv | (0x00000007 << 26);
        }
        REG_WRITE(0xC01D000C,regv);
        regv = REG_READ(0xC01D000C);
        baud_divisor = 11;
        serial_debug("HW code for uart6/7 921600 Baudrate setting.\n");
    }
    
    
    hw_uart->dlh_ier = 0x00;	//disable all interrupts
	hw_uart->lcr = LCR_BKSE | LCRVAL;
	hw_uart->rbr_thr_dll = baud_divisor & 0xff;
	hw_uart->dlh_ier = (baud_divisor >> 8) & 0xff;
	hw_uart->lcr = LCRVAL;		//data 8, stop 1, parity disable, break disable
	hw_uart->iir_fcr = FCRVAL;
}

void _serial_deinit(void)
{

}

static void _serial_enable_rtscts(void)
{
}

/*
 * Test whether a character is in the RX buffer
 */
int _serial_tstc(void)
{
	return (hwp_dbgUart->lsr & LSR_DR);
}

/* 
 * For lsr_dr: Indicate that the rx contains at least one char in RBR or the rx fifo 
 * and when the RBR is read in non-Fifo Mode, or the rx Fifo is empty, lsr:dr is cleared 
*/
int _serial_getc(void)
{
	while ((hwp_dbgUart->lsr & LSR_DR) == 0);
	return (hwp_dbgUart->rbr_thr_dll);
}
/*
 * For lsr_thre: If THRE_MODE_USE=Disabled or THRE interrrupt is disabled (IER[7]=0),
 * this bit indicate the THR Or tx fifo is empty 
 * otherwize indicate the THRE interrupt	
 */
void _serial_putc_hw(const char c)
{
	/*when THR or TX FIFO is non-empty, wait..*/
	while ((hwp_dbgUart->lsr & LSR_THRE) == 0);
	hwp_dbgUart->rbr_thr_dll = c;
}

void _serial_putc(const char c)
{
	if (c == '\n') 
		_serial_putc_hw('\r');
	
	_serial_putc_hw(c);
}

void _serial_puts(const char *s)
{
	while (*s) 
		_serial_putc(*s++);
}

void dbg_serial_init(int baudrate)
{
	_serial_init(hwp_dbgUart, baudrate);
}

#ifdef CONFIG_UPV_SERIAl
void upv_serial_init(int baudrate)
{
	_serial_init(hwp_upvUart, baudrate);
}
#endif

void serial_enable_rtscts(void)
{
	_serial_enable_rtscts();
}

int serial_getc(void)
{
	return _serial_getc();
}

int serial_tstc(void)
{
	return _serial_tstc();
}

void serial_putc(const char c)
{
	_serial_putc(c);
}

void serial_puts(const char *s)
{
		_serial_puts(s);
}

int ctrlc(void)
{
	if (serial_tstc()) {
		switch (serial_getc()) {
			case 0x03:      /* ^C - Control C */
				return 1;
			default:
				break;
		}
	}
	return 0;
}
