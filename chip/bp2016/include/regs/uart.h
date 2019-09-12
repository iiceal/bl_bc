#ifndef _REGS_UART_H_
#define _REGS_UART_H_

// only choice one macro bellow
//#define UART_IRQ_TEST
#define UART_IRQ_CALLBACK

#define REG_AP_UART0_BASE          (APB1_BASE+0x00000)
#define REG_AP_UART1_BASE          (APB1_BASE+0x10000)
#define REG_AP_UART2_BASE          (APB1_BASE+0x20000)
#define REG_AP_UART3_BASE          (APB1_BASE+0x30000)
#define REG_AP_UART4_BASE          (APB1_BASE+0x40000)
#define REG_AP_UART5_BASE          (APB1_BASE+0x50000)
#define REG_AP_UART6_BASE          (APB1_BASE+0x60000)
#define REG_AP_UART7_BASE          (APB1_BASE+0x70000)

typedef volatile struct
{

    REG32 rbr_thr_dll;         /* 0x00000000*/
    REG32 dlh_ier;             /* 0x00000004*/
    REG32 iir_fcr;             /* 0x00000008*/
    REG32 lcr;                 /* 0x0000000c*/
    REG32 mcr;                 /* 0x00000010*/
    REG32 lsr;                 /* 0x00000014*/
    REG32 msr;                 /* 0x00000018*/
    REG32 scr;                 /* 0x0000001c*/

    REG32 _pad_0x20_0x2f[4];   /* *UNDEFINED* 0x20~0x2c */
    REG32 _pad_0x30_0x6f[16];  /* *Shadow registers 0x30~0x6c*/

    REG32 far;                 /* 0x00000070*/
    REG32 tfr;                 /* 0x00000074*/
    REG32 rfw;                 /* 0x00000078*/
    REG32 usr;                 /* 0x0000007c*/

    REG32 tfl;                 /* 0x00000080*/
    REG32 rfl;                 /* 0x00000084*/
    REG32 srr;                 /* 0x00000088*/
    REG32 srts;                /* 0x0000008c*/

    REG32 sbcr;                /* 0x00000090*/
    REG32 sdmam;               /* 0x00000094*/
    REG32 sfe;                 /* 0x00000098*/
    REG32 srt;                 /* 0x0000009c*/

    REG32 stet;                /* 0x000000a0*/
    REG32 htx;                 /* 0x000000a4*/
    REG32 dmasa;               /* 0x000000a8*/
    REG32 _pad_0xac_0xf3[18];  /* *UNDEFINED* */

    REG32 cpr;                 /* 0x000000f4*/
    REG32 ucv;                 /* 0x000000f8*/
    REG32 ctr;                 /* 0x000000fc*/
} HWP_UART_T;


#define hwp_apUart0                ((HWP_UART_T*) (REG_AP_UART0_BASE))
#define hwp_apUart1                ((HWP_UART_T*) (REG_AP_UART1_BASE))
#define hwp_apUart2                ((HWP_UART_T*) (REG_AP_UART2_BASE))
#define hwp_apUart3                ((HWP_UART_T*) (REG_AP_UART3_BASE))
#define hwp_apUart4                ((HWP_UART_T*) (REG_AP_UART4_BASE))
#define hwp_apUart5                ((HWP_UART_T*) (REG_AP_UART5_BASE))
#define hwp_apUart6                ((HWP_UART_T*) (REG_AP_UART6_BASE))
#define hwp_apUart7                ((HWP_UART_T*) (REG_AP_UART7_BASE))

//ier: interrupt enable register
#define IER_ERBFI	(0x01 << 0)		/* Rx Data Availiable Interrupt and 
                                           the Char Timeout Interrrupt(Fifo mode) */
#define IER_ETBEI	(0x01 << 1)		/* Tx Holding Register Empty Interrupt */

#define IER_ELSI	(0x01 << 2)		/* RX lIne status Interrupt */
#define IER_EDSSI	(0x01 << 3)	/* Modem Status Interrupt */

#define IER_PTIME	(0x01 << 7)		/* THRE Interrupt (THRE_MODE_USE=Enable)*/

#define IER_ALL     (IER_ERBFI | IER_ETBEI | IER_ELSI | IER_EDSSI | IER_PTIME)

//fcr: fifo control regiter
#define FCR_FIFO_EN     (0x01 << 0)		/* Fifo enable */
#define FCR_RXSR        (0x01 << 1)		/* Receiver soft reset */
#define FCR_TXSR        (0x01 << 2)		/* Transmitter soft reset */
#define FCR_DMA_MODE    (0x01 << 3)		/* DMA MoDE:0-mode0 1-mode1 */

//lcr
#define LCR_WLS_MSK	0x03		/* character length slect mask */
#define LCR_WLS_5	0x00		/* 5 bit character length */
#define LCR_WLS_6	0x01		/* 6 bit character length */
#define LCR_WLS_7	0x02		/* 7 bit character length */
#define LCR_WLS_8	0x03		/* 8 bit character length */
#define LCR_8N1		0x03
#define LCR_STB		(0x01 << 2)		/* Number of stop Bits, off = 1, on = 1.5 or 2) */
#define LCR_PEN		(0x01 << 3)		/* Parity eneble */
#define LCR_EPS		(0x01 << 4)		/* Even Parity Select */
#define LCR_STKP	(0x01 << 5)		/* Stick Parity */
#define LCR_SBRK	(0x01 << 6)		/* Set Break */
#define LCR_BKSE	(0x01 << 7)		/* Bank select enable */

//lsr
#define LSR_DR		(0x01 << 0)		/* Data ready */
#define LSR_OE		(0x01 << 1)		/* Rx Overrun */
#define LSR_PE		(0x01 << 2)		/* Parity error */
#define LSR_FE		(0x01 << 3)		/* Framing error */
#define LSR_BI		(0x01 << 4)		/* Break */
#define LSR_THRE	(0x01 << 5)		/* Xmit holding register empty */
#define LSR_TEMT	(0x01 << 6)		/* Xmitter empty */
#define LSR_ERR		(0x01 << 7)		/* Error */


#endif

