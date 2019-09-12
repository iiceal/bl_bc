#include "common.h"
#include "serial_16550_low.h"
#include "regs/scm.h"

#define LCRVAL LCR_8N1								/* 8 data, 1 stop, no parity */
#define FCRVAL (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR)	/* enable FIFOs, reset rx/tx fifo */

inline U32 serial_port_valid(U32 port)
{
    if (HWP_SERIAL_PORT_NUM <= port) {
        return 0;
    }

    return 1;
}

HWP_UART_T *serial_hwp_get(U32 port_id)
{
    HWP_UART_T *hw_uart = NULL;

    switch (port_id) {
    case 0:
        hw_uart = hwp_apUart0;
        break;
    case 1:
        hw_uart = hwp_apUart1;
        break;
    case 2:
        hw_uart = hwp_apUart2;
        break;
    case 3:
        hw_uart = hwp_apUart3;
        break;
    case 4:
        hw_uart = hwp_apUart4;
        break;
    case 5:
        hw_uart = hwp_apUart5;
        break;
    case 6:
        hw_uart = hwp_apUart6;
        break;
    case 7:
        hw_uart = hwp_apUart7;
        break;
    default:
        break;
    }

    return hw_uart;
}

SYS_IRQ_ID_T serial_irq_num[HWP_SERIAL_PORT_NUM] = {
	SYS_IRQ_ID_UART0,
	SYS_IRQ_ID_UART1,
	SYS_IRQ_ID_UART2,
	SYS_IRQ_ID_UART3,
	SYS_IRQ_ID_UART4,
	SYS_IRQ_ID_UART5,
	SYS_IRQ_ID_UART6,
	SYS_IRQ_ID_UART7
};

U32 serial_irq_num_get(U32 port_id)
{
    if (port_id < HWP_SERIAL_PORT_NUM) {
        return (U32) serial_irq_num[port_id];
    } else {
        return 0xFFFFFFFF;
    }
}

/*
 * Test whether a character is in the RX buffer
 */
inline int __serial_tstc(HWP_UART_T *hw_uart)
{
    return (hw_uart->lsr & LSR_DR);
}

/*
 * For rbr_thr_dll: read a char from rx fifo in case that the rx contains at least one char in RBR
 * or the rx fifo
*/
inline int __serial_getc_direct(HWP_UART_T *hw_uart)
{
    return (hw_uart->rbr_thr_dll);
}

/*
 * For lsr_dr: Indicate that the rx contains at least one char in RBR or the rx fifo
 * and when the RBR is read in non-Fifo Mode, or the rx Fifo is empty, lsr:dr is cleared
*/
inline int __serial_getc(HWP_UART_T *hw_uart)
{
    while ((hw_uart->lsr & LSR_DR) == 0)
        ;
    return (hw_uart->rbr_thr_dll);
}

/*
 * For lsr_thre: If THRE_MODE_USE=Disabled or THRE interrrupt is disabled (IER[7]=0),
 * this bit indicate the THR Or tx fifo is empty
 * otherwize indicate the THRE interrupt
 */
inline void serial_putc_hw(HWP_UART_T *hw_uart, const char c)
{
    /*when THR or TX FIFO is non-empty, wait..*/
    while ((hw_uart->lsr & LSR_THRE) == 0)
        ;
    hw_uart->rbr_thr_dll = c;
}

inline int serial_test_te(HWP_UART_T *hw_uart)
{
    return (hw_uart->lsr & LSR_THRE);
}

inline void serial_putc_hw_direct(HWP_UART_T *hw_uart, const char c)
{
    /*put char to TX FIFO */
    hw_uart->rbr_thr_dll = c;
}

inline void serial_data_len_set(HWP_UART_T *hw_uart, U32 data_len)
{
    U32 reg_value = hw_uart->lcr;
    reg_value &= ~(0x3);
    reg_value |= data_len;
    hw_uart->lcr = reg_value;
}

inline void serial_stop_bit_set(HWP_UART_T *hw_uart, U32 stop_bit)
{
    U32 reg_value = hw_uart->lcr;
    reg_value &= ~(0x4);
    reg_value |= stop_bit;
    hw_uart->lcr = reg_value;
}

inline void serial_parity_en_set(HWP_UART_T *hw_uart, U32 en)
{
    U32 enable = ((0 == en) ? 0 : 1);
    U32 reg_value = hw_uart->lcr;
    reg_value &= ~(0x8);
    reg_value |= enable;
    hw_uart->lcr = reg_value;
}

inline void serial_DLA_en(HWP_UART_T *hw_uart, U32 en)
{
    U32 enable = ((0 == en) ? 0 : 1);
    U32 reg_value = hw_uart->lcr;
    reg_value &= ~(0x80);
    reg_value |= enable;
    hw_uart->lcr = reg_value;
}

// FIFO control reg config
inline void serial_FIFO_en(HWP_UART_T *hw_uart, U32 en)
{
    U32 enable = ((0 == en) ? 0 : 1);
    U32 reg_value = hw_uart->iir_fcr;
    reg_value &= ~(0x1);
    reg_value |= enable;
    hw_uart->iir_fcr = reg_value;
}

inline void serial_FIFO_reset(HWP_UART_T *hw_uart, U32 reset_type)
{
    hw_uart->iir_fcr |= (0x1 << reset_type);
}

inline void serial_dma_mode_set(HWP_UART_T *hw_uart, U32 dma_mode)
{
    U32 reg_value = hw_uart->iir_fcr;
    reg_value &= ~(0x1 << 3);
    reg_value |= dma_mode;
    hw_uart->iir_fcr = reg_value;
}

inline void serial_tx_trigger_set(HWP_UART_T *hw_uart, TXE_TRIGGER_VAL_E tx_trigger_val)
{
    U32 reg_value = hw_uart->iir_fcr;
    reg_value &= ~(0x3 << 4);
    reg_value |= tx_trigger_val;
    hw_uart->iir_fcr = reg_value;
}

inline void serial_rx_trigger_set(HWP_UART_T *hw_uart, RX_TRIGGER_VAL_E rx_trigger_val)
{
    U32 reg_value = hw_uart->iir_fcr;
    reg_value &= ~(0x3 << 6);
    reg_value |= rx_trigger_val;
    hw_uart->iir_fcr = reg_value;
}

// interrrupt enable reg config
inline void serial_int_enable(HWP_UART_T *hw_uart, U32 int_mask)
{
    hw_uart->dlh_ier |= int_mask;
}

inline void serial_int_disable(HWP_UART_T *hw_uart, U32 int_unmask)
{
    hw_uart->dlh_ier &= ~int_unmask;
}

// 开启时钟，预留接口，操作 CCM 寄存器 或者调用ccm模块的外部接口使能指定uart的时钟
void uart_clock_enable(U32 port_id)
{
    return;
}

U8 uart_config_set(HWP_UART_T *hw_uart, u32 data_len, U32 parity_en,
                   u32 ps, u32 stop)
{
    U8 lcr = 0;

    lcr = (lcr & (~UART_FCL_DLS_MASK)) | (data_len << UART_FCL_DLS_BIT);
    lcr = (lcr & (~UART_FCL_STOP_MASK)) | (stop << UART_FCL_STOP_BIT);
    lcr = (lcr & (~UART_FCL_PEN_MASK)) | (parity_en << UART_FCL_PEN_BIT);
    lcr = (lcr & (~UART_FCL_EPS_MASK)) | (ps << UART_FCL_EPS_BIT);

    return lcr;
}

void uart_para_config_init(HWP_UART_T *hw_uart, U8 lcr)
{
    hw_uart->lcr = (lcr | LCR_BKSE); // 8 bit data_len, No parity, 1 bit stop_bit
    hw_uart->rbr_thr_dll = 0;
    hw_uart->dlh_ier = 0;
    hw_uart->lcr &= ~(LCR_BKSE);
    return;
}

int calc_divisor(U32 clk, U32 baud_rate)
{
    unsigned int baud_divisor;

    baud_divisor = clk / 16 / baud_rate;

    if (clk > (baud_divisor * 16 * baud_rate)) {
        baud_divisor +=
            ((clk / 16 - baud_divisor * baud_rate) * 2 > baud_rate) ? 1 : 0;
    }

    return baud_divisor;
}

// caller assure that only port6 or port7 can be set 1875000 or 2850000
void uart_bautrate_set(U32 port, U32 baut_rate)
{
    U32 clock_divisor;
    unsigned int div_reg;
    unsigned int input_clk;
    HWP_UART_T *hw_uart = serial_hwp_get(port);
    switch(baut_rate){
        case 1875000:
            if((6 != port) && (7 != port)) return;
            // change clk div to assure input clk is 30MHz
            div_reg = hwp_apSCM->clk_div1;
            div_reg &= (~(0x3F << (20 + (port - 6)*6)));  // clear
            div_reg |= (0x9 << (20 + (port - 6)*6));  // set 9
            hwp_apSCM->clk_div1 = div_reg;
            mdelay(1);
            input_clk = 30000000;  // div assure input clk must be 30MHz
            break;
        case 2850000:
            if((6 != port) && (7 != port)) return;
            // change clk div to assure input clk is 45.6MHz
            div_reg = hwp_apSCM->clk_div1;
            div_reg &= (~(0x3F << (20 + (port - 6)*6)));  // clear
            div_reg |= (0xe << (20 + (port - 6)*6));  // set 0xe
            hwp_apSCM->clk_div1 = div_reg;
            mdelay(1);
            input_clk = 45600000;  // div assure input clk must be 45.6MHz
            break;
        default:
            input_clk = 95000000;  // div assure input clk must be 95MHz
            break;
    }

    clock_divisor = calc_divisor(input_clk, baut_rate);
    printf("port:%d, scm-div:0x%x, div:%d, baut:%d\n", port, hwp_apSCM->clk_div1, clock_divisor, baut_rate);

    // hw_uart->lcr = 0x83;
    hw_uart->lcr |= 0x80;
    hw_uart->rbr_thr_dll = (clock_divisor & 0xFF);
    hw_uart->dlh_ier = ((clock_divisor >> 8) & 0xFF);
    hw_uart->lcr &= ~0x80;

    return;
}
