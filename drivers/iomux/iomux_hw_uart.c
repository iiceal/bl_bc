#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

static void scm_uart0_rx(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART0_RX_BIT, SCM_IOMUX_74_UART0_RX_MASK,
                  SCM_IOMUX_74_UART0_RX_DEFAULT);
}

static void scm_uart0_tx(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART0_TX_BIT, SCM_IOMUX_74_UART0_TX_MASK,
                  SCM_IOMUX_74_UART0_TX_DEFAULT);
}

static void scm_uart1_rx(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART1_RX_BIT, SCM_IOMUX_74_UART1_RX_MASK,
                  SCM_IOMUX_74_UART1_RX_DEFAULT);
}

static void scm_uart1_tx(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART1_TX_BIT, SCM_IOMUX_74_UART1_TX_MASK,
                  SCM_IOMUX_74_UART1_TX_DEFAULT);
}

static void scm_uart2_rx(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART2_RX_BIT, SCM_IOMUX_74_UART2_RX_MASK,
                  SCM_IOMUX_74_UART2_RX_DEFAULT);
}

static void scm_uart2_tx(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART2_TX_BIT, SCM_IOMUX_74_UART2_TX_MASK,
                  SCM_IOMUX_74_UART2_TX_DEFAULT);
}

static void scm_uart3_rx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART3_RX_BIT, SCM_IOMUX_78_UART3_RX_MASK,
                  SCM_IOMUX_78_UART3_RX_DEFAULT);
}

static void scm_uart3_tx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART3_TX_BIT, SCM_IOMUX_78_UART3_TX_MASK,
                  SCM_IOMUX_78_UART3_TX_DEFAULT);
}

static void scm_uart4_rx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART4_RX_BIT, SCM_IOMUX_78_UART4_RX_MASK,
                  SCM_IOMUX_78_UART4_RX_DEFAULT);
}

static void scm_uart4_tx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART4_TX_BIT, SCM_IOMUX_78_UART4_TX_MASK,
                  SCM_IOMUX_78_UART4_TX_DEFAULT);
}

static void scm_uart5_rx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART5_RX_BIT, SCM_IOMUX_78_UART5_RX_MASK,
                  SCM_IOMUX_78_UART5_RX_DEFAULT);
}

static void scm_uart5_tx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART5_TX_BIT, SCM_IOMUX_78_UART5_TX_MASK,
                  SCM_IOMUX_78_UART5_TX_DEFAULT);
}

static void scm_uart6_rx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART6_RX_BIT, SCM_IOMUX_78_UART6_RX_MASK,
                  SCM_IOMUX_78_UART6_RX_DEFAULT);
}

static void scm_uart6_tx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART6_TX_BIT, SCM_IOMUX_78_UART6_TX_MASK,
                  SCM_IOMUX_78_UART6_TX_DEFAULT);
}

static void scm_uart7_rx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART7_RX_BIT, SCM_IOMUX_78_UART7_RX_MASK,
                  SCM_IOMUX_78_UART7_RX_DEFAULT);
}

static void scm_uart7_tx(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART7_TX_BIT, SCM_IOMUX_78_UART7_TX_MASK,
                  SCM_IOMUX_78_UART7_TX_DEFAULT);
}

static void scm_uart6_cts(void)
{
    scm_io_mux_98(SCM_IOMUX_98_UART6_CTS_BIT, SCM_IOMUX_98_UART6_CTS_MASK,
                  SCM_IOMUX_98_UART6_CTS_DEFAULT);
}

static void scm_uart6_rts(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_UART6_RTS_BIT, SCM_IOMUX_9C_UART6_RTS_MASK,
                  SCM_IOMUX_9C_UART6_RTS_DEFAULT);
}

int scm_uart_en(int index, int ts_flag)
{
    switch (index) {
    case 0:
        scm_uart0_rx();
        scm_uart0_tx();
        break;
    case 1:
        scm_uart1_rx();
        scm_uart1_tx();
        break;
    case 2:
        scm_uart2_rx();
        scm_uart2_tx();
        break;
    case 3:
        scm_uart3_rx();
        scm_uart3_tx();
        break;
    case 4:
        scm_uart4_rx();
        scm_uart4_tx();
        break;
    case 5:
        scm_uart5_rx();
        scm_uart5_tx();
        break;
    case 6:
        scm_uart6_rx();
        scm_uart6_tx();
        if (ts_flag) {
            scm_uart6_cts();
            scm_uart6_rts();
        }
        break;
    case 7:
        scm_uart7_rx();
        scm_uart7_tx();
        break;
    default:
        iomux_printf("Uart: uart index %d is error. please check it!", index);
        return DRV_OP_FAILED;
    }

    return DRV_OP_SUCCESS;
}
