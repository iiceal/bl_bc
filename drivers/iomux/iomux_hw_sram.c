#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

static void scm_asram_rd(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN4_BIT, SCM_IOMUX_8C_PRM1_DIN4_MASK,
                  SCM_IOMUX_8C_PRM1_DIN4_SRAM_RD);
}

static void scm_asram_wr(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN5_BIT, SCM_IOMUX_8C_PRM1_DIN5_MASK,
                  SCM_IOMUX_8C_PRM1_DIN5_SRAM_WR);
}

static void scm_asram_cs(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN6_BIT, SCM_IOMUX_8C_PRM1_DIN6_MASK,
                  SCM_IOMUX_8C_PRM1_DIN6_SRAM_CS);
}

static void scm_asram_a0(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN20_BIT, SCM_IOMUX_90_PRM1_DIN20_MASK,
                  SCM_IOMUX_90_PRM1_DIN20_SRAM_A0);
}

static void scm_asram_a1(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN21_BIT, SCM_IOMUX_90_PRM1_DIN21_MASK,
                  SCM_IOMUX_90_PRM1_DIN21_SRAM_A1);
}

static void scm_asram_a2(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN22_BIT, SCM_IOMUX_90_PRM1_DIN22_MASK,
                  SCM_IOMUX_90_PRM1_DIN22_SRAM_A2);
}

static void scm_asram_a3(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN23_BIT, SCM_IOMUX_90_PRM1_DIN23_MASK,
                  SCM_IOMUX_90_PRM1_DIN23_SRAM_A3);
}

static void scm_asram_d0(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN24_BIT, SCM_IOMUX_90_PRM1_DIN24_MASK,
                  SCM_IOMUX_90_PRM1_DIN24_SRAM_D0);
}

static void scm_asram_d1(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN25_BIT, SCM_IOMUX_90_PRM1_DIN25_MASK,
                  SCM_IOMUX_90_PRM1_DIN25_SRAM_D1);
}

static void scm_asram_d2(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN26_BIT, SCM_IOMUX_90_PRM1_DIN26_MASK,
                  SCM_IOMUX_90_PRM1_DIN26_SRAM_D2);
}

static void scm_asram_d3(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN27_BIT, SCM_IOMUX_94_PRM1_DIN27_MASK,
                  SCM_IOMUX_94_PRM1_DIN27_SRAM_D3);
}

static void scm_asram_d4(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN28_BIT, SCM_IOMUX_94_PRM1_DIN28_MASK,
                  SCM_IOMUX_94_PRM1_DIN28_SRAM_D4);
}

static void scm_asram_d5(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN29_BIT, SCM_IOMUX_94_PRM1_DIN29_MASK,
                  SCM_IOMUX_94_PRM1_DIN29_SRAM_D5);
}

static void scm_asram_d6(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN30_BIT, SCM_IOMUX_94_PRM1_DIN30_MASK,
                  SCM_IOMUX_94_PRM1_DIN30_SRAM_D6);
}

static void scm_asram_d7(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN31_BIT, SCM_IOMUX_94_PRM1_DIN31_MASK,
                  SCM_IOMUX_94_PRM1_DIN31_SRAM_D7);
}

void scm_asram_en(void)
{
    scm_asram_rd();
    scm_asram_wr();
    scm_asram_cs();
    scm_asram_a0();
    scm_asram_a1();
    scm_asram_a2();
    scm_asram_a3();
    scm_asram_d0();
    scm_asram_d1();
    scm_asram_d2();
    scm_asram_d3();
    scm_asram_d4();
    scm_asram_d5();
    scm_asram_d6();
    scm_asram_d7();
}
