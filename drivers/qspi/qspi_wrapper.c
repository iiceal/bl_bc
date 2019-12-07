#include <common.h>
#include <drivers/drv_qspi.h>
#include "fl_common_cmd.h"
#include "qspi_hw.h"
#include "regs/qspi.h"
#include "cp15.h"

static volatile u32 qspi_rw_opt_pending = 0;
static int using_irq_mode = 0;
static int qspi_print_log = 0;

static u8 *qspi_rq_dbuf;
static int qspi_rq_dlen;
static int irq_rw_dlen;
static int qspi_rd_thr_8;
static int qspi_rd_thr_en = 0;
static int qspi_rd_dfs = 0;

// static int dma_init = 0;

#define	QSPI_RX	0
#define	QSPI_TX	1
#define QSPI_DMA_CHANNEL_W	(0)
#define QSPI_DMA_CHANNEL	(1)


int qspi_dma_send_read_cmd(u8 cmd, u32 addr, int rr, int tx_len, int dfs);

#ifdef QSPI_HW_STAT
static unsigned long long timer_tick_save = 0;

u64 qspi_get_timer_tick_save(void)
{
    return timer_tick_save;
}
#endif

static void wait_qspi_hw_idle(void)
{
    while (qspi_is_busy() == true)
        ;
}

inline void qspi_wren(u8 cmd)
{
    wait_qspi_hw_idle();
    qspi_tx_init();
    qspi_reg_enable();
    qspi_push_data_8bit(cmd);
    wait_qspi_hw_idle();
}

static void qspi_xfer_init(u8 *dbuf, int dlen, int flags)
{
    qspi_rw_opt_pending = 1;
    qspi_rq_dlen = dlen;
    qspi_rq_dbuf = dbuf;
    irq_rw_dlen = 0;
    qspi_rd_thr_8 = 0;

    if (using_irq_mode > 0) {
        if (flags == QSPI_RX)
            qspi_rx_full_inten(1);

        if (flags == QSPI_TX)
            qspi_tx_empty_inten(1);
    }
}

static void qspi_xfer_tx_process(void)
{
    int tx_e_count = 0, i;
    int rw_len = irq_rw_dlen;
    tx_e_count = 256 - qspi_get_tx_fifo_d_num();
    for (i = 0; i < tx_e_count; i++) {
        qspi_push_data_8bit(qspi_rq_dbuf[rw_len++]);
        if (rw_len == qspi_rq_dlen) {
            if (using_irq_mode > 0)
                qspi_tx_empty_inten(0);
            qspi_rw_opt_pending = 0;
            if (qspi_print_log > 0)
                printf("Had writen %d, qspi tx: tx_e_count = %d, qspi_rq_dlen = %d\n", irq_rw_dlen,
                       tx_e_count, qspi_rq_dlen);
            irq_rw_dlen = rw_len;
            return;
        }
    }
    irq_rw_dlen = rw_len;
}

static bool qspi_xfer_read_32bit_fifo_complete(int dlen)
{
    int i;
    u32 *d32_ = (u32 *) qspi_rq_dbuf;
    for (i = 0; i < dlen; i++)
        d32_[irq_rw_dlen++] = qspi_get_data_32bit();
    if (irq_rw_dlen * 4 == qspi_rq_dlen)
        return true;
    return false;
}

static bool qspi_xfer_read_16bit_fifo_complete(int dlen)
{
    int i;
    u16 *d16_ = (u16 *) qspi_rq_dbuf;
    for (i = 0; i < dlen; i++)
        d16_[irq_rw_dlen++] = qspi_get_data_16bit();
    if (irq_rw_dlen * 2 == qspi_rq_dlen)
        return true;
    return false;
}

static bool qspi_xfer_read_8bit_fifo_complete(int dlen)
{
    int i;
    for (i = 0; i < dlen; i++)
        qspi_rq_dbuf[irq_rw_dlen++] = qspi_get_data_8bit();
    if (irq_rw_dlen == qspi_rq_dlen)
        return true;
    return false;
}

static bool qspi_xfer_read_complete(int dlen)
{
    if (qspi_rd_dfs == 8)
        return qspi_xfer_read_8bit_fifo_complete(dlen);
    if (qspi_rd_dfs == 16)
        return qspi_xfer_read_16bit_fifo_complete(dlen);
    if (qspi_rd_dfs == 32)
        return qspi_xfer_read_32bit_fifo_complete(dlen);
    return false;
}

static void qspi_xfer_rx_process(void)
{
    int rx_count = 0;
    rx_count = qspi_get_rx_fifo_d_num();
#ifdef QSPI_HW_STAT
    timer_tick_save = timer_get_tick();
#endif
    if (qspi_rd_thr_8 > 0)
        rx_count = qspi_rd_thr_8;

    if (qspi_xfer_read_complete(rx_count) == true) {
        if (using_irq_mode > 0)
            qspi_rx_full_inten(0);
        qspi_rw_opt_pending = 0;
        if (qspi_print_log > 0)
            printf("qspi rx: rx_f_count = %d, qspi_rq_dlen = %d\n", rx_count, qspi_rq_dlen);
        return;
    }
}

static void qspi_xfer_polling_process(int flags)
{
    if (using_irq_mode > 0)
        return;

    while (1) {
        if (flags == QSPI_TX) {
            while (qspi_polling_raw_status() != QSPI_SR_TX_EMPTY)
                ;
            qspi_xfer_tx_process();
        }

        if (flags == QSPI_RX) {
            while (qspi_polling_raw_status() != QSPI_SR_RX_FULL)
                ;
            qspi_xfer_rx_process();
        }
        if (qspi_rw_opt_pending == 0)
            break;
    }
}

static void qspi_xfer_wait_done(void)
{
    while (qspi_rw_opt_pending == 1)
        ;
    wait_qspi_hw_idle();
}

static int qspi_irq_handler(int irq, void *dev_id)
{
    u32 st = qspi_irq_read_status();

    if (st == QSPI_SR_RX_FULL) {
        qspi_xfer_rx_process();
    }

    if (st == QSPI_SR_TX_EMPTY) {
        qspi_xfer_tx_process();
    }
    return 0;
}

void qspi_ahb_read_enable(u8 rd_cmd, u8 dfs)
{
    wait_qspi_hw_idle();
    qspi_rx_init(rd_cmd, dfs);
    hwp_apQspi->cache_dis_update = 1;
    qspi_reg_enable();
    qspi_rx_fifo_flush();
    qspi_cache_flash();
    qspi_ahb_enable();
}

void qspi_ahb_write_enable(u8 wren_cmd)
{
    qspi_wren(wren_cmd);
    qspi_ahb_enable();
}

int qspi_send_xx_data(u8 *tx_dbuf, int tx_len)
{
    if (qspi_print_log > 0) {
        printf("qspi_send_xx_data, tx_len = %d\n", tx_len);
    }
    wait_qspi_hw_idle();
    qspi_tx_init();
    qspi_set_tx_fifo_level(128);
    qspi_xfer_init(tx_dbuf, tx_len, QSPI_TX);
    qspi_reg_enable();

    qspi_xfer_polling_process(QSPI_TX);

    qspi_xfer_wait_done();

    return tx_len;
}

int qspi_get_rx_ndf(u8 cmd, int rx_len, int dfs)
{
    if (dfs == 8)
        return rx_len; /*Byte Number*/

    switch (cmd) {
    case SPINOR_OP_READ_1_1_2:
    case SPINOR_OP_READ_1_1_4:
    case SPINOR_OP_READ_DUAL_IO:
    case SPINOR_OP_READ_QUAD_IO:
    case SPINOR_OP_READ_FAST:
        return rx_len; /*Byte Number*/

    case SPINOR_OP_READ:
        if (dfs == 16)
            return rx_len / 2;
        if (dfs == 32)
            return rx_len / 4;
        break;
    default:
        break;
    }
    return 0;
}

int qspi_get_rx_thr(int rx_len, int dfs)
{
    if (dfs == 8)
        return rx_len;
    if (dfs == 16)
        return rx_len / 2;
    if (dfs == 32)
        return rx_len / 4;

    return 0;
}

#ifdef CONFIG_QSPI_DMA_TRANSFER

/*
 *  only for read page operations, using dma mode
 */
int qspi_dma_read_start(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len, int dma_rdlr)
{
    int rx_ndf = 0, rx_thr;
    int mod_rx_len = rx_len%4;
    if(mod_rx_len)
    {
        rx_ndf = qspi_get_rx_ndf(cmd, rx_len, 8);
        rx_thr = qspi_get_rx_thr(rx_len, 8);
    }else{
        rx_ndf = qspi_get_rx_ndf(cmd, rx_len, 32);
        rx_thr = qspi_get_rx_thr(rx_len, 32);
    }

    if ((rx_len > 256) || (rx_ndf == 0))
        return 0;

    if (0 == (mod_rx_len)) {
        wait_qspi_hw_idle();
        qspi_set_dma_ctrl(0);
    }else{
        qspi_dma_send_read_cmd(cmd, addr, rx_thr, rx_ndf, 8);
    }

    if(mod_rx_len)
    {
        //qspi_rx_init(cmd, 8);
        qspi_set_dma_rdlr(0);
    }else{
        qspi_rx_init(cmd, 32);
        qspi_set_rx_fifo_level(rx_thr - 1); 
        qspi_set_rx_ndf(rx_ndf - 1); 
        qspi_set_dma_rdlr(dma_rdlr);
    }

    qspi_set_dma_ctrl(1);
    asm volatile("isb" : : : "memory");

    if (0 == (mod_rx_len)) {
        qspi_reg_enable();
        qspi_push_data_32bit((cmd << 24) | addr);
    }

    return 0;
}

extern void v7_dma_inv_range(u32, u32);
#include "drivers/drv_dma.h"
extern inline u32 get_qspiflash_dr(void);
//extern int dma_qspiflash_read_polling(U32, U32 channel_id, U32 flash_addr, U32 dst_addr, U32 length,
//                                      BOOL syn_flag);
extern int dma_qspiflash_read_polling_new(U32 id, U32 ch, U32 flash_addr, U32 dst, U32 len, 
                                        U32 src_width, U32 src_burst, BOOL syn_flag);
extern void dma_wait_channel_done_polling(U32, U32 channel_id, BOOL syn_flag);
extern inline void dmac_enable(U32);
extern bool is_dma_init(U32);
int qspi_dma_read_page(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len)
{
    unsigned int width = DMAC_CTL_TR_WIDTH_E_32;
    unsigned int burst = DMAC_CTL_MSIZE_E_16;
    int mod_rx_len = rx_len%4;
    if(mod_rx_len)
    {   
        width = DMAC_CTL_TR_WIDTH_E_8;
        burst = DMAC_CTL_MSIZE_E_1;
    }   

    // if(dma_init == 0) {
    //     dmac_enable();
    //     dma_init = 1;
    // }
    if (false == is_dma_init(0)) {
        dmac_enable(0);
    }

    v7_dma_inv_range((u32) rx_dbuf, (u32)(rx_dbuf + rx_len));

    dma_qspiflash_read_polling_new(0, QSPI_DMA_CHANNEL, get_qspiflash_dr(), (u32) rx_dbuf, rx_len, width, burst, 0);
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    qspi_dma_read_start(cmd, addr, rx_dbuf, rx_len, 15);
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    if(mod_rx_len)
        dma_wait_channel_done_polling(0, QSPI_DMA_CHANNEL_W, 0);
    dma_wait_channel_done_polling(0, QSPI_DMA_CHANNEL, 0);
    qspi_hw_init();

    return rx_len;
}
#endif
/*
 *  only for read page operations
 */

int qspi_read_page_data(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len, int dfs)
{
    int rx_thr, rx_ndf = 0;
    u32 d = 0;

    qspi_rd_dfs = dfs;
    rx_thr = qspi_get_rx_thr(rx_len, dfs);
    rx_ndf = qspi_get_rx_ndf(cmd, rx_len, dfs);

    if (qspi_print_log > 0) {
        printf("cmd = %02x, addr = 0x%x, rx_len = %d, ndf = %d, thr = %d\n", cmd, addr, rx_len,
               rx_ndf, rx_thr);
    }

    if (rx_len > 256)
        return 0;
    if ((rx_ndf == 0) || (rx_thr == 0))
        return 0;

    wait_qspi_hw_idle();
    qspi_rx_init(cmd, dfs);
    qspi_set_rx_ndf(rx_ndf - 1);

    qspi_set_rx_fifo_level(rx_thr - 1);
    qspi_xfer_init(rx_dbuf, rx_len, QSPI_RX);

    if (qspi_rd_thr_en > 0) {
        if (rx_len == 256) {
            qspi_set_rx_fifo_level(rx_thr / 8 - 1);
            qspi_rd_thr_8 = rx_thr / 8;
        }
    }

    qspi_reg_enable();

    qspi_rx_fifo_flush();
    asm volatile("ISB");
    asm volatile("DMB");
    asm volatile("DSB");

    if (dfs == 8) {
        qspi_push_data_8bit(cmd);
        qspi_push_data_8bit((addr >> 16) & 0xff);
        qspi_push_data_8bit((addr >> 8) & 0xff);
        qspi_push_data_8bit(addr & 0xff);
    } else if (dfs == 16) {
        d = cmd;
        d = d << 8;
        d |= (addr >> 16) & 0xff;
        qspi_push_data_16bit(d);
        qspi_push_data_16bit(addr & 0xffff);
    } else if (dfs == 32) {
        d = cmd;
        d = d << 24;
        d |= addr;
        qspi_push_data_32bit(d);
    }

    qspi_xfer_polling_process(QSPI_RX);
    qspi_xfer_wait_done();
    return rx_len;
}

#ifdef CONFIG_QSPI_DMA_TRANSFER
#include "drivers/drv_dma.h"
__attribute__((aligned(64))) char dma_qspiflash_write_buf[320] = {0};
//extern int dma_qspiflash_write_page_polling(U32 id, U32 ch, U32 flash_addr, U32 src, U32 len, U32 bit_w, BOOL syn_flag);
extern int dma_qspiflash_write_page_polling(U32 id, U32 ch, U32 flash_addr, U32 src, U32 len, U32 bit_w, U32 dst_burst, BOOL syn_flag);
extern inline void qspi_tx_init_4(void);
static int qspi_dma_write_start(/*u8 cmd, u32 addr, u8 * rx_dbuf, int tx_len, */ int dma_tdlr)
{
    wait_qspi_hw_idle();
    qspi_set_dma_ctrl(0);

    if(dma_tdlr == 8){
        qspi_tx_init();
    }else{
        qspi_tx_init_4();
    }

    qspi_set_tx_fifo_level(128);
    qspi_set_dma_tdlr(dma_tdlr-1);
    qspi_set_dma_ctrl(2);
    asm volatile("isb" : : : "memory");
    qspi_reg_enable();

    return 0;
}

static int qspi_dma_write_read_cmd_start(u8 cmd, int dma_tdlr, u32 rx_thr, u32 rx_ndf)
{
    wait_qspi_hw_idle();
    qspi_set_dma_ctrl(0);

    hwp_apQspi->ssienr = QSPI_SSIENR_EN(0) | QSPI_SSIENR_AHB_EN(0);
    //hwp_apQspi->ctrlr0 = QSPI_CTRLR0_DFS(7) | QSPI_CTRLR0_TMOD(3);
    hwp_apQspi->ctrlr0 = qspi_rx_get_e2prmode(cmd, 8);
    hwp_apQspi->read_cmd =
		QSPI_READCMD_CMD(cmd) | QSPI_READCMD_FLASHM(0) |
		QSPI_READCMD_DIRECT(0) | QSPI_READCMD_AHB_PREFETCH(0);

    qspi_set_rx_fifo_level(rx_thr - 1);
    qspi_set_rx_ndf(rx_ndf - 1);

    qspi_set_tx_fifo_level(128);
    qspi_set_dma_tdlr(dma_tdlr-1);
    qspi_set_dma_ctrl(2);
    asm volatile("isb" : : : "memory");
    qspi_reg_enable();

    return 0;
}

int qspi_dma_write_page(u8 cmd, u32 addr, u8 *tx_dbuf, int tx_len, int dfs)
{
    // if(dma_init == 0) {
    //     dmac_enable();
    //     dma_init = 1;
    // }
    if (false == is_dma_init(0)) {
        dmac_enable(0);
    }

    // prepare data
	if(8 == dfs){
    dma_qspiflash_write_buf[0] = cmd;
    dma_qspiflash_write_buf[1] = (addr >> 16) & 0xFF;
    dma_qspiflash_write_buf[2] = (addr >> 8) & 0xFF;
    dma_qspiflash_write_buf[3] = addr & 0xFF;
	}else if(32 == dfs){
    dma_qspiflash_write_buf[3] = cmd;
    dma_qspiflash_write_buf[2] = (addr >> 16) & 0xFF;
    dma_qspiflash_write_buf[1] = (addr >> 8) & 0xFF;
    dma_qspiflash_write_buf[0] = addr & 0xFF;
	}else{
		printf("DFS = %d is not support, return!\n", dfs);
		return -1;
	}

    if( (0!=tx_len) && (NULL != tx_dbuf) )
        memcpy((dma_qspiflash_write_buf + 4), (char *) tx_dbuf, tx_len);

    if (is_mmu_enable()) {
        v7_dma_clean_range((U32) dma_qspiflash_write_buf,
                           (U32) dma_qspiflash_write_buf + tx_len + 4);
    }

    qspi_dma_write_start(/*cmd, addr, tx_dbuf, tx_len, */ dfs);
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    //dma_qspiflash_write_page_polling(0, QSPI_DMA_CHANNEL, get_qspiflash_dr(), (u32) dma_qspiflash_write_buf,
    dma_qspiflash_write_page_polling(0, QSPI_DMA_CHANNEL, get_qspiflash_dr(), (u32) dma_qspiflash_write_buf,
                                     tx_len + 4, dfs, 1, false); // async write
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    dma_wait_channel_done_polling(0, QSPI_DMA_CHANNEL, 0);

    wait_qspi_hw_idle();
    // close qspi-dma, no tx_req
    qspi_set_dma_ctrl(0);

    return tx_len;
}

int qspi_dma_send_read_cmd(u8 cmd, u32 addr, int rx_thr, int rx_ndf, int dfs)
{
    if (false == is_dma_init(0)) {
        dmac_enable(0);
    }

    // prepare data
    if(8 == dfs){
        dma_qspiflash_write_buf[0] = cmd;
        dma_qspiflash_write_buf[1] = (addr >> 16) & 0xFF;
        dma_qspiflash_write_buf[2] = (addr >> 8) & 0xFF;
        dma_qspiflash_write_buf[3] = addr & 0xFF;
    }else if(32 == dfs){
        dma_qspiflash_write_buf[3] = cmd;
        dma_qspiflash_write_buf[2] = (addr >> 16) & 0xFF;
        dma_qspiflash_write_buf[1] = (addr >> 8) & 0xFF;
        dma_qspiflash_write_buf[0] = addr & 0xFF;
    }else{
        printf("DFS = %d is not support, return!\n", dfs);
        return -1;
    }

    if (is_mmu_enable()) {
        v7_dma_clean_range((U32) dma_qspiflash_write_buf,
                           (U32) dma_qspiflash_write_buf + 4);
    }

    qspi_dma_write_read_cmd_start(cmd, dfs, rx_thr, rx_ndf);
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    //dma_qspiflash_write_page_polling(0, QSPI_DMA_CHANNEL, get_qspiflash_dr(), (u32) dma_qspiflash_write_buf,
    dma_qspiflash_write_page_polling(0, QSPI_DMA_CHANNEL_W, get_qspiflash_dr(), (u32) dma_qspiflash_write_buf,
                                     4, dfs, 1, false); // async write
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    //wait_qspi_hw_idle();
    // close qspi-dma, no tx_req
    //qspi_set_dma_ctrl(0);

    return 4;
}

int qspi_dma_send_cmd_data(u8 cmd, u8 data)
{
    if (false == is_dma_init(0)) {
        dmac_enable(0);
    }

    dma_qspiflash_write_buf[0] = cmd;
    dma_qspiflash_write_buf[1] = (data & 0xFF);

    if (is_mmu_enable()) {
        v7_dma_clean_range((U32) dma_qspiflash_write_buf,
                           (U32) dma_qspiflash_write_buf + 2);
    }

    qspi_dma_write_start(/*cmd, addr, tx_dbuf, tx_len, */ 8);
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    //dma_qspiflash_write_page_polling(0, QSPI_DMA_CHANNEL, get_qspiflash_dr(), (u32) dma_qspiflash_write_buf,
    //                                 2, 8, false); // async write
    dma_qspiflash_write_page_polling(0, QSPI_DMA_CHANNEL, get_qspiflash_dr(), (u32) dma_qspiflash_write_buf,
                                     2, 8, 1, false); // async write
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    dma_wait_channel_done_polling(0, QSPI_DMA_CHANNEL, 0);
    wait_qspi_hw_idle();
    // close qspi-dma, no tx_req
    qspi_set_dma_ctrl(0);

    return 0;
}

int spansion_qspi_dma_send_cmd_data(u8 cmd, u8 *data, u32 dat_len)
{
    int i=0;

    if (false == is_dma_init(0)) {
        dmac_enable(0);
    }

    dma_qspiflash_write_buf[0] = cmd;

    for(i=0; i<dat_len; i++)
    {
        dma_qspiflash_write_buf[i+1] = (data[i] & 0xFF);
    }

    if (is_mmu_enable()) {
        v7_dma_clean_range((U32) dma_qspiflash_write_buf,
                           (U32) dma_qspiflash_write_buf + dat_len + 1);
    }

    qspi_dma_write_start(/*cmd, addr, tx_dbuf, tx_len, */ 8);
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    //dma_qspiflash_write_page_polling(0, QSPI_DMA_CHANNEL, get_qspiflash_dr(), (u32) dma_qspiflash_write_buf,
    //                                 2, 8, false); // async write
    dma_qspiflash_write_page_polling(0, QSPI_DMA_CHANNEL, get_qspiflash_dr(), (u32) dma_qspiflash_write_buf,
                                     1+dat_len, 8, 1, false); // async write
    asm volatile("isb" : : : "memory");
    asm volatile("dsb" : : : "memory");

    dma_wait_channel_done_polling(0, QSPI_DMA_CHANNEL, 0);
    wait_qspi_hw_idle();
    // close qspi-dma, no tx_req
    qspi_set_dma_ctrl(0);

    return 0;
}
#endif

/*
 *  only for read status/device id/readid etc. operations
 */

int qspi_read_xx_data(u8 cmd, u32 addr, int addr_vld, u8 *rx_dbuf, int rx_len)
{
    if (qspi_print_log > 0) {
        printf("cmd = %02x, rx_len = %d\n", cmd, rx_len);
    }
    qspi_rd_dfs = 8;
    wait_qspi_hw_idle();
    qspi_rx_init(SPINOR_OP_READ, 8);
    qspi_set_rx_ndf(rx_len - 1);
    qspi_set_rx_fifo_level(rx_len - 1);

    qspi_xfer_init(rx_dbuf, rx_len, QSPI_RX);
    qspi_reg_enable();

    qspi_rx_fifo_flush();

    if (addr_vld > 0) { // addr is valid
#if 0
        qspi_push_data_8bit(cmd);
        qspi_push_data_8bit((addr >> 16) & 0xff);
        qspi_push_data_8bit((addr >> 8) & 0xff);
        qspi_push_data_8bit(addr & 0xff);
#else
        unsigned int d;
        d = cmd;
        d = d << 24;
        d |= addr;
        qspi_push_data_32bit(d);
#endif
    } else
        qspi_push_data_8bit(cmd);
    qspi_xfer_polling_process(QSPI_RX);
    qspi_xfer_wait_done();
    return rx_len;
}

void qspi_init_low(int irq_enable, int disp_log, int rd_thr_en)
{
    printf("irq mode %s, log = %d, rd_thr_en = %d\n", irq_enable ? "enable" : "disabled", disp_log,
           rd_thr_en);

    qspi_hw_init();
    using_irq_mode = irq_enable;
    qspi_print_log = disp_log;
    qspi_rd_thr_en = rd_thr_en;

    if (irq_enable) {
        request_irq(SYS_IRQ_ID_QSPI, qspi_irq_handler, NULL, (void *) hwp_apQspi);
    }
}

/*
 *  only for spansion asp relative registers read/write operations
 */
int qspi_read_spansion_data(u8 cmd, u32 addr, int addr_vld, u8 *rx_dbuf, int rx_len)
{
	if (qspi_print_log > 0) {
		printf("cmd = %02x, rx_len = %d\n", cmd, rx_len);
	}

    qspi_rd_dfs = 8;
    wait_qspi_hw_idle();
    qspi_rx_init(SPINOR_OP_READ, 8);
    qspi_set_rx_ndf(rx_len - 1);
    qspi_set_rx_fifo_level(rx_len - 1);

    qspi_xfer_init(rx_dbuf, rx_len, QSPI_RX);
    qspi_reg_enable();

    qspi_rx_fifo_flush();

    if (addr_vld > 0) { // addr is valid
        qspi_push_data_8bit(cmd);
        qspi_push_data_8bit((addr >> 24) & 0xff);
        qspi_push_data_8bit((addr >> 16) & 0xff);
        qspi_push_data_8bit((addr >> 8) & 0xff);
        qspi_push_data_8bit(addr & 0xff);
    } else
        qspi_push_data_8bit(cmd);

    qspi_xfer_polling_process(QSPI_RX);
    qspi_xfer_wait_done();
    return rx_len;
}
