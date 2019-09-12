#ifndef __PLL_20180816_H__
#define __PLL_20180816_H__

#define PLL_OUT_MIN             (62500)
#define PLL_RANGE_MIN           (500000)
#define PLL_RANGE_MAX           (1500000)

#define PLL0_CPU_FREQ_KHZ       (1200000)        /*only for cpu*/
#define PLL1_DDR_FREQ_KHZ       (800000)        /*only for ddr*/
#define PLL2_BUS_FREQ_KHZ       (1140000)       /*bus*/
#define PLL3_BB_FREQ_KHZ        (800000)        /*only for bb */
#define PLL4_RDSS_FREQ_KHZ      (800000)        /*only for rdss*/

int pll0_set_rate_kHz(unsigned int ratekhz);
int pll0_get_rate_kHz(void);
int pll1_set_rate_kHz(unsigned int ratekhz);
int pll1_get_rate_kHz(void);
int pll2_set_rate_kHz(unsigned int ratekhz);
int pll2_get_rate_kHz(void);
int pll3_set_rate_kHz(unsigned int ratekhz);
int pll3_get_rate_kHz(void);

extern int pll3_ref_clk_khz;

#define RET_OK      (0)
#define RET_FAILED  (-1)

#endif //__PLL_20180816_H__
