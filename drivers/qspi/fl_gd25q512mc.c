#ifdef CONFIG_QSPI_GD25Q512MC

#include <common.h>
#include <drivers/drv_qspi.h>
#include "fl_common_cmd.h"

#define     SPINOR_OP_BE_64K                0xd8
#define     SPINOR_OP_BE_32K                0x52
#define     SPINOR_OP_BE_SECTOR             0x20
#define     SPINOR_OP_CHIP_ERASE            0xc7
#define     SPINOR_OP_BE_MAX_MS             (2000)
#define     SPINOR_OP_CE_MAX_MS             (400 * 1000)

#define     SPINOR_OP_WR_EXTADDR            0xc5
#define     SPINOR_OP_RD_EXTADDR            0xc8
#define     SPINOR_OP_EXTR_A24              0x01
#define     SPINOR_OP_EXTR_A25              0x02
#define     SPINOR_OP_EXTR_A26              0x04

#define 	SPINOR_OP_CLRR		            0x30
#define     SPINOR_OP_RDSR1                 0x05
#define     SPINOR_OP_RDSR2                 0x35
#define     SPINOR_OP_RDSR3                 0x15
#define     SR1_BP_BITS_MASK                0x3c
#define     SR1_BP_BITS_SHIFT(bp)           (bp << 2)
#define     SR3_WPS                         0x80

#define     SPINOR_OP_WRSR1                 0x01
#define     SPINOR_OP_WRSR2                 0x31
#define     SPINOR_OP_WRSR3                 0x11
#define     SR1_QUAD_EN                     0x40
#define     SR3_P_ERR_GD		            0x20	
#define     SR3_E_ERR_GD		            0x40	

#define	    SR_BP_BITS_MASK_GD         		0x3C
#define	    SR_BP_BITS_SHIFT_GD(v)	    	(v << 2)
#define     SR_TBPROT_EN_GD	        		0x40	/* Winbond TBProt I/O: 1=low address */

extern inline void qspi_wren(u8 cmd);
extern void qspi_ahb_read_enable(u8 rd_cmd, u8 dfs);
extern void qspi_ahb_write_enable(u8 wren_cmd);
extern int qspi_send_xx_data(u8 *tx_dbuf, int tx_len);
extern int qspi_read_xx_data(u8 cmd, u32 addr, int addr_vld, u8 *rx_dbuf, int rx_len);
extern int qspi_read_page_data(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len, int dfs);
extern int qspi_dma_send_cmd_data(u8 cmd, u8 data);
extern int qspi_dma_write_page(u8 cmd, u32 addr, u8 *tx_dbuf, int tx_len, int dfs);
extern inline void qspi_hw_init(void);
u8 qspi_fl_read_status(u8 cmd);
u8 qspi_gd25q512mc_read_status(void);

static u8 wbuf_s[280];
static void qspi_gd25q512mc_write_extend_addr(u8 cmd, u8 data)
{
    qspi_wren(SPINOR_OP_WREN);
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
static int qspi_gd25q512mc_clear_status(u8 clrr_cmd)
{
    qspi_send_xx_data(&clrr_cmd, 1);
    return 0;
}

static void qspi_fl_wait_wel(void)
{
#if (defined CONFIG_BP2018_ASIC) || (defined CONFIG_BP2016_ASIC)
    u8 st1 = 0;
    while(1){
        st1 = qspi_fl_read_status(SPINOR_OP_RDSR1);
        if(st1 & SR_WEL)
            break;
    }
#endif
}

/*
 * only for program/erase/write status/erase[program] security register
 */
static u8 wait_flash_idle_extend(u8 *r_st3)
{
    u8 st1 = 0, st3 = 0;

    while (1) {
        st1 = qspi_fl_read_status(SPINOR_OP_RDSR1);
        st3 = qspi_fl_read_status(SPINOR_OP_RDSR3);
        if ((st3 & SR3_E_ERR_GD) == SR3_E_ERR_GD)
            break;
        if ((st3 & SR3_P_ERR_GD) == SR3_P_ERR_GD)
            break;
        if ((st1 & SR_WIP) == 0)
            break;
    }
    *r_st3 = st3;
    return st1;
}

static void qspi_gd25q512mc_clear_status_for_err_bits(void)
{
    qspi_gd25q512mc_clear_status(SPINOR_OP_CLRR);
    printf("clear and status , st1 0x%x, st3 0x%x\n", 
            qspi_fl_read_status(SPINOR_OP_RDSR1),
            qspi_fl_read_status(SPINOR_OP_RDSR3));
    qspi_wren(SPINOR_OP_WRDI);
    printf("wren disable, status	  :0x%x\n",
            qspi_fl_read_status(SPINOR_OP_RDSR1));
}

static u8 qspi_gd25q512mc_write_status(u8 cmd, u8 sr_x)
{
    u8 status, st3 = 0;

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

#ifdef CONFIG_QSPI_DMA_TRANSFER
    qspi_dma_send_cmd_data(cmd,sr_x);
#else
    u8 wbuf[3];
    wbuf[0] = cmd;
    wbuf[1] = sr_x;
    qspi_send_xx_data(wbuf, 2);
#endif

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_P_ERR_GD)) {
        printf("write status failed, status 0x%x, st3 0x%x\n", status, st3);
        qspi_gd25q512mc_clear_status_for_err_bits();
    }
    return status;
}

static void qspi_gd25q512mc_4byte_extend_enable(u32 addr)
{
    unsigned long long s, e;
    u32 us;
    u8 ext_data = 0, ext_rd;

    ext_rd = qspi_fl_read_status(SPINOR_OP_RD_EXTADDR);
    if ((addr & (1 << 24)) > 0)
        ext_data |= SPINOR_OP_EXTR_A24;
    if ((addr & (1 << 25)) > 0)
        ext_data |= SPINOR_OP_EXTR_A25;
    if ((addr & (1 << 26)) > 0)
        ext_data |= SPINOR_OP_EXTR_A26;

    if (ext_rd != ext_data) {
        s = timer_get_tick();
        qspi_gd25q512mc_write_extend_addr(SPINOR_OP_WR_EXTADDR, ext_data);

        e = timer_get_tick();
        us = tick_to_us((unsigned long) (e - s));

        printf("extend addr register      : 0x%x, wr: 0x%x\n",
               qspi_fl_read_status(SPINOR_OP_RD_EXTADDR), ext_data);
        printf("address_extend_enable need: %d us (%d ms)\n\n", us, us / 1000);
    }
}

static void qspi_gd25q512mc_4byte_extend_disable(u32 addr)
{
    unsigned long long s, e;
    u32 us;
    u8 ext_d = 0;

    ext_d = qspi_fl_read_status(SPINOR_OP_RD_EXTADDR);

    if (ext_d != 0) {
        s = timer_get_tick();
        qspi_gd25q512mc_write_extend_addr(SPINOR_OP_WR_EXTADDR, 0);

        e = timer_get_tick();
        us = tick_to_us((unsigned long) (e - s));

        printf("extend addr register       : 0x%x\n", 
                qspi_fl_read_status(SPINOR_OP_RD_EXTADDR));

        printf("address_extend_disable need: %d us (%d ms)\n\n", us, us / 1000);
    }
}

static int qspi_gd25q512mc_quad_enable(void)
{
    unsigned long long s, e;
    u32 us;
    u8 st;
    int ret = 0;

    printf("quad enable \n");

    s = timer_get_tick();

    st = qspi_fl_read_status(SPINOR_OP_RDSR1);
    if ((st & SR1_QUAD_EN) == SR1_QUAD_EN) {
        printf("quad bit had enabled\n");
        return ret;
    }

    st = qspi_gd25q512mc_write_status(SPINOR_OP_WRSR1, st | SR1_QUAD_EN);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));

    if ((st & SR_WIP) == SR_WIP) {
        ret = -1;
    }

    printf("status 1 register   : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR1));
    printf("quad_enable need    : %d us (%d ms)\n\n", us, us / 1000);

    return ret;
}

int qspi_gd25q512mc_quad_disable(void)
{
    unsigned long long s, e;
    u32 us;
    u8 st;
    int ret = 0;

    printf("quad disable \n");

    s = timer_get_tick();

    st = qspi_fl_read_status(SPINOR_OP_RDSR1);
    if ((st & SR1_QUAD_EN) == 0) {
        printf("quad bit had disabled\n");
        return ret;
    }

    st &= ~(SR1_QUAD_EN);
    st = qspi_gd25q512mc_write_status(SPINOR_OP_WRSR1, st);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));

    if ((st & SR_WIP) == SR_WIP) {
        ret = -1;
    }

    printf("status 1 register   : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR1));
    printf("quad_disable need   :%d us (%d ms)\n\n", us, us / 1000);

    return ret;
}

static int qspi_gd25q512mc_prot_region_enable(int region)
{
    unsigned long long s, e;
    u32 us;
    u8 rdsr1 = 0, rdsr3 = 0, st;
    int ret = 0;

    if (region > 0xf)
        return -1;

    printf("\nprot enable \n");

    s = timer_get_tick();
    rdsr1 = qspi_fl_read_status(SPINOR_OP_RDSR1);
    rdsr3 = qspi_fl_read_status(SPINOR_OP_RDSR3);
    if ((rdsr3 & SR3_WPS) > 0) {
        printf("failed! need utilize individual block lock;(wps =1)\n");
        return -1;
    }

    if ((rdsr1 & SR1_BP_BITS_MASK) == SR1_BP_BITS_SHIFT(region)) {
        printf("region had enabled, rdsr1 = 0x%x, rdsr3 = 0x%x\n", rdsr1, rdsr3);
        return 0;
    }

    rdsr1 |= SR1_BP_BITS_SHIFT(region);
    st = qspi_gd25q512mc_write_status(SPINOR_OP_WRSR1, rdsr1);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    if ((st & SR_WIP) == SR_WIP) {
        ret = -1;
    }

    qspi_gd25q512mc_read_status();
    printf("prot_enable need    : %d us (%d ms), w:0x%x\n", us, us / 1000, rdsr1);

    return ret;
}

static int qspi_gd25q512mc_prot_region_disable(int region)
{
    unsigned long long s, e;
    u32 us;
    u8 rdsr1 = 0, rdsr3 = 0, st;
    int ret = 0;

    printf("\nprot disable \n");

    s = timer_get_tick();
    rdsr1 = qspi_fl_read_status(SPINOR_OP_RDSR1);
    rdsr3 = qspi_fl_read_status(SPINOR_OP_RDSR3);

    if ((rdsr3 & SR3_WPS) > 0) {
        printf("failed! need utilize individual block lock;(wps =1)\n");
        return -1;
    }

    if ((rdsr1 & SR1_BP_BITS_MASK) == 0) {
        printf("region had disabled, rdsr1 = 0x%x, rdsr3 = 0x%x\n", rdsr1, rdsr3);
        return 0;
    }

    rdsr1 &= ~SR1_BP_BITS_MASK;
    st = qspi_gd25q512mc_write_status(SPINOR_OP_WRSR1, rdsr1);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    if ((st & SR_WIP) == SR_WIP) {
        ret = -1;
    }

    qspi_gd25q512mc_read_status();
    printf("prot_disable need    : %d us (%d ms), w:0x%x\n", us, us / 1000, rdsr1);

    return ret;
}

int qspi_gd25q512mc_write_page(u32 addr, u8 *dbuf, int len)
{
    u8 status, st3;

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

    wbuf_s[0] = SPINOR_OP_BP;
    wbuf_s[1] = (addr >> 16) & 0xff;
    wbuf_s[2] = (addr >> 8) & 0xff;
    wbuf_s[3] = addr & 0xff;

    if (len <= 256)
        memcpy(wbuf_s + 4, dbuf, len);
    qspi_send_xx_data(wbuf_s, len + 4);

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_P_ERR_GD)) {
        printf("program addr 0x%x failed, status 0x%x, st3 0x%x\n", addr, status, st3);
        qspi_gd25q512mc_clear_status_for_err_bits();
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}

#ifdef CONFIG_QSPI_DMA_TRANSFER
int qspi_gd25q512mc_dma_write_page(u32 addr, u8 *dbuf, int len, int dfs)
{
    u8 status, st3;

    qspi_wren(SPINOR_OP_WREN);
    qspi_dma_write_page(SPINOR_OP_BP, addr, dbuf, len, dfs);

    //qspi_fl_wait_wel();
    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_P_ERR_GD)) {
        printf("%s:program addr 0x%x failed, status 0x%x, st3 0x%x\n", __FUNCTION__, addr, status,
               st3);
        qspi_gd25q512mc_clear_status_for_err_bits();
        qspi_hw_init();
        return QSPI_OP_FAILED;
    }
    qspi_hw_init();
    return QSPI_OP_SUCCESS;
}

u8 qspi_gd25q512mc_erase(u32 addr)
{
    u8 status, st3;

    printf("%s dma mode!\n", __func__);

    qspi_wren(SPINOR_OP_WREN);

    qspi_dma_write_page(SPINOR_OP_BE_64K, addr, NULL, 0, 8);

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_E_ERR_GD)) {
        printf("erase addr 0x%x failed, status 0x%x, st3 0x%x\n", addr, status, st3);
        qspi_gd25q512mc_clear_status_for_err_bits();
        qspi_hw_init();
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}

#else

u8 qspi_gd25q512mc_erase(u32 addr)
{
    u8 status, st3;
    u8 wbuf[4];

    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();

    wbuf[0] = SPINOR_OP_BE_64K;
    wbuf[1] = (addr >> 16) & 0xff;
    wbuf[2] = (addr >> 8) & 0xff;
    wbuf[3] = addr & 0xff;
    qspi_send_xx_data(wbuf, 4);

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_E_ERR_GD)) {
        printf("erase addr 0x%x failed, status 0x%x, st3 0x%x\n", addr, status, st3);
        qspi_gd25q512mc_clear_status_for_err_bits();
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}
#endif

u8 qspi_gd25q512mc_erase_all(void)
{
    u8 status, st3;
    u8 wbuf[4];
    qspi_wren(SPINOR_OP_WREN);
    qspi_fl_wait_wel();
    wbuf[0] = SPINOR_OP_CHIP_ERASE;
    qspi_send_xx_data(wbuf, 1);

    status = wait_flash_idle_extend(&st3);
    if ((status & SR_WIP) || (st3 & SR3_E_ERR_GD)) {
        printf("chip erase failed, status 0x%x, st3 0x%x\n", status, st3);
        qspi_gd25q512mc_clear_status_for_err_bits();
        return QSPI_OP_FAILED;
    }
    return QSPI_OP_SUCCESS;
}

void qspi_gd25q512mc_4byte_extend(u32 addr, int en)
{
    if (en > 0)
        qspi_gd25q512mc_4byte_extend_enable(addr);
    else
        qspi_gd25q512mc_4byte_extend_disable(addr);
}

int qspi_gd25q512mc_quad(int en)
{
    if (en > 0)
        return qspi_gd25q512mc_quad_enable();
    else
        return qspi_gd25q512mc_quad_disable();
}

int qspi_gd25q512mc_wps_op(int en)
{
	u8 rdsr3, st;
    rdsr3 = qspi_fl_read_status(SPINOR_OP_RDSR3);

	if(0 == en)
	{
		if ((rdsr3 & SR3_WPS) > 0) {
			printf("(wps =1), need disable it!\n");
			rdsr3 &= ~SR3_WPS;
		}else{
			return 0;
		}
	}else{
		if ((rdsr3 & SR3_WPS) ==  0) {
			printf("(wps =0), need enable it!\n");
			rdsr3 |= SR3_WPS;
		}else{
			return 0;
		}
	}

	st = qspi_gd25q512mc_write_status(SPINOR_OP_WRSR3, rdsr3);
	if ((st & SR_WIP) == SR_WIP) {
		printf("write WPS failed!");
		return -1;
	}

	return 0;
}

int qspi_gd25q512mc_prot_region(int region, int en)
{
	if(qspi_gd25q512mc_wps_op(0) < 0)
	{
		printf("%s: WPS=1 ,failed!\n", __func__);
		return -1;
	}

    if (en > 0)
        return qspi_gd25q512mc_prot_region_enable(region);
    else
        return qspi_gd25q512mc_prot_region_disable(region);
}

u8 qspi_gd25q512mc_read_status(void)
{
    printf("status-1		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR1));
    printf("status-2		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR2));
    printf("status-3		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RDSR3));
    printf("extend addr		    : 0x%x\n", qspi_fl_read_status(SPINOR_OP_RD_EXTADDR));
    return 0;
}

int qspi_gd25q512mc_reset(void)
{
    u8 cmd = SPINOR_OP_ENABLE_RESET;
    qspi_send_xx_data(&cmd, 1);

    cmd = SPINOR_OP_RESET;
    qspi_send_xx_data(&cmd, 1);
    mdelay(10); // wait for device Reset sequence complete
    return 0;
}

#define	ENOERR	(0)
//only one block status
int qspi_gd25q512mc_read_block_lock_status(u32 offs)
{
	u8 rdsr3 = 0;
	u8 lock_status=0;
	int ret=0;

    rdsr3 = qspi_fl_read_status(SPINOR_OP_RDSR3);

    if ((rdsr3 & SR3_WPS) == 0) {
        printf("(wps = 0), not in block protection mode!\n");
		return -EINVAL;
	}

	ret = qspi_read_xx_data(WPS_BLOCK_READ_LOCK, offs, 1, &lock_status, 1);
	
	if(ret != 1)
	{
		printf("read block lock status error!");
		return -EINVAL;
	}

	if(lock_status&1)
	{
		return BLOCK_PROTECTED;
	}else{
		return BLOCK_UNPROTECTED;
	}

}

static void inline qspi_gd25q512mc_send_lock_cmd(u8 cmd, u32 addr, u32 vld_addr)
{
    u8 wbuf[6];
	qspi_wren(SPINOR_OP_WREN);

    wbuf[0] = cmd;

	if(vld_addr)
	{
#ifdef CONFIG_QSPI_DMA_TRANSFER
        qspi_dma_write_page(cmd, addr, NULL, 0, 8);
#else
		wbuf[1] = (addr >> 16) & 0xff;
		wbuf[2] = (addr >> 8) & 0xff;
		wbuf[3] = addr & 0xff;
		qspi_send_xx_data(wbuf, 4);
#endif
	}else{
		qspi_send_xx_data(wbuf, 1);
	}
}

int qspi_gd25q512mc_block_lock(u32 offs)
{
	int ret=0;

	ret = qspi_gd25q512mc_wps_op(1);
	if(0 != ret)
	{
		printf("%s: enable wps failed!\n", __func__);
		return ret;
	}

	ret = qspi_gd25q512mc_read_block_lock_status(offs);

	if(BLOCK_PROTECTED == ret)
	{
		return ENOERR; 
	}

	qspi_gd25q512mc_send_lock_cmd(WPS_BLOCK_LOCK, offs, 1);

	return ENOERR;
}

int qspi_gd25q512mc_block_unlock(u32 offs)
{
	int ret=0;

	ret = qspi_gd25q512mc_wps_op(1);
	if(0 != ret)
	{
		printf("%s: enable wps failed!\n", __func__);
		return ret;
	}

	ret = qspi_gd25q512mc_read_block_lock_status(offs);

	if(BLOCK_UNPROTECTED == ret)
	{
		return ENOERR;
	}

	qspi_gd25q512mc_send_lock_cmd(WPS_BLOCK_UNLOCK, offs, 1);

	return ENOERR;
}


void qspi_gd25q512mc_global_lock(void)
{
	qspi_gd25q512mc_send_lock_cmd(WPS_GLOBAL_BLOCK_LOCK, 0, 0);
}

void qspi_gd25q512mc_global_unlock(void)
{
	qspi_gd25q512mc_send_lock_cmd(WPS_GLOBAL_BLOCK_UNLOCK, 0, 0);
}

void qspi_gd25q512mc_deep_power_down(void)
{
	u8 cmd = SPINOR_OP_DEEP_POWER_DOWN;
	qspi_send_xx_data(&cmd, 1);
}
#endif