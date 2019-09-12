#ifndef _REGS_WDT_H_
#define _REGS_WDT_H_

#define REG_AP_WDT0_BASE         (APB1_BASE+0xe0000)
#define REG_AP_WDT1_BASE         (APB1_BASE+0xf0000)

typedef volatile struct
{
    REG32                       wdt_cr;               	//0x00000000
    REG32                       wdt_torr;             	//x00000004
    REG32                       wdt_ccvr;             	//0x00000008
    REG32                       wdt_crr;		//0x0000000C
    REG32                       wdt_stat;            	//0x00000010
    REG32                       wdt_eoi;          	//0x00000014

    REG32                       _pad_0x18_0xe0[51];  	//0x00000018
    REG32			wdt_comp_params_5;	//0x000000e4
    REG32			wdt_comp_params_4;	//0x000000e8
    REG32			wdt_comp_params_3;	//0x000000ec
    REG32			wdt_comp_params_2;	//0x000000f0
    REG32			wdt_comp_params_1;	//0x000000f4
    REG32			wdt_comp_version;	//0x000000f8
    REG32			wdt_comp_type;		//0x000000fc
} HWP_WDT_AP_T;

#define hwp_apWdt0               ((HWP_WDT_AP_T*) (REG_AP_WDT0_BASE))
#define hwp_apWdt1               ((HWP_WDT_AP_T*) (REG_AP_WDT1_BASE))


//wdt_cr: WDT_CR_EN = 1, canbe cleared only by a system reset**
#define		WDT_CR_RPL_PCLK_2_DIV	0
#define		WDT_CR_RPL_PCLK_4_DIV	1
#define		WDT_CR_RPL_PCLK_8_DIV	2
#define		WDT_CR_RPL_PCLK_16_DIV	3
#define		WDT_CR_RPL_PCLK_32_DIV	4
#define		WDT_CR_RPL_PCLK_64_DIV	5
#define		WDT_CR_RPL_PCLK_128_DIV	6
#define		WDT_CR_RPL_PCLK_256_DIV	7
#define		WDT_CR_RPL_MASK		(0x1C)
#define		WDT_CR_RPL_SHIFT	(2)
#define		WDT_CR_RMOD		(1 << 1)  //response mode	
#define		WDT_CR_EN		(1 << 0)
//WDT_TORR	??
#define		WDT_CRR_TOP_INIT_MASK	(0xf0)
#define		WDT_CRR_TOP_INIT_SHIFT	(4)
#define		WDT_TORR_TOP_MASK	(0xf)
#define		WDT_TORR_TOP_SHIFT	(0)
//wdt_ccvr:the current value of the internal counter

//wdt_crr, write 0x76, clear wdt interrupt and restart wdt counter
#define		WDT_CRR_W_RST_MASK	(0xFF)
#define		WDT_CRR_W_RST_SHIFT	(0)
//wdt_stat
#define		WDT_STAT_R_INT_ACTIVE	(1 << 0)

//wdt_eoi
#define		WDT_EOI_R_CLR_BIT	(1 << 0)

#endif

