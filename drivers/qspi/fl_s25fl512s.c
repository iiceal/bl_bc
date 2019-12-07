#ifdef CONFIG_QSPI_S25FL512S
#include <common.h>
#include "drivers/drv_qspi.h"
#include "fl_common_cmd.h"
#define	SPINOR_OP_CLRR			0x30
#define SPINOR_OP_SW_RESET		0xf0	/* Software Reset */

extern inline void qspi_wren(u8 cmd);
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

static u8 wbuf_s[280];

extern u8 qspi_fl_read_status(u8 cmd);
/*not require WEN command to precede its*/
static void qspi_s25fl512s_write_bar(u8 cmd, u8 data)
{
#ifdef CONFIG_QSPI_DMA_TRANSFER
    qspi_dma_send_cmd_data(cmd, data);
#else
    u8 wbuf[2];
    wbuf[0] = cmd;
    wbuf[1] = data;
    qspi_send_xx_data(wbuf, 2);
#endif
}

/*not necessary to set WEL bit, before the clear SR command is executed*/
/* for clear erase Fail Flag and Program Fail Flag*/
static int qspi_f25fl512s_clear_status(u8 clrr_cmd)
{
    qspi_send_xx_data(&clrr_cmd, 1);
    return 0;
}

static void qspi_fl_wait_wel(void)
{
#if (defined CONFIG_BP2018_ASIC) || (defined CONFIG_BP2016_ASIC)
    u8 st = 0;
    while(1)
    {
        st = qspi_fl_read_status(SPINOR_OP_RDSR);
        if (st & SR_WEL)
            break;
    }
#endif
}

static u8 wait_flash_idle_extend(u8 rdsr_cmd, u32 wip, u32 wip_mask, u32 e_err, u32 e_err_mask,
                                 u32 p_err, u32 p_err_mask)
{
    u8 st = 0;
    while (1) {
        st = qspi_fl_read_status(rdsr_cmd);
        if ((st & wip_mask) == wip)
            break;
        if ((st & e_err_mask) == e_err)
            break;
        if ((st & p_err_mask) == p_err)
            break;
    }
    return st;
}


static u8 qspi_s25fl512s_write_status(u8 sr, u8 cr)
{
    u8 status;
    u8 wbuf[4];

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

#ifdef CONFIG_QSPI_DMA_TRANSFER
    wbuf[0] = sr;
    wbuf[1] = cr;
    spansion_qspi_dma_send_cmd_data(SPINOR_OP_WRSR, wbuf, 2);
#else
    wbuf[0] = SPINOR_OP_WRSR;
    wbuf[1] = sr;
    wbuf[2] = cr;
    qspi_send_xx_data(wbuf, 3);
#endif

    status = wait_flash_idle_extend(SPINOR_OP_RDSR, 0, 1, 
			SR_E_ERR_SPAN, SR_E_ERR_SPAN, SR_P_ERR_SPAN,SR_P_ERR_SPAN);

    if ((status & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        printf("write status failed, status 0x:%x\n", status);
        qspi_f25fl512s_clear_status(SPINOR_OP_CLRR);
        printf("clear and status	               : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
        qspi_wren(SPINOR_OP_WRDI);
    }
    return status;
}

static void qspi_s25fl512s_4byte_extend_enable(u32 addr)
{
    unsigned long long s, e;
    u32 us;
    u8 bar_data = 0, bar_d;

    bar_d = qspi_fl_read_status(SPINOR_OP_BRRD);
    if ((addr & (1 << 24)) > 0)
        bar_data |= SPINOR_OP_BR_BA24;
    if ((addr & (1 << 25)) > 0)
        bar_data |= SPINOR_OP_BR_BA25;
    if (bar_d != bar_data) {
        s = timer_get_tick();
        qspi_s25fl512s_write_bar(SPINOR_OP_BRWR, bar_data);

        e = timer_get_tick();
        us = tick_to_us((unsigned long) (e - s));

        printf("bank register             : 0x%x, wr: 0x%x\n", 
                qspi_fl_read_status(SPINOR_OP_BRRD), bar_data);
        printf("address_extend_enable need: %d us (%d ms)\n\n", us, us / 1000);
    }
}

static void qspi_s25fl512s_4byte_extend_disable(u32 addr)
{
    unsigned long long s, e;
    u32 us;
    u8 bar_d = 0;

    bar_d = qspi_fl_read_status(SPINOR_OP_BRRD);

    if (bar_d != 0) {
        s = timer_get_tick();
        qspi_s25fl512s_write_bar(SPINOR_OP_BRWR, 0);

        e = timer_get_tick();
        us = tick_to_us((unsigned long) (e - s));

        printf("bank register              : 0x%x, wr: 0\n", 
                qspi_fl_read_status(SPINOR_OP_BRRD));
        printf("address_extend_disable need: %d us (%d ms)\n\n", us, us / 1000);
    }
}

static int qspi_s25fl512s_quad_enable(void)
{
    unsigned long long s, e;
    u32 us;
    u8 rdcr, rdsr;
    int ret = 0;

    printf("quad enable \n");

    s = timer_get_tick();

    rdsr = qspi_fl_read_status(SPINOR_OP_RDSR);
    rdcr = qspi_fl_read_status(SPINOR_OP_RDCR);
    if ((rdcr & CR_QUAD_EN_SPAN) == CR_QUAD_EN_SPAN) {
        printf("quad bit had enabled\n");
        return ret;
    }
    if ((rdcr & CR_BPNV_EN_SPAN) == 0) // protect disable
        rdsr = 0;

    rdsr = qspi_s25fl512s_write_status(rdsr, CR_QUAD_EN_SPAN | rdcr);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));

    if ((rdsr & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        ret = -1;
    }

    printf("status register     : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    printf("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    printf("quad_enable need    : %d us (%d ms)\n\n", us, us / 1000);

    return ret;
}

static int qspi_s25fl512s_quad_disable(void)
{
    unsigned long long s, e;
    u32 us;
    u8 rdsr, rdcr;
    int ret = 0;

    printf("quad disable\n");
    s = timer_get_tick();

    rdsr = qspi_fl_read_status(SPINOR_OP_RDSR);
    rdcr = qspi_fl_read_status(SPINOR_OP_RDCR);
    if ((rdcr & CR_QUAD_EN_SPAN) == 0)
        return ret;

    if ((rdcr & CR_BPNV_EN_SPAN) == 0) // protect disable
        rdsr = 0;

    rdsr = qspi_s25fl512s_write_status(rdsr, rdcr & (~CR_QUAD_EN_SPAN));
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));

    if ((rdsr & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        ret = -1;
    }

    printf("status register     : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    printf("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    printf("quad_disable need   :%d us (%d ms)\n\n", us, us / 1000);

    return ret;
}

static int qspi_s25fl512s_prot_region_enable(int region)
{
    unsigned long long s, e;
    u32 us;
    u8 rdcr = 0, rdsr = 0;
    int ret = 0;

    region &= 0x7;
    if (region >= 8)
        return -1;

    printf("\nprot enable \n");

    s = timer_get_tick();
    rdsr = qspi_fl_read_status(SPINOR_OP_RDSR);
    rdcr = qspi_fl_read_status(SPINOR_OP_RDCR);

    if (((rdcr & CR_BPNV_EN_SPAN) == CR_BPNV_EN_SPAN) &&
        (rdsr & SR_BP_BITS_MASK) == SR_BP_BITS_SHIFT(region)) {
        printf("region had enabled, rdsr = 0x%x, rdcr = 0x%x\n", rdsr, rdcr);
        return 0;
    }

    rdsr = qspi_s25fl512s_write_status(SR_BP_BITS_SHIFT(region),
                                       rdcr | CR_BPNV_EN_SPAN | CR_TBPROT_EN_SPAN);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    if ((rdsr & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        ret = -1;
    }

    printf("status		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    printf("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    printf("prot_enable need    : %d us (%d ms)\n", us, us / 1000);
    return ret;
}

static int qspi_s25fl512s_prot_region_disable(int region)
{
    unsigned long long s, e;
    u32 us;
    u8 rdsr = 0, rdcr;

    region &= 0x7;
    if (region >= 8)
        return -1;

    printf("\nprot disable\n");
    s = timer_get_tick();
    rdsr = qspi_fl_read_status(SPINOR_OP_RDSR);
    rdcr = qspi_fl_read_status(SPINOR_OP_RDCR);

    if ((rdsr & SR_BP_BITS_MASK) == 0)
        return 0;

    rdsr = qspi_s25fl512s_write_status(0, rdcr | CR_BPNV_EN_SPAN | CR_TBPROT_EN_SPAN);
    if ((rdsr & SR_P_ERR_SPAN) == SR_P_ERR_SPAN) {
        return -1;
    }

    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));

    printf("status		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR));
    printf("configuration status: 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDCR));
    printf("prot_disable need   :%d us (%d ms)\n", us, us / 1000);
    return 0;
}

int qspi_s25fl512s_write_page(u32 addr, u8 *dbuf, int len)
{
    u8 status;

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

    wbuf_s[0] = SPINOR_OP_BP;
    wbuf_s[1] = (addr >> 16) & 0xff;
    wbuf_s[2] = (addr >> 8) & 0xff;
    wbuf_s[3] = addr & 0xff;

    if (len <= 256)
        memcpy(wbuf_s + 4, dbuf, len);
    qspi_send_xx_data(wbuf_s, len + 4);

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

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

    qspi_dma_write_page(SPINOR_OP_BP, addr, dbuf, len, dfs);

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

u8 qspi_s25fl512s_erase(u32 addr)
{
    u8 status;

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

#ifdef CONFIG_QSPI_DMA_TRANSFER
    qspi_dma_write_page(SPINOR_OP_SE, addr, NULL, 0, 8);
#else
    u8 wbuf[4];
    wbuf[0] = SPINOR_OP_SE;
    wbuf[1] = (addr >> 16) & 0xff;
    wbuf[2] = (addr >> 8) & 0xff;
    wbuf[3] = addr & 0xff;
    qspi_send_xx_data(wbuf, 4);
#endif

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
    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

    wbuf[0] = SPINOR_OP_CHIP_ERASE;
    qspi_send_xx_data(wbuf, 1);

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

void qspi_s25fl512s_4byte_extend(u32 addr, int en)
{
    if (en > 0)
        qspi_s25fl512s_4byte_extend_enable(addr);
    else
        qspi_s25fl512s_4byte_extend_disable(addr);
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
    u8 cmd = SPINOR_OP_SW_RESET;
    qspi_send_xx_data(&cmd, 1);
    mdelay(10); // wait for device Reset sequence complete
    return 0;
}

/******************************************************************************/
/********************** for asp relative command ******************************/
/******************************************************************************/

static u8 qspi_s25fl512s_write_registers(u8 cmd, u16 val)
{
    u8 status;
    u8 wbuf[3];

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

#ifdef CONFIG_QSPI_DMA_TRANSFER
    wbuf[0] = val&0xff;
    wbuf[1] = (val>>8)&0xff;
    spansion_qspi_dma_send_cmd_data(cmd, wbuf, 2);
#else
    wbuf[0] = cmd;
    wbuf[1] = val&0xff;
    wbuf[2] = (val>>8)&0xff;
    qspi_send_xx_data(wbuf, 3);
#endif

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
	printf("ASP Register value: 0x%x", st);
	if((st&0x6) == 0)
	{
		printf("Password mode and Persistent mode are all enabled!, error ...");
		return QSPI_OP_FAILED;
	}
		
	if((st&mode) == mode)
	{
		printf("mode is already enabled! val 0x%x", st);
		return 0;
	}

	if((st&0x6) != 6)
	{
		printf("It's already set to mode val 0x%x, don't to set to 0x%x, it will cause error!", st&0x6, mode);
		return 0;
	}

	st = (st & ~(MODE_MASK)) | mode;

	printf("write ASP Register to 0x%x", st);

	ret = qspi_s25fl512s_write_registers(SPANSION_ASPP, st);

	printf("done!");
    return ret;
}

int qspi_s25fl512s_asp_ppb_read(u32 offset)
{
	u8 data[2];

	printf("offset 0x%x\n", offset);
	qspi_read_spansion_data(SPANSION_PPBRD, offset, 1, data, 2);

	printf("offset 0x%x PPB %u %u", offset, (u32)data[0], (u32)data[1]);

	return data[0];
}

int qspi_s25fl512s_asp_ppb_program(u32 addr)
{
	int ret=0;
    u8 status;
    u8 wbuf[5];

	printf("enter!");
    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

#ifdef CONFIG_QSPI_DMA_TRANSFER
    wbuf[0] = (addr >> 24) & 0xff;
    wbuf[1] = (addr >> 16) & 0xff;
    wbuf[2] = (addr >> 8) & 0xff;
    wbuf[3] = addr & 0xff;
    spansion_qspi_dma_send_cmd_data(SPANSION_PPBP, wbuf, 4);
#else
    wbuf[0] = SPANSION_PPBP;
    wbuf[1] = (addr >> 24) & 0xff;
    wbuf[2] = (addr >> 16) & 0xff;
    wbuf[3] = (addr >> 8) & 0xff;
    wbuf[4] = addr & 0xff;
    qspi_send_xx_data(wbuf, 5);
#endif

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

	printf("done!");
	return ret;
}

int qspi_s25fl512s_asp_ppb_erase(void)
{
	int ret = QSPI_OP_SUCCESS;
    u8 status;
    u8 wbuf[2];

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

    wbuf[0] = SPANSION_PPBE;
    qspi_send_xx_data(wbuf, 1);

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

    qspi_read_xx_data(SPANSION_PLBRD, 0, 0, data, 2);

	printf("PPB lock bit status %u %u", (u32)data[0], (u32)data[1]);
	return 0;
}

u8 qspi_s25fl512s_asp_dyb_read(u32 offset)
{
	u8 data[2];

	printf("offset 0x%x\n", offset);
	qspi_read_spansion_data(SPANSION_DYBRD, offset, 1, data, 2);

	printf("offset 0x%x DYB status %u %u", offset, (u32)data[0], (u32)data[1]);
	return data[0];
}

int qspi_s25fl512s_asp_dyb_program(u32 addr, u8 val)
{
	int ret = QSPI_OP_SUCCESS;
    u8 status;
    u8 wbuf[6];

	printf("enter!");
    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

#ifdef CONFIG_QSPI_DMA_TRANSFER
    wbuf[0] = (addr >> 24) & 0xff;
    wbuf[1] = (addr >> 16) & 0xff;
    wbuf[2] = (addr >> 8) & 0xff;
    wbuf[3] = addr & 0xff;
    wbuf[4] = val;
    spansion_qspi_dma_send_cmd_data(SPANSION_DYBWR, wbuf, 5);
#else
    wbuf[0] = SPANSION_DYBWR;
    wbuf[1] = (addr >> 24) & 0xff;
    wbuf[2] = (addr >> 16) & 0xff;
    wbuf[3] = (addr >> 8) & 0xff;
    wbuf[4] = addr & 0xff;
    wbuf[5] = val;
    qspi_send_xx_data(wbuf, 6);
#endif

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

	printf("done!");
    return ret;
}

int qspi_s25fl512s_block_lock(u32 addr)
{
	//qspi_s25fl512s_asp_ppb_read(addr);
	//qspi_s25fl512s_asp_ppb_program(addr);
	//qspi_s25fl512s_asp_ppb_read(addr);
	qspi_s25fl512s_asp_dyb_program(addr, 0);
	qspi_s25fl512s_asp_dyb_read(addr);

	return 0;
}

int qspi_s25fl512s_block_unlock(u32 addr)
{
	qspi_s25fl512s_asp_ppb_erase();
	qspi_s25fl512s_asp_ppb_read(addr);
	qspi_s25fl512s_asp_dyb_program(addr, 0xff);
	qspi_s25fl512s_asp_dyb_read(addr);
	return 0;
}

int qspi_s25fl512s_read_block_status(u32 addr)
{
	qspi_s25fl512s_asp_mode_set(PERSISTENT_MODE);
	qspi_s25fl512s_asp_ppb_lockbit_read();
	qspi_s25fl512s_asp_ppb_read(addr);
	return qspi_s25fl512s_asp_dyb_read(addr);
}
#endif
