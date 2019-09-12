#include <common.h>
#include "regs/sim.h"
#include "drivers/drv_sim.h"

extern inline void sim_tx_halfempty_inten(HWP_SIM_T *hwp_sim, int en);
extern inline void sim_rx_notempty_inten(HWP_SIM_T *hwp_sim, int en);
extern inline void sim_set_gpcnt_en(HWP_SIM_T *hwp_sim, int en);
extern inline int sim_get_intr_stat(HWP_SIM_T *hwp_sim);
extern inline void sim_clr_rx_not_empty_flag(HWP_SIM_T *hwp_sim);
extern inline void sim_clr_gpcnt_flag(HWP_SIM_T *hwp_sim);
extern inline void sim_clr_tx_halfempty_flag(HWP_SIM_T *hwp_sim);
extern inline u8 sim_rx_byte(HWP_SIM_T *hwp_sim);
extern inline void sim_tx_byte(HWP_SIM_T *hwp_sim, const char c);
extern inline void sim_upper_reset(HWP_SIM_T *hwp_sim);
extern inline int sim_set_etu(HWP_SIM_T *hwp_sim, u32 simclk);
extern int sim_init_controller(HWP_SIM_T *hwp_sim, int etu, int parity);

enum sim_index_e { SIM0 = 0, SIM1, SIM2 };
enum sim_pend_status { SIM_PENDING = 0, SIM_TIMEOUT, SIM_DONE };

typedef struct sim_msg_t {
    u8 *rbuf;
    u32 rlen;
    volatile u32 rx_position;

    u8 *wbuf;
    u32 wlen;
    volatile u32 tx_position;

    u32 timeout;

    u32 flag;
    volatile u32 pending;
} sim_msg_t;

#define SIM_DEBUG_CTRL 0
#if (SIM_DEBUG_CTRL > 0)
#define sim_printf printf
#else
#define sim_printf(fmt, ...)
#endif

static HWP_SIM_T *get_sim_hwp(const u32 index)
{
    HWP_SIM_T *hwp_sim = NULL;
    switch (index) {
    case SIM0:
        hwp_sim = hwp_sim0;
        break;

    case SIM1:
        hwp_sim = hwp_sim1;
        break;

    case SIM2:
        hwp_sim = hwp_sim2;
        break;

    default:
        break;
    }
    return hwp_sim;
}

#define SIM_TOTAL_NUM 3
static struct sim_msg_t sim_priv[SIM_TOTAL_NUM];

static int sim_irq_handle(int irq, void *dev_id)
{
    u32 index = (u32) dev_id;
    HWP_SIM_T *hwp_sim = get_sim_hwp(index);
    int loops = 0;
    int i = 0;
    struct sim_msg_t *chan = &sim_priv[index];

    u32 simirq_stat = sim_get_intr_stat(hwp_sim);
    sim_printf("\nsimirq_stat = %x\n", simirq_stat);

    if (simirq_stat & SIM_INT_STAT_GPCNTI) {
        sim_clr_gpcnt_flag(hwp_sim);

        chan->timeout--;
        if (chan->timeout == 0) {
            sim_printf("\ngpcnt\n");
            chan->pending = SIM_TIMEOUT;
            sim_set_gpcnt_en(hwp_sim, 0);
            sim_tx_halfempty_inten(hwp_sim, 0);
        }
    }

    if (simirq_stat & SIM_INT_STAT_RFNEI) {
        sim_clr_rx_not_empty_flag(hwp_sim);
        chan->rbuf[chan->rx_position] = sim_rx_byte(hwp_sim);
        chan->rx_position++;
        if (chan->rx_position == chan->rlen) {
            sim_set_gpcnt_en(hwp_sim, 0);
            sim_rx_notempty_inten(hwp_sim, 0);
            chan->pending = SIM_DONE;
            sim_printf("\nrlen = 0x%x\n", chan->rlen);
        }
    }

    if (simirq_stat & SIM_INT_STAT_TDHFI) {
        sim_clr_tx_halfempty_flag(hwp_sim);
        sim_printf("\ntx empty = 0x%x\n", simirq_stat);

        loops = (chan->wlen > SIM_XMT_FIFO_HALF_DEPTH) ? SIM_XMT_FIFO_HALF_DEPTH : chan->wlen;
        for (i = 0; i < loops; i++) {
            sim_tx_byte(hwp_sim, chan->wbuf[chan->tx_position++]);
        }
        chan->wlen -= loops;

        if (chan->wlen == 0) {
            sim_tx_halfempty_inten(hwp_sim, 0);
            sim_set_gpcnt_en(hwp_sim, 0);
            chan->pending = SIM_DONE;
        }
    }

    return 0;
}

static int sim_init(u32 index)
{
    int res = 0;
    switch (index) {
    case SIM0:
        request_irq(SYS_IRQ_ID_SIM0, sim_irq_handle, NULL, (void *) index);
        break;

    case SIM1:
        request_irq(SYS_IRQ_ID_SIM1, sim_irq_handle, NULL, (void *) index);
        break;

    case SIM2:
        request_irq(SYS_IRQ_ID_SIM2, sim_irq_handle, NULL, (void *) index);
        break;

    default:
        return (-1);
    }
    return res;
}

static int wait_xfer_complete(u32 index)
{
    struct sim_msg_t *chan = &sim_priv[index];
    while (chan->pending == SIM_PENDING)
        ;
    return 0;
}

int sim_read(u32 index, void *rbuf, u32 *rlen, u32 timeout_ms)
{
    int res = 0;
    struct sim_msg_t *chan = &sim_priv[index];
    HWP_SIM_T *hwp_sim = get_sim_hwp(index);
    if(hwp_sim == NULL)
        return (SIM_RET_FAIL);

    memset(chan, 0, sizeof(struct sim_msg_t));

    chan->rbuf = rbuf;
    chan->rlen = *rlen;
    chan->timeout = (timeout_ms + 9) / 10;
    chan->pending = SIM_PENDING;
    chan->rx_position = 0;
    sim_set_gpcnt_en(hwp_sim, 1);
    sim_rx_notempty_inten(hwp_sim, 1);

    wait_xfer_complete(index);
    if (chan->pending == SIM_TIMEOUT) {
        res = SIM_RET_TIMEOUT;
    }
    *rlen = chan->rx_position;
    if (*rlen == 0) {
        res = SIM_RET_FAIL;
    }
    return res;
}

int sim_write(u32 index, void *wbuf, u32 *wlen, u32 timeout_ms)
{
    int res = 0;
    struct sim_msg_t *chan = &sim_priv[index];
    HWP_SIM_T *hwp_sim = get_sim_hwp(index);
    if(hwp_sim == NULL)
        return (SIM_RET_FAIL);

    memset(chan, 0, sizeof(struct sim_msg_t));

    chan->wbuf = wbuf;
    chan->wlen = *wlen;
    chan->timeout = (timeout_ms + 9) / 10;
    chan->pending = SIM_PENDING;
    chan->tx_position = 0;
    sim_set_gpcnt_en(hwp_sim, 1);
    sim_tx_halfempty_inten(hwp_sim, 1);

    wait_xfer_complete(index);
    *wlen = chan->wlen;
    return res;
}

int sim_atr(u32 index, void *rbuf, u32 *rlen, u32 etu, u32 parity, u32 timeout_ms)
{
    int res = 0;
    u32 len = *rlen;
    HWP_SIM_T *hwp_sim = get_sim_hwp(index);
    if(hwp_sim == NULL)
        return (SIM_RET_FAIL);

    sim_init(index);

    res = sim_init_controller(hwp_sim, etu, parity);
    if (res != 0)
        return res;
    res = sim_read(index, rbuf, &len, timeout_ms);
    if (len > 0) {
        sim_upper_reset(hwp_sim);
        *rlen = len;
        return res;
    }
    sim_upper_reset(hwp_sim);
    res = sim_read(index, rbuf, rlen, timeout_ms);
    return res;
}

int sim_set_clk(u32 index, u32 etu)
{
    int res = 0;
    HWP_SIM_T *hwp_sim = get_sim_hwp(index);
    if(hwp_sim == NULL)
        return (SIM_RET_FAIL);
    res = sim_set_etu(hwp_sim, etu);
    return res;
}
