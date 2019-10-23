#include <common.h>
#include <drivers/drv_qspi.h>
#include "fl_common_cmd.h"
#include "regs/qspi.h"

extern void qspi_init_low(int irq_enable, int disp_log, int rd_thr_en);

int qspi_fl_read_rdid(u8 rdid_cmd, int rdid_len, u8 *dbuf);
int qspi_fl_read_devid(u8 devid_cmd, int devid_len, u8 *dbuf);
int qspi_fl_read_page(u8 cmd, u32 addr, u8 *rbuf, int len, int dfs);
#ifdef CONFIG_QSPI_DMA_TRANSFER
int qspi_dma_read_page(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len);
int qspi_s25fl512s_dma_write_page(u32 addr, u8 *dbuf, int len, int dfs);
int qspi_w25q256fv_dma_write_page(u32 addr, u8 *dbuf, int len, int dfs);
int qspi_gd25q512mc_dma_write_page(u32 addr, u8 *dbuf, int len, int dfs);
int qspi_gd25q256d_dma_write_page(u32 addr, u8 *dbuf, int len, int dfs);
#endif

#ifdef CONFIG_QSPI_S25FL512S
int qspi_s25fl512s_write_page(u32 addr, u8 *dbuf, int len);
int qspi_s25fl512s_erase(u32 addr);
int qspi_s25fl512s_erase_all(void);
int qspi_s25fl512s_4byte_extend(u32 addr, int en);
int qspi_s25fl512s_quad(int en);
int qspi_s25fl512s_prot_region(int region, int en);
u8 qspi_s25fl512s_read_status(void);
int qspi_s25fl512s_reset(void);
int qspi_s25fl512s_block_lock(u32 addr);
int qspi_s25fl512s_block_unlock(u32 addr);
int qspi_s25fl512s_read_block_status(u32 addr);
#endif

#ifdef CONFIG_QSPI_W25Q256FV
int qspi_w25q256fv_write_page(u32 addr, u8 *dbuf, int len);
int qspi_w25q256fv_erase(u32 addr);
int qspi_w25q256fv_erase_all(void);
int qspi_w25q256fv_4byte_extend(u32 addr, int en);
int qspi_w25q256fv_quad(int en);
int qspi_w25q256fv_prot_region(int region, int en);
u8 qspi_w25q256fv_read_status(void);
int qspi_w25q256fv_reset(void);
int qspi_w25q256fv_block_lock(u32 offs);
int qspi_w25q256fv_block_unlock(u32 offs);
int qspi_w25q256fv_read_block_lock_status(u32 offs);
int qspi_w25q256fv_global_lock(void);
int qspi_w25q256fv_global_unlock(void);
int qspi_w25q256fv_deep_power_down(void);
#endif

#ifdef CONFIG_QSPI_GD25Q512MC
int qspi_gd25q512mc_write_page(u32 addr, u8 *dbuf, int len);
int qspi_gd25q512mc_erase(u32 addr);
int qspi_gd25q512mc_erase_all(void);
int qspi_gd25q512mc_4byte_extend(u32 addr, int en);
int qspi_gd25q512mc_quad(int en);
int qspi_gd25q512mc_prot_region(int region, int en);
u8 qspi_gd25q512mc_read_status(void);
int qspi_gd25q512mc_reset(void);
int qspi_gd25q512mc_block_lock(u32 offs);
int qspi_gd25q512mc_block_unlock(u32 offs);
int qspi_gd25q512mc_read_block_lock_status(u32 offs);
int qspi_gd25q512mc_global_lock(void);
int qspi_gd25q512mc_global_unlock(void);
int qspi_gd25q512mc_deep_power_down(void);
#endif

#ifdef CONFIG_QSPI_GD25Q256D
int qspi_gd25q256d_write_page(u32 addr, u8 *dbuf, int len);
int qspi_gd25q256d_erase(u32 addr);
int qspi_gd25q256d_erase_all(void);
int qspi_gd25q256d_4byte_extend(u32 addr, int en);
int qspi_gd25q256d_quad(int en);
int qspi_gd25q256d_prot_region(int region, int en);
u8 qspi_gd25q256d_read_status(void);
int qspi_gd25q256d_reset(void);
int qspi_gd25q256d_block_lock(u32 offs);
int qspi_gd25q256d_block_unlock(u32 offs);
int qspi_gd25q256d_deep_power_down(void);
#endif

struct qspi_fl_info fl_info[] = {
#ifdef CONFIG_QSPI_S25FL512S
    { 
      "s25fl512s",
      0x010220,
      256*1024,
	  256,
      qspi_fl_read_rdid,
      qspi_fl_read_devid,
      qspi_fl_read_page,
      qspi_s25fl512s_write_page,
#ifdef CONFIG_QSPI_DMA_TRANSFER
      qspi_s25fl512s_dma_write_page,
#endif
      qspi_s25fl512s_erase,
      qspi_s25fl512s_erase_all,
      qspi_s25fl512s_4byte_extend,
      qspi_s25fl512s_quad,
      qspi_s25fl512s_prot_region,
      qspi_s25fl512s_read_status,
      qspi_s25fl512s_reset,
	  qspi_s25fl512s_block_lock, 
	  qspi_s25fl512s_block_unlock,
	  qspi_s25fl512s_read_block_status, 
	  NULL, 
	  NULL,
	  NULL
    },
#endif
#ifdef CONFIG_QSPI_W25Q256FV
    { 
      "w25q256fv",
      0xef4019,  //0x186019
      64*1024,
	  512,
      qspi_fl_read_rdid,
      qspi_fl_read_devid,
      qspi_fl_read_page,
      qspi_w25q256fv_write_page,
#ifdef CONFIG_QSPI_DMA_TRANSFER
      qspi_w25q256fv_dma_write_page,
#endif
      qspi_w25q256fv_erase,
      qspi_w25q256fv_erase_all,
      qspi_w25q256fv_4byte_extend,
      qspi_w25q256fv_quad,
      qspi_w25q256fv_prot_region,
      qspi_w25q256fv_read_status,
      qspi_w25q256fv_reset,
	  qspi_w25q256fv_block_lock,
	  qspi_w25q256fv_block_unlock,
	  qspi_w25q256fv_read_block_lock_status,
	  qspi_w25q256fv_global_lock,
	  qspi_w25q256fv_global_unlock,
	  qspi_w25q256fv_deep_power_down
    },
#endif
#ifdef CONFIG_QSPI_GD25Q512MC
    { 
      "gd25q512mc",
      0xc84020, //0xc84019
      64*1024,
	  1024,
      qspi_fl_read_rdid,
      qspi_fl_read_devid,
      qspi_fl_read_page,
      qspi_gd25q512mc_write_page,
#ifdef CONFIG_QSPI_DMA_TRANSFER
      qspi_gd25q512mc_dma_write_page,
#endif
      qspi_gd25q512mc_erase,
      qspi_gd25q512mc_erase_all,
      qspi_gd25q512mc_4byte_extend,
      qspi_gd25q512mc_quad,
      qspi_gd25q512mc_prot_region,
      qspi_gd25q512mc_read_status,
      qspi_gd25q512mc_reset,
	  qspi_gd25q512mc_block_lock, 
	  qspi_gd25q512mc_block_unlock,
	  qspi_gd25q512mc_read_block_lock_status, 
	  qspi_gd25q512mc_global_lock,
	  qspi_gd25q512mc_global_unlock,
	  qspi_gd25q512mc_deep_power_down
    },
#endif
#ifdef CONFIG_QSPI_GD25Q256D
    { 
      "gd25q256d",
      0xc84019, 
      64*1024,
	  512,
      qspi_fl_read_rdid,
      qspi_fl_read_devid,
      qspi_fl_read_page,
      qspi_gd25q256d_write_page,
#ifdef CONFIG_QSPI_DMA_TRANSFER
      qspi_gd25q256d_dma_write_page,
#endif
      qspi_gd25q256d_erase,
      qspi_gd25q256d_erase_all,
      qspi_gd25q256d_4byte_extend,
      qspi_gd25q256d_quad,
      qspi_gd25q256d_prot_region,
      qspi_gd25q256d_read_status,
      qspi_gd25q256d_reset,
	  NULL,
	  NULL,
	  NULL,
	  NULL,
	  NULL,
	  qspi_gd25q256d_deep_power_down
    },
#endif
    {"", 0, 0, 0, NULL, NULL, NULL,NULL, NULL, NULL, 
		NULL, NULL, NULL, NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL},
};

struct qspi_fl_info *fl_dev = NULL; //&fl_info[0];

#define BUF_NUM (2048)
static u8 dbuf[BUF_NUM];

void* fl_rescan(void)
{
    u32 jedid = 0;
    struct qspi_fl_info *fl = &fl_info[0];
    int fl_count = sizeof(fl_info) / sizeof(fl_info[0]), i;

    if (fl_dev != NULL)
        return (void *)fl_dev;

    qspi_fl_read_rdid(SPINOR_OP_RDID, 3, dbuf);
    jedid = (dbuf[0] << 16) | (dbuf[1] << 8) | (dbuf[2]);
    qspi_debug("jedid = 0x%x, support device number :%d\n", jedid, fl_count);

    for (i = 0; i < fl_count; i++) {
        if (fl->blk_len == 0)
            continue;

        if (fl->jedec == jedid) {
            fl_dev = fl;
            qspi_debug("fl_rescan device match ok, and name: %s\n", fl_dev->name);
            return  (void *)fl_dev;
        }
        fl++;
    }
    printf("fl_rescan device match failed\n");
    return NULL;
}

/*
 *  0x66 + 0x99 or 0xf0
 */
int fl_reset(void)
{
    fl_dev->reset();
    printf("reset ...\n");
    return RET_PASS;
}

int fl_read_id(void)
{
    int i;
    u32  jeded;
    fl_dev->read_devid(SPINOR_OP_RD_DEVID, 2, dbuf);

    printf("read id             : %02x %02x\n", dbuf[0], dbuf[1]);

    fl_dev->read_rdid(SPINOR_OP_RDID, 3, dbuf);
    printf("rd id(JEDED)        : ");
    for (i = 0; i < 3; i++)
        printf("%02x", dbuf[i]);
    printf("\n");
    jeded = dbuf[2] + (dbuf[1] << 8) + (dbuf[0] << 16);
    fl_dev->read_status();
    if(fl_dev->jedec == jeded)
        return RET_PASS;

     return RET_FAIL;
}

int fl_protect(u32 argc, const char **argv)
{
    unsigned long long s, e;
    unsigned int addr = 0, us = 0;

    int prot = 0, ret;

    if (argc > 2) {
        if (strcmp(argv[2], "disable") == 0)
            prot = 0;

        if (strcmp(argv[2], "enable") == 0)
            prot = 1;
    }
    if (argc > 3)
        addr = simple_strtoul(argv[3], NULL, 10);

    addr = addr & 0xf;
    printf("region = %d %s\n", addr, prot > 0 ? "enable" : "disable");
    s = timer_get_tick();
    ret = fl_dev->config_prot_region(addr, prot);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    printf("Test need %d us (%d ms)\n", us, us / 1000);
    if(ret == 0)
        return RET_PASS;
    
     return RET_FAIL;
}

int fl_read(int rd_cmd, u32 addr, u8 *buf, u32 len, u32 dump_regs, int dfs)
{
    unsigned long long s, e;
    unsigned int us;
    int rx_count;
#ifdef QSPI_HW_STAT
    unsigned int us_spi;
    unsigned long long e_spi;
    extern u64 qspi_get_timer_tick_save(void);
#endif

    printf("cmd %02x \n", rd_cmd);
    s = timer_get_tick();
    rx_count = fl_dev->read_page(rd_cmd, addr, buf, len, dfs);
    if (dump_regs > 0)
        printf_buf_fmt_32bit((u32 *) hwp_apQspi, (u32) hwp_apQspi, 24);
    e = timer_get_tick();
    printf_buf(buf, rx_count);
#ifdef QSPI_HW_STAT
    e_spi = qspi_get_timer_tick_save();
    us_spi = tick_to_us((unsigned long) (e_spi - s));
    printf("spi need %d\n", us_spi);
#endif
    us = tick_to_us((unsigned long) (e - s));
    printf("Test need %d us (%d ms)\n", us, us / 1000);
    return 0;
}

#ifdef CONFIG_QSPI_DMA_TRANSFER
int fl_dma_read(int rd_cmd, u32 fl_addr, u8 *buf, u32 len, u32 dump_regs)
{
#ifdef CONFIG_QSPI_TRACE
    unsigned long long s, e;
    unsigned int us;
    printf("dma mode:cmd %02x\n", rd_cmd);
    s = timer_get_tick();
#endif
    int rx_count;

    if(len > 256)
        return 0;

    rx_count = qspi_dma_read_page(rd_cmd, fl_addr, buf, len);
#ifdef CONFIG_QSPI_TRACE
    if (dump_regs > 0)
        printf_buf_fmt_32bit((u32 *) hwp_apQspi, (u32) hwp_apQspi, 36);
    e = timer_get_tick();
    printf_buf(buf, rx_count);
    us = tick_to_us((unsigned long) (e - s));
    printf("Test need %d us (%d ms)\n", us, us / 1000);
#endif
    if(rx_count < 0)
        return QSPI_OP_FAILED;

    return RET_PASS;
}
#endif

extern void qspi_ahb_read_enable(u8 rd_cmd, u8 dfs);
int fl_ahbread(int rd_cmd, u32 fl_addr, u8 *buf, u32 len, u32 dump_regs, u32 dfs)
{
    unsigned long long s, e;
    unsigned int us;

    printf("cmd %02x , %d\n", rd_cmd, dfs);
    switch (dfs) {
    case 8:
    case 16:
    case 32:
        break;
    default:
        printf("dfs %d invalid\n", dfs);
        return RET_FAIL;
    }

    s = timer_get_tick();
    qspi_ahb_read_enable(rd_cmd, dfs);
    if (dump_regs > 0)
        printf_buf_fmt_32bit((u32 *) hwp_apQspi, (u32) hwp_apQspi, 24);

    memcpy(buf, (void *) SPIFLASH_D_BASE + fl_addr, len);
    e = timer_get_tick();

    printf_buf(buf, len);

    us = tick_to_us((unsigned long) (e - s));
    printf("Test need %d us (%d ms)\n", us, us / 1000);
    return RET_PASS;
}

#ifndef CONFIG_QSPI_DMA_TRANSFER
static u8 upgrade_version_to_flash(u32 flash_addr, u8 *buff, u32 len)
{
    U32 i, wloops, eloops;
    int length = len + 0xff;
    u8 status;

    fl_dev->config_prot_region(0, 0);

    length &= ~(0xff);
    wloops = length >> 8;
    eloops = (len + fl_dev->blk_len - 1) / fl_dev->blk_len; // 256k bytes block size aligned
    //printf("upgrade flash: page num = %d, erase blk num = %d\n", wloops, eloops);
    for (i = 0; i < eloops; i++) {
        status = fl_dev->erase(flash_addr + i * fl_dev->blk_len);
        if (status != QSPI_OP_SUCCESS)
            return -1;
    }

    for (i = 0; i < wloops; i++) {
        status = fl_dev->write_page(flash_addr + i * 256, (u8 *) (buff + i * 256), 256);
        if (status != QSPI_OP_SUCCESS)
            return -1;
    }

    return 0;
}

#else
static u8 upgrade_version_to_flash_dma(u32 flash_addr, u8 *buff, u32 len)
{
    U32 i, wloops, eloops;
    int length = len + 0xff;
    u8 status;

    fl_dev->config_prot_region(0, 0);

    length &= ~(0xff);
    wloops = length >> 8;
    eloops = (len + fl_dev->blk_len - 1) / fl_dev->blk_len; // 256k bytes block size aligned
    // printf("upgrade flash: page num = %d, erase blk num = %d\n", wloops, eloops);
    for (i = 0; i < eloops; i++) {
        status = fl_dev->erase(flash_addr + i * fl_dev->blk_len);
        if (status != QSPI_OP_SUCCESS)
            return -1;
    }

    for (i = 0; i < wloops; i++) {
        status = fl_dev->dma_write_page(flash_addr + i * 256, (u8 *) (buff + i * 256), 256, 8);
        if (status != QSPI_OP_SUCCESS)
            return -1;
    }

    return 0;
}
#endif

bool init_spinor(void)
{
    if (fl_dev == NULL) {
        fl_dev = fl_rescan();
        if (!fl_dev) {
            printf("fl_rescan failed\n");
            return false;
        }
    }
    return true;
}

int spinor_hw_read_page(u32 fl_addr, u8* rbuf, int len)
{
    return fl_dev->read_page(SPINOR_OP_READ, fl_addr, rbuf, len, 8);
}

int fl_upgrade_if(u32 fl_addr, u32 mem_addr, u32 len)
{
    if (fl_dev == NULL) {
        fl_dev = fl_rescan();
        if (!fl_dev) {
            printf("fl_upgrade failed\n");
            return RET_NA;
        }
    }

    if (fl_addr % fl_dev->blk_len) {
        printf("addr invalid. Must be aligned to %K(sector size of flash)!\n",
               fl_dev->blk_len / 1024);
        return RET_FAIL;
    }

    if ((0xFFFFFFFF == mem_addr) /*|| (0 == addr)*/) {
        printf("no enough para. Input help to get description!\n");
        return RET_FAIL;
    }

#ifdef CONFIG_QSPI_DMA_TRANSFER
    if(upgrade_version_to_flash_dma(fl_addr, (unsigned char *) mem_addr, len) < 0)
        return RET_FAIL;
#else
    if(upgrade_version_to_flash(fl_addr, (unsigned char *) mem_addr, len) < 0)
        return RET_FAIL;
#endif

    return RET_PASS;
}
