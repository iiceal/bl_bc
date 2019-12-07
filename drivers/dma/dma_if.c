#include <common.h>
#include "dma_imp.h"
#include "drivers/drv_dma.h"
#include "cp15.h"

static volatile U32 dma_ch_busy_flag[2][HW_DMAC_CH_NUM_MAX] = {{0}};

#define vir_to_phy(addr) (addr)
#define phy_to_vir(addr) (addr)

static volatile U32 dma_ch_done[2][HW_DMAC_CH_NUM_MAX] = {{0}};
static volatile BOOL dma_lli_func_flag = false;
static volatile BOOL dmac_en_flag[2] = {false, false};
static volatile U32 ch_lli_addr[2][HW_DMAC_CH_NUM_MAX] = {{0}};

#ifdef CONFIG_USE_IRQ
static int dmac_irq_handler(int irq, void *dev_id);
#endif

#ifdef DMA_TIME_COUNT
DMA_TIME_COUNT_ST dma_time_record[2][HW_DMAC_CH_NUM_MAX];

void get_dma_count_info(U32 id, U32 ch, DMA_TIME_COUNT_ST * st)
{
    memcpy((void *)st, (void *)(&dma_time_record[id][ch]), sizeof(DMA_TIME_COUNT_ST));
}

void dma_count_init(U32 id, U32 ch)
{
    memset((void *)(&dma_time_record[id][ch]), 0, sizeof(DMA_TIME_COUNT_ST));
}
#endif

void dma_init(U32 id)
{
#ifdef CONFIG_USE_IRQ
    U32 irq_id = 0xFFFF;
    U32 irq_data = 0xFFFF;
#endif

    if (true == dmac_en_flag[id])
        return;

#ifdef CONFIG_USE_IRQ
    switch(id){
    case 0:
        irq_id = SYS_IRQ_ID_DMAC0;
        irq_data = HW_DMAC_CONTROLLER_BASE0;
        break;
    case 1:
        irq_id = SYS_IRQ_ID_DMAC1;
        irq_data = HW_DMAC_CONTROLLER_BASE1;
        break;
    default:
        return;
    }

    request_irq(irq_id, dmac_irq_handler, NULL, (void *) (irq_data));
    dmac_int_enable(id);
#endif
    dma_enable(id);
    dmac_en_flag[id] = true;
}

void dma_deinit(U32 id)
{
#ifdef CONFIG_USE_IRQ
    U32 irq_id;
    switch(id){
    case 0:
        irq_id = SYS_IRQ_ID_DMAC0;
        break;
    case 1:
        irq_id = SYS_IRQ_ID_DMAC1;
        break;
    default:
        return;
    }

    unrequest_irq(irq_id);
#endif
    dma_disable(id);
    dmac_int_disable(id);
    dmac_en_flag[id] = false;
}

void dmac_enable(U32 id)
{
    if(!DMAC_INDEX_VALID(id)){
        return;
    }
    dma_enable(id);
}

bool is_dma_init(U32 id)
{
    if(!DMAC_INDEX_VALID(id)){
        return false;
    }
    return dmac_en_flag[id];
}

void dma_ch_disable(U32 id, U32 ch)
{
    if((DMAC_INDEX_VALID(id)) && (ch < HW_DMAC_CH_NUM_MAX)) {
        dmac_ch_disable(id, ch);
    }
}

/* extern interface */
void syn_lock(void)
{
    return;
}

void syn_unlock(void)
{
    return;
}

// need a spinlock for protecting the flag
BOOL lli_lock(void)
{
    if (false == dma_lli_func_flag) {
        dma_lli_func_flag = true;
        return true;
    }
    return false;
}

void lli_unlock(void)
{
    dma_lli_func_flag = false;
    return;
}

U32 get_lli_node_start_addr(U32 id, U32 ch)
{
    if(!DMAC_INDEX_VALID(id)){
      return DMAC_ERR_E_PARA_INVALID;
    }
    return ch_lli_addr[id][ch];
}

// 0--m2m; 1--qspiflash; other
U32 dma_get_valid_ch(U32 id, DMA_TRANS_TYPE_E trans_type)
{
    U32 i, loop_s, loop_e;
    if(!DMAC_INDEX_VALID(id)){
      return DMAC_ERR_E_PARA_INVALID;
    }

    if (false == dmac_en_flag[id])
        return DMAC_ERR_E_NOT_INIT;
    syn_lock();
    switch (trans_type) {
    case DMA_TRANS_TYPE_E_M2M:
        loop_s = 2;
        loop_e = 3;
        break;
    case DMA_TRANS_TYPE_E_QSPIFLASH:
        loop_s = 0;
        loop_e = 1;
        break;
    case DMA_TRANS_TYPE_E_OTHER:
    default:
        loop_s = 4;
        loop_e = 7;
        break;
    }

    for (i = loop_s; i <= loop_e; i++) {
        if (0 == dma_ch_busy_flag[id][i]) {
            dma_ch_busy_flag[id][i] = 1;
            syn_unlock();
            return i;
        }
    }

    syn_unlock();
    return -1;
}

inline void dma_free_ch(U32 id, U32 ch_id)
{
    if(DMAC_INDEX_VALID(id) && (ch_id < HW_DMAC_CH_NUM_MAX)) {
        dma_ch_busy_flag[id][ch_id] = 0;
    }
}

void dma_wait_ch_done(U32 id, U32 ch_id, BOOL syn_flag)
{
    if((!DMAC_INDEX_VALID(id)) || (ch_id >= HW_DMAC_CH_NUM_MAX)) {
        return;
    }
    if (true == syn_flag)
        return;
    while (!dma_ch_done[id][ch_id]) {
        ;
    }
    dma_ch_done[id][ch_id] = 0;
}

void dma_wait_channel_done_polling(U32 id, U32 ch_id, BOOL syn_flag)
{
    DMAC_CH_CONTROLLER_ST *hwp_dma = NULL;
    if (true == syn_flag) {
        return;
    }
    if((!DMAC_INDEX_VALID(id)) || (ch_id >= HW_DMAC_CH_NUM_MAX)) {
        return;
    }

    //hwp_dma = HW_DMAC_CH(ch_id);
    hwp_dma = get_hwp_dma(id, ch_id);
    while (!(dmac_ch_int_status_get(hwp_dma) & DMAC_CH_INTSTATUS_ENABLE_DMA_TFR_DONE_MASK)) {
        ;
    }
    dmac_ch_int_clear(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE);
}

void dma_close_ch(U32 id, U32 ch_id)
{
    if((!DMAC_INDEX_VALID(id)) || (ch_id >= HW_DMAC_CH_NUM_MAX)) {
        return;
    }
    dma_free_ch(id, ch_id);
}

void dma_ch_sar_set(U32 id, U32 ch_id, U32 src)
{
    DMAC_CH_CONTROLLER_ST *hwp_dma = NULL;
    if((!DMAC_INDEX_VALID(id)) || (ch_id >= HW_DMAC_CH_NUM_MAX)) {
        return;
    }

    //hwp_dma = HW_DMAC_CH(ch_id);
    hwp_dma = get_hwp_dma(id, ch_id);
    dmac_ch_sar_set(hwp_dma, src);
}

void dma_ch_dar_set(U32 id, U32 ch_id, U32 dst)
{
    DMAC_CH_CONTROLLER_ST *hwp_dma = NULL;
    if((!DMAC_INDEX_VALID(id)) || (ch_id >= HW_DMAC_CH_NUM_MAX)) {
        return;
    }

    //hwp_dma = HW_DMAC_CH(ch_id);
    hwp_dma = get_hwp_dma(id, ch_id);
    dmac_ch_dar_set(hwp_dma, dst);
}

void dma_ch_int_ctl(U32 id, U32 ch_id, DMAC_CH_INT_TYPE_E int_type, U32 en)
{
    U32 en_flag = 0;
    DMAC_CH_CONTROLLER_ST *hwp_dma = NULL;
    if ((!DMAC_INDEX_VALID(id)) || (ch_id >= HW_DMAC_CH_NUM_MAX) || (int_type >= DMAC_CH_INT_TYPE_E_END)) {
        return;
    }
    //hwp_dma = HW_DMAC_CH(ch_id);
    hwp_dma = get_hwp_dma(id, ch_id);
    if (en)
        en_flag = 1;
    dmac_ch_int_status_ctl(hwp_dma, int_type, en_flag);
    dmac_ch_int_signal_ctl(hwp_dma, int_type, en_flag);
}

#ifdef CONFIG_USE_IRQ
static int dmac_irq_handler(int irq, void *dev_id)
{
    U32 i, ch_id, dmac_base, id;
    DMAC_CONTROLLER_ST *hwp_dma_ctrl = (DMAC_CONTROLLER_ST *) dev_id;
    dmac_base = (U32)dev_id;

    DMAC_CH_CONTROLLER_ST *hwp_dma_ch = NULL;
    U32 int_status = hwp_dma_ctrl->DMAC_INTSTATUS;
    U32 ch_int_status = 0;

    if(HW_DMAC_CONTROLLER_BASE0 == dmac_base){
        id = 0;
    }else if(HW_DMAC_CONTROLLER_BASE1 == dmac_base){
        id = 1;
    }else {
        return 0;
    }

    // printf("int_status:0x%x!\n", int_status);

    if (int_status & 0x10000) {
        // clear common reg int
        //REG_WRITE_UINT32((HW_DMAC_CONTROLLER_BASE + HW_DMAC_COMMONREG_INTCLEAR_REG), 0xFF);
        REG_WRITE_UINT32((dmac_base + HW_DMAC_COMMONREG_INTCLEAR_REG), 0xFF);
    }

    for (i = 0; i < HW_DMAC_CH_NUM_MAX; i++) {
        if (int_status & (0x1 << i)) {
            ch_id = i;
            //hwp_dma_ch = HW_DMAC_CH(i);
            hwp_dma_ch = get_hwp_dma(id, ch_id);
            ch_int_status = hwp_dma_ch->INTSTATUS;
            // printf("ch %d int status:0x%x!\n", i, ch_int_status);

            if (ch_int_status & DMAC_CH_INTSTATUS_DMA_TFR_DONE_MASK) {
                dma_ch_done[id][ch_id] = 1;
                // clear ch int status
                dmac_ch_int_clear(hwp_dma_ch, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE);
            }
            if (ch_int_status & DMAC_CH_INTSTATUS_BLK_TFR_DONE_MASK) {
                // clear ch int status
                dmac_ch_int_clear(hwp_dma_ch, DMAC_CH_INT_TYPE_E_BLK_TFR_DONE);
            }

            if (ch_int_status & (~(DMAC_CH_INTSTATUS_DMA_TFR_DONE_MASK))) {
                // printf("ch %d---0x%x-unknown int, clear all!\n", i, ch_int_status);
                REG_WRITE_UINT32(((U32) hwp_dma_ch + 0x98), 0xFFFFFFFF);
            }
        }
    }

    // clear int status
    return 0;
}
#endif

static inline DMAC_CTL_TR_WIDTH_E get_tr_width(U32 addr)
{
    if (addr % 16 == 0) {
        return DMAC_CTL_TR_WIDTH_E_128;
    } else if (addr % 8 == 0) {
        return DMAC_CTL_TR_WIDTH_E_64;
    } else if (addr % 4 == 0) {
        return DMAC_CTL_TR_WIDTH_E_32;
    } else if (addr % 2 == 0) {
        return DMAC_CTL_TR_WIDTH_E_16;
    } else {
        return DMAC_CTL_TR_WIDTH_E_8;
    }
}

#ifdef CONFIG_CMD_DMA
U32 dma_m2m_single_max(U32 src, U32 dst, U32 len)
{
    return DMAC_ERR_E_OK;
}

static int dma_m2m_base(U32 id, U32 ch, U32 src, U32 dst, U32 blk_ts, U32 src_width, U32 dst_width,
                        U32 m_size, bool sync)
{
    U32 cfg_upper, cfg, ctl_upper, ctl;
    //DMAC_CH_CONTROLLER_ST *hwp_dma = HW_DMAC_CH(ch);
    DMAC_CH_CONTROLLER_ST *hwp_dma = get_hwp_dma(id, ch);
    // config regs for dma
    cfg_upper = DMAC_CH_CFG_DST_OSR_LMT_MASK | DMAC_CH_CFG_SRC_OSR_LMT_MASK |
          (DMAC_CFG_TT_FC_E_M_TO_M_DMA << DMAC_CH_CFG_TT_FC_BIT);
    cfg = (DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_CONTIGUOUS << DMAC_CH_DST_MULTBLK_TYPE_BIT) |
          (DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_CONTIGUOUS << DMAC_CH_SRC_MULTBLK_TYPE_BIT);
    hwp_dma->CFG = cfg;
    hwp_dma->CFG_UPPER = cfg_upper;

    dmac_ch_sar_set(hwp_dma, src);
    dmac_ch_dar_set(hwp_dma, dst);
    dmac_ch_block_ts_set(hwp_dma, blk_ts);

    ctl = (src_width << DMAC_CH_CTL_SRC_TR_WIDTH_BIT) |
          (dst_width << DMAC_CH_CTL_DST_TR_WIDTH_BIT) |
          (m_size << DMAC_CH_CTL_SRC_MSIZE_BIT) |
          (m_size << DMAC_CH_CTL_DST_MSIZE_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_SINC_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_DINC_BIT);
    ctl_upper = 0;

    hwp_dma->CTL = ctl;
    hwp_dma->CTL_UPPER = ctl_upper;

    // clear int status
    REG_WRITE_UINT32(((U32) hwp_dma + 0x98), 0xFFFFFFFF);
    // enable int
    dmac_ch_int_disable_all(hwp_dma);
    dmac_ch_int_status_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    dmac_ch_int_signal_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    asm volatile("ISB");
    asm volatile("DMB");
    asm volatile("DSB");
#ifdef DMA_TIME_COUNT
    dma_time_record[id][ch].config_e = arch_counter_get_cntpct();
#endif
    dmac_ch_enable(id, ch);

    if (true == sync) {
        while (!dma_ch_done[id][ch]) {
            ;
        }
#ifdef DMA_TIME_COUNT
    dma_time_record[id][ch].trans_e = arch_counter_get_cntpct();
    dma_time_record[id][ch].config_total += (U32)(dma_time_record[id][ch].config_e - dma_time_record[id][ch].config_s);
    dma_time_record[id][ch].trans_total += (U32)(dma_time_record[id][ch].trans_e - dma_time_record[id][ch].config_e);
#endif
        dma_ch_done[id][ch] = 0;
        // unrequest_irq(dma_irq);
        // printf("ch %d trans done!\n", ch);
    } else {
        // printf("ch %d trans start!\n", ch);
    }

    return DMAC_ERR_E_OK;
}

int dma_m2m_single(U32 id, U32 ch, U32 src, U32 dst, U32 len, bool sync)
{
    // check input para
    if ((2 != ch) && (3 != ch)) {
        return DMAC_ERR_E_CH_INVALID;
    }
    if ((!DMAC_INDEX_VALID(id)) || (len > HW_DMAC_CH_BLK_MAX)) {
        return DMAC_ERR_E_PARA_INVALID;
    }
    // judge the ch is idle for using
    if (!dmac_ch_idle(id, ch)) {
        // ch busy
        // printf("ch %d is busy, return!\n", ch);
        return DMAC_ERR_E_CH_BUSY;
    }

    return dma_m2m_base(id, ch, src, dst, len, 0, 0, 32, sync);
}

int dma_m2m_single_performance(U32 id, U32 ch, U32 src, U32 dst, U32 len, bool sync)
{
    U32 blk_ts;
    U32 src_width = get_tr_width(src);
    U32 dst_width = get_tr_width(dst);
    U32 len_width = get_tr_width(len);
#ifdef DMA_TIME_COUNT
    dma_time_record[id][ch].config_s = arch_counter_get_cntpct();
#endif
    // check input para
    if ((2 != ch) && (3 != ch)) {
        return DMAC_ERR_E_CH_INVALID;
    }
    if (!DMAC_INDEX_VALID(id)) {
        return DMAC_ERR_E_PARA_INVALID;
    }
    if ((src % 4) || (dst % 4) || (len % 4)) {
        return DMAC_ERR_E_PARA_INVALID;
    }

    // caculate real src width and blk_ts
    src_width = (src_width >= len_width) ? len_width : src_width;
    blk_ts = len / (0x1 << src_width);
    if (blk_ts > HW_DMAC_CH_BLK_MAX) {
        return DMAC_ERR_E_PARA_INVALID;
    }
    // judge the ch is idle for using
    if (!dmac_ch_idle(id, ch)) {
        // ch busy
        // printf("ch %d is busy, return!\n", ch_id);
        return DMAC_ERR_E_CH_BUSY;
    }

    return dma_m2m_base(id, ch, src, dst, blk_ts, src_width, dst_width, 4, sync);
}
#endif

// msize set to 4---20170928
int dma_dir_base(U32 id, U32 ch, U32 src, U32 dst, U32 blk_ts, U32 per_type,
                 U32 src_width, U32 dst_width, U32 src_burst, U32 dst_burst, DMA_DIR_E dir,bool sync)
{
    U32 ctl;
    //DMAC_CH_CONTROLLER_ST *hwp_dma = HW_DMAC_CH(ch);
    DMAC_CH_CONTROLLER_ST *hwp_dma = get_hwp_dma(id, ch);

    // config rx regs
    hwp_dma->CFG = 0;
    if (DMA_DIR_E_P2M == dir) {
        ctl = (DMAC_CTL_ADDR_INC_E_NO_CHANGE << DMAC_CH_CTL_SINC_BIT) |
              (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_DINC_BIT);
        hwp_dma->CFG_UPPER = (DMAC_CH_CFG_DST_OSR_LMT_MASK | DMAC_CH_CFG_SRC_OSR_LMT_MASK |
              (DMAC_CFG_TT_FC_E_P_TO_M_DMA << DMAC_CH_CFG_TT_FC_BIT) |
              (per_type << DMAC_CH_CFG_SRC_PER_BIT));
#if 0
        hwp_dma->CTL = ctl |
        (src_width << DMAC_CH_CTL_SRC_TR_WIDTH_BIT) |
        (dst_width << DMAC_CH_CTL_DST_TR_WIDTH_BIT) |
        (DMAC_CTL_MSIZE_E_4 << DMAC_CH_CTL_SRC_MSIZE_BIT) |
        (DMAC_CTL_MSIZE_E_4 << DMAC_CH_CTL_DST_MSIZE_BIT);
#endif
    }else{
        ctl = (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_SINC_BIT) |
              (DMAC_CTL_ADDR_INC_E_NO_CHANGE << DMAC_CH_CTL_DINC_BIT);
        hwp_dma->CFG_UPPER = (DMAC_CH_CFG_DST_OSR_LMT_MASK | DMAC_CH_CFG_SRC_OSR_LMT_MASK |
              (DMAC_CFG_TT_FC_E_M_TO_P_DMA << DMAC_CH_CFG_TT_FC_BIT) |
              (per_type << DMAC_CH_CFG_DEST_PER_BIT));
#if 0
        hwp_dma->CTL = ctl |
        (src_width << DMAC_CH_CTL_SRC_TR_WIDTH_BIT) |
        (dst_width << DMAC_CH_CTL_DST_TR_WIDTH_BIT) |
        (DMAC_CTL_MSIZE_E_32 << DMAC_CH_CTL_SRC_MSIZE_BIT) |
        (DMAC_CTL_MSIZE_E_32 << DMAC_CH_CTL_DST_MSIZE_BIT);
#endif
    }

    hwp_dma->CTL = ctl |
        (src_width << DMAC_CH_CTL_SRC_TR_WIDTH_BIT) |
        (dst_width << DMAC_CH_CTL_DST_TR_WIDTH_BIT) |
        (src_burst << DMAC_CH_CTL_SRC_MSIZE_BIT) |
        (dst_burst << DMAC_CH_CTL_DST_MSIZE_BIT);

    hwp_dma->CTL_UPPER = 0;
    dmac_ch_sar_set(hwp_dma, src);
    dmac_ch_dar_set(hwp_dma, dst);
    dmac_ch_block_ts_set(hwp_dma, blk_ts);
    dmac_ch_int_disable_all(hwp_dma);
    dmac_ch_int_status_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    dmac_ch_int_signal_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    asm volatile("ISB");
    asm volatile("DMB");
    asm volatile("DSB");
    dmac_ch_enable(id, ch);
    if (true == sync) {
        while (!dma_ch_done[id][ch]) {
            ;
        }
        dma_ch_done[id][ch] = 0;
    }
    return DMAC_ERR_E_OK;
}

int dma_p2m_base(U32 id, U32 ch, U32 src, U32 dst, U32 blk_ts, U32 per_type, U32 src_width, U32 dst_width,
                U32 src_burst, U32 dst_burst, bool sync)
{
    return dma_dir_base(id, ch, src, dst, blk_ts, per_type, src_width, dst_width, src_burst, dst_burst, DMA_DIR_E_P2M, sync);
}

int dma_p2m_single(U32 id, U32 ch, U32 src, U32 dst, U32 len, U32 src_width, U32 src_burst, U32 per_type, bool sync)
{
    U32 blk_ts = len;
    U32 len_width = get_tr_width(len);
    U32 dst_width = get_tr_width(dst);
    U32 dst_burst = DMAC_CTL_MSIZE_E_32;

    if ((ch >= HW_DMAC_CH_NUM_MAX) || (!DMAC_INDEX_VALID(id))) {
        return DMAC_ERR_E_CH_INVALID;
    }
    if ((per_type >= DMAC_PERIPHERAL_TYPE_E_UNKNOWN) ||
        (src_width > DMAC_CTL_TR_WIDTH_E_32) ||
        (src_burst >= DMAC_CTL_MSIZE_E_ERR)) {
        return DMAC_ERR_E_PARA_INVALID;
    }

    // len % src_width 
    if(len % (0x1 << src_width))
    {
        return DMAC_ERR_E_PARA_INVALID;
    }

#if 0
    dst_width = (dst_width >= len_width) ? len_width : dst_width;
    // width of qspi is 32bit, others are 8 bit
    if (per_type < 2) {
        src_width = DMAC_CTL_TR_WIDTH_E_32;
        if (len % 4) {
            return DMAC_ERR_E_PARA_INVALID;
        }
        blk_ts = len / 4;
        if (blk_ts >= HW_DMAC_CH_BLK_MAX) {
            return DMAC_ERR_E_PARA_INVALID;
        }
    }
#else
    dst_width = (dst_width >= len_width) ? len_width : dst_width;
    if(ch >= 4){
        src_width = DMAC_CTL_TR_WIDTH_E_8;
        dst_width = DMAC_CTL_TR_WIDTH_E_8;
    }
    blk_ts = len / (0x1 << src_width);
    if (blk_ts >= HW_DMAC_CH_BLK_MAX) {
        return DMAC_ERR_E_PARA_INVALID;
    }
#endif
    return dma_p2m_base(id, ch, src, dst, blk_ts, per_type, src_width, dst_width, src_burst, dst_burst, sync);
}

int dma_m2p_base(U32 id, U32 ch, U32 src, U32 dst, U32 blk_ts, U32 per_type, U32 src_width, U32 dst_width,
        U32 src_burst, U32 dst_burst, bool sync)
{
    return dma_dir_base(id, ch, src, dst, blk_ts, per_type, src_width, dst_width, src_burst, dst_burst, DMA_DIR_E_M2P, sync);
}

int dma_m2p_single(U32 id, U32 ch, U32 src, U32 dst, U32 len, U32 bit_w, U32 dst_burst, U32 per_type, bool sync)
{
    U32 dst_width = DMAC_CTL_TR_WIDTH_E_8;
    U32 blk_ts = len;
    U32 len_width = get_tr_width(len);
    U32 src_width = get_tr_width(dst);
    U32 src_burst = DMAC_CTL_MSIZE_E_32;
    if ((2 == ch) || (3 == ch) || (ch >= HW_DMAC_CH_NUM_MAX) || (!DMAC_INDEX_VALID(id))) {
        return DMAC_ERR_E_CH_INVALID;
    }
    if ((per_type >= DMAC_PERIPHERAL_TYPE_E_UNKNOWN) ||
        (dst_burst >= DMAC_CTL_MSIZE_E_ERR)) {
        return DMAC_ERR_E_PARA_INVALID;
    }
#if 1
    src_width = (src_width >= len_width) ? len_width : src_width;
    blk_ts = len / (0x1 << src_width);
    if (bit_w == 32) {
        dst_width = DMAC_CTL_TR_WIDTH_E_32;
        if (len % 4) {
            return DMAC_ERR_E_PARA_INVALID;
        }
        //blk_ts = len / 4;
    }else if(bit_w == 8){
        dst_width = DMAC_CTL_TR_WIDTH_E_8;
        //blk_ts = len;
    }

    if(ch >= 4){
        src_width = DMAC_CTL_TR_WIDTH_E_8;
        dst_width = DMAC_CTL_TR_WIDTH_E_8;
    }

    blk_ts = len / (0x1 << src_width);
    if (blk_ts >= HW_DMAC_CH_BLK_MAX) {
        return DMAC_ERR_E_PARA_INVALID;
    }
#else
    // width of qspi is 32bit, others are 8 bit
    if (per_type < 2) {
        src_width = DMAC_CTL_TR_WIDTH_E_32;
        dst_width = DMAC_CTL_TR_WIDTH_E_32;
        if (len % 4) {
            return DMAC_ERR_E_PARA_INVALID;
        }
        blk_ts = len / 4;
        if (blk_ts >= HW_DMAC_CH_BLK_MAX) {
            return DMAC_ERR_E_PARA_INVALID;
        }
    }
#endif
    return dma_m2p_base(id, ch, src, dst, blk_ts, per_type, src_width, dst_width, src_burst, dst_burst, sync);
}

#if 0
int dma_qspiflash_read_polling(U32 id, U32 ch, U32 flash_addr, U32 dst, U32 len, BOOL syn_flag)
{
    int err;
    dmac_int_disable(id);
    // if(is_mmu_enable()){
    //     v7_dma_inv_range(dst, dst+len);
    // }
    err = dma_p2m_single(id, ch, flash_addr, dst, len, DMAC_PERIPHERAL_TYPE_E_QSPI_RX, syn_flag);
    dmac_int_enable(id);
    return err;
}
#else
int dma_qspiflash_read_polling_new(U32 id, U32 ch, U32 flash_addr, U32 dst, U32 len, U32 src_width, U32 src_burst, BOOL syn_flag)
{
    int err;
    dmac_int_disable(id);
    // if(is_mmu_enable()){
    //     v7_dma_inv_range(dst, dst+len);
    // }
    err = dma_p2m_single(id, ch, flash_addr, dst, len, src_width, src_burst, DMAC_PERIPHERAL_TYPE_E_QSPI_RX, syn_flag);
    dmac_int_enable(id);
    return err;
}
#endif

int dma_qspiflash_write_page_polling(U32 id, U32 ch, U32 flash_addr, U32 src, U32 len, U32 bit_w, U32 dst_burst, BOOL syn_flag)
{
    int err;
    dmac_int_disable(id);
    // if(is_mmu_enable()){
    //     v7_dma_clean_range(src, src+len);
    // }
    err = dma_m2p_single(id, ch, src, flash_addr, len, bit_w, dst_burst, DMAC_PERIPHERAL_TYPE_E_QSPI_TX, syn_flag);
    dmac_int_enable(id);
    return err;
}

#ifdef CONFIG_CMD_DMA
int dma_spi_read_polling(U32 id, U32 ch, U32 addr, U32 dst, U32 len, U32 src_width, U32 src_burst, BOOL syn_flag)
{
    int err;
    dmac_int_disable(id);
    // if(is_mmu_enable()){
    //     v7_dma_inv_range(dst, dst+len);
    // }
    err = dma_p2m_single(id, ch, addr, dst, len, src_width, src_burst, DMAC_PERIPHERAL_TYPE_E_SPI0_RX, syn_flag);
    dmac_int_enable(id);
    return err;
}

int dma_spi_write_page_polling(U32 id, U32 ch, U32 flash_addr, U32 src, U32 len, BOOL syn_flag)
{
    int err;
    dmac_int_disable(id);
    // if(is_mmu_enable()){
    //     v7_dma_clean_range(src, src+len);
    // }
    err = dma_m2p_single(id, ch, src, flash_addr, len, 8, 1, DMAC_PERIPHERAL_TYPE_E_SPI0_TX, syn_flag);
    dmac_int_enable(id);
    return err;
}

// lli interface
void dma_m2m_lli_node_set(U32 id, U32 ch, U32 src, U32 dst, U32 len, void *node_buff, void *last_node_buff,
                          bool last)
{
    LLI_NODE_ST *lli = (LLI_NODE_ST *) node_buff;
    LLI_NODE_ST *lli_last = (LLI_NODE_ST *) last_node_buff;
    U32 ctl, ctl_upper;

    if ((NULL == lli) || (!DMAC_INDEX_VALID(id))) {
        return;
    }
    if (NULL == lli_last) {
        ch_lli_addr[id][ch] = (U32) node_buff;
        // printf("dmac %d ch[%d] lli base addr:0x%x\n", id, ch, ch_lli_addr[id][ch]);
    }

    lli->sar = src;
    lli->dar = dst;
    lli->blk_ts = len - 1;
    ctl = (DMAC_CTL_TR_WIDTH_E_8 << DMAC_CH_CTL_SRC_TR_WIDTH_BIT) |
          (DMAC_CTL_TR_WIDTH_E_8 << DMAC_CH_CTL_DST_TR_WIDTH_BIT) |
          (DMAC_CTL_MSIZE_E_32 << DMAC_CH_CTL_SRC_MSIZE_BIT) |
          (DMAC_CTL_MSIZE_E_32 << DMAC_CH_CTL_DST_MSIZE_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_SINC_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_DINC_BIT);
    if (true == last)
        ctl_upper = DMAC_CH_CTL_LLI_VALID_MASK | DMAC_CH_CTL_LLI_LAST_MASK;
    else
        ctl_upper = DMAC_CH_CTL_LLI_VALID_MASK;
    lli->ctl_upper = ctl_upper;
    lli->ctl = ctl;
    lli->llp_upper = 0;
    lli->llp = 0x0;
    // v7_dma_clean_range((U32) node_buff, (U32) node_buff + sizeof(LLI_NODE_ST));
    if (NULL != lli_last) {
        lli_last->llp = (U32) lli;
        // printf("ch[%d] lli_last:0x%x-->lli:0x%x\n", ch, (U32) lli_last, (U32) lli);
        // v7_dma_clean_range((U32) lli_last, (U32) lli_last + sizeof(LLI_NODE_ST));
    }
}

int dma_m2m_lli_start(U32 id, U32 ch, bool sync)
{
    U32 cfg, cfg_upper;
    if((!DMAC_INDEX_VALID(id)) || (ch >= HW_DMAC_CH_NUM_MAX)) {
        return DMAC_ERR_E_PARA_INVALID;
    }
    //DMAC_CH_CONTROLLER_ST *hwp_dma = HW_DMAC_CH(ch);
    DMAC_CH_CONTROLLER_ST *hwp_dma = get_hwp_dma(id, ch);
    // config dmac controller
    cfg_upper = DMAC_CH_CFG_DST_OSR_LMT_MASK | DMAC_CH_CFG_SRC_OSR_LMT_MASK | 
          (DMAC_CFG_TT_FC_E_M_TO_M_DMA << DMAC_CH_CFG_TT_FC_BIT);
    cfg = (DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_LINK_LIST << DMAC_CH_DST_MULTBLK_TYPE_BIT) |
          (DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_LINK_LIST << DMAC_CH_SRC_MULTBLK_TYPE_BIT);
    hwp_dma->CFG = cfg;
    hwp_dma->CFG_UPPER = cfg_upper;

    hwp_dma->LLP_UPPER = 0;
    hwp_dma->LLP = ch_lli_addr[id][ch];

    // clear int status
    REG_WRITE_UINT32(((U32) hwp_dma + 0x98), 0xFFFFFFFF);
    // enable int
    dmac_ch_int_disable_all(hwp_dma);
    dmac_ch_int_status_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    dmac_ch_int_signal_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    // enable ch
    asm volatile("ISB");
    asm volatile("DMB");
    asm volatile("DSB");
    dmac_ch_enable(id, ch);
    if (true == sync) {
        while (!dma_ch_done[id][ch]) {
            ;
        }
        dma_ch_done[id][ch] = 0;
    }

    return DMAC_ERR_E_OK;
}

DMAC_PERIPHERAL_TYPE_E get_uart_tx_per(U32 port)
{
    if (6 == port)
        return DMAC_PERIPHERAL_TYPE_E_UART6_TX;
    if (7 == port)
        return DMAC_PERIPHERAL_TYPE_E_UART7_TX;
    if (1 == port)
        return DMAC_PERIPHERAL_TYPE_E_UART1_TX;

    return 0xFF;
}

DMAC_PERIPHERAL_TYPE_E get_uart_rx_per(U32 port)
{
    if (6 == port)
        return DMAC_PERIPHERAL_TYPE_E_UART6_RX;
    if (7 == port)
        return DMAC_PERIPHERAL_TYPE_E_UART7_RX;
    if (1 == port)
        return DMAC_PERIPHERAL_TYPE_E_UART1_RX;

    return 0xFF;
}

int dma_ap_single(U32 id, U32 ch, U32 src, U32 dst, U32 len)
{
    if((!DMAC_INDEX_VALID(id)) || (ch >= HW_DMAC_CH_NUM_MAX)) {
        return DMAC_ERR_E_PARA_INVALID;
    }
    //DMAC_CH_CONTROLLER_ST *hwp_dma = HW_DMAC_CH(ch);
    DMAC_CH_CONTROLLER_ST *hwp_dma = get_hwp_dma(id, ch);
    U32 blk_ts = len / 16;
    // judge the ch is idle for using
    if (!dmac_ch_idle(id, ch)) {
        // ch busy
        // printf("ch %d is busy, return!\n", ch);
        return DMAC_ERR_E_CH_BUSY;
    }

    // config regs for dma
    hwp_dma->CFG = (DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_CONTIGUOUS << DMAC_CH_DST_MULTBLK_TYPE_BIT) |
          (DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_CONTIGUOUS << DMAC_CH_SRC_MULTBLK_TYPE_BIT);;
    hwp_dma->CFG_UPPER = DMAC_CH_CFG_DST_OSR_LMT_MASK | DMAC_CH_CFG_SRC_OSR_LMT_MASK | 
          (DMAC_CFG_TT_FC_E_M_TO_M_DMA << DMAC_CH_CFG_TT_FC_BIT);;

    dmac_ch_sar_set(hwp_dma, src);
    dmac_ch_dar_set(hwp_dma, dst);
    dmac_ch_block_ts_set(hwp_dma, blk_ts);

    hwp_dma->CTL = (DMAC_CTL_TR_WIDTH_E_128 << DMAC_CH_CTL_SRC_TR_WIDTH_BIT) |
          (DMAC_CTL_TR_WIDTH_E_128 << DMAC_CH_CTL_DST_TR_WIDTH_BIT) |
          (DMAC_CTL_MSIZE_E_32 << DMAC_CH_CTL_SRC_MSIZE_BIT) |
          (DMAC_CTL_MSIZE_E_32 << DMAC_CH_CTL_DST_MSIZE_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_SINC_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_DINC_BIT);;
    hwp_dma->CTL_UPPER = 0;

    // clear int status
    REG_WRITE_UINT32(((U32) hwp_dma + 0x98), 0xFFFFFFFF);
    // enable int
    dmac_ch_int_disable_all(hwp_dma);
    dmac_ch_int_status_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    dmac_ch_int_signal_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    asm volatile("ISB");
    asm volatile("DMB");
    asm volatile("DSB");
#ifdef DMA_TIME_COUNT
    dma_time_record[id][ch].config_e = arch_counter_get_cntpct();
#endif
    dmac_ch_enable(id, ch);
    // wait channel done
    while (!dma_ch_done[id][ch]) {
        ;
    }
#ifdef DMA_TIME_COUNT
    dma_time_record[id][ch].trans_e = arch_counter_get_cntpct();
    dma_time_record[id][ch].config_total += (U32)(dma_time_record[id][ch].config_e - dma_time_record[id][ch].config_s);
    dma_time_record[id][ch].trans_total += (U32)(dma_time_record[id][ch].trans_e - dma_time_record[id][ch].config_e);
#endif
    dma_ch_done[id][ch] = 0;

    return DMAC_ERR_E_OK;
}

// __attribute__((aligned(64))) LLI_NODE_ST lli_48k_test[384];
// U32  dma_lli_node_addr_base = (U32)&lli_48k_test[0];    // total 24kbytes
// U32  lli_node_len[HW_DMAC_CH_NUM_MAX] = {0};
U32  lli_node_blk_ts[2][HW_DMAC_CH_NUM_MAX] = {{0}};
void dma_lli_node_len_set(U32 id, U32 ch, U32 len)
{
    if((!DMAC_INDEX_VALID(id)) || (ch >= HW_DMAC_CH_NUM_MAX)) {
        return ;
    }
    lli_node_blk_ts[id][ch] = len/16 - 1;
}
// lli interface
void dma_ap_lli_node_set(U32 id, U32 ch, U32 src, U32 dst, void *node_buff, void *last_node_buff,
                          bool last)
{
    if((!DMAC_INDEX_VALID(id)) || (ch >= HW_DMAC_CH_NUM_MAX)) {
        return ;
    }
    LLI_NODE_ST *lli = (LLI_NODE_ST *) node_buff;
    LLI_NODE_ST *lli_last = (LLI_NODE_ST *) last_node_buff;
    U32 ctl, ctl_upper;

    // if (NULL == lli) {
    //     return;
    // }
    if (NULL == lli_last) {
        ch_lli_addr[id][ch] = (U32) node_buff;
        // printf("dma %d ch[%d] lli base addr:0x%x\n", id, ch, ch_lli_addr[id][ch]);
    }

    lli->sar = src;
    lli->dar = dst;
    lli->blk_ts = lli_node_blk_ts[id][ch];
    ctl = (DMAC_CTL_TR_WIDTH_E_128 << DMAC_CH_CTL_SRC_TR_WIDTH_BIT) |
          (DMAC_CTL_TR_WIDTH_E_128 << DMAC_CH_CTL_DST_TR_WIDTH_BIT) |
          (DMAC_CTL_MSIZE_E_32 << DMAC_CH_CTL_SRC_MSIZE_BIT) |
          (DMAC_CTL_MSIZE_E_32 << DMAC_CH_CTL_DST_MSIZE_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_SINC_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_DINC_BIT);
    if (true == last)
        ctl_upper = DMAC_CH_CTL_LLI_VALID_MASK | DMAC_CH_CTL_LLI_LAST_MASK /*| DMAC_CH_CTL_BLK_TFR_INT_EN_MASK*/;
    else
        ctl_upper = DMAC_CH_CTL_LLI_VALID_MASK/* | DMAC_CH_CTL_BLK_TFR_INT_EN_MASK*/;
    lli->ctl_upper = ctl_upper;
    lli->ctl = ctl;
    lli->llp_upper = 0;
    lli->llp = 0x0;
    // v7_dma_clean_range((U32) node_buff, (U32) node_buff + sizeof(LLI_NODE_ST));
    if (NULL != lli_last) {
        lli_last->llp = (U32) lli;
        // printf("ch[%d] lli_last:0x%x-->lli:0x%x\n", ch, (U32) lli_last, (U32) lli);
        // v7_dma_clean_range((U32) lli_last, (U32) lli_last + sizeof(LLI_NODE_ST));
    }
}

int dma_ap_lli_start(U32 id, U32 ch)
{
    DMAC_CH_CONTROLLER_ST *hwp_dma;
    if((!DMAC_INDEX_VALID(id)) || (ch >= HW_DMAC_CH_NUM_MAX)) {
        return DMAC_ERR_E_PARA_INVALID;
    }
    //DMAC_CH_CONTROLLER_ST *hwp_dma = HW_DMAC_CH(ch);
    hwp_dma = get_hwp_dma(id, ch);
    // config dmac controller
    hwp_dma->CFG = (DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_LINK_LIST << DMAC_CH_DST_MULTBLK_TYPE_BIT) |
          (DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_LINK_LIST << DMAC_CH_SRC_MULTBLK_TYPE_BIT);;
    hwp_dma->CFG_UPPER = DMAC_CH_CFG_DST_OSR_LMT_MASK | DMAC_CH_CFG_SRC_OSR_LMT_MASK | 
          (DMAC_CFG_TT_FC_E_M_TO_M_DMA << DMAC_CH_CFG_TT_FC_BIT);;

    hwp_dma->LLP_UPPER = 0;
    hwp_dma->LLP = ch_lli_addr[id][ch];

    // clear int status
    REG_WRITE_UINT32(((U32) hwp_dma + 0x98), 0xFFFFFFFF);
    // enable int
    dmac_ch_int_disable_all(hwp_dma);
    dmac_ch_int_status_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    dmac_ch_int_signal_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    // enable ch
    asm volatile("ISB");
    asm volatile("DMB");
    asm volatile("DSB");
    #ifdef DMA_TIME_COUNT
    dma_time_record[id][ch].config_e = arch_counter_get_cntpct();
    #endif
    dmac_ch_enable(id, ch);
    // wait channel done
    while (!dma_ch_done[id][ch]) {
        ;
    }
#ifdef DMA_TIME_COUNT
    dma_time_record[id][ch].trans_e = arch_counter_get_cntpct();
    dma_time_record[id][ch].config_total += (U32)(dma_time_record[id][ch].config_e - dma_time_record[id][ch].config_s);
    dma_time_record[id][ch].trans_total += (U32)(dma_time_record[id][ch].trans_e - dma_time_record[id][ch].config_e);
#endif
    dma_ch_done[id][ch] = 0;
    return DMAC_ERR_E_OK;
}
#endif

// TODO: add qspi marco here
// speed up read interface
extern inline u32 get_qspiflash_dr(void);
extern int qspi_dma_read_start_new(u8 cmd, u32 addr);
extern inline void qspi_hw_init(void);
u32 rdch = 1;
u32 wrch = 0;
int flash_read_dma_init_once(u32 len)
{
    U32 ctl;
    dmac_int_disable(0);

    DMAC_CH_CONTROLLER_ST *hwp_dma = get_hwp_dma(0, rdch);

    // config rx regs
    hwp_dma->CFG = 0;
    ctl = (DMAC_CTL_ADDR_INC_E_NO_CHANGE << DMAC_CH_CTL_SINC_BIT) |
          (DMAC_CTL_ADDR_INC_E_INC << DMAC_CH_CTL_DINC_BIT);
    hwp_dma->CFG_UPPER = (DMAC_CH_CFG_DST_OSR_LMT_MASK | DMAC_CH_CFG_SRC_OSR_LMT_MASK |
                          (DMAC_CFG_TT_FC_E_P_TO_M_DMA << DMAC_CH_CFG_TT_FC_BIT) |
                          (DMAC_PERIPHERAL_TYPE_E_QSPI_RX << DMAC_CH_CFG_SRC_PER_BIT));

    hwp_dma->CTL = ctl | (DMAC_CTL_TR_WIDTH_E_32 << DMAC_CH_CTL_SRC_TR_WIDTH_BIT) |
                   (DMAC_CTL_TR_WIDTH_E_32 << DMAC_CH_CTL_DST_TR_WIDTH_BIT) |
                   (DMAC_CTL_MSIZE_E_16 << DMAC_CH_CTL_SRC_MSIZE_BIT) |
                   (DMAC_CTL_MSIZE_E_16 << DMAC_CH_CTL_DST_MSIZE_BIT);

    hwp_dma->CTL_UPPER = 0;
    dmac_ch_sar_set(hwp_dma, get_qspiflash_dr());
    dmac_ch_block_ts_set(hwp_dma, (len / 4));
    dmac_ch_int_disable_all(hwp_dma);
    dmac_ch_int_status_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    dmac_ch_int_signal_ctl(hwp_dma, DMAC_CH_INT_TYPE_E_DMA_TFR_DONE, 0x1);
    asm volatile("ISB");
    asm volatile("DMB");
    asm volatile("DSB");
    return 0;
}

int flash_dma_read_start(u32 fl_addr, u8 *rx_dbuf, int rx_len)
{
    DMAC_CH_CONTROLLER_ST *hwp_dma = get_hwp_dma(0, rdch);
    dmac_ch_dar_set(hwp_dma, (u32) rx_dbuf);
    dmac_ch_enable(0, rdch);
    qspi_dma_read_start_new(0xeb, fl_addr);
    return 0;
}

int flash_dma_read_wait_done(void)
{
    dma_wait_channel_done_polling(0, rdch, 0);
    return 0;
}

int flash_dma_read_finished(void)
{
    // disable qspi dma feature
    qspi_hw_init();
    dmac_int_enable(0);
    return 0;
}
