#include <common.h>
#include <pll.h>
#include <regs/scm.h>
#ifdef CONFIG_DRV_DDR
#include <drivers/drv_ddr.h>
#endif
#include <drivers/iomux/iomux_hw.h>
#include <drivers/drv_qspi.h>

#include "regs/uart.h"


#ifdef CHIP_DEBUG
#define chip_debug(fmt, args...)  printf("Chip: " fmt "\n", ##args);
#else
#define chip_debug(fmt, args...)
#endif




void pll_init(void)
{
    //pll0_set_rate_kHz(PLL0_CPU_FREQ_KHZ);
    pll1_set_rate_kHz(PLL1_DDR_FREQ_KHZ);
    pll2_set_rate_kHz(PLL2_BUS_FREQ_KHZ);
    pll3_set_rate_kHz(PLL3_BB_FREQ_KHZ);
}

void pll_print(void)
{
    chip_debug("pll01 reg = 0x%x \n", hwp_apSCM->pll01_ctrl);
    chip_debug("pll23 reg = 0x%x \n", hwp_apSCM->pll23_ctrl);

    chip_debug("PLL0=%d kHz\n", pll0_get_rate_kHz());
    chip_debug("PLL1=%d kHz\n", pll1_get_rate_kHz());
    chip_debug("PLL2=%d kHz\n", pll2_get_rate_kHz());
    chip_debug("PLL3=%d kHz\n", pll3_get_rate_kHz());
}

void pll_test(void)
{
    int i =1;
    //pll0_set_rate_kHz(PLL0_CPU_FREQ_KHZ/2);
    for(i=5; i<16; i++)
    {
        chip_debug("########################################\n");
        pll1_set_rate_kHz(i*100000);
        pll2_set_rate_kHz(i*100000);
        pll3_set_rate_kHz(i*100000);
        pll_print();
        chip_debug("########################################\n");
    }

}


void pin_mux_set(void)
{
#ifdef CONFIG_DRV_IOMUX //wwzz add 
    scm_uart_en(CONFIG_DBG_SERIAL_NUM, 1);

#ifdef CONFIG_UPV_SERIAl
    scm_uart_en(CONFIG_UPV_SERIAL_NUM, 1);
#endif
#ifdef CONFIG_BOOT_FROM_QSPI
    scm_qspi_en();
#endif
#endif
}

void prcm_init(void)
{
    pll_init();
    //pll_test();
}

__attribute__((weak)) int chip_early_init_f(void)
{

    pin_mux_set();

    dbg_serial_init(CONFIG_SERIAL_BAUDRATE);

    arch_timer_starting_cpu();

	//prcm_init();

	return 0;
}

__attribute__((weak)) void early_system_init(void)
{

#ifdef CONFIG_UPV_SERIAl
    if(hwp_upvUart == hwp_apUart6 || hwp_upvUart == hwp_apUart7)
    {
        upv_serial_init(CONFIG_BAUDRATE_921600);
        chip_debug("UPV Uart%s BaudRate:%d.\n",(hwp_upvUart== hwp_apUart6?"6":"7"),CONFIG_BAUDRATE_921600);
    }else{
        chip_debug("UPV Uart%s BaudRate:%d.\n",(hwp_upvUart== hwp_apUart6?"6":"7"),CONFIG_BAUDRATE_115200);
        upv_serial_init(CONFIG_BAUDRATE_115200);
    }
#endif

#ifdef CONFIG_MMU_ENABLE
    extern void mmu_setup(void);
    mmu_setup();
#endif

#ifdef CONFIG_USE_IRQ
    interrupt_init();
#endif

#ifdef CONFIG_DRV_DDR
    ddrc_init();
#endif

}
