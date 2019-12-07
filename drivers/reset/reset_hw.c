#include <common.h>
#include <regs/scm.h>

#ifdef CONFIG_ENABLE_TRACE
#define reset_debug(fmt, args...)  printf(fmt"", ##args);
#else
#define reset_debug(fmt, args...)
#endif

static void control_swrst_ctrl_regs(int bit)
{
	u32 swrst = hwp_apSCM->swrst_ctrl;
	swrst &= ~(1 << bit);
	hwp_apSCM->swrst_ctrl = swrst;
#ifndef RESET_DEBUG
	mdelay(10);
#endif
	reset_debug("reset value: 0x%x", hwp_apSCM->swrst_ctrl);
	swrst |= (1 << bit);
	hwp_apSCM->swrst_ctrl = swrst;
	reset_debug("reset done: 0x%x", hwp_apSCM->swrst_ctrl);
}

void swrst_cpu1_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_CPU1);
}

void swrst_ddr_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_DDR);
}

#if 0
void swrst_62M_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_62M);
}
#endif

void swrst_rom_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_ROM);
}

void swrst_ram_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_RAM);
}

void swrst_bb_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_BB);
}

void swrst_mp_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_MP);
}

void swrst_dmac0_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_DMAC0);
}

void swrst_dmac1_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_DMAC1);
}

void swrst_qspi_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_QSPI);
}

void swrst_sd_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_SD);
}

void swrst_uart0_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_UART0);
}

void swrst_uart1_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_UART1);
}

void swrst_uart2_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_UART2);
}

void swrst_uart3_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_UART3);
}

void swrst_uart4_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_UART4);
}

void swrst_uart5_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_UART5);
}

void swrst_uart6_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_UART6);
}

void swrst_uart7_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_UART7);
}

void swrst_i2c0_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_I2C0);
}

void swrst_i2c1_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_I2C1);
}

void swrst_spi0_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_SPI0);
}

void swrst_spi1_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_SPI1);
}

void swrst_spi2_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_SPI2);
}

void swrst_spi3_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_SPI3);
}

void swrst_sim0_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_SIM0);
}

void swrst_sim1_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_SIM1);
}

void swrst_sim2_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_SIM2);
}

void swrst_pwm_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_PWM);
}

void swrst_gpio_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_GPIO);
}

void swrst_timer_reset(void)
{
    control_swrst_ctrl_regs(SCM_SWRST_TIMER);
}

void swrst_global_reset(void)
{
	u32 glbrst = hwp_apSCM->global_rst;
	glbrst &= ~(1);
	hwp_apSCM->global_rst = glbrst;
#ifndef RESET_DEBUG
	mdelay(10);
#endif
	reset_debug("reset value: 0x%x", hwp_apSCM->global_rst);
	glbrst |= (1);
	hwp_apSCM->global_rst = glbrst;
	reset_debug("reset done: 0x%x", hwp_apSCM->global_rst);
}

void swrst_gic_reset(void)
{
	u32 glbrst = hwp_apSCM->global_rst;
	glbrst &= ~(2);
	hwp_apSCM->global_rst = glbrst;
#ifndef RESET_DEBUG
	mdelay(10);
#endif
	reset_debug("reset value: 0x%x", hwp_apSCM->global_rst);
	glbrst |= (2);
	hwp_apSCM->global_rst = glbrst;
	reset_debug("reset done: 0x%x", hwp_apSCM->global_rst);
}
