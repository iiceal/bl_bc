#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

static void scm_prm0_data0(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D0_BIT, SCM_IOMUX_7C_PRM0_D0_MASK,
                  SCM_IOMUX_7C_PRM0_D0_DEFAULT);
}

static void scm_prm0_data1(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D1_BIT, SCM_IOMUX_7C_PRM0_D1_MASK,
                  SCM_IOMUX_7C_PRM0_D1_DEFAULT);
}

static void scm_prm0_data2(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D2_BIT, SCM_IOMUX_7C_PRM0_D2_MASK,
                  SCM_IOMUX_7C_PRM0_D2_DEFAULT);
}

static void scm_prm0_data3(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D3_BIT, SCM_IOMUX_7C_PRM0_D3_MASK,
                  SCM_IOMUX_7C_PRM0_D3_DEFAULT);
}

static void scm_prm0_data4(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D4_BIT, SCM_IOMUX_7C_PRM0_D4_MASK,
                  SCM_IOMUX_7C_PRM0_D4_DEFAULT);
}

static void scm_prm0_data5(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D5_BIT, SCM_IOMUX_7C_PRM0_D5_MASK,
                  SCM_IOMUX_7C_PRM0_D5_DEFAULT);
}

static void scm_prm0_data6(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D6_BIT, SCM_IOMUX_7C_PRM0_D6_MASK,
                  SCM_IOMUX_7C_PRM0_D6_DEFAULT);
}

static void scm_prm0_data7(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D7_BIT, SCM_IOMUX_7C_PRM0_D7_MASK,
                  SCM_IOMUX_7C_PRM0_D7_DEFAULT);
}

static void scm_prm0_data8(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D8_BIT, SCM_IOMUX_7C_PRM0_D8_MASK,
                  SCM_IOMUX_7C_PRM0_D8_DEFAULT);
}

static void scm_prm0_data9(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D9_BIT, SCM_IOMUX_7C_PRM0_D9_MASK,
                  SCM_IOMUX_7C_PRM0_D9_DEFAULT);
}

static void scm_prm0_data10(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D10_BIT, SCM_IOMUX_7C_PRM0_D10_MASK,
                  SCM_IOMUX_7C_PRM0_D10_DEFAULT);
}

static void scm_prm0_data11(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D11_BIT, SCM_IOMUX_7C_PRM0_D11_MASK,
                  SCM_IOMUX_7C_PRM0_D11_DEFAULT);
}

static void scm_prm0_data12(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D12_BIT, SCM_IOMUX_7C_PRM0_D12_MASK,
                  SCM_IOMUX_7C_PRM0_D12_DEFAULT);
}

static void scm_prm0_data13(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D13_BIT, SCM_IOMUX_7C_PRM0_D13_MASK,
                  SCM_IOMUX_7C_PRM0_D13_DEFAULT);
}

static void scm_prm0_data14(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_D14_BIT, SCM_IOMUX_80_PRM0_D14_MASK,
                  SCM_IOMUX_80_PRM0_D14_DEFAULT);
}

static void scm_prm0_data15(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_D15_BIT, SCM_IOMUX_80_PRM0_D15_MASK,
                  SCM_IOMUX_80_PRM0_D15_DEFAULT);
}

static void scm_prm0_clk0(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK0_BIT, SCM_IOMUX_80_PRM0_CLK0_MASK,
                  SCM_IOMUX_80_PRM0_CLK0_DEFAULT);
}

static void scm_prm0_clk1(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK1_BIT, SCM_IOMUX_80_PRM0_CLK1_MASK,
                  SCM_IOMUX_80_PRM0_CLK1_DEFAULT);
}

static void scm_prm0_clk2(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK2_BIT, SCM_IOMUX_80_PRM0_CLK2_MASK,
                  SCM_IOMUX_80_PRM0_CLK2_DEFAULT);
}

static void scm_prm0_clk3(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK3_BIT, SCM_IOMUX_80_PRM0_CLK3_MASK,
                  SCM_IOMUX_80_PRM0_CLK3_DEFAULT);
}

static void scm_prm0_clk4(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK4_BIT, SCM_IOMUX_80_PRM0_CLK4_MASK,
                  SCM_IOMUX_80_PRM0_CLK4_DEFAULT);
}

static void scm_prm0_clk5(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK5_BIT, SCM_IOMUX_80_PRM0_CLK5_MASK,
                  SCM_IOMUX_80_PRM0_CLK5_DEFAULT);
}

static void scm_prm0_clk6(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK6_BIT, SCM_IOMUX_80_PRM0_CLK6_MASK,
                  SCM_IOMUX_80_PRM0_CLK6_DEFAULT);
}

static void scm_prm0_clk7(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK7_BIT, SCM_IOMUX_80_PRM0_CLK7_MASK,
                  SCM_IOMUX_80_PRM0_CLK7_DEFAULT);
}

static void scm_prm0_clk8(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK8_BIT, SCM_IOMUX_80_PRM0_CLK8_MASK,
                  SCM_IOMUX_80_PRM0_CLK8_DEFAULT);
}

static void scm_prm0_clk9(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK9_BIT, SCM_IOMUX_80_PRM0_CLK9_MASK,
                  SCM_IOMUX_80_PRM0_CLK9_DEFAULT);
}

static void scm_prm0_clk10(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK10_BIT, SCM_IOMUX_80_PRM0_CLK10_MASK,
                  SCM_IOMUX_80_PRM0_CLK10_DEFAULT);
}

static void scm_prm0_clk11(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK11_BIT, SCM_IOMUX_80_PRM0_CLK11_MASK,
                  SCM_IOMUX_80_PRM0_CLK11_DEFAULT);
}

static void scm_prm0_clk12(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK12_BIT, SCM_IOMUX_80_PRM0_CLK12_MASK,
                  SCM_IOMUX_80_PRM0_CLK12_DEFAULT);
}

static void scm_prm0_clk13(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK13_BIT, SCM_IOMUX_80_PRM0_CLK13_MASK,
                  SCM_IOMUX_80_PRM0_CLK13_DEFAULT);
}

static void scm_prm0_clk14(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM0_CLK14_BIT, SCM_IOMUX_84_PRM0_CLK14_MASK,
                  SCM_IOMUX_84_PRM0_CLK14_DEFAULT);
}

static void scm_prm0_clk15(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM0_CLK15_BIT, SCM_IOMUX_84_PRM0_CLK15_MASK,
                  SCM_IOMUX_84_PRM0_CLK15_DEFAULT);
}

static void scm_prm0_reset(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM0_RST_BIT, SCM_IOMUX_84_PRM0_RST_MASK,
                  SCM_IOMUX_84_PRM0_RST_DEFAULT);
}

static void scm_prm1_data0(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D0_BIT, SCM_IOMUX_84_PRM1_D0_MASK,
                  SCM_IOMUX_84_PRM1_D0_DEFAULT);
}

static void scm_prm1_data1(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D1_BIT, SCM_IOMUX_84_PRM1_D1_MASK,
                  SCM_IOMUX_84_PRM1_D1_DEFAULT);
}

static void scm_prm1_data2(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D2_BIT, SCM_IOMUX_84_PRM1_D2_MASK,
                  SCM_IOMUX_84_PRM1_D2_DEFAULT);
}

static void scm_prm1_data3(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D3_BIT, SCM_IOMUX_84_PRM1_D3_MASK,
                  SCM_IOMUX_84_PRM1_D3_DEFAULT);
}

static void scm_prm1_data4(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D4_BIT, SCM_IOMUX_84_PRM1_D4_MASK,
                  SCM_IOMUX_84_PRM1_D4_DEFAULT);
}

static void scm_prm1_data5(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D5_BIT, SCM_IOMUX_84_PRM1_D5_MASK,
                  SCM_IOMUX_84_PRM1_D5_DEFAULT);
}

static void scm_prm1_data6(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D6_BIT, SCM_IOMUX_84_PRM1_D6_MASK,
                  SCM_IOMUX_84_PRM1_D6_DEFAULT);
}

static void scm_prm1_data7(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D7_BIT, SCM_IOMUX_84_PRM1_D7_MASK,
                  SCM_IOMUX_84_PRM1_D7_DEFAULT);
}

static void scm_prm1_data8(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D8_BIT, SCM_IOMUX_84_PRM1_D8_MASK,
                  SCM_IOMUX_84_PRM1_D8_DEFAULT);
}

static void scm_prm1_data9(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D9_BIT, SCM_IOMUX_84_PRM1_D9_MASK,
                  SCM_IOMUX_84_PRM1_D9_DEFAULT);
}

static void scm_prm1_data10(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D10_BIT, SCM_IOMUX_84_PRM1_D10_MASK,
                  SCM_IOMUX_84_PRM1_D10_DEFAULT);
}

static void scm_prm1_data11(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D11_BIT, SCM_IOMUX_84_PRM1_D11_MASK,
                  SCM_IOMUX_84_PRM1_D11_DEFAULT);
}

static void scm_prm1_data12(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_D12_BIT, SCM_IOMUX_88_PRM1_D12_MASK,
                  SCM_IOMUX_88_PRM1_D12_DEFAULT);
}

static void scm_prm1_data13(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_D13_BIT, SCM_IOMUX_88_PRM1_D13_MASK,
                  SCM_IOMUX_88_PRM1_D13_DEFAULT);
}

static void scm_prm1_data14(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_D14_BIT, SCM_IOMUX_88_PRM1_D14_MASK,
                  SCM_IOMUX_88_PRM1_D14_DEFAULT);
}

static void scm_prm1_data15(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_D15_BIT, SCM_IOMUX_88_PRM1_D15_MASK,
                  SCM_IOMUX_88_PRM1_D15_DEFAULT);
}

static void scm_prm1_clk0(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK0_BIT, SCM_IOMUX_88_PRM1_CLK0_MASK,
                  SCM_IOMUX_88_PRM1_CLK0_DEFAULT);
}

static void scm_prm1_clk1(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK1_BIT, SCM_IOMUX_88_PRM1_CLK1_MASK,
                  SCM_IOMUX_88_PRM1_CLK1_DEFAULT);
}

static void scm_prm1_clk2(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK2_BIT, SCM_IOMUX_88_PRM1_CLK2_MASK,
                  SCM_IOMUX_88_PRM1_CLK2_DEFAULT);
}

static void scm_prm1_clk3(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK3_BIT, SCM_IOMUX_88_PRM1_CLK3_MASK,
                  SCM_IOMUX_88_PRM1_CLK3_DEFAULT);
}

static void scm_prm1_clk4(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK4_BIT, SCM_IOMUX_88_PRM1_CLK4_MASK,
                  SCM_IOMUX_88_PRM1_CLK4_DEFAULT);
}

static void scm_prm1_clk5(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK5_BIT, SCM_IOMUX_88_PRM1_CLK5_MASK,
                  SCM_IOMUX_88_PRM1_CLK5_DEFAULT);
}

static void scm_prm1_clk6(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK6_BIT, SCM_IOMUX_88_PRM1_CLK6_MASK,
                  SCM_IOMUX_88_PRM1_CLK6_DEFAULT);
}

static void scm_prm1_clk7(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK7_BIT, SCM_IOMUX_88_PRM1_CLK7_MASK,
                  SCM_IOMUX_88_PRM1_CLK7_DEFAULT);
}

static void scm_prm1_clk8(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK8_BIT, SCM_IOMUX_88_PRM1_CLK8_MASK,
                  SCM_IOMUX_88_PRM1_CLK8_DEFAULT);
}

static void scm_prm1_clk9(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK9_BIT, SCM_IOMUX_88_PRM1_CLK9_MASK,
                  SCM_IOMUX_88_PRM1_CLK9_DEFAULT);
}

static void scm_prm1_clk10(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK10_BIT, SCM_IOMUX_88_PRM1_CLK10_MASK,
                  SCM_IOMUX_88_PRM1_CLK10_DEFAULT);
}

static void scm_prm1_clk11(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK11_BIT, SCM_IOMUX_88_PRM1_CLK11_MASK,
                  SCM_IOMUX_88_PRM1_CLK11_DEFAULT);
}

static void scm_prm1_clk12(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_CLK12_BIT, SCM_IOMUX_8C_PRM1_CLK12_MASK,
                  SCM_IOMUX_8C_PRM1_CLK12_DEFAULT);
}

static void scm_prm1_clk13(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_CLK13_BIT, SCM_IOMUX_8C_PRM1_CLK13_MASK,
                  SCM_IOMUX_8C_PRM1_CLK13_DEFAULT);
}

static void scm_prm1_clk14(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_CLK14_BIT, SCM_IOMUX_8C_PRM1_CLK14_MASK,
                  SCM_IOMUX_8C_PRM1_CLK14_DEFAULT);
}

static void scm_prm1_clk15(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_CLK15_BIT, SCM_IOMUX_8C_PRM1_CLK15_MASK,
                  SCM_IOMUX_8C_PRM1_CLK15_DEFAULT);
}

static void scm_prm1_reset(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_RST_BIT, SCM_IOMUX_8C_PRM1_RST_MASK,
                  SCM_IOMUX_8C_PRM1_RST_DEFAULT);
}

static void scm_prm_sysclk(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM_SYSCLK_BIT, SCM_IOMUX_84_PRM_SYSCLK_MASK,
                  SCM_IOMUX_84_PRM_SYSCLK_DEFAULT);
}

void scm_prm0_en(void)
{
    scm_prm_sysclk();
    scm_prm0_reset();

    scm_prm0_data0();
    scm_prm0_clk0();
    scm_prm0_data1();
    scm_prm0_clk1();
    scm_prm0_data2();
    scm_prm0_clk2();
    scm_prm0_data3();
    scm_prm0_clk3();
    scm_prm0_data4();
    scm_prm0_clk4();
    scm_prm0_data5();
    scm_prm0_clk5();
    scm_prm0_data6();
    scm_prm0_clk6();
    scm_prm0_data7();
    scm_prm0_clk7();
    scm_prm0_data8();
    scm_prm0_clk8();
    scm_prm0_data9();
    scm_prm0_clk9();
    scm_prm0_data10();
    scm_prm0_clk10();
    scm_prm0_data11();
    scm_prm0_clk11();
    scm_prm0_data12();
    scm_prm0_clk12();
    scm_prm0_data13();
    scm_prm0_clk13();
    scm_prm0_data14();
    scm_prm0_clk14();
    scm_prm0_data15();
    scm_prm0_clk15();
}

void scm_prm1_en(void)
{
    scm_prm_sysclk();
    scm_prm1_reset();

    scm_prm1_data0();
    scm_prm1_clk0();
    scm_prm1_data1();
    scm_prm1_clk1();
    scm_prm1_data2();
    scm_prm1_clk2();
    scm_prm1_data3();
    scm_prm1_clk3();
    scm_prm1_data4();
    scm_prm1_clk4();
    scm_prm1_data5();
    scm_prm1_clk5();
    scm_prm1_data6();
    scm_prm1_clk6();
    scm_prm1_data7();
    scm_prm1_clk7();
    scm_prm1_data8();
    scm_prm1_clk8();
    scm_prm1_data9();
    scm_prm1_clk9();
    scm_prm1_data10();
    scm_prm1_clk10();
    scm_prm1_data11();
    scm_prm1_clk11();
    scm_prm1_data12();
    scm_prm1_clk12();
    scm_prm1_data13();
    scm_prm1_clk13();
    scm_prm1_data14();
    scm_prm1_clk14();
    scm_prm1_data15();
    scm_prm1_clk15();
}
