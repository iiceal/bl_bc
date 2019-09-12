#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

static void scm_spi0_clk(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_CLK_BIT, SCM_IOMUX_70_SPI0_CLK_MASK,
                  SCM_IOMUX_70_SPI0_CLK_DEFAULT);
}

static void scm_spi0_cs_n(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_CS_BIT, SCM_IOMUX_70_SPI0_CS_MASK,
                  SCM_IOMUX_70_SPI0_CS_DEFAULT);
}

static void scm_spi0_miso(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_MISO_BIT, SCM_IOMUX_70_SPI0_MISO_MASK,
                  SCM_IOMUX_70_SPI0_MISO_DEFAULT);
}

static void scm_spi0_mosi(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_MOSI_BIT, SCM_IOMUX_70_SPI0_MOSI_MASK,
                  SCM_IOMUX_70_SPI0_MOSI_DEFAULT);
}

static void scm_spi1_clk(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_CLK_BIT, SCM_IOMUX_70_SPI1_CLK_MASK,
                  SCM_IOMUX_70_SPI1_CLK_DEFAULT);
}

static void scm_spi1_cs0(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_CS0_BIT, SCM_IOMUX_70_SPI1_CS0_MASK,
                  SCM_IOMUX_70_SPI1_CS0_DEFAULT);
}

static void scm_spi1_cs1(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_CS1_BIT, SCM_IOMUX_70_SPI1_CS1_MASK,
                  SCM_IOMUX_70_SPI1_CS1_DEFAULT);
}

static void scm_spi1_miso(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_MISO_BIT, SCM_IOMUX_70_SPI1_MISO_MASK,
                  SCM_IOMUX_70_SPI1_MISO_DEFAULT);
}

static void scm_spi1_mosi(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_MOSI_BIT, SCM_IOMUX_70_SPI1_MOSI_MASK,
                  SCM_IOMUX_70_SPI1_MOSI_DEFAULT);
}

static void scm_spi2_clk(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CLK_BIT, SCM_IOMUX_70_SPI2_CLK_MASK,
                  SCM_IOMUX_70_SPI2_CLK_DEFAULT);
}

static void scm_spi2_cs0(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS0_BIT, SCM_IOMUX_70_SPI2_CS0_MASK,
                  SCM_IOMUX_70_SPI2_CS0_DEFAULT);
}

static void scm_spi2_cs1(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS1_BIT, SCM_IOMUX_70_SPI2_CS1_MASK,
                  SCM_IOMUX_70_SPI2_CS1_DEFAULT);
}

static void scm_spi2_cs2(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS2_BIT, SCM_IOMUX_70_SPI2_CS2_MASK,
                  SCM_IOMUX_70_SPI2_CS2_DEFAULT);
}

static void scm_spi2_cs3(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS3_BIT, SCM_IOMUX_70_SPI2_CS3_MASK,
                  SCM_IOMUX_70_SPI2_CS3_DEFAULT);
}

static void scm_spi2_cs4(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS4_BIT, SCM_IOMUX_70_SPI2_CS4_MASK,
                  SCM_IOMUX_70_SPI2_CS4_DEFAULT);
}

static void scm_spi2_cs5(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS5_BIT, SCM_IOMUX_70_SPI2_CS5_MASK,
                  SCM_IOMUX_70_SPI2_CS5_DEFAULT);
}

static void scm_spi2_cs6(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_CS6_BIT, SCM_IOMUX_74_SPI2_CS6_MASK,
                  SCM_IOMUX_74_SPI2_CS6_DEFAULT);
}

static void scm_spi2_cs7(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_CS7_BIT, SCM_IOMUX_74_SPI2_CS7_MASK,
                  SCM_IOMUX_74_SPI2_CS7_DEFAULT);
}

static void scm_spi2_miso(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_MISO_BIT, SCM_IOMUX_74_SPI2_MISO_MASK,
                  SCM_IOMUX_74_SPI2_MISO_DEFAULT);
}

static void scm_spi2_mosi(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_MOSI_BIT, SCM_IOMUX_74_SPI2_MOSI_MASK,
                  SCM_IOMUX_74_SPI2_MOSI_DEFAULT);
}

static void scm_spi3_clk(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_CLK_BIT, SCM_IOMUX_70_SPI0_CLK_MASK,
                  SCM_IOMUX_70_SPI0_CLK_SPI3_CLK);
}

static void scm_spi3_cs_n(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_CS_BIT, SCM_IOMUX_70_SPI0_CS_MASK,
                  SCM_IOMUX_70_SPI0_CS_SPI3_CS);
}

static void scm_spi3_mosi(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_MISO_BIT, SCM_IOMUX_70_SPI0_MISO_MASK,
                  SCM_IOMUX_70_SPI0_MISO_SPI3_MOSI);
}

static void scm_spi3_miso(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_MOSI_BIT, SCM_IOMUX_70_SPI0_MOSI_MASK,
                  SCM_IOMUX_70_SPI0_MOSI_SPI3_MISO);
}

static void scm_spi4_clk(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CLK_BIT, SCM_IOMUX_70_SPI2_CLK_MASK,
                  SCM_IOMUX_70_SPI2_CLK_SPI4_CLK);
}

static void scm_spi4_sen0(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS0_BIT, SCM_IOMUX_70_SPI2_CS0_MASK,
                  SCM_IOMUX_70_SPI2_CS0_SPI4_SEN0);
}

static void scm_spi4_sen1(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS1_BIT, SCM_IOMUX_70_SPI2_CS1_MASK,
                  SCM_IOMUX_70_SPI2_CS1_SPI4_SEN1);
}

static void scm_spi4_io(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_MISO_BIT, SCM_IOMUX_74_SPI2_MISO_MASK,
                  SCM_IOMUX_74_SPI2_MISO_SPI4_IO);
}

void scm_qspi_en(void)
{
    // qspi clk/qspi_cs/qspi_data0/qspi_data1/qspi_data2/qspi_data3
    scm_io_mux_74(SCM_IOMUX_74_QSPI_IF_BIT, SCM_IOMUX_74_QSPI_IF_MASK,
                  SCM_IOMUX_74_QSPI_IF_DEFAULT);
}

void scm_spi0_en(void)
{
    scm_spi0_clk();
    scm_spi0_cs_n();
    scm_spi0_miso();
    scm_spi0_mosi();
}

int scm_spi1_en(int cs0, int cs1)
{
    if ((!cs0) && (!cs1)) {
        iomux_printf("spi1: please select at least one cs! exit ...");
        return DRV_OP_FAILED;
    }

    scm_spi1_clk();

    if (cs0)
        scm_spi1_cs0();

    if (cs1)
        scm_spi1_cs1();

    scm_spi1_miso();
    scm_spi1_mosi();

    return DRV_OP_SUCCESS;
}

int scm_spi2_en(int cs0, int cs1, int cs2, int cs3, int cs4, int cs5, int cs6, int cs7)
{
    if ((!cs0) && (!cs1) && (!cs2) && (!cs3) && (!cs4) && (!cs5) && (!cs6) && (!cs7)) {
        iomux_printf("spi2: please select at least one cs! exit ...");
        return DRV_OP_FAILED;
    }

    scm_spi2_clk();

    if (cs0)
        scm_spi2_cs0();

    if (cs1)
        scm_spi2_cs1();

    if (cs2)
        scm_spi2_cs2();

    if (cs3)
        scm_spi2_cs3();

    if (cs4)
        scm_spi2_cs4();

    if (cs5)
        scm_spi2_cs5();

    if (cs6)
        scm_spi2_cs6();

    if (cs7)
        scm_spi2_cs7();

    scm_spi2_miso();
    scm_spi2_mosi();

    return DRV_OP_SUCCESS;
}

void scm_spi3_en(void)
{
    scm_spi3_clk();
    scm_spi3_cs_n();
    scm_spi3_miso();
    scm_spi3_mosi();
}

int scm_spi4_en(int cs0, int cs1)
{
    if ((!cs0) && (!cs1)) {
        iomux_printf("spi4: please select at least one cs! exit ...");
        return DRV_OP_FAILED;
    }

    scm_spi4_clk();

    if (cs0)
        scm_spi4_sen0();

    if (cs1)
        scm_spi4_sen1();

    scm_spi4_io();

    return DRV_OP_SUCCESS;
}
