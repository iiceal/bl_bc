#include <common.h>
#include "drivers/drv_qspi.h"
#include "fl_common_cmd.h"
#define	SPINOR_OP_CLRR			0x30
#define SPINOR_OP_SW_RESET		0xf0	/* Software Reset */

extern inline int qspi_wren(u8 cmd);
extern void qspi_ahb_read_enable(u8 rd_cmd, u8 dfs);
extern void qspi_ahb_write_enable(u8 wren_cmd);
extern int qspi_send_xx_data(u8 *tx_dbuf, int tx_len);
extern int qspi_read_xx_data(u8 cmd, u32 addr, int addr_vld, u8 *rx_dbuf, int rx_len);
extern int qspi_read_page_data(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len, int dfs);
extern int qspi_dma_send_cmd_data(u8 cmd, u8 data);
extern int spansion_qspi_dma_send_cmd_data(u8 cmd, u8 *data, u32 dat_len);
extern int qspi_dma_write_page(u8 cmd, u32 addr, u8 *tx_dbuf, int tx_len, int dfs);
extern inline void qspi_hw_init(void);
int qspi_read_spansion_data(u8 cmd, u32 addr, int addr_vld, u8 *rx_dbuf, int rx_len);
extern int qspi_fl_wait_wel(void);

static u8 wbuf_s[280];

extern u8 qspi_fl_read_status(u8 cmd);
/*not require WEN command to precede its*/
static int qspi_s25fl512s_write_bar(u8 cmd, u8 data)
{
    int ret;
#ifdef CONFIG_QSPI_DMA_TRANSFER
    ret = qspi_dma_send_cmd_data(cmd, data);
#else
    u8 wbuf[2];
    wbuf[0] = cmd;
    wbuf[1] = data;
    ret = qspi_send_xx_data(wbuf, 2);
#endif

    if(ret < 0)
        return QSPI_OP_FAILED;

    return QSPI_OP_SUCCESS;
}

/*not necessary to set WEL bit, before the clear SR command is executed*/
/* for clear erase Fail Flag and Program Fail Flag*/
static int qspi_f25fl512s_clear_status(u8 clrr_cmd)
{
    qspi_send_xx_data(&clrr_cmd, 1);
    return 0;
}

#define WAIT_TIME_OUT   (2500000)
static u8 wait_flash_idle_extend(u8 rdsr_cmd, u32 wip, u32 wip_mask, u32 e_err, u32 e_err_mask,
                                 u32 p_err, u32 p_err_mask)
{
    u8 st = 0;
    int timeout=0;

    while (1) {
        st = qspi_fl_read_status(rdsr_cmd);
        if ((st & wip_mask) == wip)
            break;
        if ((st & e_err_mask) == e_err)
            break;
        if ((st & p_err_mask) == p_err)
            break;

        timeout++;
        if(timeout > WAIT_TIME_OUT)
            break;
    }
    return st;
}


static int qspi_s25fl512s_write_status(u8 sr, u8 cr)
{
    u8 status;
    u8 wbuf[4];
	int ret = 0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
        return ret;
    ret = qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

#ifdef CONFIG_QSPI_DMA_TRANSFER
    wbuf[0] = sr;
    wbuf[1] = cr;
    ret = spansion_qspi_dma_send_cmd_data(SPINOR_OP_WRSR, wbuf, 2);
#else
    wbuf[0] = SPINOR_OP_WRSR;
    wbuf[1] = sr;
    wbuf[2] = cr;
    ret = qspi_send_xx_data(wbuf, 3);
#endif
	if(ret<0)
	{
		qspi_debug("error! return ...");
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, 
			SR_E_ERR_SPAN, SR_E_ERR_SPAN, SR_P_ERR_SPAN,SR_P_ERR_SPAN);

    if ((status & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        printf("write status failed, status 0x:%x\n", status);
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
        ret = qspi_wren(SPINOR_OP_WRDI);
        if(ret < 0)
            return ret;
    }
    return status;
}

static int qspi_s25fl512s_4byte_extend_enable(u32 addr)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 bar_data = 0, bar_d;
    int ret=QSPI_OP_SUCCESS;

    bar_d = qspi_fl_read_status(SPINOR_OP_BRRD);
    if ((addr & (1 << 24)) > 0)
        bar_data |= SPINOR_OP_BR_BA24;
    if ((addr & (1 << 25)) > 0)
        bar_data |= SPINOR_OP_BR_BA25;
    if (bar_d != bar_data) {
#ifdef CONFIG_QSPI_TRACE
        s = timer_get_tick();
#endif
        ret = qspi_s25fl512s_write_bar(SPINOR_OP_BRWR, bar_data);

#ifdef CONFIG_QSPI_TRACE
        e = timer_get_tick();
        us = tick_to_us((unsigned long) (e - s));
#endif

        qspi_debug("bank register             : 0x%x, wr: 0x%x\n", 
                qspi_fl_read_status(SPINOR_OP_BRRD), bar_data);
        qspi_debug("address_extend_enable need: %d us (%d ms)\n\n", us, us / 1000);
    }

    return ret;
}

static int qspi_s25fl512s_4byte_extend_disable(u32 addr)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 bar_d = 0;
    int ret=0;

    bar_d = qspi_fl_read_status(SPINOR_OP_BRRD);

    if (bar_d != 0) {
#ifdef CONFIG_QSPI_TRACE
        s = timer_get_tick();
#endif
        ret = qspi_s25fl512s_write_bar(SPINOR_OP_BRWR, 0);

#ifdef CONFIG_QSPI_TRACE
        e = timer_get_tick();
        us = tick_to_us((unsigned long) (e - s));
#endif

        qspi_debug("bank register              : 0x%x, wr: 0\n", 
                qspi_fl_read_status(SPINOR_OP_BRRD));
        qspi_debug("address_extend_disable need: %d us (%d ms)\n\n", us, us / 1000);
    }

    return ret;
}

static int qspi_s25fl512s_quad_enable(void)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 rdcr, rdsr;
    int ret = 0;

    qspi_debug("quad enable \n");

#ifdef CONFIG_QSPI_TRACE
    s = timer_get_tick();
#endif

    rdsr = qspi_fl_read_status(SPINOR_OP_RDSR);
    rdcr = qspi_fl_read_status(SPINOR_OP_RDCR);
    if ((rdcr & CR_QUAD_EN_SPAN) == CR_QUAD_EN_SPAN) {
        qspi_debug("quad bit had enabled\n");
        return ret;
    }
    if ((rdcr & CR_BPNV_EN_SPAN) == 0) // protect disable
        rdsr = 0;

    rdsr = qspi_s25fl512s_write_status(rdsr, CR_QUAD_EN_SPAN | rdcr);
#ifdef CONFIG_QSPI_TRACE
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
#endif

    if ((rdsr & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        ret = QSPI_OP_FAILED;
    }
    if(rdsr < 0)
        ret = QSPI_OP_FAILED;

    qspi_debug("status register     : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    qspi_debug("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    qspi_debug("quad_enable need    : %d us (%d ms)\n\n", us, us / 1000);

    return ret;
}

static int qspi_s25fl512s_quad_disable(void)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 rdsr, rdcr;
    int ret = 0;

    qspi_debug("quad disable\n");
#ifdef CONFIG_QSPI_TRACE
    s = timer_get_tick();
#endif

    rdsr = qspi_fl_read_status(SPINOR_OP_RDSR);
    rdcr = qspi_fl_read_status(SPINOR_OP_RDCR);
    if ((rdcr & CR_QUAD_EN_SPAN) == 0)
        return ret;

    if ((rdcr & CR_BPNV_EN_SPAN) == 0) // protect disable
        rdsr = 0;

    rdsr = qspi_s25fl512s_write_status(rdsr, rdcr & (~CR_QUAD_EN_SPAN));
#ifdef CONFIG_QSPI_TRACE
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
#endif

    if ((rdsr & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        ret = QSPI_OP_FAILED;
    }
    if(rdsr < 0)
        ret = QSPI_OP_FAILED;

    qspi_debug("status register     : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    qspi_debug("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    qspi_debug("quad_disable need   :%d us (%d ms)\n\n", us, us / 1000);

    return ret;
}

static int qspi_s25fl512s_prot_region_enable(int region)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 rdcr = 0, rdsr = 0;
    int ret = 0;

    region &= 0x7;
    if (region >= 8)
        return QSPI_OP_FAILED;

#ifdef CONFIG_QSPI_TRACE
    printf("\nprot enable \n");
    s = timer_get_tick();
#endif
    rdsr = qspi_fl_read_status(SPINOR_OP_RDSR);
    rdcr = qspi_fl_read_status(SPINOR_OP_RDCR);

    if (((rdcr & CR_BPNV_EN_SPAN) == CR_BPNV_EN_SPAN) &&
        (rdsr & SR_BP_BITS_MASK) == SR_BP_BITS_SHIFT(region)) {
        qspi_debug("region had enabled, rdsr = 0x%x, rdcr = 0x%x\n", rdsr, rdcr);
        return QSPI_OP_SUCCESS;
    }

    rdsr = qspi_s25fl512s_write_status(SR_BP_BITS_SHIFT(region),
                                       rdcr | CR_BPNV_EN_SPAN | CR_TBPROT_EN_SPAN);
#ifdef CONFIG_QSPI_TRACE
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
#endif
    if ((rdsr & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        ret = QSPI_OP_FAILED;
    }
    if(rdsr < 0)
        ret = QSPI_OP_FAILED;

    qspi_debug("status		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    qspi_debug("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    qspi_debug("prot_enable need    : %d us (%d ms)\n", us, us / 1000);
    return ret;
}

static int qspi_s25fl512s_prot_region_disable(int region)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    u32 us;
#endif
    u8 rdsr = 0, rdcr;

    region &= 0x7;
    if (region >= 8)
        return QSPI_OP_FAILED;

#ifdef CONFIG_QSPI_TRACE
    printf("\nprot disable\n");
    s = timer_get_tick();
#endif
    rdsr = qspi_fl_read_status(SPINOR_OP_RDSR);
    rdcr = qspi_fl_read_status(SPINOR_OP_RDCR);

    if ((rdsr & SR_BP_BITS_MASK) == 0)
        return QSPI_OP_SUCCESS;

    rdsr = qspi_s25fl512s_write_status(0, rdcr | CR_BPNV_EN_SPAN | CR_TBPROT_EN_SPAN);
    if ((rdsr & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        return QSPI_OP_FAILED;
    }
    if(rdsr < 0)
        return QSPI_OP_FAILED;

#ifdef CONFIG_QSPI_TRACE
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
#endif

    qspi_debug("status		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    qspi_debug("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    qspi_debug("prot_disable need   :%d us (%d ms)\n", us, us / 1000);
    return QSPI_OP_SUCCESS;
}

int qspi_s25fl512s_write_page(u32 addr, u8 *dbuf, int len)
{
    u8 status;
	int ret=QSPI_OP_SUCCESS;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
        return ret;
    ret=qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

    wbuf_s[0] = SPINOR_OP_BP;
    wbuf_s[1] = (addr >> 16) & 0xff;
    wbuf_s[2] = (addr >> 8) & 0xff;
    wbuf_s[3] = addr & 0xff;

    if (len <= 256)
        memcpy(wbuf_s + 4, dbuf, len);
    ret = qspi_send_xx_data(wbuf_s, len + 4);
	if(ret<0)
	{
		qspi_debug("error! return ...");
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, SR_E_ERR_SPAN, SR_E_ERR_SPAN,
                                    SR_P_ERR_SPAN, SR_P_ERR_SPAN);
    if ((status & SR_P_ERR_SPAN) == SR_P_ERR_SPAN)
        printf("program addr 0x%x failed, status 0x:%x\n", addr, status);

    if (status & SR_WIP) {
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
        qspi_wren(SPINOR_OP_WRDI);
        return QSPI_OP_FAILED;
    }

    return QSPI_OP_SUCCESS;
}

#ifdef CONFIG_QSPI_DMA_TRANSFER
int qspi_s25fl512s_dma_write_page(u32 addr, u8 *dbuf, int len, int dfs)
{
    u8 status;
    int ret=0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
        return ret;
    ret=qspi_fl_wait_wel();
    if(ret<0)
        return ret;

    ret = qspi_dma_write_page(SPINOR_OP_BP, addr, dbuf, len, dfs);
    if(ret < 0)
        return ret;

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, SR_E_ERR_SPAN, SR_E_ERR_SPAN,
                                    SR_P_ERR_SPAN, SR_P_ERR_SPAN);
    if ((status & SR_P_ERR_SPAN) == SR_P_ERR_SPAN)
        printf("%s:program addr 0x%x failed, status 0x:%x\n", __FUNCTION__, addr, status);

    if (status & SR_WIP) {
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("%s:clear and status	               : 0x%x\n", __FUNCTION__,
               qspi_fl_read_status(SPINOR_OP_RDSR));
        qspi_wren(SPINOR_OP_WRDI);
        qspi_hw_init();
        return QSPI_OP_FAILED;
    }

    qspi_hw_init();
    return QSPI_OP_SUCCESS;
}
#endif

int qspi_s25fl512s_erase(u32 addr)
{
    u8 status;
	int ret = 0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
        return ret;
    ret=qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

#ifdef CONFIG_QSPI_DMA_TRANSFER
    ret = qspi_dma_write_page(SPINOR_OP_SE, addr, NULL, 0, 8);
#else
    u8 wbuf[4];
    wbuf[0] = SPINOR_OP_SE;
    wbuf[1] = (addr >> 16) & 0xff;
    wbuf[2] = (addr >> 8) & 0xff;
    wbuf[3] = addr & 0xff;
    ret = qspi_send_xx_data(wbuf, 4);
#endif
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, SR_E_ERR_SPAN, SR_E_ERR_SPAN,
                                    SR_P_ERR_SPAN, SR_P_ERR_SPAN);
    if ((status & SR_E_ERR_SPAN) == SR_E_ERR_SPAN) // st = 0x3f
        printf("erase addr 0x%x failed, status 0x:%x\n", addr, status);

    if (status & SR_WIP) {
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n",
               qspi_fl_read_status(SPINOR_OP_RDSR)); // st = 0x1e
        qspi_wren(SPINOR_OP_WRDI);
        printf("wren disable     	               : 0x%x\n",
               qspi_fl_read_status(SPINOR_OP_RDSR)); // st = 0x1c
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}

u8 qspi_s25fl512s_erase_all(void)
{
    u8 status;
    u8 wbuf[4];
	int ret = 0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
        return ret;
    ret=qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

    wbuf[0] = SPINOR_OP_CHIP_ERASE;
    ret = qspi_send_xx_data(wbuf, 1);
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, SR_E_ERR_SPAN, SR_E_ERR_SPAN,
                                    SR_P_ERR_SPAN, SR_P_ERR_SPAN);

    if ((status & SR_E_ERR_SPAN) == SR_E_ERR_SPAN)
        printf("chip erase failed, status 0x:%x\n", status);

    if (status & SR_WIP) {
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}

int qspi_s25fl512s_4byte_extend(u32 addr, int en)
{
    if (en > 0)
        return qspi_s25fl512s_4byte_extend_enable(addr);
    else
        return qspi_s25fl512s_4byte_extend_disable(addr);
}

int qspi_s25fl512s_quad(int en)
{
    if (en > 0)
        return qspi_s25fl512s_quad_enable();
    else
        return qspi_s25fl512s_quad_disable();
}

int qspi_s25fl512s_prot_region(int region, int en)
{
    if (en > 0)
        return qspi_s25fl512s_prot_region_enable(region);
    else
        return qspi_s25fl512s_prot_region_disable(region);
}

u8 qspi_s25fl512s_read_status(void)
{
    printf("status		        : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    printf("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    printf("bank register       : 0x%x\n", qspi_fl_read_status(SPINOR_OP_BRRD));
    return 0;
}

int qspi_s25fl512s_reset(void)
{
	int ret = 0;
    u8 cmd = SPINOR_OP_SW_RESET;
    ret = qspi_send_xx_data(&cmd, 1);
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}
    mdelay(10); // wait for device Reset sequence complete
    return QSPI_OP_SUCCESS;
}

/******************************************************************************/
/********************** for asp relative command ******************************/
/******************************************************************************/

static u8 qspi_s25fl512s_write_registers(u8 cmd, u16 val)
{
    u8 status;
    u8 wbuf[3];
	int ret=0;

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret<0)
        return ret;
    ret=qspi_fl_wait_wel();
    if(ret<0)
        return ret;

#ifdef CONFIG_QSPI_DMA_TRANSFER
    wbuf[0] = val&0xff;
    wbuf[1] = (val>>8)&0xff;
    ret = spansion_qspi_dma_send_cmd_data(cmd, wbuf, 2);
#else
    wbuf[0] = cmd;
    wbuf[1] = val&0xff;
    wbuf[2] = (val>>8)&0xff;
    ret = qspi_send_xx_data(wbuf, 3);
#endif
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, SR_E_ERR_SPAN, SR_E_ERR_SPAN,
                                    SR_P_ERR_SPAN, SR_P_ERR_SPAN);

    if (((status & SR_P_ERR_SPAN) == SR_P_ERR_SPAN)
			|| (status & SR_WIP)) {
        printf("write status failed, status 0x:%x\n", status);
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
        qspi_wren(SPINOR_OP_WRDI);
		return QSPI_OP_FAILED;
    }
    
	return QSPI_OP_SUCCESS;
}

u16 qspi_fl_read_registers(u8 cmd)
{
    u8 data[2];
	u16 val=0;
    qspi_read_xx_data(cmd, 0, 0, data, 2);

	val = (data[0] | (data[1]<<8));
    return val;
}

/* DYB Read */
#define SPANSION_DYBRD	(0xE0)
/* DYB Write */
#define SPANSION_DYBWR	(0xE1)
/* PPB Read */
#define SPANSION_PPBRD	(0xE2)
/* PPB Program */
#define SPANSION_PPBP	(0xE3)
/* PPB Erase */
#define SPANSION_PPBE	(0xE4)
/* ASP Read */
#define SPANSION_ASPRD	(0x2B)
/* ASP Program */
#define SPANSION_ASPP	(0x2F)
/* PPB Lock Bit Read */
#define SPANSION_PLBRD	(0xA7)
/* PPB Lock Bit Write */
#define SPANSION_PLBWR	(0xA6)

#define	PASSWORD_MODE	(2)
#define	PERSISTENT_MODE	(4)
#define	MODE_MASK		((0x11)<1)

int qspi_s25fl512s_asp_mode_set(u8 mode)
{
	int ret;
	u16 st;

	if((mode != PASSWORD_MODE) && (mode != PERSISTENT_MODE))
	{
		printf("mode %d error!", mode);
		return QSPI_OP_FAILED;
	}

	st = qspi_fl_read_registers(SPANSION_ASPRD);
	qspi_debug("ASP Register value: 0x%x", st);
	if((st&0x6) == 0)
	{
		printf("Password mode and Persistent mode are all enabled!, error ...");
		return QSPI_OP_FAILED;
	}
		
	if((st&mode) == mode)
	{
		qspi_debug("mode is already enabled! val 0x%x", st);
		return QSPI_OP_SUCCESS;
	}

	if((st&0x6) != 6)
	{
		qspi_debug("It's already set to mode val 0x%x, don't to set to 0x%x, it will cause error!", st&0x6, mode);
		return QSPI_OP_SUCCESS;
	}

	st = (st & ~(MODE_MASK)) | mode;

	qspi_debug("write ASP Register to 0x%x", st);

	ret = qspi_s25fl512s_write_registers(SPANSION_ASPP, st);

	qspi_debug("done!");
    return ret;
}

int qspi_s25fl512s_asp_ppb_read(u32 offset)
{
	u8 data[2];

	qspi_debug("offset 0x%x\n", offset);
	if(qspi_read_spansion_data(SPANSION_PPBRD, offset, 1, data, 2) < 0)
        return QSPI_OP_FAILED;

	qspi_debug("offset 0x%x PPB %u %u", offset, (u32)data[0], (u32)data[1]);

	return data[0];
}

int qspi_s25fl512s_asp_ppb_program(u32 addr)
{
    u8 status;
    u8 wbuf[5];
	int ret = QSPI_OP_SUCCESS;

	qspi_debug("enter!");
    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
        return ret;
    ret=qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

#ifdef CONFIG_QSPI_DMA_TRANSFER
    wbuf[0] = (addr >> 24) & 0xff;
    wbuf[1] = (addr >> 16) & 0xff;
    wbuf[2] = (addr >> 8) & 0xff;
    wbuf[3] = addr & 0xff;
    ret = spansion_qspi_dma_send_cmd_data(SPANSION_PPBP, wbuf, 4);
#else
    wbuf[0] = SPANSION_PPBP;
    wbuf[1] = (addr >> 24) & 0xff;
    wbuf[2] = (addr >> 16) & 0xff;
    wbuf[3] = (addr >> 8) & 0xff;
    wbuf[4] = addr & 0xff;
    ret = qspi_send_xx_data(wbuf, 5);
#endif
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, SR_E_ERR_SPAN, SR_E_ERR_SPAN,
                                    SR_P_ERR_SPAN, SR_P_ERR_SPAN);

    if ((status & SR_P_ERR_SPAN) == SR_P_ERR_SPAN)
	{
        printf("PPB program addr 0x%x failed, status 0x:%x\n", addr, status);
		ret = QSPI_OP_FAILED;
	}

    if (status & SR_WIP) {
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
        qspi_wren(SPINOR_OP_WRDI);
        ret = QSPI_OP_FAILED;
    }

	qspi_debug("done!");
	return ret;
}

int qspi_s25fl512s_asp_ppb_erase(void)
{
	int ret = QSPI_OP_SUCCESS;
    u8 status;
    u8 wbuf[2];

    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
        return ret;

    ret=qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

    wbuf[0] = SPANSION_PPBE;
    ret = qspi_send_xx_data(wbuf, 1);
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, SR_E_ERR_SPAN, SR_E_ERR_SPAN,
                                    SR_P_ERR_SPAN, SR_P_ERR_SPAN);

    if ((status & SR_P_ERR_SPAN) == SR_P_ERR_SPAN)
	{
        printf("failed! status 0x:%x\n", status);
		ret = QSPI_OP_FAILED;
	}

    if (status & SR_WIP) {
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
        qspi_wren(SPINOR_OP_WRDI);
        ret = QSPI_OP_FAILED;
    }

    return ret;
}

int qspi_s25fl512s_asp_ppb_lockbit_read(void)
{
	u8 data[2];
    int ret;

    ret = qspi_read_xx_data(SPANSION_PLBRD, 0, 0, data, 2);

	qspi_debug("PPB lock bit status %u %u", (u32)data[0], (u32)data[1]);
	return ret;
}

u8 qspi_s25fl512s_asp_dyb_read(u32 offset)
{
	u8 data[2];
    int ret=0;

	qspi_debug("offset 0x%x\n", offset);
	ret = qspi_read_spansion_data(SPANSION_DYBRD, offset, 1, data, 2);
	if(ret<0)
	{
		qspi_debug("read dyb failed!");
		return QSPI_OP_FAILED;
	}
	qspi_debug("offset 0x%x DYB status %u %u", offset, (u32)data[0], (u32)data[1]);
	return data[0];
}

int qspi_s25fl512s_asp_dyb_program(u32 addr, u8 val)
{
	int ret = QSPI_OP_SUCCESS;
    u8 status;
    u8 wbuf[6];

	qspi_debug("enter!");
    ret = qspi_wren(SPINOR_OP_WREN);
    if(ret < 0)
        return ret;
    ret=qspi_fl_wait_wel();
    if(ret < 0)
        return ret;

#ifdef CONFIG_QSPI_DMA_TRANSFER
    wbuf[0] = (addr >> 24) & 0xff;
    wbuf[1] = (addr >> 16) & 0xff;
    wbuf[2] = (addr >> 8) & 0xff;
    wbuf[3] = addr & 0xff;
    wbuf[4] = val;
    ret = spansion_qspi_dma_send_cmd_data(SPANSION_DYBWR, wbuf, 5);
#else
    wbuf[0] = SPANSION_DYBWR;
    wbuf[1] = (addr >> 24) & 0xff;
    wbuf[2] = (addr >> 16) & 0xff;
    wbuf[3] = (addr >> 8) & 0xff;
    wbuf[4] = addr & 0xff;
    wbuf[5] = val;
    ret = qspi_send_xx_data(wbuf, 6);
#endif
	if(ret<0)
	{
		return QSPI_OP_FAILED;
	}

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, SR_E_ERR_SPAN, SR_E_ERR_SPAN,
                                    SR_P_ERR_SPAN, SR_P_ERR_SPAN);

    if ((status & SR_P_ERR_SPAN) == SR_P_ERR_SPAN)
	{
        printf("DYB program addr 0x%x failed, status 0x:%x\n", addr, status);
		ret = QSPI_OP_FAILED;
	}

    if (status & SR_WIP) {
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
        qspi_wren(SPINOR_OP_WRDI);
        ret = QSPI_OP_FAILED;
    }

	qspi_debug("done!");
    return ret;
}

int qspi_s25fl512s_block_lock(u32 addr)
{
	//qspi_s25fl512s_asp_ppb_read(addr);
	//qspi_s25fl512s_asp_ppb_program(addr);
	//qspi_s25fl512s_asp_ppb_read(addr);
	if(qspi_s25fl512s_asp_dyb_program(addr, 0) < 0)
        return QSPI_OP_FAILED;

	if(qspi_s25fl512s_asp_dyb_read(addr) < 0)
        return QSPI_OP_FAILED;

	return QSPI_OP_SUCCESS;
}

int qspi_s25fl512s_block_unlock(u32 addr)
{
	if(qspi_s25fl512s_asp_ppb_erase() < 0)
        return QSPI_OP_FAILED;

	if(qspi_s25fl512s_asp_ppb_read(addr) < 0)
        return QSPI_OP_FAILED;

	if(qspi_s25fl512s_asp_dyb_program(addr, 0xff) < 0)
        return QSPI_OP_FAILED;

	if(qspi_s25fl512s_asp_dyb_read(addr) < 0)
        return QSPI_OP_FAILED;

	return QSPI_OP_SUCCESS;
}

int qspi_s25fl512s_read_block_status(u32 addr)
{
	if(qspi_s25fl512s_asp_mode_set(PERSISTENT_MODE) < 0)
        return QSPI_OP_FAILED;

	if(qspi_s25fl512s_asp_ppb_lockbit_read() < 0)
        return QSPI_OP_FAILED;

	if(qspi_s25fl512s_asp_ppb_read(addr) < 0)
        return QSPI_OP_FAILED;

	return qspi_s25fl512s_asp_dyb_read(addr);
}
