#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

static void scm_mmc0_clk(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK0_BIT, SCM_IOMUX_88_PRM1_CLK0_MASK,
                  SCM_IOMUX_88_PRM1_CLK0_MMC0_CLK);
}

static void scm_mmc1_clk(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK0_BIT, SCM_IOMUX_88_PRM1_CLK0_MASK,
                  SCM_IOMUX_88_PRM1_CLK0_MMC1_CLK);
}

static void scm_mmc0_cmd(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK1_BIT, SCM_IOMUX_88_PRM1_CLK1_MASK,
                  SCM_IOMUX_88_PRM1_CLK1_MMC0_CMD);
}

static void scm_mmc1_cmd(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK1_BIT, SCM_IOMUX_88_PRM1_CLK1_MASK,
                  SCM_IOMUX_88_PRM1_CLK1_MMC1_CMD);
}

static void scm_mmc0_data0(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK2_BIT, SCM_IOMUX_88_PRM1_CLK2_MASK,
                  SCM_IOMUX_88_PRM1_CLK2_MMC0_D0);
}

static void scm_mmc1_data0(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK2_BIT, SCM_IOMUX_88_PRM1_CLK2_MASK,
                  SCM_IOMUX_88_PRM1_CLK2_MMC1_D0);
}

static void scm_mmc0_data1(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK3_BIT, SCM_IOMUX_88_PRM1_CLK3_MASK,
                  SCM_IOMUX_88_PRM1_CLK3_MMC0_D1);
}

static void scm_mmc1_data1(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK3_BIT, SCM_IOMUX_88_PRM1_CLK3_MASK,
                  SCM_IOMUX_88_PRM1_CLK3_MMC1_D1);
}

static void scm_mmc0_data2(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK4_BIT, SCM_IOMUX_88_PRM1_CLK4_MASK,
                  SCM_IOMUX_88_PRM1_CLK4_MMC0_D2);
}

static void scm_mmc1_data2(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK4_BIT, SCM_IOMUX_88_PRM1_CLK4_MASK,
                  SCM_IOMUX_88_PRM1_CLK4_MMC1_D2);
}

static void scm_mmc0_data3(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK5_BIT, SCM_IOMUX_88_PRM1_CLK5_MASK,
                  SCM_IOMUX_88_PRM1_CLK5_MMC0_D3);
}

static void scm_mmc1_data3(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK5_BIT, SCM_IOMUX_88_PRM1_CLK5_MASK,
                  SCM_IOMUX_88_PRM1_CLK5_MMC1_D3);
}

static void scm_mmc0_wp(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK6_BIT, SCM_IOMUX_88_PRM1_CLK6_MASK,
                  SCM_IOMUX_88_PRM1_CLK6_MMC0_WP);
}

static void scm_mmc1_wp(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK6_BIT, SCM_IOMUX_88_PRM1_CLK6_MASK,
                  SCM_IOMUX_88_PRM1_CLK6_MMC1_WP);
}

static void scm_mmc0_cd(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK7_BIT, SCM_IOMUX_88_PRM1_CLK7_MASK,
                  SCM_IOMUX_88_PRM1_CLK7_MMC0_CD);
}

static void scm_mmc1_cd(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK7_BIT, SCM_IOMUX_88_PRM1_CLK7_MASK,
                  SCM_IOMUX_88_PRM1_CLK7_MMC1_CD);
}

static void scm_mmc1_psw(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK8_BIT, SCM_IOMUX_88_PRM1_CLK8_MASK,
                  SCM_IOMUX_88_PRM1_CLK8_MMC1_PSW);
}

int scm_mmc_en(index)
{
    switch (index) {
    case 0:
        scm_mmc0_clk();
        scm_mmc0_cmd();
        scm_mmc0_data0();
        scm_mmc0_data1();
        scm_mmc0_data2();
        scm_mmc0_data3();
        scm_mmc0_wp();
        scm_mmc0_cd();
        break;
    case 1:
        scm_mmc1_clk();
        scm_mmc1_cmd();
        scm_mmc1_data0();
        scm_mmc1_data1();
        scm_mmc1_data2();
        scm_mmc1_data3();
        scm_mmc1_wp();
        scm_mmc1_cd();
        scm_mmc1_psw();
        break;
    default:
        iomux_printf("MMC: index %d error.", index);
        return DRV_OP_FAILED;
    }

    return DRV_OP_SUCCESS;
}
