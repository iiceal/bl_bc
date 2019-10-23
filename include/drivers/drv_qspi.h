#ifndef __FLASH_H___
#define	__FLASH_H___

#ifdef CONFIG_QSPI_TRACE
#define qspi_debug(fmt, args...)  printf("%s: %d " fmt, __func__, __LINE__, ##args)
#else
#define qspi_debug(fmt, args...)
#endif

/* Flash opcodes. */
#define SPINOR_OP_WREN		0x06	/* Write enable */
#define SPINOR_OP_WRDI		0x04	/* Write enable */
#define SPINOR_OP_RDSR		0x05	/* Read status register */
#define SPINOR_OP_WRSR		0x01	/* Write status register 1 byte */
#define SPINOR_OP_READ		0x03	/* Read data bytes (low frequency) */
#define SPINOR_OP_READ_FAST	0x0b	/* Read data bytes (high frequency) */
#define SPINOR_OP_READ_1_1_2	0x3b	/* Read data bytes (Dual SPI) */
#define SPINOR_OP_READ_1_1_4	0x6b	/* Read data bytes (Quad SPI) */
#define	SPINOR_OP_READ_DUAL_IO  0xbb    /* Read data bytes (Dual io SPI */
#define	SPINOR_OP_READ_QUAD_IO	0xeb    /* Read data bytes (Quad SPI*/
#define SPINOR_OP_PP		0x02	/* Page program (up to 256 bytes) */
#define SPINOR_OP_BE_4K		0x20	/* Erase 4KiB block */
#define SPINOR_OP_BE_4K_PMC	0xd7	/* Erase 4KiB block on PMC chips */
#define SPINOR_OP_BE_32K	0x52	/* Erase 32KiB block */
#define SPINOR_OP_CHIP_ERASE	0xc7	/* Erase whole flash chip */
#define SPINOR_OP_SE		0xd8	/* Sector erase (usually 64KiB) */
#define SPINOR_OP_RD_DEVID	0x90	/* Read Manu ID/ DEV ID */
#define SPINOR_OP_RDID		0x9f	/* Read JEDEC ID */
#define SPINOR_OP_RDCR		0x35	/* Read configuration register */
#define SPINOR_OP_RDFSR		0x70	/* Read flag status register */
#define SPINOR_OP_RESET          0x99
#define SPINOR_OP_ENABLE_RESET   0x66

struct qspi_fl_info{
    char name[16];
    u32  jedec;         /*0x010220 s25fl512s; 0xc84020 gd*/ 
    u32  blk_len;
	u32  blk_num;

    int  (*read_rdid)(u8 rdid_cmd, int rdid_len, u8 * dbuf);
    int  (*read_devid)(u8 devid_cmd, int devid_len, u8 * dbuf);
    int  (*read_page)(u8 cmd, u32 addr, u8 * rbuf, int len, int dfs);
    int  (*write_page)(u32 addr, u8 * dbuf, int len);
#ifdef CONFIG_QSPI_DMA_TRANSFER
    int  (*dma_write_page)(u32 addr, u8 * dbuf, int len, int dfs);
#endif
    int   (*erase)(u32 addr);
    int   (*erase_chip)(void);

    int (*config_4byte_extend)(u32 addr, int en);

    int  (*config_quad)(int en);
    int  (*config_prot_region)(int region, int en);
    u8   (*read_status)(void);
    int  (*reset)(void);
    int (*block_lock)(u32 offs);
    int (*block_unlock)(u32 offs);
    int (*get_block_lock_status)(u32 offs);
    int (*global_lock)(void);
    int (*global_unlock)(void);
    int (*deep_power_down)(void);
};

#define QSPI_HW_STAT

#define QSPI_OP_SUCCESS     0
#define QSPI_OP_FAILED      -1

extern struct qspi_fl_info *fl_dev;
extern void *fl_rescan(void);

#ifdef CONFIG_QSPI_DMA_TRANSFER
int fl_dma_read(int rd_cmd, u32 fl_addr, u8 *buf, u32 len, u32 dump_regs);
#endif

bool init_spinor(void);

int spinor_hw_read_page(u32 fl_addr, u8* rbuf, int len);
int fl_ahbread(int rd_cmd, u32 fl_addr, u8 *buf, u32 len, u32 dump_regs, u32 dfs);
int fl_upgrade_if(u32 fl_addr, u32 mem_addr, u32 len);

#endif //__FLASH_H___
