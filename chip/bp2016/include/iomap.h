#ifndef __IOMAP_HGBP2016_H
#define __IOMAP_HGBP2016_H

#define _ROM_BASE	    	(0x00000000)
#define _IRAM_BASE	    	(0x00100000)
#define _ASRAM_BASE	    	(0x00400000)
#define _BB_BASE	    	(0x00500000)
#define _SPIFLASH_D_BASE       	(0x10000000)
#define _DDR_BASE       	(0x20000000)
#define	_GIC400_BASE	   	(0xa2000000)

#define _APB0_BASE           	(0xb0000000)	/*qspi/asram/sd/dmac0/dmac1*/
#define _APB1_BASE           	(0xc0000000)    /*uart/spi/i2c/wdt*/
#define _APB2_BASE           	(0xc0100000)    /*sim/gpio/timer/pwm/ddr/bb/scm*/


#define APB0_BASE           _APB0_BASE
#define APB1_BASE           _APB1_BASE
#define APB2_BASE           _APB2_BASE
#define SPIFLASH_D_BASE       _SPIFLASH_D_BASE
#define ROM_BASE	    _ROM_BASE 
#define IRAM_BASE	    _IRAM_BASE
#define	GIC400_BASE	    _GIC400_BASE

#define	SCM_BASE	    (APB2_BASE + 0xd0000)

#define	hwp_apTimer	    	hwp_apTimer0

#define hwp_apQspi	        hwp_apQspi0
//#define hwp_sim	        	hwp_sim0
	

#if (CONFIG_DBG_SERIAL_NUM == 0)
#define    hwp_dbgUart   hwp_apUart0
#elif (CONFIG_DBG_SERIAL_NUM == 1)
#define    hwp_dbgUart   hwp_apUart1
#elif (CONFIG_DBG_SERIAL_NUM == 2)
#define    hwp_dbgUart   hwp_apUart2
#elif (CONFIG_DBG_SERIAL_NUM == 3)
#define    hwp_dbgUart   hwp_apUart3
#elif (CONFIG_DBG_SERIAL_NUM == 4)
#define    hwp_dbgUart   hwp_apUart4
#elif (CONFIG_DBG_SERIAL_NUM == 5)
#define    hwp_dbgUart   hwp_apUart5
#elif (CONFIG_DBG_SERIAL_NUM == 6)
#define    hwp_dbgUart   hwp_apUart6
#elif (CONFIG_DBG_SERIAL_NUM == 7)
#define    hwp_dbgUart   hwp_apUart7
#endif

#if (CONFIG_UPV_SERIAL_NUM == 0)
#define    hwp_upvUart   hwp_apUart0
#elif (CONFIG_UPV_SERIAL_NUM == 1)
#define    hwp_upvUart   hwp_apUart1
#elif (CONFIG_UPV_SERIAL_NUM == 2)
#define    hwp_upvUart   hwp_apUart2
#elif (CONFIG_UPV_SERIAL_NUM == 3)
#define    hwp_upvUart   hwp_apUart3
#elif (CONFIG_UPV_SERIAL_NUM == 4)
#define    hwp_upvUart   hwp_apUart4
#elif (CONFIG_UPV_SERIAL_NUM == 5)
#define    hwp_upvUart   hwp_apUart5
#elif (CONFIG_UPV_SERIAL_NUM == 6)
#define    hwp_upvUart   hwp_apUart6
#elif (CONFIG_UPV_SERIAL_NUM == 7)
#define    hwp_upvUart   hwp_apUart7
#endif

#endif /* __IOMAP_HGBP2016_H */
