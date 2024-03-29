/*
 * (C) Copyright 2008
 * Texas Instruments, <www.ti.com>
 * MA 02111-1307 USA
 */

#ifndef MMC_H
#define MMC_H

#include "mmc_host_def.h"

/* All supported commands */
#define __CMD_DMA                   (1 << 0)
#define __CMD_BCE                   (1 << 1)
#define __CMD_ACE_12                (1 << 2)
#define __CMD_ACE_23                (2 << 2)
#define __CMD_READ                  (1 << 4)
#define __CMD_MULTI_BLK             (1 << 5)
#define __CMD_RESPONSE_136BITS      (1 << 16)
#define __CMD_RESPONSE_48BITS       (2 << 16)
#define __CMD_RESPONSE_48BITS_BUSY  (3 << 16)
#define __CMD_CRC_CHECKED           (1 << 19)
#define __CMD_INDEX_CHECKED         (1 << 20)
#define __CMD_DATA_PRESENT          (1 << 21)

#define __SD_CMD_NO_RESP(cmd)               ((cmd) << 24)
#define __SD_CMD_48BIT_RESP(cmd)            (((cmd) << 24) | __CMD_CRC_CHECKED | __CMD_INDEX_CHECKED | __CMD_RESPONSE_48BITS)
#define __SD_CMD_48BIT_RESP_BUSY(cmd)       (((cmd) << 24) | __CMD_CRC_CHECKED | __CMD_INDEX_CHECKED | __CMD_RESPONSE_48BITS_BUSY)
#define __SD_CMD_48BIT_RESP_RDATA(cmd)      (((cmd) << 24) | __CMD_CRC_CHECKED | __CMD_INDEX_CHECKED | __CMD_RESPONSE_48BITS | \
                                            __CMD_DATA_PRESENT | __CMD_READ | __CMD_ACE_12)

#define __SD_CMD_48BIT_RESP_WDATA(cmd)      (((cmd) << 24) | __CMD_CRC_CHECKED | __CMD_INDEX_CHECKED | __CMD_RESPONSE_48BITS | \
                                            __CMD_DATA_PRESENT | __CMD_ACE_12)

#define __SD_CMD_48BIT_RESP_RDMA(cmd)       (((cmd) << 24) | __CMD_CRC_CHECKED | __CMD_DMA | __CMD_BCE | __CMD_READ | \
                                            __CMD_INDEX_CHECKED | __CMD_RESPONSE_48BITS | __CMD_DATA_PRESENT | __CMD_ACE_12)

#define __SD_CMD_48BIT_RESP_WDMA(cmd)       (((cmd) << 24) | __CMD_CRC_CHECKED | __CMD_DMA | __CMD_BCE | __CMD_INDEX_CHECKED | \
                                            __CMD_RESPONSE_48BITS | __CMD_DATA_PRESENT | __CMD_ACE_12)

#define __SD_CMD_136BIT_RESP(cmd)           (((cmd) << 24) | __CMD_CRC_CHECKED | __CMD_RESPONSE_136BITS)

#define SDMMC_CMD0              __SD_CMD_NO_RESP(0)
#define SDMMC_CMD1              0x01020000
#define SDMMC_CMD2              __SD_CMD_136BIT_RESP(2)
#define SDMMC_CMD3              __SD_CMD_48BIT_RESP(3)
#define SDMMC_CMD5              0x05020000
#define SDMMC_CMD6              __SD_CMD_48BIT_RESP(6)
#define SDMMC_CMD7_DES          __SD_CMD_NO_RESP(7)
#define SDMMC_CMD7_SEL          ((7 << 24) | __CMD_CRC_CHECKED | __CMD_INDEX_CHECKED | (3 << 16))
#define SDMMC_CMD8              __SD_CMD_48BIT_RESP(8)
#define SDMMC_CMD9              __SD_CMD_136BIT_RESP(9)
#define SDMMC_CMD10             __SD_CMD_136BIT_RESP(10)
#define SDMMC_CMD12             __SD_CMD_48BIT_RESP_BUSY(12)
#define SDMMC_CMD13             __SD_CMD_48BIT_RESP(13)
#define SDMMC_CMD16             __SD_CMD_48BIT_RESP(16)
// single block read
#define SDMMC_CMD17             __SD_CMD_48BIT_RESP_RDATA(17)
#define SDMMC_CMD18             __SD_CMD_48BIT_RESP_RDATA(18)
// single block write
#define SDMMC_CMD24             __SD_CMD_48BIT_RESP_WDATA(24)
#define SDMMC_CMD25             __SD_CMD_48BIT_RESP_WDATA(25)

// dma read
#define SDMMC_CMD17_DMA         __SD_CMD_48BIT_RESP_RDMA(17)
#define SDMMC_CMD18_DMA         __SD_CMD_48BIT_RESP_RDMA(18)

// dma write
#define SDMMC_CMD24_DMA         __SD_CMD_48BIT_RESP_WDMA(24)
#define SDMMC_CMD25_DMA         __SD_CMD_48BIT_RESP_WDMA(25)

#define SDMMC_ACMD13            __SD_CMD_48BIT_RESP(13)
#define SDMMC_ACMD41            0x29020000
#define SDMMC_ACMD51            0x333A0010
#define SDMMC_CMD55             __SD_CMD_48BIT_RESP(55)

/*---------------------------------
//cmd status
#define E_OK               0
#define E_TUNING           26
#define E_ADMA             25
#define E_AUTO_CMD         24
#define E_CURRENT_LIMIT    23
#define E_DATA_END_BIT     22
#define E_DATA_CRC         21
#define E_DATA_TIMEOUT     20
#define E_COMMAND_INDEX    19
#define E_COMMAND_END_BIT  18
#define E_COMMAND_CRC      17
#define E_COMMAND_TIMEOUT  16
-----------------------------------*/

#define MMC_AC_CMD_RCA_MASK     	(unsigned int)(0xFFFF << 16)
#define MMC_BC_CMD_DSR_MASK     	(unsigned int)(0xFFFF << 16)
#define MMC_DSR_DEFAULT         	(0x0404)
#define SD_CMD8_CHECK_PATTERN		(0xAA)
#define SD_CMD8_2_7_3_6_V_RANGE		(0x01 << 8)

/* Clock Configurations and Macros */

//#define MMC_RELATIVE_CARD_ADDRESS	(0x1234)
#define MMC_RELATIVE_CARD_ADDRESS	(0x1)

#define R1CS_CURRENT_STATE_MASK		0x00001e00
#define R1CS_CURRENT_STATE(x)		(((x)&R1CS_CURRENT_STATE_MASK)>>9)

#define MMC_OCR_REG_ACCESS_MODE_MASK			(0x3 << 29)
#define MMC_OCR_REG_ACCESS_MODE_BYTE    		(0x0 << 29)
#define MMC_OCR_REG_ACCESS_MODE_SECTOR  		(0x2 << 29)

#define MMC_OCR_REG_HOST_CAPACITY_SUPPORT_MASK		(0x1 << 30)
#define MMC_OCR_REG_HOST_CAPACITY_SUPPORT_BYTE		(0x0 << 30)
#define MMC_OCR_REG_HOST_CAPACITY_SUPPORT_SECTOR	(0x1 << 30)
#define MMC_OCR_REG_VOLTAGE_SWITCH			(0x1 << 24)

#define MMC_SD2_CSD_C_SIZE_LSB_MASK         (0xFFFF)
#define MMC_SD2_CSD_C_SIZE_MSB_MASK         (0x003F)
#define MMC_SD2_CSD_C_SIZE_MSB_OFFSET       (16)
#define MMC_CSD_C_SIZE_LSB_MASK             (0x0003)
#define MMC_CSD_C_SIZE_MSB_MASK             (0x03FF)
#define MMC_CSD_C_SIZE_MSB_OFFSET           (2)

#define MMC_CSD_TRAN_SPEED_UNIT_MASK        (0x07 << 0)
#define MMC_CSD_TRAN_SPEED_FACTOR_MASK      (0x0F << 3)
#define MMC_CSD_TRAN_SPEED_UNIT_100MHZ      (0x3 << 0)
#define MMC_CSD_TRAN_SPEED_FACTOR_1_0       (0x01 << 3)
#define MMC_CSD_TRAN_SPEED_FACTOR_8_0       (0x0F << 3)

#define SD2_0_CHECK_PATTERN			(0xAA)
#define SD2_0_VHS				(0x1 << 8)


#endif /* MMC_H */
