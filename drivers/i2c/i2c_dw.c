#include <common.h>
#include <drivers/drv_i2c.h>
//#define TIMEOUT 20 /* ms */

#define readl(addr) ((volatile u32)(addr))
#define writel(val, addr) ((addr) = (val))

static unsigned char rdbuf[1024];

void print_i2c_regs(HWP_I2C_AP_T *hwp_i2c)
{
    printf("\nDW_IC_CON\t= 0x%x\n", (hwp_i2c->dw_ic_con));
    printf("DW_IC_TAR\t= 0x%x\n", (hwp_i2c->dw_ic_tar));
    printf("DW_IC_INTR_MASK\t= 0x%x\n", (hwp_i2c->dw_ic_intr_mask));
    printf("DW_IC_RAW_INTR_STAT = 0x%x\n", (hwp_i2c->dw_ic_raw_intr_stat));
    printf("DW_IC_INTR_STAT\t= 0x%x\n", (hwp_i2c->dw_ic_intr_stat));
    printf("DW_IC_ENABLE\t= 0x%x\n", (hwp_i2c->dw_ic_enable));
    printf("DW_IC_STATUS\t= 0x%x\n", (hwp_i2c->dw_ic_status));
    printf("DW_IC_TX_ABRT_SOURCE = 0x%x\n", (hwp_i2c->dw_ic_tx_abrt_source));
}

int dw_i2c_send_bytes(HWP_I2C_AP_T *hwp_i2c, unsigned char *out_buf, unsigned int len)
{
    int res = 0;
    unsigned int i = 1;
    writel(out_buf[0], hwp_i2c->dw_ic_data_cmd);
    while ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_START_DET) != DW_IC_INTR_START_DET)
        ;

    while (i < len && (readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_STOP_DET) != DW_IC_INTR_STOP_DET) {
        if ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_TX_ABRT) == DW_IC_INTR_TX_ABRT) {
            printf("write abrt = 0x%x\n", (hwp_i2c->dw_ic_tx_abrt_source));
            res = (-1);
            break;
        }
        if ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_TX_EMPTY) == DW_IC_INTR_TX_EMPTY) {
            writel(out_buf[i], hwp_i2c->dw_ic_data_cmd);
            i++;
        }
    }
    while ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_STOP_DET) != DW_IC_INTR_STOP_DET)
        ;
    // printf("write len is %d \n", i);
    print_i2c_regs(hwp_i2c);

    readl(hwp_i2c->dw_ic_clr_intr);
    readl(hwp_i2c->dw_ic_clr_rx_under);
    readl(hwp_i2c->dw_ic_clr_rx_over);
    readl(hwp_i2c->dw_ic_clr_tx_over);
    readl(hwp_i2c->dw_ic_clr_rd_req);
    readl(hwp_i2c->dw_ic_clr_tx_abrt);
    readl(hwp_i2c->dw_ic_clr_rx_done);
    readl(hwp_i2c->dw_ic_clr_activity);
    readl(hwp_i2c->dw_ic_clr_stop_det);
    readl(hwp_i2c->dw_ic_clr_start_det);
    readl(hwp_i2c->dw_ic_clr_gen_call);

    readl(hwp_i2c->dw_ic_clr_restart_det);
    readl(hwp_i2c->dw_ic_clr_scl_stuck_det);
    readl(hwp_i2c->dw_ic_clr_smbus_intr);

    return res;
}

int dw_i2c_read(HWP_I2C_AP_T *hwp_i2c, unsigned char *rbuf, unsigned int rlen)
{
    int res = 0;
    unsigned int i, j;

    writel(0x100, hwp_i2c->dw_ic_data_cmd);
    i = 0;
    j = 1;
    while (i < rlen && (readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_STOP_DET) != DW_IC_INTR_STOP_DET) {
        if (j < rlen && (readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_TX_EMPTY) == DW_IC_INTR_TX_EMPTY) {
            writel(0x100, hwp_i2c->dw_ic_data_cmd);
            j++;
        }

        if ((readl(hwp_i2c->dw_ic_status) & DW_IC_STATUS_RFNE) == DW_IC_STATUS_RFNE) // 1: Receive FIFO is not empty
            rbuf[i++] = (unsigned char) readl(hwp_i2c->dw_ic_data_cmd);
        if ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_TX_ABRT) == DW_IC_INTR_TX_ABRT) {
            printf("read abrt = 0x%x\n", (hwp_i2c->dw_ic_tx_abrt_source));
            res = (-1);
            break;
        }
    }

    while ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_STOP_DET) != DW_IC_INTR_STOP_DET)
        ;

    printf("\nread length is = %d \n", j);
    for (i = 0; i < j; i++) {
        printf("%02x ", rbuf[i]);
    }
    printf("\n");
    print_i2c_regs(hwp_i2c);

    readl(hwp_i2c->dw_ic_clr_intr);
    readl(hwp_i2c->dw_ic_clr_rx_under);
    readl(hwp_i2c->dw_ic_clr_rx_over);
    readl(hwp_i2c->dw_ic_clr_tx_over);
    readl(hwp_i2c->dw_ic_clr_rd_req);
    readl(hwp_i2c->dw_ic_clr_tx_abrt);
    readl(hwp_i2c->dw_ic_clr_rx_done);
    readl(hwp_i2c->dw_ic_clr_activity);
    readl(hwp_i2c->dw_ic_clr_stop_det);
    readl(hwp_i2c->dw_ic_clr_start_det);
    readl(hwp_i2c->dw_ic_clr_gen_call);

    readl(hwp_i2c->dw_ic_clr_restart_det);
    readl(hwp_i2c->dw_ic_clr_scl_stuck_det);
    readl(hwp_i2c->dw_ic_clr_smbus_intr);

    return res;
}

int dw_i2c_smbus_read(HWP_I2C_AP_T *hwp_i2c, unsigned char *out_buf, unsigned int wlen, unsigned char *rbuf,
                       unsigned int rlen)
{
    int res = 0;
    unsigned int i = 1, j = 0;

    writel(out_buf[0], hwp_i2c->dw_ic_data_cmd);

    while ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_START_DET) != DW_IC_INTR_START_DET)
        ;

    while (i < wlen) {
        if ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_TX_EMPTY) == DW_IC_INTR_TX_EMPTY) {
            writel(out_buf[i], hwp_i2c->dw_ic_data_cmd);
            i++;
        }
        if ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_TX_ABRT) == DW_IC_INTR_TX_ABRT) {
            printf("smbus abrt = 0x%x\n", (hwp_i2c->dw_ic_tx_abrt_source));
            res = (-1);
            break;
        }
    }

    writel(0x100, hwp_i2c->dw_ic_data_cmd);
    i = 0;
    j = 1;
    while (i < rlen && (readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_STOP_DET) != DW_IC_INTR_STOP_DET) {
        if (j < rlen && (readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_TX_EMPTY) == DW_IC_INTR_TX_EMPTY) {
            writel(0x100, hwp_i2c->dw_ic_data_cmd);
            j++;
        }

        if ((readl(hwp_i2c->dw_ic_status) & DW_IC_STATUS_RFNE) == DW_IC_STATUS_RFNE) // 1: Receive FIFO is not empty
            rbuf[i++] = (unsigned char) readl(hwp_i2c->dw_ic_data_cmd);

        if ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_TX_ABRT) == DW_IC_INTR_TX_ABRT) {
            printf("\nsmbus txabrt\n");
            res = (-1);
            break;
        }
    }

    while ((readl(hwp_i2c->dw_ic_raw_intr_stat) & DW_IC_INTR_STOP_DET) != DW_IC_INTR_STOP_DET)
        ;

    printf("\nsmbus read length is = %d \n", j);
    for (i = 0; i < j; i++) {
        printf("%02x ", rbuf[i]);
    }
    printf("\n");
    print_i2c_regs(hwp_i2c);

    readl(hwp_i2c->dw_ic_clr_intr);
    readl(hwp_i2c->dw_ic_clr_rx_under);
    readl(hwp_i2c->dw_ic_clr_rx_over);
    readl(hwp_i2c->dw_ic_clr_tx_over);
    readl(hwp_i2c->dw_ic_clr_rd_req);
    readl(hwp_i2c->dw_ic_clr_tx_abrt);
    readl(hwp_i2c->dw_ic_clr_rx_done);
    readl(hwp_i2c->dw_ic_clr_activity);
    readl(hwp_i2c->dw_ic_clr_stop_det);
    readl(hwp_i2c->dw_ic_clr_start_det);
    readl(hwp_i2c->dw_ic_clr_gen_call);

    readl(hwp_i2c->dw_ic_clr_restart_det);
    readl(hwp_i2c->dw_ic_clr_scl_stuck_det);
    readl(hwp_i2c->dw_ic_clr_smbus_intr);
    return res;
}

void dw_i2c_master_init(HWP_I2C_AP_T *hwp_i2c, unsigned char slv_addr, const unsigned int mode)
{
    unsigned int ic_con = mode;
    writel(0, hwp_i2c->dw_ic_enable);

    ic_con |= DW_IC_CON_MASTER | DW_IC_CON_SLAVE_DISABLE | DW_IC_CON_RESTART_EN;

    //writel(0x28, hwp_i2c->dw_ic_ss_scl_hcnt);
    //writel(0x2f, hwp_i2c->dw_ic_ss_scl_lcnt);
    //writel(0x6, hwp_i2c->dw_ic_fs_scl_hcnt);
    //writel(0xd, hwp_i2c->dw_ic_fs_scl_lcnt);
    writel(ic_con, hwp_i2c->dw_ic_con);

    writel(slv_addr, hwp_i2c->dw_ic_tar);

    writel(0, hwp_i2c->dw_ic_intr_mask);
    writel(0, hwp_i2c->dw_ic_rx_tl);
    writel(6, hwp_i2c->dw_ic_tx_tl);

    readl(hwp_i2c->dw_ic_clr_intr);
    readl(hwp_i2c->dw_ic_clr_rx_under);
    readl(hwp_i2c->dw_ic_clr_rx_over);
    readl(hwp_i2c->dw_ic_clr_tx_over);
    readl(hwp_i2c->dw_ic_clr_rd_req);
    readl(hwp_i2c->dw_ic_clr_tx_abrt);
    readl(hwp_i2c->dw_ic_clr_rx_done);
    readl(hwp_i2c->dw_ic_clr_activity);
    readl(hwp_i2c->dw_ic_clr_stop_det);
    readl(hwp_i2c->dw_ic_clr_start_det);
    readl(hwp_i2c->dw_ic_clr_gen_call);

    readl(hwp_i2c->dw_ic_clr_restart_det);
    readl(hwp_i2c->dw_ic_clr_scl_stuck_det);
    readl(hwp_i2c->dw_ic_clr_smbus_intr);

    writel(1, hwp_i2c->dw_ic_enable);
    print_i2c_regs(hwp_i2c);
}

static u32 i2c_dw_read_clear_intrbits(HWP_I2C_AP_T *hwp_i2c, u32 *abort_source)
{
    u32 stat;
    /*
     * The IC_INTR_STAT register just indicates "enabled" interrupts.
     * Ths unmasked raw version of interrupt status bits are available
     * in the IC_RAW_INTR_STAT register.
     *
     * That is,
     *   stat = dw_readl(IC_INTR_STAT);
     * equals to,
     *   stat = dw_readl(IC_RAW_INTR_STAT) & dw_readl(IC_INTR_MASK);
     *
     * The raw version might be useful for debugging purposes.
     */
    stat = readl(hwp_i2c->dw_ic_intr_stat);

    /*
     * Do not use the IC_CLR_INTR register to clear interrupts, or
     * you'll miss some interrupts, triggered during the period from
     * dw_readl(IC_INTR_STAT) to dw_readl(IC_CLR_INTR).
     *
     * Instead, use the separately-prepared IC_CLR_* registers.
     */
    if (stat & DW_IC_INTR_RX_UNDER)
        readl(hwp_i2c->dw_ic_clr_rx_under);
    if (stat & DW_IC_INTR_RX_OVER)
        readl(hwp_i2c->dw_ic_clr_rx_over);
    if (stat & DW_IC_INTR_TX_OVER)
        readl(hwp_i2c->dw_ic_clr_tx_over);
    if (stat & DW_IC_INTR_RD_REQ)
        readl(hwp_i2c->dw_ic_clr_rd_req);
    if (stat & DW_IC_INTR_TX_ABRT) {
        /*
         * The IC_TX_ABRT_SOURCE register is cleared whenever
         * the IC_CLR_TX_ABRT is read.  Preserve it beforehand.
         */
        *abort_source = readl(hwp_i2c->dw_ic_tx_abrt_source);
        readl(hwp_i2c->dw_ic_clr_tx_abrt);
    }
    if (stat & DW_IC_INTR_RX_DONE)
        readl(hwp_i2c->dw_ic_clr_rx_done);
    if (stat & DW_IC_INTR_ACTIVITY)
        readl(hwp_i2c->dw_ic_clr_activity);
    if (stat & DW_IC_INTR_STOP_DET)
        readl(hwp_i2c->dw_ic_clr_stop_det);
    if (stat & DW_IC_INTR_START_DET)
        readl(hwp_i2c->dw_ic_clr_start_det);
    if (stat & DW_IC_INTR_GEN_CALL)
        readl(hwp_i2c->dw_ic_clr_gen_call);

    return stat;
}

#define I2C_DW_RX 0
#define I2C_DW_TX 1
static volatile u32 i2c_irq_tx_finish_pending = 0;
static int irq_log_print = 0;

static int using_irq_mode = 1;
static u8 *i2c_rq_dbuf = NULL;
static int i2c_rq_dlen = 0;
static int i2c_tx_len_latch = 0, i2c_rx_len_latch = 0;
static volatile int i2c_rw_len = 0;
static volatile int i2c_dw_rd_flag = I2C_DW_TX; // 0:write, 1:read

static volatile u8 i2c_dw_log[256] = {0};
static volatile int i2c_dw_log_p = 0;

static void i2c_dw_xfer_init(HWP_I2C_AP_T *hwp_i2c, u8 *dbuf, int dlen, int flags)
{
    i2c_tx_len_latch = dlen;
    i2c_rx_len_latch = dlen;

    i2c_irq_tx_finish_pending = 0;
    i2c_rq_dlen = dlen;
    i2c_rq_dbuf = dbuf;
    i2c_rw_len = 0;
    i2c_dw_log_p = 0;

    if (using_irq_mode > 0) {
        if (flags == I2C_DW_RX)
            writel(DW_IC_INTR_DEFAULT_MASK, hwp_i2c->dw_ic_intr_mask);

        if (flags == I2C_DW_TX)
            writel(DW_IC_INTR_DEFAULT_MASK, hwp_i2c->dw_ic_intr_mask);
    }
}

void i2c_dw_xfer_tx_process(HWP_I2C_AP_T *hwp_i2c)
{
#define BIT(nr) (1 << (nr))
    u32 cmd = 0;
    int tx_limit;
    int rx_limit;

    tx_limit = 8 - readl(hwp_i2c->dw_ic_txflr);
    rx_limit = 8 - readl(hwp_i2c->dw_ic_rxflr);

    i2c_dw_log[i2c_dw_log_p++] = tx_limit;

    while (i2c_rq_dlen > 0 && tx_limit > 0 && rx_limit > 0) {
        if (i2c_dw_rd_flag == I2C_DW_RX) {
            cmd |= BIT(8);	//1:read 0:write
            writel(cmd, hwp_i2c->dw_ic_data_cmd);
            rx_limit--;
        } else {
            if (i2c_rq_dlen == 1)
                cmd |= BIT(9);
            writel((cmd | i2c_rq_dbuf[i2c_rw_len++]), hwp_i2c->dw_ic_data_cmd);
        }

        tx_limit--;
        i2c_rq_dlen--;
    }

    if (i2c_rq_dlen == 0) {
        hwp_i2c->dw_ic_intr_mask &= ~DW_IC_INTR_TX_EMPTY;
        // printf("\ntxpend = %d\n", i2c_irq_tx_finish_pending);
    }
}

int i2c_dw_xfer_msg(HWP_I2C_AP_T *hwp_i2c, unsigned char *out_buf, unsigned int len)
{
    i2c_dw_rd_flag = I2C_DW_TX; // 0:write, 1:read
    // int i = 0;
    writel(6, hwp_i2c->dw_ic_tx_tl);
    i2c_dw_xfer_init(hwp_i2c, (u8 *) out_buf, (int) len, I2C_DW_TX);
    while (i2c_rw_len < len)
        ;
#if 0
    for( i =0; i< i2c_dw_log_p;i++){
        printf("\nlog[%d] = %02x ",i, i2c_dw_log[i]);
    }
#endif
    return i2c_rw_len;
}

void i2c_dw_xfer_rx_process(HWP_I2C_AP_T *hwp_i2c)
{
    int rx_count = 0, i;

    rx_count = readl(hwp_i2c->dw_ic_rxflr);

    for (i = 0; i < rx_count; i++) {
        rdbuf[i2c_rw_len++] = readl(hwp_i2c->dw_ic_data_cmd);
    }

    if (i2c_rw_len == i2c_rx_len_latch) {
        printf("\nrxlen\n");
    }
}

int i2c_dw_read(HWP_I2C_AP_T *hwp_i2c, unsigned char *out_buf, unsigned int len)
{
    int i = 0;
    i2c_dw_rd_flag = I2C_DW_RX; // 0:write, 1:read
    i2c_rx_len_latch = len;
    i2c_rw_len = 0;
    writel(0, hwp_i2c->dw_ic_rx_tl);
    i2c_dw_xfer_init(hwp_i2c, (u8 *) out_buf, (int) len, I2C_DW_RX);

    while (i2c_rw_len < len)
        ;
    for (i = 0; i < i2c_dw_log_p; i++) {
        printf("\nlog[%d] = %02x ", i, i2c_dw_log[i]);
    }

    printf("\n");
    for (i = 0; i < i2c_rw_len; i++) {
        printf("%02x ", rdbuf[i]);
    }

    return i2c_rw_len;
}

int i2c_irq_handler(int irq, void *dev_id)
{
    HWP_I2C_AP_T *hwp_i2c = (HWP_I2C_AP_T *) dev_id;
    u32 abrt_source = 0;
    u32 irq_cause = i2c_dw_read_clear_intrbits(hwp_i2c, &abrt_source);
    if (irq_cause & DW_IC_INTR_TX_ABRT) {
        goto tx_aborted;
    }

    if (irq_cause & DW_IC_INTR_RX_FULL) {
        readl(hwp_i2c->dw_ic_clr_intr);
        i2c_dw_xfer_rx_process(hwp_i2c);
    }
    if (irq_cause & DW_IC_INTR_TX_EMPTY) {
        readl(hwp_i2c->dw_ic_clr_intr);
        i2c_dw_xfer_tx_process(hwp_i2c);
        i2c_irq_tx_finish_pending++;
    }

    if (irq_log_print > 0)
        printf("i2c_irq_handler: irq = %d, irq_cause = 0x%x\n", irq, irq_cause);

tx_aborted:
    if (irq_cause & (DW_IC_INTR_TX_ABRT | DW_IC_INTR_STOP_DET)) {
        hwp_i2c->dw_ic_intr_mask &= ~DW_IC_INTR_TX_EMPTY;
    }

    return 0;
}

enum i2c_index {
    I2C0 = 0,
    I2C1 = 1,
};

HWP_I2C_AP_T *get_i2c_hwp(u32 index)
{
    HWP_I2C_AP_T *hwp_i2c = NULL;

    switch (index) {
        case I2C0:
            hwp_i2c = hwp_apI2c0;
            break;
        case I2C1:
            hwp_i2c = hwp_apI2c1;
            break;
        default:
            break;
    }
    return hwp_i2c;
}
