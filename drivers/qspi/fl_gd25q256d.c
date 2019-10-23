#include <common.h>
#include "drivers/drv_qspi.h"
#include "fl_common_cmd.h"

#define     SPINOR_OP_BE_64k                0xd8
#define     SPINOR_OP_BE_32k                0x52
#define     SPINOR_OP_BE_SECTOR             0x20
#define     SPINOR_OP_CHIP_ERASE            0xc7
#define     SPINOR_OP_BE_MAX_MS             (2000)
#define     SPINOR_OP_CE_MAX_MS             (400 * 1000)

#define     SPINOR_OP_WR_EXTADDR            0xc5
#define     SPINOR_OP_RD_EXTADDR            0xc8
#define     SPINOR_OP_EXTR_A24              0x01

#define 	SPINOR_OP_CLRR		            0x30
#define     SPINOR_OP_RDSR1                 0x05
#define     SPINOR_OP_RDSR2                 0x35
#define     SPINOR_OP_RDSR3                 0x15
#define     SR1_BP_BITS_MASK                0x3c
#define     SR1_BP_BITS_SHIFT(bp)           (bp << 2)

#define     SPINOR_OP_WRSR1                 0x01
#define     SPINOR_OP_WRSR2                 0x31
#define     SPINOR_OP_WRSR3                 0x11
#define     SR2_QUAD_EN                     0x02
#define     SR3_P_ERR_GD		            0x04	
#define     SR3_E_ERR_GD		            0x08	

extern inline int qspi_wren(u8 cmd);
extern void qspi_ahb_read_enable(u8 rd_cmd, u8 dfs);
extern void qspi_ahb_write_enable(u8 wren_cmd);
extern int qspi_send_xx_data(u8 *tx_dbuf, int tx_len);
extern int qspi_read_xx_data(u8 cmd, u32 addr, int addr_vld, u8 *rx_dbuf, int rx_len);
extern int qspi_read_page_data(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len, int dfs);
extern int qspi_dma_send_cmd_data(u8 cmd, u8 data);
extern int qspi_dma_write_page(u8 cmd, u32 addr, u8 *tx_dbuf, int tx_len, int dfs);
extern inline void qspi_hw_init(void);
u8 qspi_fl_read_status(u8 cmd);
u8 qspi_gd25q256d_read_status(void);
extern int qspi_fl_wait_wel(void);

static u8 wbuf_s[280];
static int qspi_gd25q256d_write_extend_addr(u8 cmd, u8 data)
{
    int ret;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }
    ret = qspi_fl_wait_wel();
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }
#ifdef CONFIG_QSPI_DMA_TRANSFER
    ret = qspi_dma_send_cmd_data(cmd, data);
#else
    u8 wbuf[2];
    wbuf[0] = cmd;
    wbuf[1] = data;
    ret = qspi_send_xx_data(wbuf, 2);
#endif

    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }else{
        return QSPI_OP_SUCCESS;
    }
}

/*not necessary to set WEL bit, before the clear SR command is executed*/
/* for clear erase Fail Flag and Program Fail Flag*/
static int qspi_gd25q256d_clear_status(u8 clrr_cmd)
{
    int ret;
    ret = qspi_send_xx_data(&clrr_cmd, 1);
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }else{
        return QSPI_OP_SUCCESS;
    }
}

/*
 * only for program/erase/write status
 */
#define WAIT_TIME_OUT   (2500000)
static u8 wait_flash_idle_extend(u8 *r_st3)
{
    u8 st1 = 0, st3 = 0;
    int timeout=0;

    while (1) {
        st1 = qspi_fl_read_status(SPINOR_OP_RDSR1);
        st3 = qspi_fl_read_status(SPINOR_OP_RDSR3);
        if ((st3 & SR3_E_ERR_GD) == SR3_E_ERR_GD)
            break;
        if ((st3 & SR3_P_ERR_GD) == SR3_P_ERR_GD)
            break;
        if ((st1 & SR_WIP) == 0)
            break;

        udelay(1);
        timeout++;
        if(timeout > WAIT_TIME_OUT)
            break;
    }
    *r_st3 = st3;
    return st1;
}

/**/
static int qspi_gd25q256d_clear_status_for_err_bits(void)
{
    int ret = QSPI_OP_SUCCESS;

    //wip = 1, p_err =1
    ret = qspi_gd25q256d_clear_status(SPINOR_OP_CLRR);
    if(ret < 0)
        return QSPI_OP_FAILED;

    printf("clear and status , st1 0x%x, st3 0x%x\n", 
            qspi_fl_read_status(SPINOR_OP_RDSR1),
            qspi_fl_read_status(SPINOR_OP_RDSR3));
    //wip = 1, p_err = 0;        
    ret = qspi_wren(SPINOR_OP_WRDI);
    //wip = 0, p_err = 0;
    printf("wren disable, status	  :0x%x\n",
            qspi_fl_read_status(SPINOR_OP_RDSR1));

    return ret;
}

static int qspi_gd25q256d_write_status(u8 cmd, u8 sr_x)
{
    int ret = 0;
    u8 status, st3 = 0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }
    ret=qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

#ifdef CONFIG_QSPI_DMA_TRANSFER
    ret = qspi_dma_send_cmd_data(cmd,sr_x);
#else
    u8 wbuf[3];
    wbuf[0] = cmd;
    wbuf[1] = sr_x;
    ret = qspi_send_xx_data(wbuf, 2);
#endif
	if(ret < 0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_P_ERR_GD)) { //both these bits all config to 1
        printf("write status failed, status 0x%x, st3 0x%x\n", status, st3);
        qspi_gd25q256d_clear_status_for_err_bits();
        status |= SR_WIP;  //maybe not need this code.
    }
    return status;
}

static int qspi_gd25q256d_4byte_extend_enable(u32 addr)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    int ret=QSPI_OP_SUCCESS;

    u8 ext_data = 0, ext_rd;

    ext_rd = qspi_fl_read_status(SPINOR_OP_RD_EXTADDR);
    if ((addr & (1 << 24)) > 0)
        ext_data |= SPINOR_OP_EXTR_A24;

    if (ext_rd != ext_data) {
#ifdef CONFIG_QSPI_TRACE
        s = timer_get_tick();
#endif
        ret = qspi_gd25q256d_write_extend_addr(SPINOR_OP_WR_EXTADDR, ext_data);

#ifdef CONFIG_QSPI_TRACE
        e = timer_get_tick();
        us = tick_to_us((unsigned long) (e - s));
#endif

        qspi_debug("extend addr register      : 0x%x, wr: 0x%x\n",
               qspi_fl_read_status(SPINOR_OP_RD_EXTADDR), ext_data);
        qspi_debug("address_extend_enable need: %d us (%d ms)\n\n", us, us / 1000);
    }

    return ret;
}

static int qspi_gd25q256d_4byte_extend_disable(u32 addr)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 ext_d = 0;
    int ret=QSPI_OP_SUCCESS;

    ext_d = qspi_fl_read_status(SPINOR_OP_RD_EXTADDR);

    if (ext_d != 0) {
#ifdef CONFIG_QSPI_TRACE
        s = timer_get_tick();
#endif
        ret = qspi_gd25q256d_write_extend_addr(SPINOR_OP_WR_EXTADDR, 0);

#ifdef CONFIG_QSPI_TRACE
        e = timer_get_tick();
        us = tick_to_us((unsigned long) (e - s));
#endif

        qspi_debug("extend addr register       : 0x%x\n", 
                qspi_fl_read_status(SPINOR_OP_RD_EXTADDR));

        qspi_debug("address_extend_disable need: %d us (%d ms)\n\n", us, us / 1000);
    }

    return ret;
}

static int qspi_gd25q256d_quad_enable(void)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 st;
    int ret = QSPI_OP_SUCCESS;

    qspi_debug("quad enable \n");

#ifdef CONFIG_QSPI_TRACE
    s = timer_get_tick();
#endif

    st = qspi_fl_read_status(SPINOR_OP_RDSR2);
    if ((st & SR2_QUAD_EN) == SR2_QUAD_EN) {
        qspi_debug("quad bit had enabled\n");
        return ret;
    }

    st = qspi_gd25q256d_write_status(SPINOR_OP_WRSR2, st | SR2_QUAD_EN);
#ifdef CONFIG_QSPI_TRACE
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
#endif

    if ((st & SR_WIP) == SR_WIP) {
        ret = QSPI_OP_FAILED;
    }
    if(st < 0)
        ret = QSPI_OP_FAILED;

    qspi_debug("status 2 register   : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR2));
    qspi_debug("quad_enable need    : %d us (%d ms)\n\n", us, us / 1000);

    return ret;
}

int qspi_gd25q256d_quad_disable(void)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 st;
    int ret = 0;

    qspi_debug("quad disable \n");

#ifdef CONFIG_QSPI_TRACE
    s = timer_get_tick();
#endif

    st = qspi_fl_read_status(SPINOR_OP_RDSR2);
    if ((st & SR2_QUAD_EN) == 0) {
        qspi_debug("quad bit had disabled\n");
        return ret;
    }

    st &= ~(SR2_QUAD_EN);
    st = qspi_gd25q256d_write_status(SPINOR_OP_WRSR2, st);
#ifdef CONFIG_QSPI_TRACE
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
#endif

    if ((st & SR_WIP) == SR_WIP) {
        ret = QSPI_OP_FAILED;
    }
    if(st < 0)
        ret = QSPI_OP_FAILED;

    qspi_debug("status 2 register   : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR2));
    qspi_debug("quad_disable need   :%d us (%d ms)\n\n", us, us / 1000);

    return ret;
}

static int qspi_gd25q256d_prot_region_enable(int region)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 rdsr1 = 0, st;
    int ret = 0;

    if (region > 0xf)
        return QSPI_OP_FAILED;

    qspi_debug("\nprot enable \n");

#ifdef CONFIG_QSPI_TRACE
    s = timer_get_tick();
#endif
    rdsr1 = qspi_fl_read_status(SPINOR_OP_RDSR1);

    if ((rdsr1 & SR1_BP_BITS_MASK) == SR1_BP_BITS_SHIFT(region)) {
        qspi_debug("region had enabled, rdsr1 = 0x%x\n", rdsr1);
        return QSPI_OP_SUCCESS;
    }

    rdsr1 |= SR1_BP_BITS_SHIFT(region);
    st = qspi_gd25q256d_write_status(SPINOR_OP_WRSR1, rdsr1);  
#ifdef CONFIG_QSPI_TRACE
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
#endif
    if ((st & SR_WIP) == SR_WIP) {
        ret = QSPI_OP_FAILED;
    }
    if(st < 0)
        ret = QSPI_OP_FAILED;

    qspi_gd25q256d_read_status();
    qspi_debug("prot_enable need    : %d us (%d ms), w:0x%x\n", us, us / 1000, rdsr1);

    return ret;
}

static int qspi_gd25q256d_prot_region_disable(int region)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 rdsr1 = 0, st;
    int ret = 0;

    qspi_debug("\nprot disable \n");

#ifdef CONFIG_QSPI_TRACE
    s = timer_get_tick();
#endif
    rdsr1 = qspi_fl_read_status(SPINOR_OP_RDSR1);

    if ((rdsr1 & SR1_BP_BITS_MASK) == 0) {
        qspi_debug("region had disabled, rdsr1 = 0x%x\n", rdsr1);
        return 0;
    }

    rdsr1 &= ~SR1_BP_BITS_MASK;
    st = qspi_gd25q256d_write_status(SPINOR_OP_WRSR1, rdsr1);
#ifdef CONFIG_QSPI_TRACE
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
#endif
    if ((st & SR_WIP) == SR_WIP) {
        ret = QSPI_OP_FAILED;
    }
    if(st < 0)
        ret = QSPI_OP_FAILED;

    qspi_gd25q256d_read_status();
    qspi_debug("prot_disable need    : %d us (%d ms), w:0x%x\n", us, us / 1000, rdsr1);

    return ret;
}

int qspi_gd25q256d_write_page(u32 addr, u8 *dbuf, int len)
{
    u8 status, st3;
	int ret=0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }
    ret = qspi_fl_wait_wel();
	if(ret < 0)
	{
		return QSPI_OP_FAILED;
	}

    wbuf_s[0] = SPINOR_OP_BP;
    wbuf_s[1] = (addr >> 16) & 0xff;
    wbuf_s[2] = (addr >> 8) & 0xff;
    wbuf_s[3] = addr & 0xff;

    if (len <= 256)
        memcpy(wbuf_s + 4, dbuf, len);
    ret = qspi_send_xx_data(wbuf_s, len + 4);
	if(ret < 0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(&st3); //wip = 1, p_err = 1
    if ((status & SR_WIP) || (st3 & SR3_P_ERR_GD)) {
        printf("program addr 0x%x failed, status 0x%x, st3 0x%x\n", addr, status, st3);
        qspi_gd25q256d_clear_status_for_err_bits();
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}

#ifdef CONFIG_QSPI_DMA_TRANSFER
int qspi_gd25q256d_dma_write_page(u32 addr, u8 *dbuf, int len, int dfs)
{
    u8 status, st3;
    int ret;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }
    ret = qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

    ret = qspi_dma_write_page(SPINOR_OP_BP, addr, dbuf, len, dfs);
    if(ret < 0)
        return ret;

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_P_ERR_GD)) {
        printf("%s:program addr 0x%x failed, status 0x%x, st3 0x%x\n", __FUNCTION__, addr, status,
               st3);
        qspi_gd25q256d_clear_status_for_err_bits();
        qspi_hw_init();
        return QSPI_OP_FAILED;
    }
    qspi_hw_init();
    return QSPI_OP_SUCCESS;
}

int qspi_gd25q256d_erase(u32 addr)
{
    int ret=0;
    u8 status, st3;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }
    ret = qspi_fl_wait_wel();
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    ret = qspi_dma_write_page(SPINOR_OP_BE_64K, addr, NULL, 0, 8);
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_E_ERR_GD)) {
        printf("erase addr 0x%x failed, status 0x%x, st3 0x%x\n", addr, status, st3);
        qspi_gd25q256d_clear_status_for_err_bits();
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}
#else
int qspi_gd25q256d_erase(u32 addr)
{
    u8 status, st3;
    u8 wbuf[4];
    int ret=0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }
    ret = qspi_fl_wait_wel();
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    wbuf[0] = SPINOR_OP_BE_64k;
    wbuf[1] = (addr >> 16) & 0xff;
    wbuf[2] = (addr >> 8) & 0xff;
    wbuf[3] = addr & 0xff;
    ret = qspi_send_xx_data(wbuf, 4);
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_E_ERR_GD)) {
        printf("erase addr 0x%x failed, status 0x%x, st3 0x%x\n", addr, status, st3);
        qspi_gd25q256d_clear_status_for_err_bits();
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}
#endif

int qspi_gd25q256d_erase_all(void)
{
    u8 status, st3;
    u8 wbuf[4];
    int ret=0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
    {
        return QSPI_OP_FAILED;
    }
    ret = qspi_fl_wait_wel();
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    wbuf[0] = SPINOR_OP_CHIP_ERASE;
    ret = qspi_send_xx_data(wbuf, 1);
	if(ret < 0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_E_ERR_GD)) {
        printf("chip erase failed, status 0x%x, st3 0x%x\n", status, st3);
        qspi_gd25q256d_clear_status_for_err_bits();
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}

int qspi_gd25q256d_4byte_extend(u32 addr, int en)
{
    if (en > 0)
        return qspi_gd25q256d_4byte_extend_enable(addr);
    else
        return qspi_gd25q256d_4byte_extend_disable(addr);
}

int qspi_gd25q256d_quad(int en)
{
    if (en > 0)
        return qspi_gd25q256d_quad_enable();
    else
        return  qspi_gd25q256d_quad_disable();
}

int qspi_gd25q256d_prot_region(int region, int en)
{
    if (en > 0)
        return qspi_gd25q256d_prot_region_enable(region);
    else
        return qspi_gd25q256d_prot_region_disable(region);
}

u8 qspi_gd25q256d_read_status(void)
{
    printf("status-1		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR1));
    printf("status-2		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR2));
    printf("status-3		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR3));
    printf("extend addr		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RD_EXTADDR));
    return 0;
}

int qspi_gd25q256d_reset(void)
{
    u8 cmd = SPINOR_OP_ENABLE_RESET;
    int ret;

    ret = qspi_send_xx_data(&cmd, 1);
    if(ret < 0)
        return ret;

    cmd = SPINOR_OP_RESET;
    ret = qspi_send_xx_data(&cmd, 1);
    if(ret < 0)
        return ret;
    mdelay(10); // wait for device Reset sequence complete
    return QSPI_OP_SUCCESS;
}

int qspi_gd25q256d_deep_power_down(void)
{
	u8 cmd = SPINOR_OP_DEEP_POWER_DOWN;
	return qspi_send_xx_data(&cmd, 1);
}
