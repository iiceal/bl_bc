#ifndef __DRV_DMA_H_
#define __DRV_DMA_H_

#include "regs/dma.h"

#define DMAC_LLI_NODE_MAX (0x8)
#define DMAC_UART_BLK_MAX (HW_DMAC_CH_BLK_MAX)

typedef struct lli_node {
    U32 sar;
    U32 sar_upper;
    U32 dar;
    U32 dar_upper;
    U32 blk_ts;
    U32 resv0;
    U32 llp;
    U32 llp_upper;
    U32 ctl;
    U32 ctl_upper;
    U32 sstat;
    U32 dstat;
    U32 llp_status;
    U32 llp_status_upper;
    U32 resv1;
    U32 resv2;
} LLI_NODE_ST;

typedef enum {
    DMAC_ERR_E_OK = 0,
    DMAC_ERR_E_NOT_INIT = 1,
    DMAC_ERR_E_CH_INVALID = 2,
    DMAC_ERR_E_CH_BUSY = 3,
    DMAC_ERR_E_CH_TIMEOUT = 4,
    DMAC_ERR_E_PARA_INVALID,
    DMAC_ERR_E_MAX
} DMAC_ERR_E;

typedef enum {
    DMA_TRANS_TYPE_E_M2M = 0,
	DMA_TRANS_TYPE_E_QSPIFLASH,
    DMA_TRANS_TYPE_E_OTHER
}DMA_TRANS_TYPE_E;

#define DMA_TIME_COUNT
#ifdef DMA_TIME_COUNT
typedef struct dma_time_count {
    U64 config_s;
    U64 config_e;
    U64 trans_e;
    U32 loop_total;
    U32 config_total;
    U32 trans_total;
    U32 inv_total;
    U32 clean_total;
} DMA_TIME_COUNT_ST;
void get_dma_count_info(U32, U32 ch, DMA_TIME_COUNT_ST * st);
void dma_count_init(U32, U32 ch);
#endif
// function declare
void dma_init(U32);

void dma_deinit(U32);

void dmac_enable(U32);

bool is_dma_init(U32);

U32 dma_get_valid_ch(U32, DMA_TRANS_TYPE_E trans_type);

void dma_wait_ch_done(U32, U32 ch_id, BOOL sync);

void dma_wait_channel_done_polling(U32, U32 ch_id, BOOL sync);

void dma_close_ch(U32, U32 ch_id);

void dma_ch_sar_set(U32, U32 ch_id, U32 src);

void dma_ch_dar_set(U32, U32 ch_id, U32 dst);

int dma_qspiflash_read_polling(U32, U32 ch_id, U32 flash_addr,
                               U32 dst_addr, U32 length, BOOL sync);
//int dma_qspiflash_write_page_polling(U32, U32 ch, U32 flash_addr, U32 src, 
//                                     U32 len, U32 bit_w, BOOL sync);
int dma_qspiflash_write_page_polling(U32 id, U32 ch, U32 flash_addr, U32 src, U32 len, U32 bit_w, U32 dst_burst, BOOL syn_flag);
int dma_m2m_single(U32, U32 ch, U32 src, U32 dst, U32 len, bool sync);
int dma_m2m_single_performance(U32, U32 ch, U32 src, U32 dst, U32 len, bool sync);
int dma_p2m_single(U32 id, U32 ch, U32 src, U32 dst, U32 len, U32 src_width, U32 src_burst, U32 per_type, bool sync);
//int dma_m2p_single(U32, U32 ch, U32 src, U32 dst, U32 len, U32 bit_w, U32 per_type,bool sync);
int dma_m2p_single(U32 id, U32 ch, U32 src, U32 dst, U32 len, U32 bit_w, U32 dst_burst, U32 per_type, bool sync);
void dma_m2m_lli_node_set(U32, U32 ch, U32 src, U32 dst, U32 len, void *node_buff,
                          void *last_node_buff, bool last);
int dma_m2m_lli_start(U32, U32 ch, bool sync);

DMAC_PERIPHERAL_TYPE_E get_uart_tx_per(U32 port);
DMAC_PERIPHERAL_TYPE_E get_uart_rx_per(U32 port);

int dma_ap_single(U32, U32 ch, U32 src, U32 dst, U32 len);
void dma_lli_node_len_set(U32, U32 ch, U32 len);
void dma_ap_lli_node_set(U32, U32 ch, U32 src, U32 dst, void *node_buff, void *last_node_buff,
                          bool last);
int dma_ap_lli_start(U32, U32 ch);
#endif  /*__DRV_DMA_H_*/
