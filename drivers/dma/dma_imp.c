#include <common.h>
#include "dma_imp.h"
// static volatile u32 timer_irq_pending = 0;
// static volatile DMA_CH_CONTROLLER_ST * hwp_dma_ch_controller =
// NULL;

inline DMAC_CONTROLLER_ST *get_dmac_controller(U32 id)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = NULL;

    switch(id){
    case 0:
        hwp_apDMA = hwp_apDMA0;
        break;
    case 1:
        hwp_apDMA = hwp_apDMA1;
        break;
    default:
        break;
    }

    return hwp_apDMA;
}

inline DMAC_CH_CONTROLLER_ST * get_hwp_dma(U32 id, U32 ch_id)
{
    U32 dmac_base;
    switch(id){
    default:
    case 0:
        dmac_base = HW_DMAC_CONTROLLER_BASE0;
        break;

    case 1:
        dmac_base = HW_DMAC_CONTROLLER_BASE1;
        break;
    }
    return ((DMAC_CH_CONTROLLER_ST *)(dmac_base + HW_DMAC_CONTROLLER_CH_BASE \
                + (ch_id) * HW_DMAC_CONTROLLER_INTERVAL));
}

inline U32 dmac_read_id(U32 id)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return 0xFFFFFFFF;
    }
    return hwp_apDMA->ID;
}

inline U32 dmac_read_compver(U32 id)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return 0xFFFFFFFF;
    }
    return hwp_apDMA->DMAC_COMPVER;
}

inline void dma_enable(U32 id)
{
    U32 reg_value;
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }

    reg_value = hwp_apDMA->DMAC_CFG;
    reg_value |= DMAC_CFG_DMAC_EN_MASK;
    hwp_apDMA->DMAC_CFG = reg_value;
}

inline void dma_disable(U32 id)
{
    U32 reg_value;
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }

    reg_value = hwp_apDMA->DMAC_CFG;
    reg_value &= ~DMAC_CFG_DMAC_EN_MASK;
    hwp_apDMA->DMAC_CFG = reg_value;
}

inline void dmac_int_enable(U32 id)
{
    U32 reg_value;
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }

    reg_value = hwp_apDMA->DMAC_CFG;
    reg_value |= DMAC_CFG_INT_EN_MASK;
    hwp_apDMA->DMAC_CFG = reg_value;
}

inline void dmac_int_disable(U32 id)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }

    U32 reg_value = hwp_apDMA->DMAC_CFG;
    reg_value &= ~DMAC_CFG_INT_EN_MASK;
    hwp_apDMA->DMAC_CFG = reg_value;
}

// return 1--ch idle; 0-- ch busy;
inline U32 dmac_ch_idle(U32 id, U32 ch)
{
    U32 ch_status;
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return 0;
    }

    ch_status = hwp_apDMA->DMAC_CHEN;
    if (ch_status & (0x1 << ch)) {
        // busy
        return 0;
    } else {
        // idle
        return 1;
    }
}

inline void dmac_ch_enable(U32 id, U32 ch)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }

    hwp_apDMA->DMAC_CHEN = (0x101 << ch);
}

inline void dmac_ch_disable(U32 id, U32 ch)
{
    DMAC_CH_CONTROLLER_ST *hwp_dma;
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }
    hwp_dma = get_hwp_dma(id, ch);
    if(NULL == hwp_dma){
        return ;
    }
    hwp_apDMA->DMAC_CHEN = (0x100 << ch);
    while (hwp_apDMA->DMAC_CHEN & (0x1 << ch)) {
        ;
    }
    // clear ch_disabled int bit
    REG_WRITE_UINT32(((U32) hwp_dma + 0x98), (0x1 << 30));
}

inline void dmac_ch_suspend(U32 id, U32 ch)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }

    hwp_apDMA->DMAC_CHEN = (0x1010000 << ch);
}

inline void dmac_ch_abort(U32 id, U32 ch)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }

    hwp_apDMA->DMAC_CHEN_UPPER = (0x101 << ch);
}

inline U32 dmac_int_status_get(U32 id)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return 0;
    }

    return hwp_apDMA->DMAC_INTSTATUS;
}

// reset whole dmac
inline void dmac_reset(U32 id)
{
    DMAC_CONTROLLER_ST *hwp_apDMA = get_dmac_controller(id);
    if(NULL == hwp_apDMA){
        return ;
    }

    hwp_apDMA->DMAC_RST = 0x1;
    while (hwp_apDMA->DMAC_RST) {
        ;
    }
}

// ch function
inline void dmac_ch_sar_set(DMAC_CH_CONTROLLER_ST *hwp_dma, U32 addr)
{
    hwp_dma->SAR = addr;
}

inline void dmac_ch_dar_set(DMAC_CH_CONTROLLER_ST *hwp_dma, U32 addr)
{
    hwp_dma->DAR = addr;
}

inline void dmac_ch_block_ts_set(DMAC_CH_CONTROLLER_ST *hwp_dma, U32 block_ts)
{
    hwp_dma->BLOCK_TS = ((block_ts - 1) & 0x1FFFFF);
}

// CTL REG
inline void dmac_ch_ctl_lli_enable(DMAC_CH_CONTROLLER_ST *hwp_dma)
{
    U32 reg_value = hwp_dma->CTL_UPPER;
    reg_value |= DMAC_CH_CTL_LLI_VALID_MASK;
    hwp_dma->CTL_UPPER = reg_value;
}

inline void dmac_ch_ctl_lli_disable(DMAC_CH_CONTROLLER_ST *hwp_dma)
{
    U32 reg_value = hwp_dma->CTL_UPPER;
    reg_value &= ~DMAC_CH_CTL_LLI_VALID_MASK;
    hwp_dma->CTL_UPPER = reg_value;
}

inline void dmac_ch_ctl_IOC_BlkTfr_enable(DMAC_CH_CONTROLLER_ST *hwp_dma)
{
    U32 reg_value = hwp_dma->CTL_UPPER;
    reg_value |= DMAC_CH_CTL_BLK_TFR_INT_EN_MASK;
    hwp_dma->CTL_UPPER = reg_value;
}

inline void dmac_ch_ctl_IOC_BlkTfr_disable(DMAC_CH_CONTROLLER_ST *hwp_dma)
{
    U32 reg_value = hwp_dma->CTL_UPPER;
    reg_value &= ~DMAC_CH_CTL_BLK_TFR_INT_EN_MASK;
    hwp_dma->CTL_UPPER = reg_value;
}

inline void dmac_ch_ctl_src_msize_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CTL_MSIZE_E msize)
{
    U32 reg_value = hwp_dma->CTL;
    hwp_dma->CTL = ((reg_value & (~DMAC_CH_CTL_SRC_MSIZE_MASK)) |
                    (msize << DMAC_CH_CTL_SRC_MSIZE_BIT));
}

inline void dmac_ch_ctl_dst_msize_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CTL_MSIZE_E msize)
{
    U32 reg_value = hwp_dma->CTL;
    hwp_dma->CTL = ((reg_value & (~DMAC_CH_CTL_DST_MSIZE_MASK)) |
                    (msize << DMAC_CH_CTL_DST_MSIZE_BIT));
}

inline void dmac_ch_ctl_src_tr_width_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                         DMAC_CTL_TR_WIDTH_E tr_width)
{
    U32 reg_value = hwp_dma->CTL;
    hwp_dma->CTL = ((reg_value & (~DMAC_CH_CTL_SRC_TR_WIDTH_MASK)) |
                    (tr_width << DMAC_CH_CTL_SRC_TR_WIDTH_BIT));
}

inline void dmac_ch_ctl_dst_tr_width_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                         DMAC_CTL_TR_WIDTH_E tr_width)
{
    U32 reg_value = hwp_dma->CTL;
    hwp_dma->CTL = ((reg_value & (~DMAC_CH_CTL_DST_TR_WIDTH_MASK)) |
                    (tr_width << DMAC_CH_CTL_DST_TR_WIDTH_BIT));
}

inline void dmac_ch_ctl_src_inc_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CTL_ADDR_INC_E inc_type)
{
    U32 reg_value = hwp_dma->CTL;
    hwp_dma->CTL = ((reg_value & (~DMAC_CH_CTL_SINC_MASK)) |
                    (inc_type << DMAC_CH_CTL_SINC_BIT));
}

inline void dmac_ch_ctl_dst_inc_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CTL_ADDR_INC_E inc_type)
{
    U32 reg_value = hwp_dma->CTL;
    hwp_dma->CTL = ((reg_value & (~DMAC_CH_CTL_DINC_MASK)) |
                    (inc_type << DMAC_CH_CTL_DINC_BIT));
}

// CFG REG setting
inline void dmac_ch_cfg_priority_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CFG_PRIORITY_E priority)
{
    U32 reg_value = hwp_dma->CFG_UPPER;
    hwp_dma->CFG_UPPER = (reg_value & (~DMAC_CH_CFG_CH_PRIOR_MASK)) |
                         (priority << DMAC_CH_CFG_CH_PRIOR_BIT);
}

inline void dmac_ch_cfg_dst_per_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_PERIPHERAL_TYPE_E dst_per)
{
    U32 reg_value = hwp_dma->CFG_UPPER;
    hwp_dma->CFG_UPPER = (reg_value & (~DMAC_CH_CFG_DEST_PER_MASK)) |
                         (dst_per << DMAC_CH_CFG_DEST_PER_BIT);
}

inline void dmac_ch_cfg_src_per_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_PERIPHERAL_TYPE_E src_per)
{
    U32 reg_value = hwp_dma->CFG_UPPER;
    hwp_dma->CFG_UPPER = (reg_value & (~DMAC_CH_CFG_SRC_PER_MASK)) |
                         (src_per << DMAC_CH_CFG_SRC_PER_BIT);
}

inline void dmac_ch_cfg_tt_fc_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CFG_TT_FC_E tt_fc)
{
    U32 reg_value = hwp_dma->CFG_UPPER;
    hwp_dma->CFG_UPPER = (reg_value & (~DMAC_CH_CFG_TT_FC_MASK)) |
                         (tt_fc << DMAC_CH_CFG_TT_FC_BIT);
}

inline void dmac_ch_cfg_dst_multblk_type_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                             DMAC_MULTI_BLOCK_TRANSFER_TYPE_E multblk_type)
{
    U32 reg_value = hwp_dma->CFG;
    hwp_dma->CFG = (reg_value & (~DMAC_CH_DST_MULTBLK_TYPE_MASK)) |
                   (multblk_type << DMAC_CH_DST_MULTBLK_TYPE_BIT);
}

inline void dmac_ch_cfg_src_multblk_type_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                             DMAC_MULTI_BLOCK_TRANSFER_TYPE_E multblk_type)
{
    U32 reg_value = hwp_dma->CFG;
    hwp_dma->CFG = (reg_value & (~DMAC_CH_SRC_MULTBLK_TYPE_MASK)) |
                   (multblk_type << DMAC_CH_SRC_MULTBLK_TYPE_BIT);
}

// LLP setting
inline void dmac_ch_llp_loc_set(DMAC_CH_CONTROLLER_ST *hwp_dma, U32 addr)
{
    hwp_dma->LLP = (addr & (~DMAC_CH_LLP_LOC_MASK)) | 0x0;
}

// CH STATUS REG
inline U32 dmac_ch_data_left_in_fifo(DMAC_CH_CONTROLLER_ST *hwp_dma)
{
    return (hwp_dma->STATUS_UPPER & DMAC_CH_STATUS_DATA_LEFT_IN_FIFO_MASK);
}

inline U32 dmac_ch_completed_block_trans_size(DMAC_CH_CONTROLLER_ST *hwp_dma)
{
    return (hwp_dma->STATUS & DMAC_CH_STATUS_CMPLTD_BLK_TFR_SIZE_MASK);
}

// CH INT
inline U32 dmac_ch_int_status_get(DMAC_CH_CONTROLLER_ST *hwp_dma)
{
    return hwp_dma->INTSTATUS;
}

inline void dmac_ch_int_status_ctl(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CH_INT_TYPE_E int_type,
                                   U32 en_or_dis)
{
    U32 int_bit, reg_value;
    switch (int_type) {
    case DMAC_CH_INT_TYPE_E_DMA_TFR_DONE:
        int_bit = DMAC_CH_INTSTATUS_ENABLE_DMA_TFR_DONE_BIT;
        break;

    case DMAC_CH_INT_TYPE_E_BLK_TFR_DONE:
        int_bit = DMAC_CH_INTSTATUS_ENABLE_BLK_TFR_DONE_BIT;
        break;

    case DMAC_CH_INT_TYPE_E_SHADOWREG_OR_LLI_INVALID_ERR:
        int_bit = DMAC_CH_INTSTATUS_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_BIT;
        break;

    default:
        return;
    }

    if (0 == en_or_dis) // disable
    {
        reg_value = hwp_dma->INTSTATUS_ENABLE;
        reg_value &= (~(0x1 << int_bit));
        hwp_dma->INTSTATUS_ENABLE = reg_value;
    } else // enable
    {
        reg_value = hwp_dma->INTSTATUS_ENABLE;
        reg_value |= (0x1 << int_bit);
        hwp_dma->INTSTATUS_ENABLE = reg_value;
    }

    return;
}

inline void dmac_ch_int_signal_ctl(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CH_INT_TYPE_E int_type,
                                   U32 en_or_dis)
{
    U32 int_bit, reg_value;
    switch (int_type) {
    case DMAC_CH_INT_TYPE_E_DMA_TFR_DONE:
        int_bit = DMAC_CH_INTSIGNAL_ENABLE_DMA_TFR_DONE_BIT;
        break;

    case DMAC_CH_INT_TYPE_E_BLK_TFR_DONE:
        int_bit = DMAC_CH_INTSTATUS_ENABLE_BLK_TFR_DONE_BIT;
        break;

    case DMAC_CH_INT_TYPE_E_SHADOWREG_OR_LLI_INVALID_ERR:
        int_bit = DMAC_CH_INTSTATUS_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_BIT;
        break;

    default:
        return;
    }

    if (0 == en_or_dis) // disable
    {
        reg_value = hwp_dma->INTSIGNAL_ENABLE;
        reg_value &= (~(0x1 << int_bit));
        hwp_dma->INTSIGNAL_ENABLE = reg_value;
    } else // enable
    {
        reg_value = hwp_dma->INTSIGNAL_ENABLE;
        reg_value |= (0x1 << int_bit);
        hwp_dma->INTSIGNAL_ENABLE = reg_value;
    }

    return;
}

inline void dmac_ch_int_disable_all(DMAC_CH_CONTROLLER_ST *hwp_dma)
{
    hwp_dma->INTSIGNAL_ENABLE = 0;
    hwp_dma->INTSTATUS_ENABLE = 0;
}

inline void dmac_ch_int_clear(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CH_INT_TYPE_E int_type)
{
    U32 int_bit = 0;
    switch (int_type) {
    case DMAC_CH_INT_TYPE_E_DMA_TFR_DONE:
        int_bit = DMAC_CH_INTCLEAR_DMA_TFR_DONE_BIT;
        break;

    case DMAC_CH_INT_TYPE_E_BLK_TFR_DONE:
        int_bit = DMAC_CH_INTCLEAR_BLK_TFR_DONE_BIT;
        break;

    case DMAC_CH_INT_TYPE_E_SHADOWREG_OR_LLI_INVALID_ERR:
        int_bit = DMAC_CH_INTSTATUS_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_BIT;
        break;

    default:
        return;
    }

    REG_WRITE_UINT32(((U32) hwp_dma + 0x98), (0x1 << int_bit));

    return;
}
