#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

static void scm_sim0_reset(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS3_BIT, SCM_IOMUX_70_SPI2_CS3_MASK,
                  SCM_IOMUX_70_SPI2_CS3_SIM0_RST);
}

static void scm_sim0_clk(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CLK_BIT, SCM_IOMUX_70_SPI2_CLK_MASK,
                  SCM_IOMUX_70_SPI2_CLK_SIM0_CLK);
}

static void scm_sim0_io(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS2_BIT, SCM_IOMUX_70_SPI2_CS2_MASK,
                  SCM_IOMUX_70_SPI2_CS2_SIM0_IO);
}

static void scm_sim1_reset(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_MOSI_BIT, SCM_IOMUX_70_SPI0_MOSI_MASK,
                  SCM_IOMUX_70_SPI0_MOSI_SIM1_RST);
}

static void scm_sim1_clk(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_CLK_BIT, SCM_IOMUX_70_SPI0_CLK_MASK,
                  SCM_IOMUX_70_SPI0_CLK_SIM1_CLK);
}

static void scm_sim1_io(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_MISO_BIT, SCM_IOMUX_70_SPI0_MISO_MASK,
                  SCM_IOMUX_70_SPI0_MISO_SIM1_IO);
}

static void scm_sim2_reset(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA03_BIT, SCM_IOMUX_78_GPIOA03_MASK,
                  SCM_IOMUX_78_GPIOA03_SIM2_RST);
}

static void scm_sim2_clk(void)
{
    scm_io_mux_98(SCM_IOMUX_98_CLK5M_BIT, SCM_IOMUX_98_CLK5M_MASK, SCM_IOMUX_98_CLK5M_SIM2_CLK);
}

static void scm_sim2_io(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA04_BIT, SCM_IOMUX_78_GPIOA04_MASK,
                  SCM_IOMUX_78_GPIOA04_SIM2_IO);
}

int scm_sim_en(int sim_index)
{
    switch (sim_index) {
    case 0:
        scm_sim0_reset();
        scm_sim0_clk();
        scm_sim0_io();
        break;
    case 1:
        scm_sim1_reset();
        scm_sim1_clk();
        scm_sim1_io();
        break;
    case 2:
        scm_sim2_reset();
        scm_sim2_clk();
        scm_sim2_io();
        break;
    default:
        iomux_printf("sim index %d  error", sim_index);
        return DRV_OP_FAILED;
    }

    return DRV_OP_SUCCESS;
}
