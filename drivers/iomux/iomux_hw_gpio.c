#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

static void scm_gpio_a00(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA00_BIT, SCM_IOMUX_78_GPIOA00_MASK,
                  SCM_IOMUX_78_GPIOA00_DEFAULT);
}

static void scm_gpio_a01(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA01_BIT, SCM_IOMUX_78_GPIOA01_MASK,
                  SCM_IOMUX_78_GPIOA01_DEFAULT);
}

static void scm_gpio_a02(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA02_BIT, SCM_IOMUX_78_GPIOA02_MASK,
                  SCM_IOMUX_78_GPIOA02_DEFAULT);
}

static void scm_gpio_a03(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA03_BIT, SCM_IOMUX_78_GPIOA03_MASK,
                  SCM_IOMUX_78_GPIOA03_DEFAULT);
}

static void scm_gpio_a04(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA04_BIT, SCM_IOMUX_78_GPIOA04_MASK,
                  SCM_IOMUX_78_GPIOA04_DEFAULT);
}

static void scm_gpio_a05(void)
{
    scm_io_mux_78(SCM_IOMUX_78_GPIOA05_BIT, SCM_IOMUX_78_GPIOA05_MASK,
                  SCM_IOMUX_78_GPIOA05_DEFAULT);
}

static void scm_gpio_a06(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_GPIOA06_BIT, SCM_IOMUX_7C_GPIOA06_MASK,
                  SCM_IOMUX_7C_GPIOA06_DEFAULT);
}

static void scm_gpio_a07(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_GPIOA07_BIT, SCM_IOMUX_7C_GPIOA07_MASK,
                  SCM_IOMUX_7C_GPIOA07_DEFAULT);
}

static void scm_gpio_a08(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA08_BIT, SCM_IOMUX_9C_GPIOA08_MASK,
                  SCM_IOMUX_9C_GPIOA08_DEFAULT);
}

static void scm_gpio_a09(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA09_BIT, SCM_IOMUX_9C_GPIOA09_MASK,
                  SCM_IOMUX_9C_GPIOA09_DEFAULT);
}

static void scm_gpio_a10(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA10_BIT, SCM_IOMUX_9C_GPIOA10_MASK,
                  SCM_IOMUX_9C_GPIOA10_DEFAULT);
}

static void scm_gpio_a11(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA11_BIT, SCM_IOMUX_9C_GPIOA11_MASK,
                  SCM_IOMUX_9C_GPIOA11_DEFAULT);
}

static void scm_gpio_a12(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA12_BIT, SCM_IOMUX_9C_GPIOA12_MASK,
                  SCM_IOMUX_9C_GPIOA12_DEFAULT);
}

static void scm_gpio_a13(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA13_BIT, SCM_IOMUX_9C_GPIOA13_MASK,
                  SCM_IOMUX_9C_GPIOA13_DEFAULT);
}

static void scm_gpio_a14(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA14_BIT, SCM_IOMUX_9C_GPIOA14_MASK,
                  SCM_IOMUX_9C_GPIOA14_DEFAULT);
}

static void scm_gpio_a15(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA15_BIT, SCM_IOMUX_9C_GPIOA15_MASK,
                  SCM_IOMUX_9C_GPIOA15_DEFAULT);
}

static void scm_gpio_a16(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA16_BIT, SCM_IOMUX_9C_GPIOA16_MASK,
                  SCM_IOMUX_9C_GPIOA16_DEFAULT);
}

static void scm_gpio_a17(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA17_BIT, SCM_IOMUX_9C_GPIOA17_MASK,
                  SCM_IOMUX_9C_GPIOA17_DEFAULT);
}

static void scm_gpio_a18(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA18_BIT, SCM_IOMUX_9C_GPIOA18_MASK,
                  SCM_IOMUX_9C_GPIOA18_DEFAULT);
}

static void scm_gpio_a19(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA19_BIT, SCM_IOMUX_9C_GPIOA19_MASK,
                  SCM_IOMUX_9C_GPIOA19_DEFAULT);
}

static void scm_gpio_a20(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA20_BIT, SCM_IOMUX_9C_GPIOA20_MASK,
                  SCM_IOMUX_9C_GPIOA20_DEFAULT);
}

static void scm_gpio_a21(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA21_BIT, SCM_IOMUX_9C_GPIOA21_MASK,
                  SCM_IOMUX_9C_GPIOA21_DEFAULT);
}

static void scm_gpio_a22(void)
{
    scm_io_mux_9c(SCM_IOMUX_9C_GPIOA22_BIT, SCM_IOMUX_9C_GPIOA22_MASK,
                  SCM_IOMUX_9C_GPIOA22_DEFAULT);
}

static void scm_gpio_a23(void)
{
    scm_io_mux_a0(SCM_IOMUX_A0_GPIOA23_BIT, SCM_IOMUX_A0_GPIOA23_MASK,
                  SCM_IOMUX_A0_GPIOA23_DEFAULT);
}

static void scm_gpio_a24(void)
{
    scm_io_mux_98(SCM_IOMUX_98_I2C0_SCL_BIT, SCM_IOMUX_98_I2C0_SCL_MASK,
                  SCM_IOMUX_98_I2C0_SCL_GPIOA24);
}

static void scm_gpio_a25(void)
{
    scm_io_mux_98(SCM_IOMUX_98_I2C0_SDA_BIT, SCM_IOMUX_98_I2C0_SDA_MASK,
                  SCM_IOMUX_98_I2C0_SDA_GPIOA25);
}
static void scm_gpio_a26(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_CLK_BIT, SCM_IOMUX_70_SPI0_CLK_MASK,
                  SCM_IOMUX_70_SPI0_CLK_GPIOA26);
}

static void scm_gpio_a27(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_CS_BIT, SCM_IOMUX_70_SPI0_CS_MASK,
                  SCM_IOMUX_70_SPI0_CS_GPIOA27);
}

static void scm_gpio_a28(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_MISO_BIT, SCM_IOMUX_70_SPI0_MISO_MASK,
                  SCM_IOMUX_70_SPI0_MISO_GPIOA28);
}

static void scm_gpio_a29(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI0_MOSI_BIT, SCM_IOMUX_70_SPI0_MOSI_MASK,
                  SCM_IOMUX_70_SPI0_MISO_GPIOA29);
}

static void scm_gpio_a30(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_CLK_BIT, SCM_IOMUX_70_SPI1_CLK_MASK,
                  SCM_IOMUX_70_SPI1_CLK_GPIOA30);
}

static void scm_gpio_a31(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_CS0_BIT, SCM_IOMUX_70_SPI1_CS0_MASK,
                  SCM_IOMUX_70_SPI1_CS0_GPIOA31);
}

static void scm_gpio_b00(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_CS1_BIT, SCM_IOMUX_70_SPI1_CS1_MASK,
                  SCM_IOMUX_70_SPI1_CS1_GPIOB00);
}

static void scm_gpio_b01(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_MISO_BIT, SCM_IOMUX_70_SPI1_MISO_MASK,
                  SCM_IOMUX_70_SPI1_MISO_GPIOB01);
}

static void scm_gpio_b02(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI1_MOSI_BIT, SCM_IOMUX_70_SPI1_MOSI_MASK,
                  SCM_IOMUX_70_SPI1_MISO_GPIOB02);
}

static void scm_gpio_b03(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CLK_BIT, SCM_IOMUX_70_SPI2_CLK_MASK,
                  SCM_IOMUX_70_SPI2_CLK_GPIOB03);
}

static void scm_gpio_b04(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS0_BIT, SCM_IOMUX_70_SPI2_CS0_MASK,
                  SCM_IOMUX_70_SPI2_CS0_GPIOB04);
}

static void scm_gpio_b05(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS1_BIT, SCM_IOMUX_70_SPI2_CS1_MASK,
                  SCM_IOMUX_70_SPI2_CS1_GPIOB05);
}

static void scm_gpio_b06(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS2_BIT, SCM_IOMUX_70_SPI2_CS2_MASK,
                  SCM_IOMUX_70_SPI2_CS2_GPIOB06);
}

static void scm_gpio_b07(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS3_BIT, SCM_IOMUX_70_SPI2_CS3_MASK,
                  SCM_IOMUX_70_SPI2_CS3_GPIOB07);
}

static void scm_gpio_b08(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS4_BIT, SCM_IOMUX_70_SPI2_CS4_MASK,
                  SCM_IOMUX_70_SPI2_CS4_GPIOB08);
}

static void scm_gpio_b09(void)
{
    scm_io_mux_70(SCM_IOMUX_70_SPI2_CS5_BIT, SCM_IOMUX_70_SPI2_CS5_MASK,
                  SCM_IOMUX_70_SPI2_CS5_GPIOB09);
}

static void scm_gpio_b10(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_CS6_BIT, SCM_IOMUX_74_SPI2_CS6_MASK,
                  SCM_IOMUX_74_SPI2_CS6_GPIOB10);
}

static void scm_gpio_b11(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_CS7_BIT, SCM_IOMUX_74_SPI2_CS7_MASK,
                  SCM_IOMUX_74_SPI2_CS7_GPIOB11);
}

static void scm_gpio_b12(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_MISO_BIT, SCM_IOMUX_74_SPI2_MISO_MASK,
                  SCM_IOMUX_74_SPI2_MISO_GPIOB12);
}

static void scm_gpio_b13(void)
{
    scm_io_mux_74(SCM_IOMUX_74_SPI2_MOSI_BIT, SCM_IOMUX_74_SPI2_MOSI_MASK,
                  SCM_IOMUX_74_SPI2_MOSI_GPIOB13);
}

static void scm_gpio_b14(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART0_RX_BIT, SCM_IOMUX_74_UART0_RX_MASK,
                  SCM_IOMUX_74_UART0_RX_GPIOB14);
}

static void scm_gpio_b15(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART0_TX_BIT, SCM_IOMUX_74_UART0_TX_MASK,
                  SCM_IOMUX_74_UART0_TX_GPIOB15);
}

static void scm_gpio_b16(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART1_RX_BIT, SCM_IOMUX_74_UART1_RX_MASK,
                  SCM_IOMUX_74_UART1_RX_GPIOB16);
}

static void scm_gpio_b17(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART1_TX_BIT, SCM_IOMUX_74_UART1_TX_MASK,
                  SCM_IOMUX_74_UART1_TX_GPIOB17);
}

static void scm_gpio_b18(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART2_RX_BIT, SCM_IOMUX_74_UART2_RX_MASK,
                  SCM_IOMUX_74_UART2_RX_GPIOB18);
}

static void scm_gpio_b19(void)
{
    scm_io_mux_74(SCM_IOMUX_74_UART2_TX_BIT, SCM_IOMUX_74_UART2_TX_MASK,
                  SCM_IOMUX_74_UART2_TX_GPIOB19);
}

static void scm_gpio_b20(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART3_RX_BIT, SCM_IOMUX_78_UART3_RX_MASK,
                  SCM_IOMUX_78_UART3_RX_GPIOB20);
}

static void scm_gpio_b21(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART3_TX_BIT, SCM_IOMUX_78_UART3_TX_MASK,
                  SCM_IOMUX_78_UART3_TX_GPIOB21);
}

static void scm_gpio_b22(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART4_RX_BIT, SCM_IOMUX_78_UART4_RX_MASK,
                  SCM_IOMUX_78_UART4_RX_GPIOB22);
}

static void scm_gpio_b23(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART4_TX_BIT, SCM_IOMUX_78_UART4_TX_MASK,
                  SCM_IOMUX_78_UART4_TX_GPIOB23);
}

static void scm_gpio_b24(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART5_RX_BIT, SCM_IOMUX_78_UART5_RX_MASK,
                  SCM_IOMUX_78_UART5_RX_GPIOB24);
}

static void scm_gpio_b25(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART5_TX_BIT, SCM_IOMUX_78_UART5_TX_MASK,
                  SCM_IOMUX_78_UART5_TX_GPIOB25);
}

static void scm_gpio_b26(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART6_RX_BIT, SCM_IOMUX_78_UART6_RX_MASK,
                  SCM_IOMUX_78_UART6_RX_GPIOB26);
}

static void scm_gpio_b27(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART6_TX_BIT, SCM_IOMUX_78_UART6_TX_MASK,
                  SCM_IOMUX_78_UART6_TX_GPIOB27);
}

static void scm_gpio_b28(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART7_RX_BIT, SCM_IOMUX_78_UART7_RX_MASK,
                  SCM_IOMUX_78_UART7_RX_GPIOB28);
}

static void scm_gpio_b29(void)
{
    scm_io_mux_78(SCM_IOMUX_78_UART7_TX_BIT, SCM_IOMUX_78_UART7_TX_MASK,
                  SCM_IOMUX_78_UART7_TX_GPIOB29);
}

static void scm_gpio_b30(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D0_BIT, SCM_IOMUX_7C_PRM0_D0_MASK,
                  SCM_IOMUX_7C_PRM0_D0_GPIOB30);
}

static void scm_gpio_b31(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D1_BIT, SCM_IOMUX_7C_PRM0_D1_MASK,
                  SCM_IOMUX_7C_PRM0_D1_GPIOB31);
}

static void scm_gpio_c00(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D2_BIT, SCM_IOMUX_7C_PRM0_D2_MASK,
                  SCM_IOMUX_7C_PRM0_D2_GPIOC00);
}

static void scm_gpio_c01(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D3_BIT, SCM_IOMUX_7C_PRM0_D3_MASK,
                  SCM_IOMUX_7C_PRM0_D3_GPIOC01);
}

static void scm_gpio_c02(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D4_BIT, SCM_IOMUX_7C_PRM0_D4_MASK,
                  SCM_IOMUX_7C_PRM0_D4_GPIOC02);
}

static void scm_gpio_c03(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D5_BIT, SCM_IOMUX_7C_PRM0_D5_MASK,
                  SCM_IOMUX_7C_PRM0_D5_GPIOC03);
}

static void scm_gpio_c04(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D6_BIT, SCM_IOMUX_7C_PRM0_D6_MASK,
                  SCM_IOMUX_7C_PRM0_D6_GPIOC04);
}

static void scm_gpio_c05(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D7_BIT, SCM_IOMUX_7C_PRM0_D7_MASK,
                  SCM_IOMUX_7C_PRM0_D7_GPIOC05);
}

static void scm_gpio_c06(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D8_BIT, SCM_IOMUX_7C_PRM0_D8_MASK,
                  SCM_IOMUX_7C_PRM0_D8_GPIOC06);
}

static void scm_gpio_c07(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D9_BIT, SCM_IOMUX_7C_PRM0_D9_MASK,
                  SCM_IOMUX_7C_PRM0_D9_GPIOC07);
}

static void scm_gpio_c08(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D10_BIT, SCM_IOMUX_7C_PRM0_D10_MASK,
                  SCM_IOMUX_7C_PRM0_D10_GPIOC08);
}

static void scm_gpio_c09(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D11_BIT, SCM_IOMUX_7C_PRM0_D11_MASK,
                  SCM_IOMUX_7C_PRM0_D11_GPIOC09);
}

static void scm_gpio_c10(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D12_BIT, SCM_IOMUX_7C_PRM0_D12_MASK,
                  SCM_IOMUX_7C_PRM0_D12_GPIOC10);
}

static void scm_gpio_c11(void)
{
    scm_io_mux_7c(SCM_IOMUX_7C_PRM0_D13_BIT, SCM_IOMUX_7C_PRM0_D13_MASK,
                  SCM_IOMUX_7C_PRM0_D13_GPIOC11);
}

static void scm_gpio_c12(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_D14_BIT, SCM_IOMUX_80_PRM0_D14_MASK,
                  SCM_IOMUX_80_PRM0_D14_GPIOC12);
}

static void scm_gpio_c13(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_D15_BIT, SCM_IOMUX_80_PRM0_D15_MASK,
                  SCM_IOMUX_80_PRM0_D15_GPIOC13);
}

static void scm_gpio_c14(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK0_BIT, SCM_IOMUX_80_PRM0_CLK0_MASK,
                  SCM_IOMUX_80_PRM0_CLK0_GPIOC14);
}

static void scm_gpio_c15(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK1_BIT, SCM_IOMUX_80_PRM0_CLK1_MASK,
                  SCM_IOMUX_80_PRM0_CLK1_GPIOC15);
}

static void scm_gpio_c16(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK2_BIT, SCM_IOMUX_80_PRM0_CLK2_MASK,
                  SCM_IOMUX_80_PRM0_CLK2_GPIOC16);
}

static void scm_gpio_c17(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK3_BIT, SCM_IOMUX_80_PRM0_CLK3_MASK,
                  SCM_IOMUX_80_PRM0_CLK3_GPIOC17);
}

static void scm_gpio_c18(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK4_BIT, SCM_IOMUX_80_PRM0_CLK4_MASK,
                  SCM_IOMUX_80_PRM0_CLK4_GPIOC18);
}

static void scm_gpio_c19(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK5_BIT, SCM_IOMUX_80_PRM0_CLK5_MASK,
                  SCM_IOMUX_80_PRM0_CLK5_GPIOC19);
}

static void scm_gpio_c20(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK6_BIT, SCM_IOMUX_80_PRM0_CLK6_MASK,
                  SCM_IOMUX_80_PRM0_CLK6_GPIOC20);
}

static void scm_gpio_c21(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK7_BIT, SCM_IOMUX_80_PRM0_CLK7_MASK,
                  SCM_IOMUX_80_PRM0_CLK7_GPIOC21);
}

static void scm_gpio_c22(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK8_BIT, SCM_IOMUX_80_PRM0_CLK8_MASK,
                  SCM_IOMUX_80_PRM0_CLK8_GPIOC22);
}

static void scm_gpio_c23(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK9_BIT, SCM_IOMUX_80_PRM0_CLK9_MASK,
                  SCM_IOMUX_80_PRM0_CLK9_GPIOC23);
}

static void scm_gpio_c24(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK10_BIT, SCM_IOMUX_80_PRM0_CLK10_MASK,
                  SCM_IOMUX_80_PRM0_CLK10_GPIOC24);
}

static void scm_gpio_c25(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK11_BIT, SCM_IOMUX_80_PRM0_CLK11_MASK,
                  SCM_IOMUX_80_PRM0_CLK11_GPIOC25);
}

static void scm_gpio_c26(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK12_BIT, SCM_IOMUX_80_PRM0_CLK12_MASK,
                  SCM_IOMUX_80_PRM0_CLK12_GPIOC26);
}

static void scm_gpio_c27(void)
{
    scm_io_mux_80(SCM_IOMUX_80_PRM0_CLK13_BIT, SCM_IOMUX_80_PRM0_CLK13_MASK,
                  SCM_IOMUX_80_PRM0_CLK13_GPIOC27);
}

static void scm_gpio_c28(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM0_CLK14_BIT, SCM_IOMUX_84_PRM0_CLK14_MASK,
                  SCM_IOMUX_84_PRM0_CLK14_GPIOC28);
}

static void scm_gpio_c29(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM0_CLK15_BIT, SCM_IOMUX_84_PRM0_CLK15_MASK,
                  SCM_IOMUX_84_PRM0_CLK15_GPIOC29);
}

static void scm_gpio_c30(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM0_RST_BIT, SCM_IOMUX_84_PRM0_RST_MASK,
                  SCM_IOMUX_84_PRM0_RST_GPIOC30);
}

static void scm_gpio_c31(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM_SYSCLK_BIT, SCM_IOMUX_84_PRM_SYSCLK_MASK,
                  SCM_IOMUX_84_PRM_SYSCLK_GPIOC31);
}

static void scm_gpio_d00(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D0_BIT, SCM_IOMUX_84_PRM1_D0_MASK,
                  SCM_IOMUX_84_PRM1_D0_GPIOD00);
}

static void scm_gpio_d01(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D1_BIT, SCM_IOMUX_84_PRM1_D1_MASK,
                  SCM_IOMUX_84_PRM1_D1_GPIOD01);
}

static void scm_gpio_d02(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D2_BIT, SCM_IOMUX_84_PRM1_D2_MASK,
                  SCM_IOMUX_84_PRM1_D2_GPIOD02);
}

static void scm_gpio_d03(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D3_BIT, SCM_IOMUX_84_PRM1_D3_MASK,
                  SCM_IOMUX_84_PRM1_D3_GPIOD03);
}

static void scm_gpio_d04(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D4_BIT, SCM_IOMUX_84_PRM1_D4_MASK,
                  SCM_IOMUX_84_PRM1_D4_GPIOCD04);
}

static void scm_gpio_d05(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D5_BIT, SCM_IOMUX_84_PRM1_D5_MASK,
                  SCM_IOMUX_84_PRM1_D5_GPIOD05);
}

static void scm_gpio_d06(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D6_BIT, SCM_IOMUX_84_PRM1_D6_MASK,
                  SCM_IOMUX_84_PRM1_D6_GPIOD06);
}

static void scm_gpio_d07(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D7_BIT, SCM_IOMUX_84_PRM1_D7_MASK,
                  SCM_IOMUX_84_PRM1_D7_GPIOD07);
}

static void scm_gpio_d08(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK0_BIT, SCM_IOMUX_88_PRM1_CLK0_MASK,
                  SCM_IOMUX_88_PRM1_CLK0_GPIOD08);
}

static void scm_gpio_d09(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK1_BIT, SCM_IOMUX_88_PRM1_CLK1_MASK,
                  SCM_IOMUX_88_PRM1_CLK1_GPIOD09);
}

static void scm_gpio_d10(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK2_BIT, SCM_IOMUX_88_PRM1_CLK2_MASK,
                  SCM_IOMUX_88_PRM1_CLK2_GPIOD10);
}

static void scm_gpio_d11(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK3_BIT, SCM_IOMUX_88_PRM1_CLK3_MASK,
                  SCM_IOMUX_88_PRM1_CLK3_GPIOD11);
}

static void scm_gpio_d12(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK4_BIT, SCM_IOMUX_88_PRM1_CLK4_MASK,
                  SCM_IOMUX_88_PRM1_CLK4_GPIOD12);
}

static void scm_gpio_d13(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK5_BIT, SCM_IOMUX_88_PRM1_CLK5_MASK,
                  SCM_IOMUX_88_PRM1_CLK5_GPIOD13);
}

static void scm_gpio_d14(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK6_BIT, SCM_IOMUX_88_PRM1_CLK6_MASK,
                  SCM_IOMUX_88_PRM1_CLK6_GPIOD14);
}

static void scm_gpio_d15(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_CLK7_BIT, SCM_IOMUX_88_PRM1_CLK7_MASK,
                  SCM_IOMUX_88_PRM1_CLK7_GPIOD15);
}

static void scm_gpio_d16(void)
{
    scm_io_mux_98(SCM_IOMUX_98_HWPWM0_BIT, SCM_IOMUX_98_HWPWM0_MASK, SCM_IOMUX_98_HWPWM0_GPIOD16);
}

static void scm_gpio_d17(void)
{
    scm_io_mux_98(SCM_IOMUX_98_HWPWM1_BIT, SCM_IOMUX_98_HWPWM1_MASK, SCM_IOMUX_98_HWPWM1_GPIOD17);
}

static void scm_gpio_d18(void)
{
    scm_io_mux_98(SCM_IOMUX_98_HWPWM2_BIT, SCM_IOMUX_98_HWPWM2_MASK, SCM_IOMUX_98_HWPWM2_GPIOD18);
}

static void scm_gpio_d19(void)
{
    scm_io_mux_98(SCM_IOMUX_98_HWPWM3_BIT, SCM_IOMUX_98_HWPWM3_MASK, SCM_IOMUX_98_HWPWM3_GPIOD19);
}

static void scm_gpio_d20(void)
{
    scm_io_mux_98(SCM_IOMUX_98_SWPWM0_BIT, SCM_IOMUX_98_SWPWM0_MASK, SCM_IOMUX_98_SWPWM0_GPIOD20);
}

static void scm_gpio_d21(void)
{
    scm_io_mux_a0(SCM_IOMUX_A0_SWPWM1_BIT, SCM_IOMUX_A0_SWPWM1_MASK, SCM_IOMUX_A0_SWPWM1_GPIOD21);
}

static void scm_gpio_d22(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PPS0_BIT, SCM_IOMUX_98_PPS0_MASK, SCM_IOMUX_98_PPS0_GPIOD22);
}

static void scm_gpio_d23(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PPS1_BIT, SCM_IOMUX_98_PPS1_MASK, SCM_IOMUX_98_PPS1_GPIOD23);
}

static void scm_gpio_d24(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN40_BIT, SCM_IOMUX_94_PRM1_DIN40_MASK,
                  SCM_IOMUX_94_PRM1_DIN40_GPIOD24);
}

static void scm_gpio_d25(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN41_BIT, SCM_IOMUX_94_PRM1_DIN41_MASK,
                  SCM_IOMUX_94_PRM1_DIN41_GPIOD25);
}

static void scm_gpio_d26(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN42_BIT, SCM_IOMUX_94_PRM1_DIN42_MASK,
                  SCM_IOMUX_94_PRM1_DIN42_GPIOD26);
}

static void scm_gpio_d27(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN43_BIT, SCM_IOMUX_98_PRM1_DIN43_MASK,
                  SCM_IOMUX_98_PRM1_DIN43_GPIOD27);
}

static void scm_gpio_d28(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN44_BIT, SCM_IOMUX_98_PRM1_DIN44_MASK,
                  SCM_IOMUX_98_PRM1_DIN44_GPIOD28);
}

static void scm_gpio_d29(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN45_BIT, SCM_IOMUX_98_PRM1_DIN45_MASK,
                  SCM_IOMUX_98_PRM1_DIN45_GPIOD29);
}

static void scm_gpio_d30(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN46_BIT, SCM_IOMUX_98_PRM1_DIN46_MASK,
                  SCM_IOMUX_98_PRM1_DIN46_GPIOD30);
}

static void scm_gpio_d31(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN47_BIT, SCM_IOMUX_98_PRM1_DIN47_MASK,
                  SCM_IOMUX_98_PRM1_DIN47_GPIOD31);
}

int scm_gpioa_en(int index)
{
    switch (index) {
    case 0:
        scm_gpio_a00();
        break;
    case 1:
        scm_gpio_a01();
        break;
    case 2:
        scm_gpio_a02();
        break;
    case 3:
        scm_gpio_a03();
        break;
    case 4:
        scm_gpio_a04();
        break;
    case 5:
        scm_gpio_a05();
        break;
    case 6:
        scm_gpio_a06();
        break;
    case 7:
        scm_gpio_a07();
        break;
    case 8:
        scm_gpio_a08();
        break;
    case 9:
        scm_gpio_a09();
        break;
    case 10:
        scm_gpio_a10();
        break;
    case 11:
        scm_gpio_a11();
        break;
    case 12:
        scm_gpio_a12();
        break;
    case 13:
        scm_gpio_a13();
        break;
    case 14:
        scm_gpio_a14();
        break;
    case 15:
        scm_gpio_a15();
        break;
    case 16:
        scm_gpio_a16();
        break;
    case 17:
        scm_gpio_a17();
        break;
    case 18:
        scm_gpio_a18();
        break;
    case 19:
        scm_gpio_a19();
        break;
    case 20:
        scm_gpio_a20();
        break;
    case 21:
        scm_gpio_a21();
        break;
    case 22:
        scm_gpio_a22();
        break;
    case 23:
        scm_gpio_a23();
        break;
    case 24:
        scm_gpio_a24();
        break;
    case 25:
        scm_gpio_a25();
        break;
    case 26:
        scm_gpio_a26();
        break;
    case 27:
        scm_gpio_a27();
        break;
    case 28:
        scm_gpio_a28();
        break;
    case 29:
        scm_gpio_a29();
        break;
    case 30:
        scm_gpio_a30();
        break;
    case 31:
        scm_gpio_a31();
        break;
    default:
        iomux_printf("GPIO A: index %d error.", index);
        return DRV_OP_FAILED;
    }

    return DRV_OP_SUCCESS;
}

int scm_gpiob_en(int index)
{
    switch (index) {
    case 0:
        scm_gpio_b00();
        break;
    case 1:
        scm_gpio_b01();
        break;
    case 2:
        scm_gpio_b02();
        break;
    case 3:
        scm_gpio_b03();
        break;
    case 4:
        scm_gpio_b04();
        break;
    case 5:
        scm_gpio_b05();
        break;
    case 6:
        scm_gpio_b06();
        break;
    case 7:
        scm_gpio_b07();
        break;
    case 8:
        scm_gpio_b08();
        break;
    case 9:
        scm_gpio_b09();
        break;
    case 10:
        scm_gpio_b10();
        break;
    case 11:
        scm_gpio_b11();
        break;
    case 12:
        scm_gpio_b12();
        break;
    case 13:
        scm_gpio_b13();
        break;
    case 14:
        scm_gpio_b14();
        break;
    case 15:
        scm_gpio_b15();
        break;
    case 16:
        scm_gpio_b16();
        break;
    case 17:
        scm_gpio_b17();
        break;
    case 18:
        scm_gpio_b18();
        break;
    case 19:
        scm_gpio_b19();
        break;
    case 20:
        scm_gpio_b20();
        break;
    case 21:
        scm_gpio_b21();
        break;
    case 22:
        scm_gpio_b22();
        break;
    case 23:
        scm_gpio_b23();
        break;
    case 24:
        scm_gpio_b24();
        break;
    case 25:
        scm_gpio_b25();
        break;
    case 26:
        scm_gpio_b26();
        break;
    case 27:
        scm_gpio_b27();
        break;
    case 28:
        scm_gpio_b28();
        break;
    case 29:
        scm_gpio_b29();
        break;
    case 30:
        scm_gpio_b30();
        break;
    case 31:
        scm_gpio_b31();
        break;
    default:
        iomux_printf("GPIO B: index %d error.", index);
        return DRV_OP_FAILED;
    }

    return DRV_OP_SUCCESS;
}

int scm_gpioc_en(int index)
{
    switch (index) {
    case 0:
        scm_gpio_c00();
        break;
    case 1:
        scm_gpio_c01();
        break;
    case 2:
        scm_gpio_c02();
        break;
    case 3:
        scm_gpio_c03();
        break;
    case 4:
        scm_gpio_c04();
        break;
    case 5:
        scm_gpio_c05();
        break;
    case 6:
        scm_gpio_c06();
        break;
    case 7:
        scm_gpio_c07();
        break;
    case 8:
        scm_gpio_c08();
        break;
    case 9:
        scm_gpio_c09();
        break;
    case 10:
        scm_gpio_c10();
        break;
    case 11:
        scm_gpio_c11();
        break;
    case 12:
        scm_gpio_c12();
        break;
    case 13:
        scm_gpio_c13();
        break;
    case 14:
        scm_gpio_c14();
        break;
    case 15:
        scm_gpio_c15();
        break;
    case 16:
        scm_gpio_c16();
        break;
    case 17:
        scm_gpio_c17();
        break;
    case 18:
        scm_gpio_c18();
        break;
    case 19:
        scm_gpio_c19();
        break;
    case 20:
        scm_gpio_c20();
        break;
    case 21:
        scm_gpio_c21();
        break;
    case 22:
        scm_gpio_c22();
        break;
    case 23:
        scm_gpio_c23();
        break;
    case 24:
        scm_gpio_c24();
        break;
    case 25:
        scm_gpio_c25();
        break;
    case 26:
        scm_gpio_c26();
        break;
    case 27:
        scm_gpio_c27();
        break;
    case 28:
        scm_gpio_c28();
        break;
    case 29:
        scm_gpio_c29();
        break;
    case 30:
        scm_gpio_c30();
        break;
    case 31:
        scm_gpio_c31();
        break;
    default:
        iomux_printf("GPIO C: index %d error.", index);
        return DRV_OP_FAILED;
    }

    return DRV_OP_SUCCESS;
}

int scm_gpiod_en(int index)
{
    switch (index) {
    case 0:
        scm_gpio_d00();
        break;
    case 1:
        scm_gpio_d01();
        break;
    case 2:
        scm_gpio_d02();
        break;
    case 3:
        scm_gpio_d03();
        break;
    case 4:
        scm_gpio_d04();
        break;
    case 5:
        scm_gpio_d05();
        break;
    case 6:
        scm_gpio_d06();
        break;
    case 7:
        scm_gpio_d07();
        break;
    case 8:
        scm_gpio_d08();
        break;
    case 9:
        scm_gpio_d09();
        break;
    case 10:
        scm_gpio_d10();
        break;
    case 11:
        scm_gpio_d11();
        break;
    case 12:
        scm_gpio_d12();
        break;
    case 13:
        scm_gpio_d13();
        break;
    case 14:
        scm_gpio_d14();
        break;
    case 15:
        scm_gpio_d15();
        break;
    case 16:
        scm_gpio_d16();
        break;
    case 17:
        scm_gpio_d17();
        break;
    case 18:
        scm_gpio_d18();
        break;
    case 19:
        scm_gpio_d19();
        break;
    case 20:
        scm_gpio_d20();
        break;
    case 21:
        scm_gpio_d21();
        break;
    case 22:
        scm_gpio_d22();
        break;
    case 23:
        scm_gpio_d23();
        break;
    case 24:
        scm_gpio_d24();
        break;
    case 25:
        scm_gpio_d25();
        break;
    case 26:
        scm_gpio_d26();
        break;
    case 27:
        scm_gpio_d27();
        break;
    case 28:
        scm_gpio_d28();
        break;
    case 29:
        scm_gpio_d29();
        break;
    case 30:
        scm_gpio_d30();
        break;
    case 31:
        scm_gpio_d31();
        break;
    default:
        iomux_printf("GPIO D: index %d error.", index);
        return DRV_OP_FAILED;
    }

    return DRV_OP_SUCCESS;
}
