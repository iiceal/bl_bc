#ifndef _UART_16549_LOW_H_
#define _UART_16550_LOW_H_
#include  "common.h"
#include  "regs/uart.h"

#define HWP_SERIAL_PORT_NUM        (0x8)
#define HWP_SERIAL_RX_BUF_MAX      (0x100)
#define HWP_UART_IRQ(port)         (SYS_IRQ_ID_UART0 + port)

#define UART0_5_TX_FIFO_DEPTH      (128)
#define UART0_5_RX_FIFO_DEPTH      (128)
#define UART6_7_TX_FIFO_DEPTH      (256)
#define UART6_7_RX_FIFO_DEPTH      (256)

#define UART_FCL_PARITY_ENABLE     (1)
#define UART_FCL_PARITY_DISABLE    (0)

#define UART_FCL_DLAB_BIT          (7)
#define UART_FCL_EPS_BIT           (4)
#define UART_FCL_PEN_BIT           (3)
#define UART_FCL_STOP_BIT          (2)
#define UART_FCL_DLS_BIT           (0)
#define UART_FCL_DLAB_MASK         (0x1 << UART_FCL_DLAB_BIT)
#define UART_FCL_EPS_MASK          (0x1 << UART_FCL_EPS_BIT)
#define UART_FCL_PEN_MASK          (0x1 << UART_FCL_PEN_BIT)
#define UART_FCL_STOP_MASK         (0x1 << UART_FCL_STOP_BIT)
#define UART_FCL_DLS_MASK          (0x3 << UART_FCL_DLS_BIT)

typedef enum {
    RX_TRIGGER_VAL_E_1_CHAR = 0,
    RX_TRIGGER_VAL_E_QUARTER_FULL,
    RX_TRIGGER_VAL_E_HALF_FULL,
    RX_TRIGGER_VAL_E_2_CHAR_LESS_THAN_FULL,
}RX_TRIGGER_VAL_E;

typedef enum {
    TXE_TRIGGER_VAL_E_EMPTY = 0,
    TXE_TRIGGER_VAL_E_2_LEFT,
    TXE_TRIGGER_VAL_E_HALF_LEFT,
    TXE_TRIGGER_VAL_E_QUARTER_LEFT,
}TXE_TRIGGER_VAL_E;

#define FIFO_RESET_TX   (0x2)
#define FIFO_RESET_RX   (0x1)

#define FIFO_DMA_MODE_0    (0x0)
#define FIFO_DMA_MODE_1    (0x1)

/* func declaration */
extern inline U32 serial_port_valid(U32 port);
extern U32 serial_irq_num_get(U32 port_id);
extern HWP_UART_T *serial_hwp_get(U32 port_id);
extern inline int __serial_tstc(HWP_UART_T *hw_uart);
extern inline int __serial_getc_direct(HWP_UART_T *hw_uart);
extern inline int __serial_getc(HWP_UART_T *hw_uart);
extern inline void serial_putc_hw(HWP_UART_T *hw_uart, const char c);
extern inline int serial_test_te(HWP_UART_T *hw_uart);
extern inline void serial_putc_hw_direct(HWP_UART_T *hw_uart, const char c);
extern inline void serial_data_len_set(HWP_UART_T *hw_uart, U32 data_len);
extern inline void serial_stop_bit_set(HWP_UART_T *hw_uart, U32 stop_bit);
extern inline void serial_parity_en_set(HWP_UART_T *hw_uart, U32 en);
extern inline void serial_DLA_en(HWP_UART_T *hw_uart, U32 en);
extern inline void serial_FIFO_en(HWP_UART_T *hw_uart, U32 en);
extern inline void serial_FIFO_reset(HWP_UART_T *hw_uart, U32 reset_type);
extern inline void serial_dma_mode_set(HWP_UART_T *hw_uart, U32 dma_mode);
extern inline void serial_tx_trigger_set(HWP_UART_T *hw_uart, TXE_TRIGGER_VAL_E tx_trigger_val);
extern inline void serial_rx_trigger_set(HWP_UART_T *hw_uart, RX_TRIGGER_VAL_E rx_trigger_val);
extern inline void serial_int_enable(HWP_UART_T *hw_uart, U32 int_mask);
extern inline void serial_int_disable(HWP_UART_T *hw_uart, U32 int_unmask);
extern void uart_clock_enable(U32 port_id);
extern U8 uart_config_set(HWP_UART_T *hw_uart, u32 data_len, U32 parity_en,
        u32 ps, u32 stop);
extern void uart_para_config_init(HWP_UART_T *hw_uart, U8 lcr);
extern void uart_bautrate_set(U32 port, U32 baut_rate);

#endif  /*_UART_16549_LOW_H_*/
