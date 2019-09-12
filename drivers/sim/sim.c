#include "common.h"
#include "regs/sim.h"

typedef enum {
    SIM_OK = 0,
    SIM_RCV_TIMEOUT = (-1),
    SIM_RSVD = 2,
} SIM_ERROR_STATUS_E;

static u32 baudsel_table[7] = {372, 256, 128, 64, 32, 32, 16};

inline void sim_tx_byte(HWP_SIM_T *hwp_sim, const char c)
{
    hwp_sim->xmt_buf = c;
}

inline u8 sim_rx_byte(HWP_SIM_T *hwp_sim)
{
    return (hwp_sim->rcv_buf);
}

static inline void sim_enable(HWP_SIM_T *hwp_sim)
{
    hwp_sim->enable = (SIM_ENABLE_SIM_EN | SIM_ENABLE_RCV_EN | SIM_ENABLE_XMT_EN);
}

inline void sim_disable(HWP_SIM_T *hwp_sim)
{
    hwp_sim->enable = 0;
}

static inline void sim_reset_controller(HWP_SIM_T *hwp_sim)
{
    hwp_sim->sres = SIM_SRES_SOFT_RES;
}

static inline void sim_lower_reset(HWP_SIM_T *hwp_sim)
{
    hwp_sim->port &= (~SIM_PORT_SRST);
}

inline void sim_upper_reset(HWP_SIM_T *hwp_sim)
{
    hwp_sim->port |= SIM_PORT_SRST;
}

static inline void sim_reset_card(HWP_SIM_T *hwp_sim)
{
    hwp_sim->port |= SIM_PORT_SRST;
    mdelay(10); // need delay
    hwp_sim->port &= (~SIM_PORT_SRST);
}

static inline void sim_auto_poweroff(HWP_SIM_T *hwp_sim)
{
    hwp_sim->port |= SIM_PORT_SAPD; /*sim_clk_out = 0*/
}

inline int sim_set_etu(HWP_SIM_T *hwp_sim, u32 simclk)
{
    int i = 0;
    char flag = 0;

    if (simclk == 0)
        simclk = baudsel_table[0];

    for (i = 0; i < 7; i++) {
        if (baudsel_table[i] == simclk) {
            flag++;
            break;
        }
    }
    if (flag) {
        hwp_sim->cntl &= ~SIM_CNTL_BAUD_SEL_MASK;
        hwp_sim->cntl |= ((i << 4) & 0x70);
    } else {
        return (-1);
    }
    return 0;
}

static inline void sim_set_protect_etu(HWP_SIM_T *hwp_sim)
{
    hwp_sim->guard = SIM_GUARD_PROT_2ETU; // default 2 etu  protect cycle
}

static inline void sim_gpcnt_inten(HWP_SIM_T *hwp_sim, int en)
{
    if (en > 0)
        hwp_sim->int_enable |= SIM_INT_ENALBE_GPCNTE;
    else
        hwp_sim->int_enable &= (~SIM_INT_ENALBE_GPCNTE);
}

inline void sim_clr_gpcnt_flag(HWP_SIM_T *hwp_sim)
{
    hwp_sim->int_stat = SIM_INT_ENALBE_GPCNTE;
}

inline void sim_tx_halfempty_inten(HWP_SIM_T *hwp_sim, int en)
{
    if (en > 0)
        hwp_sim->int_enable |= SIM_INT_ENALBE_TDHFE;
    else
        hwp_sim->int_enable &= (~SIM_INT_ENALBE_TDHFE);
}

inline void sim_clr_tx_halfempty_flag(HWP_SIM_T *hwp_sim)
{
    hwp_sim->int_stat = SIM_INT_ENALBE_TDHFE;
}

static inline void sim_tx_notfull_inten(HWP_SIM_T *hwp_sim, int en)
{
    if (en > 0)
        hwp_sim->int_enable |= SIM_INT_ENALBE_TDNFE;
    else
        hwp_sim->int_enable &= (~SIM_INT_ENALBE_TDNFE);
}

static inline void sim_clr_tx_not_full_flag(HWP_SIM_T *hwp_sim)
{
    hwp_sim->int_stat = SIM_INT_ENALBE_TDNFE;
}

inline void sim_rx_notempty_inten(HWP_SIM_T *hwp_sim, int en)
{
    if (en > 0)
        hwp_sim->int_enable |= SIM_INT_ENALBE_RFNEE;
    else
        hwp_sim->int_enable &= (~SIM_INT_ENALBE_RFNEE);
}

inline void sim_clr_rx_not_empty_flag(HWP_SIM_T *hwp_sim)
{
    hwp_sim->int_stat = SIM_INT_ENALBE_RFNEE;
}

static inline void sim_clr_intr(HWP_SIM_T *hwp_sim)
{
    hwp_sim->int_stat = 0xfff;
}

inline int sim_get_intr_stat(HWP_SIM_T *hwp_sim)
{
    int irq_status = hwp_sim->int_stat;
    // sim_clr_intr(hwp_sim);
    return irq_status;
}

static inline int sim_polling_raw_stat(HWP_SIM_T *hwp_sim)
{
    int irq_status = hwp_sim->int_raw_stat;
    sim_clr_intr(hwp_sim);
    return irq_status;
}

static inline void sim_output_clk(HWP_SIM_T *hwp_sim)
{
    hwp_sim->port |= SIM_PORT_SCEN;
}

static inline void sim_set_parity_en(HWP_SIM_T *hwp_sim, int en)
{
    if (en > 0)
        hwp_sim->port |= SIM_PORT_PE;
    else
        hwp_sim->port &= (~SIM_PORT_PE);
}

inline void sim_set_gpcnt_en(HWP_SIM_T *hwp_sim, int en)
{
    if (en > 0)
        hwp_sim->cntl |= SIM_CNTL_GPEN;
    else
        hwp_sim->cntl &= (~SIM_CNTL_GPEN);
}

static inline void sim_set_gpcnt(HWP_SIM_T *hwp_sim)
{
    hwp_sim->gpcnt = (DEFAULT_GP_CNT & 0xFFFF);
}

// init controllor
int sim_init_controller(HWP_SIM_T *hwp_sim, int etu, int parity)
{
    int res;
    sim_reset_controller(hwp_sim);
    mdelay(10);
    sim_disable(hwp_sim);
    sim_set_gpcnt(hwp_sim);
    sim_gpcnt_inten(hwp_sim, 1);
    sim_lower_reset(hwp_sim);
    sim_output_clk(hwp_sim);
    sim_set_parity_en(hwp_sim, parity);
    res = sim_set_etu(hwp_sim, etu);
    sim_set_protect_etu(hwp_sim);
    sim_enable(hwp_sim);
    return res;
}

