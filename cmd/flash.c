#include <common.h>
#include <drivers/drv_qspi.h>
#include "regs/qspi.h"
#include "cmd_qspi.h"

extern void qspi_ahb_write_enable(u8 wren_cmd);
extern void qspi_init_low(int irq_enable, int disp_log, int rd_thr_en);

int fl_performance_dmaread(void);
#define BUF_NUM (2048)
static u8 dbuf[BUF_NUM];
static u32 qspi_controller_init = 0;

void fl_get_parse_parmeter(int test_type, int *rd_cmd, int *quad_en)
{
    switch (test_type) {
    case QSPI_TEST_READ:
    case QSPI_TEST_AHB_READ:
    case QSPI_TEST_DMA_READ:
        *rd_cmd = SPINOR_OP_READ;
        break;
    case QSPI_TEST_READ_FAST:
    case QSPI_TEST_AHB_READ_FAST:
    case QSPI_TEST_DMA_READ_FAST:
        *rd_cmd = SPINOR_OP_READ_FAST;
        break;
    case QSPI_TEST_READ_DUAL:
    case QSPI_TEST_AHB_READ_DUAL:
    case QSPI_TEST_DMA_READ_DUAL:
        *rd_cmd = SPINOR_OP_READ_1_1_2;
        break;
    case QSPI_TEST_READ_QUAD:
    case QSPI_TEST_AHB_READ_QUAD:
    case QSPI_TEST_DMA_READ_QUAD:
        *rd_cmd = SPINOR_OP_READ_1_1_4;
        *quad_en = 1;
        break;
    case QSPI_TEST_READ_DUAL_IO:
    case QSPI_TEST_AHB_READ_DUAL_IO:
    case QSPI_TEST_DMA_READ_DUAL_IO:
        *rd_cmd = SPINOR_OP_READ_DUAL_IO;
        break;

    case QSPI_TEST_READ_QUAD_IO:
    case QSPI_TEST_AHB_READ_QUAD_IO:
    case QSPI_TEST_DMA_READ_QUAD_IO:
        *rd_cmd = SPINOR_OP_READ_QUAD_IO;
        *quad_en = 1;
        break;
    default:
        break;
    }
}

int fl_get_test_type(u32 argc, const char **argv)
{
    if (argc < 2)
        return QSPI_TEST_UNKNOWN;

    if (strcmp(argv[1], "init") == 0)
        return QSPI_TEST_INIT;

    if (strcmp(argv[1], "reset") == 0)
        return QSPI_TEST_RESET;

    if (strcmp(argv[1], "readid") == 0)
        return QSPI_TEST_READ_ID;

    if (strcmp(argv[1], "read") == 0)
        return QSPI_TEST_READ;

    if (strcmp(argv[1], "readfast") == 0)
        return QSPI_TEST_READ_FAST;

    if (strcmp(argv[1], "readdual") == 0)
        return QSPI_TEST_READ_DUAL;

    if (strcmp(argv[1], "readquad") == 0)
        return QSPI_TEST_READ_QUAD;

    if (strcmp(argv[1], "readdualio") == 0)
        return QSPI_TEST_READ_DUAL_IO;

    if (strcmp(argv[1], "readquadio") == 0)
        return QSPI_TEST_READ_QUAD_IO;

    if (strcmp(argv[1], "ahbread") == 0)
        return QSPI_TEST_AHB_READ;

    if (strcmp(argv[1], "ahbreadfast") == 0)
        return QSPI_TEST_AHB_READ_FAST;

    if (strcmp(argv[1], "ahbreaddual") == 0)
        return QSPI_TEST_AHB_READ_DUAL;

    if (strcmp(argv[1], "ahbreadquad") == 0)
        return QSPI_TEST_AHB_READ_QUAD;

    if (strcmp(argv[1], "ahbreaddualio") == 0)
        return QSPI_TEST_AHB_READ_DUAL_IO;

    if (strcmp(argv[1], "ahbreadquadio") == 0)
        return QSPI_TEST_AHB_READ_QUAD_IO;

    if (strcmp(argv[1], "dmaread") == 0)
        return QSPI_TEST_DMA_READ;

    if (strcmp(argv[1], "dmareadfast") == 0)
        return QSPI_TEST_DMA_READ_FAST;

    if (strcmp(argv[1], "dmareaddual") == 0)
        return QSPI_TEST_DMA_READ_DUAL;

    if (strcmp(argv[1], "dmareadquad") == 0)
        return QSPI_TEST_DMA_READ_QUAD;

    if (strcmp(argv[1], "dmareaddualio") == 0)
        return QSPI_TEST_DMA_READ_DUAL_IO;

    if (strcmp(argv[1], "dmareadquadio") == 0)
        return QSPI_TEST_DMA_READ_QUAD_IO;

    if (strcmp(argv[1], "program") == 0)
        return QSPI_TEST_PROGRAM;

    if (strcmp(argv[1], "dmaprogram") == 0)
        return QSPI_TEST_DMA_PROGRAM;

    if (strcmp(argv[1], "ahbprogram") == 0)
        return QSPI_TEST_AHB_PROGRAM;

    if (strcmp(argv[1], "erase") == 0)
        return QSPI_TEST_ERASE_SECT;

    if (strcmp(argv[1], "eraseall") == 0)
        return QSPI_TEST_ERASE_ALL;

    if (strcmp(argv[1], "protect") == 0)
        return QSPI_TEST_PROTECT_SECT;

    if (strcmp(argv[1], "upgradeflash") == 0)
        return QSPI_TEST_UPGRADE_FLASH;

    if (strcmp(argv[1], "upv") == 0)
        return QSPI_TEST_UPGRADE_VERSION;

    if (strcmp(argv[1], "auto") == 0)
        return QSPI_TEST_AUTO;

    if (strcmp(argv[1], "prs") == 0)
        return QSPI_TEST_ALL;

    if (strcmp(argv[1], "perf") == 0)
        return QSPI_PERF;

    if (strcmp(argv[1], "lock") == 0)
        return QSPI_TEST_BLOCK_LOCK;

    if (strcmp(argv[1], "unlock") == 0)
        return QSPI_TEST_BLOCK_UNLOCK;

    if (strcmp(argv[1], "lockstatus") == 0)
        return QSPI_GET_LOCK_STATUS;

    if (strcmp(argv[1], "glock") == 0)
        return QSPI_GLOBAL_LOCK;

    if (strcmp(argv[1], "gunlock") == 0)
        return QSPI_GLOBAL_UNLOCK;

    if (strcmp(argv[1], "pdown") == 0)
        return QSPI_TEST_POWER_DOWN;

    return QSPI_TEST_UNKNOWN;
}

int qspi_ctroller_low_init(u32 argc, const char **argv)
{
    int irq_mode = 0, disp_log = 0, rd_thr_en = 0;

    qspi_controller_init = 1;

#ifdef CONFIG_SUPPORT_FLASH_XIP
    return RET_PASS;
#endif
    if (argc > 2 && (strcmp(argv[2], "irq") == 0))
        irq_mode = 1;

    if (argc > 3 && (strcmp(argv[3], "log") == 0))
        disp_log = 1;

    if (argc > 4)
        rd_thr_en = simple_strtoul(argv[4], NULL, 10);

    qspi_init_low(irq_mode, disp_log, rd_thr_en);
    return RET_PASS;
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

int fl_auto_verify(void)
{
    int i, ret = 0;

    ret = fl_dev->config_prot_region(0, 0);
    if(ret != 0)
        return RET_FAIL;

    printf("1. ######read page 0 ...\n");
    fl_read(SPINOR_OP_READ, 0, dbuf, 256, 0, 8);

    printf("2. ######erase addr 0 ...");
    if(fl_dev->erase(0) != 0)
       return RET_FAIL;

    printf("3. ###### read page 0 ...\n");
    fl_read(SPINOR_OP_READ, 0, dbuf, 256, 0, 8);

    printf("4. ###### verify erase data");
    for(i = 0; i< 256; i++) {
        if(dbuf[i] != 0xff)
            return RET_FAIL;
    }        
    printf(" Pass\n");

    printf("5. ###### write page 0 ...\n");
    for (i = 0; i < 256; i++) {
        dbuf[i] = (i+128) & 0xff;
    }

    if(fl_dev->write_page(0, dbuf, 256) != 0)
        return RET_FAIL;

    memset(dbuf, 0 ,256);

    printf("6. ###### read page 0 ...");
    fl_read(SPINOR_OP_READ, 0, dbuf, 256, 0, 8);

    printf("7. ###### verify write page \n");
    for(i = 0; i< 256; i++) {
        if(dbuf[i] != ((i+128) & 0xff))
            return RET_FAIL;
    }        
    printf(" Pass\n");

    return RET_PASS;
}

int fl_all_verify(void);

int fl_cmd_entry_funcing(u32 argc, const char **argv)
{
    unsigned long long s, e;
    unsigned int us, addr, len, dump_regs, wdata, dfs;
    int quad_enable, rd_cmd, i;
    int test_type = fl_get_test_type(argc, argv);

    if (test_type == QSPI_TEST_UNKNOWN)
        return RET_NA;

#ifndef CONFIG_SUPPORT_FLASH_XIP
    if (qspi_controller_init == 0) {
        qspi_controller_init = 1;
        qspi_init_low(0, 0, 0); /*disable all interrupt ,and polling regsiter is isr, nor risr*/
    }
#else
    printf("qspi is in xip mode\n");
#endif
    fl_dev = fl_rescan();
    if (!fl_dev)
        return RET_NA;

    switch (test_type) {
    case QSPI_TEST_INIT:
        return qspi_ctroller_low_init(argc, argv);
    case QSPI_TEST_RESET:
        return fl_reset();
    case QSPI_TEST_READ_ID:
        return fl_read_id();
    case QSPI_TEST_PROTECT_SECT:
        return fl_protect(argc, argv);
    case QSPI_TEST_AUTO:
        return fl_auto_verify();
    case QSPI_TEST_ALL:
        return fl_all_verify();
    case QSPI_PERF:
        return fl_performance_dmaread();
    default:
        break;
    }

    addr = 0x0;
    len = 256;
    us = 0;
    dump_regs = 0;
    quad_enable = 0;
    wdata = 0xffffffff;
    rd_cmd = 0;
    dfs = 8;

    memset(dbuf, 0, 256);

    fl_get_parse_parmeter(test_type, &rd_cmd, &quad_enable);
    if (argc > 2)
        addr = simple_strtoul(argv[2], NULL, 16);

    if (argc > 3)
        len = simple_strtoul(argv[3], NULL, 10);

    if (argc > 4)
        dump_regs = simple_strtoul(argv[4], NULL, 10);

    if (argc > 5) {
        wdata = simple_strtoul(argv[5], NULL, 16);
        dfs = wdata & 0xff;
    }

    printf("addr = 0x%x, len = %d, dfs = %d\n", addr, len, dfs);

    if(fl_dev->config_quad(quad_enable) != 0)
        return RET_FAIL;

    fl_dev->config_4byte_extend(addr, (addr & 0x3000000));

    s = timer_get_tick();

    switch (test_type) {
    case QSPI_TEST_UPGRADE_VERSION:
        return fl_upgrade_if(addr, wdata, len);
    case QSPI_TEST_READ:
    case QSPI_TEST_READ_FAST:
    case QSPI_TEST_READ_DUAL:
    case QSPI_TEST_READ_DUAL_IO:
    case QSPI_TEST_READ_QUAD:
    case QSPI_TEST_READ_QUAD_IO:
        return fl_read(rd_cmd, addr, dbuf, len, dump_regs, dfs);

    case QSPI_TEST_AHB_READ:
    case QSPI_TEST_AHB_READ_FAST:
    case QSPI_TEST_AHB_READ_DUAL:
    case QSPI_TEST_AHB_READ_DUAL_IO:
    case QSPI_TEST_AHB_READ_QUAD:
    case QSPI_TEST_AHB_READ_QUAD_IO:
        return fl_ahbread(rd_cmd, addr, dbuf, len, dump_regs, dfs);

#ifdef CONFIG_QSPI_DMA_TRANSFER
    case QSPI_TEST_DMA_READ:
    case QSPI_TEST_DMA_READ_FAST:
    case QSPI_TEST_DMA_READ_DUAL:
    case QSPI_TEST_DMA_READ_DUAL_IO:
    case QSPI_TEST_DMA_READ_QUAD:
    case QSPI_TEST_DMA_READ_QUAD_IO:
        printf("\nQSPI_TEST_DMA_READ*\n");
        fl_dma_read(rd_cmd, addr, dbuf, len, dump_regs);
        break;
#endif
#ifdef CONFIG_QSPI_DMA_TRANSFER
    case QSPI_TEST_DMA_PROGRAM:
        for (i = 0; i < 256; i++) {
            dbuf[i] = 255 - i;
        }
        s = timer_get_tick();
        // fl_dev->write_page(addr, dbuf, len);
        printf("\nQSPI_TEST_DMA_PROGRAM\n");
        // return fl_dma_write_page(addr, dbuf, len);
        fl_dev->dma_write_page(addr, dbuf, len, dfs);
        break;
#endif
    case QSPI_TEST_PROGRAM:
        for (i = 0; i < 256; i++) {
            dbuf[i] = 255 - i;
        }
        s = timer_get_tick();
        fl_dev->write_page(addr, dbuf, len);
        break;
    case QSPI_TEST_AHB_PROGRAM:
        qspi_ahb_write_enable(SPINOR_OP_WREN);
        break;
    case QSPI_TEST_ERASE_SECT:
        fl_dev->erase(addr);
        break;
    case QSPI_TEST_ERASE_ALL:
        fl_dev->erase_chip();
        break;

    case QSPI_TEST_BLOCK_LOCK:
        if(fl_dev->block_lock)
        {
            fl_dev->block_lock(addr);
        }else{
            printf("NOT support block lock!");
        }
        break;

    case QSPI_TEST_BLOCK_UNLOCK:
        if(fl_dev->block_unlock)
        {
            fl_dev->block_unlock(addr);
        }else{
            printf("NOT support block unlock!");
        }
        break;

    case QSPI_GET_LOCK_STATUS:
        if(fl_dev->get_block_lock_status)
        {
            fl_dev->get_block_lock_status(addr);
        }else{
            printf("NOT support get block status!");

        }
        break;

    case QSPI_GLOBAL_LOCK:
        if(fl_dev->global_lock)
        {
            fl_dev->global_lock();
        }else{
            printf("NOT support global lock!");
        }
        break;

    case QSPI_GLOBAL_UNLOCK:
        if(fl_dev->global_unlock)
        {
            fl_dev->global_unlock();
        }else{
            printf("NOT support global unlock!");
        }
        break;

    case QSPI_TEST_POWER_DOWN:
        if(fl_dev->deep_power_down)
        {
            fl_dev->deep_power_down();
        }else{
            printf("NOT support deep power dowm command!");
        }
        break;
    }

    e = timer_get_tick();
    us += tick_to_us((unsigned long) (e - s));
    printf("Test need %d us (%d ms)\n", us, us / 1000);
    return RET_PASS;
}

#define SRAM_FLASH_WRITE_TEST_ADDR      (U32)0X200000
#define SRAM_FLASH_READ_TEST_ADDR       (U32)0X280000
u8 *wrbuf = (u8 *) (SRAM_FLASH_WRITE_TEST_ADDR);
u8 *frdbuf = (u8 *) (SRAM_FLASH_READ_TEST_ADDR);

#define TEST_DIV	(16)

int fl_all_verify(void)
{
    int i, j, ret = 0;
    unsigned int chip_size=0;

    ret |= fl_dev->config_prot_region(15, 0);
    if(ret != 0)
        return RET_FAIL;

    printf("erase all chip sector by sector!\n");
    printf("block num: %d, block size: %d\n", fl_dev->blk_num, fl_dev->blk_len); 
    for(i=0; i<fl_dev->blk_num; i++)
    {
        fl_dev->config_4byte_extend(i* fl_dev->blk_len, (i* fl_dev->blk_len)&0x3000000);
        fl_dev->erase(i* fl_dev->blk_len);
        if(!(i%10))
            printf("erase block %d done!\n", i);
    }

    printf("check erase result:\n");

    chip_size = fl_dev->blk_len*fl_dev->blk_num;
    for(i=0; i<chip_size;)
    {
        fl_dev->config_4byte_extend(i, i&0x3000000);
        fl_dev->read_page(SPINOR_OP_READ, i, frdbuf, 256, 8);

        for(j=0; j<256; j++)
        {
            if(0xff != frdbuf[j])
            {
                printf("erase failed! pos 0x%x value 0x%x\n", i+j, frdbuf[j]); 
                return RET_FAIL;
            }
        }
        i+=256;

        if(!(i%262144))
            printf(".");
    }

    printf("done!\n");
    printf("write all chip to 0, check if corrected!\n");

    memset(wrbuf, 0, 1024);
    for(i=0; i<chip_size;)
    {
        fl_dev->config_4byte_extend(i, i&0x3000000);
        fl_dev->write_page(i, wrbuf, 256);
        i+=256;
        if(!(i%262144))
            printf(".");
    }
    printf("write done!\n");

    for(i=0; i<chip_size;)
    {
        fl_dev->config_4byte_extend(i, i&0x3000000);
        fl_dev->read_page(SPINOR_OP_READ, i, frdbuf, 256, 8);

        for(j=0; j<256; j++)
        {
            if(0 != frdbuf[j])
            {
                printf("erase failed! pos 0x%x value 0x%x\n", i+j, frdbuf[j]); 
                return RET_FAIL;
            }
        }

        i+=256;

        if(!(i%262144))
            printf(".");
    }
    printf("check done!\n");

    printf("write all chip to 0x55, check if corrected!\n");
    for(i=0; i<fl_dev->blk_num; i++)
    {
        fl_dev->config_4byte_extend(i* fl_dev->blk_len, (i* fl_dev->blk_len)&0x3000000);
        fl_dev->erase(i* fl_dev->blk_len);
        if(!(i%10))
            printf("erase block %d done!\n", i);
    }

    memset(wrbuf, 0x55, 1024);
    for(i=0; i<chip_size/TEST_DIV;)
    {
        fl_dev->config_4byte_extend(i, i&0x3000000);
        fl_dev->write_page(i, wrbuf, 256);
        i+=256;

        if(!(i%262144))
            printf(".");
    }
    printf("write done!\n");

    for(i=0; i<chip_size/TEST_DIV;)
    {
        fl_dev->config_4byte_extend(i, i&0x3000000);
        fl_dev->read_page(SPINOR_OP_READ, i, frdbuf, 256, 8);

        for(j=0; j<256; j++)
        {
            if(0x55 != frdbuf[j])
            {
                printf("erase failed! pos 0x%x value 0x%x\n", i+j, frdbuf[j]); 
                return RET_FAIL;
            }
        }

        i+=256;

        if(!(i%262144))
            printf(".");
    }
    printf("check done!\n");

    printf("write all chip to 0xaa, check if corrected!\n");

    for(i=0; i<fl_dev->blk_num; i++)
    {
        fl_dev->config_4byte_extend(i* fl_dev->blk_len, (i* fl_dev->blk_len)&0x3000000);
        fl_dev->erase(i* fl_dev->blk_len);
        if(!(i%10))
            printf("erase block %d done!\n", i);
    }

    memset(wrbuf, 0xaa, 1024);
    for(i=0; i<chip_size/TEST_DIV;)
    {
        fl_dev->config_4byte_extend(i, i&0x3000000);
        fl_dev->write_page(i, wrbuf, 256);
        i+=256;
        if(!(i%262144))
            printf(".");
    }
    printf("write done!\n");

    for(i=0; i<chip_size/TEST_DIV;)
    {
        fl_dev->config_4byte_extend(i, i&0x3000000);
        fl_dev->read_page(SPINOR_OP_READ, i, frdbuf, 256, 8);

        for(j=0; j<256; j++)
        {
            if(0xaa != frdbuf[j])
            {
                printf("erase failed! pos 0x%x value 0x%x\n", i+j, frdbuf[j]); 
                return RET_FAIL;
            }
        }

        i+=256;
        if(!(i%262144))
            printf(".");
    }
    printf("check done!\n");

    memset(wrbuf, 0x51, 1024);
    fl_dev->erase(0);
    fl_dev->write_page(500, wrbuf, 500);
    fl_dev->read_page(SPINOR_OP_READ, 500, frdbuf, 500, 8);

    return RET_PASS;
}

int vvflag = 0;
int fl_performance_dmaread(void)
{
	unsigned int chip_size=0;
    unsigned long long s, e;
    u32 us=0;
    int i=0;
    int cmd = SPINOR_OP_READ_QUAD_IO;
    int dfs = 32;

    if(fl_dev->config_quad(1) != 0)
        return -1;

    //printf("############## dmaread cmd 0x%x dfs %d ####################\n", cmd, dfs);
	chip_size = fl_dev->blk_len*fl_dev->blk_num;

	for(i=0; i<chip_size;)
	{
        if((0==i) || (i== 0x1000000) || (i==0x3000000))
        {
		    fl_dev->config_4byte_extend(i, i&0x3000000);
            vvflag = 0;
        }

        s = timer_get_tick();
        fl_dma_read(cmd, i, frdbuf, 256, 0);
        e = timer_get_tick();
		i+=256;

        us += tick_to_us((unsigned long) (e - s));
	}

    //us = tick_to_us((unsigned long) (e - s));
    printf("###### dmaread cmd 0x%x dfs=%d all chip need: %d us (%d ms)\n\n", cmd, dfs, us, us / 1000);
    return 0;
}

