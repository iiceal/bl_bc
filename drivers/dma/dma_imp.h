#ifndef _DMA_IMP_H_
#define _DMA_IMP_H_

#include "regs/dma.h"

// function declare
extern inline DMAC_CONTROLLER_ST *get_dmac_controller(U32 id);
extern inline DMAC_CH_CONTROLLER_ST * get_hwp_dma(U32 id, U32 ch_id);
extern inline U32 dmac_read_id(U32);

extern inline U32 dmac_read_compver(U32);

extern inline void dma_enable(U32);

extern inline void dma_disable(U32);

extern inline void dmac_int_enable(U32);

extern inline void dmac_int_disable(U32);

// return 1--ch idle; 0-- ch busy;
extern inline U32 dmac_ch_idle(U32, U32 ch_id);

extern inline void dmac_ch_enable(U32, U32 ch_id);

extern inline void dmac_ch_disable(U32, U32 ch_id);

extern inline void dmac_ch_suspend(U32, U32 ch_id);

extern inline void dmac_ch_abort(U32, U32 ch_id);

extern inline U32 dmac_int_status_get(U32);

// reset whole dmac
extern inline void dmac_reset(U32);

// ch function
extern inline void dmac_ch_sar_set(DMAC_CH_CONTROLLER_ST *hwp_dma, U32 addr);

extern inline void dmac_ch_dar_set(DMAC_CH_CONTROLLER_ST *hwp_dma, U32 addr);

extern inline void dmac_ch_block_ts_set(DMAC_CH_CONTROLLER_ST *hwp_dma, U32 block_ts);

// CTL REG
extern inline void dmac_ch_ctl_lli_enable(DMAC_CH_CONTROLLER_ST *hwp_dma);

extern inline void dmac_ch_ctl_lli_disable(DMAC_CH_CONTROLLER_ST *hwp_dma);

extern inline void dmac_ch_ctl_IOC_BlkTfr_enable(DMAC_CH_CONTROLLER_ST *hwp_dma);

extern inline void dmac_ch_ctl_IOC_BlkTfr_disable(DMAC_CH_CONTROLLER_ST *hwp_dma);

extern inline void dmac_ch_ctl_src_msize_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                             DMAC_CTL_MSIZE_E msize);

extern inline void dmac_ch_ctl_dst_msize_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                             DMAC_CTL_MSIZE_E msize);

extern inline void dmac_ch_ctl_src_tr_width_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                                DMAC_CTL_TR_WIDTH_E tr_width);

extern inline void dmac_ch_ctl_dst_tr_width_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                                DMAC_CTL_TR_WIDTH_E tr_width);

extern inline void dmac_ch_ctl_src_inc_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                           DMAC_CTL_ADDR_INC_E inc_type);

extern inline void dmac_ch_ctl_dst_inc_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                           DMAC_CTL_ADDR_INC_E inc_type);

// CFG REG setting
extern inline void dmac_ch_cfg_priority_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                            DMAC_CFG_PRIORITY_E priority);

extern inline void dmac_ch_cfg_dst_per_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                           DMAC_PERIPHERAL_TYPE_E dst_per);

extern inline void dmac_ch_cfg_src_per_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                           DMAC_PERIPHERAL_TYPE_E src_per);

extern inline void dmac_ch_cfg_tt_fc_set(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CFG_TT_FC_E tt_fc);

extern inline void dmac_ch_cfg_dst_multblk_type_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                                    DMAC_MULTI_BLOCK_TRANSFER_TYPE_E multblk_type);

extern inline void dmac_ch_cfg_src_multblk_type_set(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                                    DMAC_MULTI_BLOCK_TRANSFER_TYPE_E multblk_type);

// LLP setting
extern inline void dmac_ch_llp_loc_set(DMAC_CH_CONTROLLER_ST *hwp_dma, U32 addr);

// CH STATUS REG
extern inline U32 dmac_ch_data_left_in_fifo(DMAC_CH_CONTROLLER_ST *hwp_dma);

extern inline U32 dmac_ch_completed_block_trans_size(DMAC_CH_CONTROLLER_ST *hwp_dma);

// CH INT
extern inline U32 dmac_ch_int_status_get(DMAC_CH_CONTROLLER_ST *hwp_dma);

extern inline void dmac_ch_int_status_ctl(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                          DMAC_CH_INT_TYPE_E int_type, U32 en_or_dis);

extern inline void dmac_ch_int_signal_ctl(DMAC_CH_CONTROLLER_ST *hwp_dma,
                                          DMAC_CH_INT_TYPE_E int_type, U32 en_or_dis);

extern inline void dmac_ch_int_disable_all(DMAC_CH_CONTROLLER_ST *hwp_dma);

extern inline void dmac_ch_int_clear(DMAC_CH_CONTROLLER_ST *hwp_dma, DMAC_CH_INT_TYPE_E int_type);

#endif
