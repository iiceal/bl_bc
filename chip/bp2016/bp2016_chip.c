#include <common.h>
#include <pll.h>
#include <regs/scm.h>
#ifdef CONFIG_DRV_DDR
#include <drivers/drv_ddr.h>
#endif
#include <drivers/iomux/iomux_hw.h>
#include <drivers/drv_qspi.h>

void pll_init(void)
{
    //pll0_set_rate_kHz(PLL0_CPU_FREQ_KHZ);
    pll1_set_rate_kHz(PLL1_DDR_FREQ_KHZ);
    pll2_set_rate_kHz(PLL2_BUS_FREQ_KHZ);
    pll3_set_rate_kHz(PLL3_BB_FREQ_KHZ);
}

void pll_print(void)
{
    printf("pll01 reg = 0x%x \n", hwp_apSCM->pll01_ctrl);
    printf("pll23 reg = 0x%x \n", hwp_apSCM->pll23_ctrl);

    printf("PLL0=%d kHz\n", pll0_get_rate_kHz());
    printf("PLL1=%d kHz\n", pll1_get_rate_kHz());
    printf("PLL2=%d kHz\n", pll2_get_rate_kHz());
    printf("PLL3=%d kHz\n", pll3_get_rate_kHz());
}

void pll_test(void)
{
    int i =1;
    //pll0_set_rate_kHz(PLL0_CPU_FREQ_KHZ/2);
    for(i=5; i<16; i++)
    {
        printf("########################################\n");
        pll1_set_rate_kHz(i*100000);
        pll2_set_rate_kHz(i*100000);
        pll3_set_rate_kHz(i*100000);
        pll_print();
        printf("########################################\n");
    }

}

#ifdef CONFIG_DRV_CLK
#include <drivers/drv_clk.h>
void clk_en_set(void)
{
    clk_enable(CLK_DMAC0);
    clk_enable(CLK_QSPI);
    clk_enable(CLK_UART0);
    clk_enable(CLK_UART1);
    clk_enable(CLK_UART6);
}
#else
void clk_en_set(void)
{
}
#endif

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
    clk_en_set();

    pin_mux_set();

    dbg_serial_init(CONFIG_SERIAL_BAUDRATE);

    arch_timer_starting_cpu();

	//prcm_init();

	return 0;
}

__attribute__((weak)) void early_system_init(void)
{
#ifdef CONFIG_UPV_SERIAl
    upv_serial_init(CONFIG_UPV_SERIAL_BAUDRATE);
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

#ifdef CONFIG_DRV_DMA
    extern void dma_init(U32 id);
    extern void dma_deinit(U32 id);
    extern inline void dmac_int_disable(U32 id);
    dma_deinit(0);
    dma_init(0);
    dmac_int_disable(0);
#endif
}
