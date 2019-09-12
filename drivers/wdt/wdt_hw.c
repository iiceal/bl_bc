#include <common.h>
#include <regs/wdt.h>

inline void wdt_cr(HWP_WDT_AP_T* hwp_apWdt, u32 en, u32 reponse_mode, u32 rpl)
{
    u32 cr = 0;
    if (en > 0)
        cr |= WDT_CR_EN;

    if (reponse_mode > 0)
        cr |= WDT_CR_RMOD;

    cr |= ((rpl << WDT_CR_RPL_SHIFT) & WDT_CR_RPL_MASK);

    hwp_apWdt->wdt_cr = cr;
}

inline u32 wdt_ccvr(HWP_WDT_AP_T* hwp_apWdt)
{
    return (u32) hwp_apWdt->wdt_ccvr;
}

/*
 *  Restart wdt register, and write 0x7 to restart its
 */
inline void wdt_crr(HWP_WDT_AP_T* hwp_apWdt, u32 rst_v) // 0x76 restart wdt counter
{
    hwp_apWdt->wdt_crr = rst_v & WDT_CRR_W_RST_MASK;
}

inline u32 wdt_stat(HWP_WDT_AP_T* hwp_apWdt)
{
    return hwp_apWdt->wdt_stat & WDT_STAT_R_INT_ACTIVE;
}

inline u32 wdt_eoi(HWP_WDT_AP_T* hwp_apWdt)
{
    return hwp_apWdt->wdt_eoi;
}

/*
 *  
 */
inline void wdt_torr(HWP_WDT_AP_T* hwp_apWdt, u32 top, u32 top_init)
{
	hwp_apWdt->wdt_torr = (top & WDT_TORR_TOP_MASK) | 
		((top_init << WDT_CRR_TOP_INIT_SHIFT) & WDT_CRR_TOP_INIT_MASK);
}

u32 get_torr_index(u32 max_v)
{
    switch (max_v) {
    case 0xff:
        return 0;
    case 0x1ffff:
        return 1;
    case 0x3ffff:
        return 2;
    case 0x7ffff:
        return 3;
    case 0xfffff:
        return 4;
    case 0x1fffff:
        return 5;
    case 0x3fffff:
        return 6;
    case 0x7fffff:
        return 7;
    case 0xffffff:
        return 8;
    case 0x1ffffff:
        return 9;
    case 0x3ffffff:
        return 10;
    case 0x7ffffff:
        return 11;
    case 0xfffffff:
        return 12;
    case 0x1fffffff:
        return 13;
    case 0x3fffffff:
        return 14;
    case 0x7fffffff:
        return 15;
    default:
        return 0;
    }
}

inline u32 get_torr_range_max_v(int index)
{
    switch (index) {
    case 0:
        return 0xff;
    case 1:
        return 0x1ffff;
    case 2:
        return 0x3ffff;
    case 3:
        return 0x7ffff;
    case 4:
        return 0xfffff;
    case 5:
        return 0x1fffff;
    case 6:
        return 0x3fffff;
    case 7:
        return 0x7fffff;
    case 8:
        return 0xffffff;
    case 9:
        return 0x1ffffff;
    case 10:
        return 0x3ffffff;
    case 11:
        return 0x7ffffff;
    case 12:
        return 0xfffffff;
    case 13:
        return 0x1fffffff;
    case 14:
        return 0x3fffffff;
    case 15:
        return 0x7fffffff;
    default:
        return 0x7fffffff;
    }
}
