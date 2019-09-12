#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

static void scm_i2c0_scl(void)
{
    scm_io_mux_98(SCM_IOMUX_98_I2C0_SCL_BIT, SCM_IOMUX_98_I2C0_SCL_MASK,
                  SCM_IOMUX_98_I2C0_SCL_DEFAULT);
}

static void scm_i2c0_sda(void)
{
    scm_io_mux_98(SCM_IOMUX_98_I2C0_SDA_BIT, SCM_IOMUX_98_I2C0_SDA_MASK,
                  SCM_IOMUX_98_I2C0_SDA_DEFAULT);
}

static void scm_i2c1_scl(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_CS6_BIT, SCM_IOMUX_74_SPI2_CS6_MASK,
                  SCM_IOMUX_74_SPI2_CS6_I2C1_SCL);
}

static void scm_i2c1_sda(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_CS7_BIT, SCM_IOMUX_74_SPI2_CS7_MASK,
                  SCM_IOMUX_74_SPI2_CS7_I2C1_SDA);
}

static void scm_odo_pulse(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS4_BIT, SCM_IOMUX_70_SPI2_CS4_MASK,
                  SCM_IOMUX_70_SPI2_CS4_ODO_PULSE);
}

static void scm_odo_direct(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS5_BIT, SCM_IOMUX_70_SPI2_CS5_MASK,
                  SCM_IOMUX_70_SPI2_CS5_ODO_DIRECT);
}

void scm_pps0(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PPS0_BIT, SCM_IOMUX_98_PPS0_MASK, SCM_IOMUX_98_PPS0_DEFAULT);
}

void scm_pps1(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PPS1_BIT, SCM_IOMUX_98_PPS1_MASK, SCM_IOMUX_98_PPS1_DEFAULT);
}

void scm_clk5m(void)
{
    scm_io_mux_98(SCM_IOMUX_98_CLK5M_BIT, SCM_IOMUX_98_CLK5M_MASK, SCM_IOMUX_98_CLK5M_DEFAULT);
}

static void scm_irigb_pps(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA22_BIT, SCM_IOMUX_9C_GPIOA22_MASK,
                  SCM_IOMUX_9C_GPIOA22_IRIGB_PPS);
}

static void scm_irigb_gjb(void)
{
    scm_io_mux_a0(SCM_IOMUX_A0_GPIOA23_BIT, SCM_IOMUX_A0_GPIOA23_MASK,
                  SCM_IOMUX_A0_GPIOA23_IRIGB_GJB);
}

static void scm_irigb_pps_in(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA01_BIT, SCM_IOMUX_78_GPIOA01_MASK,
                  SCM_IOMUX_78_GPIOA01_IRIGB_I);
}

void scm_i2c0_en(void)
{
    scm_i2c0_scl();
    scm_i2c0_sda();
}

void scm_i2c1_en(void)
{
    scm_i2c1_scl();
    scm_i2c1_sda();
}

void scm_odo_en(void)
{
    scm_odo_pulse();
    scm_odo_direct();
}

void scm_irigb_en(void)
{
    scm_irigb_pps();
    scm_irigb_gjb();
    scm_irigb_pps_in();
}
