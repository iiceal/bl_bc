#ifndef QSPI_HW_H__
#define QSPI_HW_H__

enum qspi_sr_status {
    QSPI_SR_RX_FULL,
    QSPI_SR_TX_EMPTY,
    QSPI_SR_TX_OVERFLOW,
    QSPI_SR_RX_UNDERFLOW,
    QSPI_SR_RX_OVERFLOW,
    QSPI_SR_NONE
};

inline void qspi_hw_init(void);
inline void qspi_ahb_enable(void);
inline void qspi_reg_enable(void);
inline void qspi_push_data_8bit(u8 d);
inline void qspi_push_data_16bit(u16 d);
inline void qspi_push_data_32bit(u32 d);
inline u8 qspi_get_data_8bit(void);
inline u16 qspi_get_data_16bit(void);
inline u32 qspi_get_data_32bit(void);
inline void qspi_set_rx_fifo_level(int rft);
inline void qspi_set_tx_fifo_level(int tft);
inline int qspi_get_rx_fifo_d_num(void);
inline int qspi_get_tx_fifo_d_num(void);
inline int qspi_cache_flash(void);
inline void qspi_set_baudr(u32 clk_div);
inline u32 qspi_clr_all_interrupts(void);
inline u32 qspi_get_interrupt_status(void);
inline void qspi_rx_full_inten(int en);
inline void qspi_tx_empty_inten(int en);
inline void qspi_set_rx_ndf(u32 ndf);
inline bool qspi_is_busy(void);
inline void qspi_tx_init(void);
inline bool qspi_rx_init(u8 cmd, u8 dfs);
int qspi_polling_raw_status(void);
int qspi_irq_read_status(void);

inline u32 qspi_rx_fifo_flush(void);
inline void qspi_set_dma_ctrl(u32 en);
inline void qspi_set_dma_tdlr(u32 tdlr);
inline void qspi_set_dma_rdlr(u32 rdlr);

inline u32 qspi_rx_get_e2prmode(u8 cmd, u8 dfs);
#endif
