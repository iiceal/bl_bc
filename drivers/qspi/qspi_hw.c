#include <common.h>
#include "fl_common_cmd.h"
#include "regs/qspi.h"
#include "drivers/drv_qspi.h"
#include "qspi_hw.h"

#ifdef CONFIG_SUPPORT_FLASH_XIP
static u32 save_ssienr, save_ctrlr0, save_read_cmd, save_imr;
void save_controller_context(void)
{
    save_ssienr = hwp_apQspi->ssienr;
    save_ctrlr0 = hwp_apQspi->ctrlr0;
    save_read_cmd = hwp_apQspi->read_cmd;
    save_imr = hwp_apQspi->imr;
}

void restore_controller_context(void)
{
    hwp_apQspi->ssienr = 0;
    hwp_apQspi->ctrlr0 = save_ctrlr0;
    hwp_apQspi->read_cmd = save_read_cmd;
    hwp_apQspi->imr = save_imr;
    hwp_apQspi->ssienr = save_ssienr;
}
#endif

/*
 * Disable qspi and disable all interrupts
 */
inline void qspi_hw_init(void)
{
    hwp_apQspi->ssienr = QSPI_SSIENR_EN(0) | QSPI_SSIENR_AHB_EN(0);
    hwp_apQspi->imr = QSPI_IMR_TXEIM(0) | QSPI_IMR_TXOIM(0) | QSPI_IMR_RXUIM(0) |
                      QSPI_IMR_RXOIM(0) | QSPI_IMR_RXFIM(0) | QSPI_IMR_AHBIM(0);
    hwp_apQspi->dma_ctrl = 0;
}

inline void qspi_ahb_enable(void)
{
    hwp_apQspi->ssienr = QSPI_SSIENR_AHB_EN(1);
}

inline void qspi_reg_enable(void)
{
    hwp_apQspi->ssienr = QSPI_SSIENR_EN(1);
}

inline void qspi_push_data_8bit(u8 d)
{
    hwp_apQspi->dr = d;
}

inline void qspi_push_data_16bit(u16 d)
{
    hwp_apQspi->dr = d;
}

inline void qspi_push_data_32bit(u32 d)
{
    hwp_apQspi->dr = d;
}

inline u8 qspi_get_data_8bit(void)
{
    return hwp_apQspi->dr & 0xff;
}

inline u16 qspi_get_data_16bit(void)
{
    return hwp_apQspi->dr & 0xffff;
}

inline u32 qspi_get_data_32bit(void)
{
    return hwp_apQspi->dr;
}

inline void qspi_set_rx_fifo_level(int rft)
{
    hwp_apQspi->rxftlr = QSPI_RXFTLR_RFT(rft);
}

inline void qspi_set_tx_fifo_level(int tft)
{
    hwp_apQspi->txftlr = QSPI_TXFTLR_TFT(tft);
}

inline int qspi_get_rx_fifo_d_num(void)
{
    return hwp_apQspi->rxflr;
}

inline int qspi_get_tx_fifo_d_num(void)
{
    return hwp_apQspi->txflr;
}

inline int qspi_cache_flash(void)
{
    return hwp_apQspi->cache_flush;
}

inline void qspi_set_baudr(u32 clk_div)
{
    hwp_apQspi->baudr = QSPI_BAUDR_DIV_MASK & clk_div;
}

inline u32 qspi_clr_all_interrupts(void)
{
    return hwp_apQspi->icr;
}

inline u32 qspi_get_interrupt_status(void)
{
    return hwp_apQspi->risr;
}

inline void qspi_rx_full_inten(int en)
{
    if (en > 0)
        // hwp_apQspi->imr |= QSPI_IMR_RXFIM(1);
        hwp_apQspi->imr = QSPI_IMR_RXFIM(1);
    else
        hwp_apQspi->imr &= ~(QSPI_IMR_RXFIM_MASK);
}

inline void qspi_tx_empty_inten(int en)
{
    if (en > 0)
        //		hwp_apQspi->imr |= QSPI_IMR_TXEIM(1);
        hwp_apQspi->imr = QSPI_IMR_TXEIM(1);
    else
        hwp_apQspi->imr &= ~(QSPI_IMR_TXEIM_MASK);
}

inline void qspi_set_rx_ndf(u32 ndf)
{
    hwp_apQspi->ctrlr1 = QSPI_CTRLR1_NDF(ndf);
}

inline u32 qspi_rx_fifo_flush(void)
{
    return hwp_apQspi->rxfifo_flush;
}

inline bool qspi_is_busy(void)
{
    return ((hwp_apQspi->sr & QSPI_SR_BUSY) == QSPI_SR_BUSY) ? true : false;
}

/*
 * Enable transmit mode and data_frame_size is 8bit
 * Spi mode is mode0
 * Note: Program only support 1-wire mode.
 */
inline void qspi_tx_init(void)
{
    hwp_apQspi->ssienr = QSPI_SSIENR_EN(0) | QSPI_SSIENR_AHB_EN(0);
    hwp_apQspi->ctrlr0 = QSPI_CTRLR0_DFS(7) | QSPI_CTRLR0_TMOD(1);
    hwp_apQspi->read_cmd =
		QSPI_READCMD_CMD(3) | QSPI_READCMD_FLASHM(0) |
		QSPI_READCMD_DIRECT(0) | QSPI_READCMD_AHB_PREFETCH(0);
}

inline void qspi_tx_init_4(void)
{
    hwp_apQspi->ssienr = QSPI_SSIENR_EN(0) | QSPI_SSIENR_AHB_EN(0);
    hwp_apQspi->ctrlr0 = QSPI_CTRLR0_DFS(0xF) | QSPI_CTRLR0_TMOD(1) | 0x10000;
    hwp_apQspi->read_cmd =
		QSPI_READCMD_CMD(3) | QSPI_READCMD_FLASHM(0) |
		QSPI_READCMD_DIRECT(0) | QSPI_READCMD_AHB_PREFETCH(0);
    hwp_apQspi->pgm_cmd = 0x2;
}

/*
 * Enable eeprom_read mode and data_frame_size is 8bit/(or)16bit/(or)32bit
 * Spi mode is mode0
 * Configure read_mode is read/fast/dual/quad/dualio/quadio
 */
inline u32 qspi_rx_get_e2prmode(u8 cmd, u8 dfs)
{
    u32 ctrl0, rd_index = 0;

    ctrl0 = QSPI_CTRLR0_TMOD(3);
    switch (dfs) {
    case 8:
        ctrl0 |= QSPI_CTRLR0_DFS(7);
        break;
    case 16:
        ctrl0 |= QSPI_CTRLR0_DFS(0xf);
        break;
    case 32:
        ctrl0 |= QSPI_CTRLR0_DFS(0xf) | QSPI_CTRLR0_DFS_HIGH;
        break;
    default:
        return false;
    }

    switch (cmd) {
    case SPINOR_OP_READ:
        rd_index = 0;
        break;
    case SPINOR_OP_READ_FAST:
        rd_index = 1;
        break;
    case SPINOR_OP_READ_1_1_2:
        rd_index = 2;
        break;
    case SPINOR_OP_READ_1_1_4:
        rd_index = 3;
        break;
    case SPINOR_OP_READ_DUAL_IO:
        rd_index = 4;
        break;
    case SPINOR_OP_READ_QUAD_IO:
        rd_index = 5;
        break;
    default:
        return false;
    }

    ctrl0 |= QSPI_CTRLR0_E2PRMODE(rd_index);

    return ctrl0;
}

/*
 * Enable eeprom_read mode and data_frame_size is 8bit/(or)16bit/(or)32bit
 * Spi mode is mode0
 * Configure read_mode is read/fast/dual/quad/dualio/quadio
 */
inline bool qspi_rx_init(u8 cmd, u8 dfs)
{
    u32 ctrl0, rd_index = 0;

    ctrl0 = QSPI_CTRLR0_TMOD(3);
    switch (dfs) {
    case 8:
        ctrl0 |= QSPI_CTRLR0_DFS(7);
        break;
    case 16:
        ctrl0 |= QSPI_CTRLR0_DFS(0xf);
        break;
    case 32:
        ctrl0 |= QSPI_CTRLR0_DFS(0xf) | QSPI_CTRLR0_DFS_HIGH;
        break;
    default:
        return false;
    }

    switch (cmd) {
    case SPINOR_OP_READ:
        rd_index = 0;
        break;
    case SPINOR_OP_READ_FAST:
        rd_index = 1;
        break;
    case SPINOR_OP_READ_1_1_2:
        rd_index = 2;
        break;
    case SPINOR_OP_READ_1_1_4:
        rd_index = 3;
        break;
    case SPINOR_OP_READ_DUAL_IO:
        rd_index = 4;
        break;
    case SPINOR_OP_READ_QUAD_IO:
        rd_index = 5;
        break;
    default:
        return false;
    }

    ctrl0 |= QSPI_CTRLR0_E2PRMODE(rd_index);
    hwp_apQspi->ssienr = QSPI_SSIENR_EN(0) | QSPI_SSIENR_AHB_EN(0);
    hwp_apQspi->ctrlr0 = ctrl0;
    hwp_apQspi->read_cmd = QSPI_READCMD_CMD(cmd) | QSPI_READCMD_DIRECT(rd_index);
    return true;
}

inline void qspi_set_dma_ctrl(u32 en)
{
    hwp_apQspi->dma_ctrl = en;
}

inline void qspi_set_dma_tdlr(u32 tdlr)
{
    hwp_apQspi->dma_tdlr = tdlr;
}

inline void qspi_set_dma_rdlr(u32 rdlr)
{
    hwp_apQspi->dma_rdlr = rdlr;
}

int qspi_polling_raw_status(void)
{
    u32 irq_cause = hwp_apQspi->isr;

    if (irq_cause & QSPI_ISR_RXFIS) {
        return QSPI_SR_RX_FULL;
    }

    if (irq_cause & QSPI_ISR_TXEIS) {
        return QSPI_SR_TX_EMPTY;
    }

    qspi_clr_all_interrupts();

    if (irq_cause & QSPI_ISR_TXOIS) {
        return QSPI_SR_TX_OVERFLOW;
    }
    if (irq_cause & QSPI_ISR_RXUIS) {
        return QSPI_SR_RX_UNDERFLOW;
    }
    if (irq_cause & QSPI_ISR_RXOIS) {
        return QSPI_SR_RX_OVERFLOW;
    }

    return QSPI_SR_NONE;
}

int qspi_irq_read_status(void)
{
    u32 irq_cause = hwp_apQspi->risr;

    if (irq_cause & QSPI_RISR_RXFIS) {
        return QSPI_SR_RX_FULL;
    }

    if (irq_cause & QSPI_RISR_TXEIS) {
        return QSPI_SR_TX_EMPTY;
    }

    qspi_clr_all_interrupts();

    if (irq_cause & QSPI_RISR_TXOIS) {
        return QSPI_SR_TX_OVERFLOW;
    }
    if (irq_cause & QSPI_RISR_RXUIS) {
        return QSPI_SR_RX_UNDERFLOW;
    }

    if (irq_cause & QSPI_RISR_RXOIS) {
        return QSPI_SR_RX_OVERFLOW;
    }

    return QSPI_SR_NONE;
}

inline u32 get_qspiflash_dr(void)
{
    return (REG_AP_QSPI0_BASE + 0x6c);
}
