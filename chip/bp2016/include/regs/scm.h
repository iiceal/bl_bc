#ifndef _REGS_SCM_H_
#define _REGS_SCM_H_

#define REG_AP_SCM_BASE         (APB2_BASE+0xd0000)

typedef volatile struct 
{
    REG32       addr;
    REG32       size;
}CPU_COMM_REGS;

typedef volatile struct
{
    REG32              version;          //0x00000000

    REG32              clk_ctrl;         //0x00000004
    REG32              clk_div0;         //0x00000008
    REG32              clk_div1;         //0x0000000C
    REG32              clk_div2;         //0x00000010

    REG32              pll01_ctrl;       //0x00000014
    REG32              pll23_ctrl;       //0x00000018

    REG32              clken_ctrl0;      //0x0000001c
    REG32              clken_ctrl1;      //0x00000020

    REG32              swrst_ctrl;       //0x00000024
    REG32              pwrgate_ctrl;     //0x00000028


    REG32              cpu1right_ctrl;   //0x0000002c
    REG32              cpu1zero_addr;    //0x00000030
    REG32              cpu1trap_addr;    //0x00000034
    REG32              cpu1rom_baddr;    //0x00000038
    REG32              cpu1rom_eaddr;    //0x0000003c
    REG32              cpu1ram_baddr;    //0x00000040
    REG32              cpu1ram_eaddr;    //0x00000044
    REG32              cpu1qspi_baddr;   //0x00000048
    REG32              cpu1qspi_eaddr;   //0x0000004c
    REG32              cpu1ddr_baddr;    //0x00000050
    REG32              cpu1ddr_eaddr;    //0x00000054
  
    REG32              rsv_58_5c[2];     //0x00000058~5c

    REG32			   spi_ctrl;	     //0x00000060
    REG32			   dma_ctrl;	     //0x00000064
    REG32			   din_clk_ctrl;     //0x00000068
    REG32			   strappingstatus;  //0x0000006c
 
    REG32			   io_mux_70;        //0x00000070
    REG32			   io_mux_74;        //0x00000074
    REG32			   io_mux_78;        //0x00000078
    REG32			   io_mux_7c;        //0x0000007c
    REG32			   io_mux_80;        //0x00000080
    REG32			   io_mux_84;        //0x00000084
    REG32			   io_mux_88;        //0x00000088
    REG32			   io_mux_8c;        //0x0000008c
    REG32			   io_mux_90;        //0x00000090
    REG32			   io_mux_94;        //0x00000094
    REG32			   io_mux_98;        //0x00000098
    REG32			   io_mux_9c;        //0x0000009c
    REG32			   io_mux_a0;        //0x000000a0
 
    REG32			   pll4_ctrl;        //0x000000a4               //del this register
    REG32			   rsv_a8_ac[2];     //0x000000a8~ac
    	
    REG32			   efuse_dw0;        //0x000000b0
    REG32			   efuse_dw1;        //0x000000b4
    REG32			   efuse_dw2;        //0x000000b8
    REG32			   efuse_dw3;        //0x000000bc
    REG32			   efuse_dw4;        //0x000000c0
    REG32			   efuse_dw5;        //0x000000c4
    REG32			   efuse_dw6;        //0x000000c8
    REG32			   efuse_dw7;        //0x000000cc
    
    REG32			   efuse_ctr0;       //0x000000d0
    REG32			   efuse_ctr1;       //0x000000d4
    REG32			   efuse_ctr2;       //0x000000d8
    REG32			   efuse_ctr3;       //0x000000dc

    REG32			   adc_ctl;          //0x000000e0
    REG32			   adc_status;       //0x000000e4
    REG32			   adc_tb;           //0x000000e8
    REG32			   misc;             //0x000000ec

    REG32			   rsv_d0_ffc[964];  //0x000000f0~ffc

    CPU_COMM_REGS      cpu0_int_regs[16];//0x00001000~0x107c
    CPU_COMM_REGS      cpu1_int_regs[16];//0x00001080~0x10fc
    REG32			   sw_bp_magic;      //0x00001100
    REG32			   sw_bp_jaddr;	     //0x00001104
    REG32			   sw_ap_magic;      //0x00001108
    REG32			   sw_ap_jaddr;	     //0x0000110c

    REG32              rsv_1110_114c[16];//0x1110~0x114c
    REG32              cpu0to1_int_clr;  //0x00001150
    REG32              cpu1to0_int_clr;  //0x00001154
    REG32              cpu0to1_int;      //0x00001158
    REG32              cpu1to0_int;      //0x0000115c
    REG32              rsv_1160_11ec[36];//0x1160~0x11ec

    REG32			   cpu_r_f_addr;     //0x000011f0
    REG32			   cpu_w_f_addr;     //0x000011f4
    REG32			   dma1_r_f_addr;    //0x000011f8
    REG32			   dma1_w_f_addr;    //0x000011fc

    REG32			   lock_dev_0;	     //0x000001200
    REG32			   lock_dev_1;	     //0x000001204
    REG32			   lock_dev_2;	     //0x000001208
    REG32			   lock_dev_3;	     //0x00000120c
    REG32			   lock_dev_4;	     //0x000001210
    REG32			   lock_dev_5;	     //0x000001214
    REG32			   lock_dev_6;	     //0x000001218
    REG32			   lock_dev_7;	     //0x00000121c

    REG32			   rsv_1220_1ffc[888]; //0x0000001220~1ffc
    REG32			   lock_ctrl;	     //0x000002000
    REG32			   rsv_2004_2ffc[1023]; //0x0000002004~2ffc
    REG32			   global_rst;	     //0x000003000

} HWP_SCM_AP_T;

#define hwp_apSCM               ((HWP_SCM_AP_T*) (REG_AP_SCM_BASE))

#define	LOCK_CTRL_DISABLE_JTAG0		(1<<0)

//strap
#define	SCM_STRAP_SW_MASK		        (3)
#define	BP2016_BOOT_SPIFLASH_AHBREAD 	(0) //cache enable + spi ahbread
#define	BP2016_BOOT_SPIFLASH_QUAD 	    (1) //cache enable + spi quadread
#define	BP2016_BOOT_SPIFLASH_QUAD_DMA 	(2) //cache enable + spi quadread + dma
#define	BP2016_BOOT_SIMPLE 	            (3) //disable mmu + bypass security_sign
                                            //only boot bootloader

#define SCM_STRAP_REFCLK_MASK           (0xc)
#define SCM_STRAP_REFCLK_10M            (0x0)
#define SCM_STRAP_REFCLK_20M            (0x4)
#define SCM_STRAP_REFCLK_25M            (0x8)
#define SCM_STRAP_REFCLK_EFUSE          (0xc)
//clk gating
#define     SCM_CLK_EN_CPU1             (0)
#define     SCM_CLK_EN_DDR              (1)
#define     SCM_CLK_EN_62M              (2)
#define     SCM_CLK_EN_ROM              (3)
#define     SCM_CLK_EN_RAM              (4)
#define     SCM_CLK_EN_BB               (5)
#define     SCM_CLK_EN_MP               (6)
#define     SCM_CLK_EN_DMAC0            (7)
#define     SCM_CLK_EN_DMAC1            (8)
#define     SCM_CLK_EN_QSPI             (9)
#define     SCM_CLK_EN_SD               (10)
#define     SCM_CLK_EN_UART0            (11)
#define     SCM_CLK_EN_UART1            (12)
#define     SCM_CLK_EN_UART2            (13)
#define     SCM_CLK_EN_UART3            (14)
#define     SCM_CLK_EN_UART4            (15)
#define     SCM_CLK_EN_UART5            (16)
#define     SCM_CLK_EN_UART6            (17)
#define     SCM_CLK_EN_UART7            (18)
#define     SCM_CLK_EN_I2C0             (19)
#define     SCM_CLK_EN_I2C1             (20)
#define     SCM_CLK_EN_SPI0             (21)
#define     SCM_CLK_EN_SPI1             (22)
#define     SCM_CLK_EN_SPI2             (23)
#define     SCM_CLK_EN_SPI3             (24)
#define     SCM_CLK_EN_SIM0             (25)
#define     SCM_CLK_EN_SIM1             (26)
#define     SCM_CLK_EN_SIM2             (27)
#define     SCM_CLK_EN_PWM              (28)
#define     SCM_CLK_EN_GPIO             (29)
#define     SCM_CLK_EN_TIMER            (30)
#define     SCM_CLK_EN_EFUSE            (31)
#define     SCM_CLK_EN_SYS32K           (0)
#define     SCM_CLK_EN_5M               (1)
#define     SCM_CLK_EN_RDSS             (2)

//clk div
#define     SCM_CLK_DIV_CPU0_BITS           (0)
#define     SCM_CLK_DIV_CPU0_MASK           (0x0f)
#define     SCM_CLK_DIV_CPU1_BITS           (4)
#define     SCM_CLK_DIV_CPU1_MASK           (0x0f)
#define     SCM_CLK_DIV_AXI_BITS            (8)
#define     SCM_CLK_DIV_AXI_MASK            (0x0f)
#define     SCM_CLK_DIV_AHB_BITS            (12)
#define     SCM_CLK_DIV_AHB_MASK            (0x0f)
#define     SCM_CLK_DIV_APB_BITS            (16)
#define     SCM_CLK_DIV_APB_MASK            (0x0f)
#define     SCM_CLK_DIV_DDR_BITS            (20)
#define     SCM_CLK_DIV_DDR_MASK            (0x0f)
#define     SCM_CLK_DIV_62M_BITS            (24)
#define     SCM_CLK_DIV_62M_MASK            (0x0f)
#define     SCM_CLK_DIV_RAM_BITS            (28)
#define     SCM_CLK_DIV_RAM_MASK            (0x0f)

#define     SCM_CLK_DIV_SD_BITS              (0)
#define     SCM_CLK_DIV_SD_MASK              (0x0f)
#define     SCM_CLK_DIV_BB_BITS              (4)
#define     SCM_CLK_DIV_BB_MASK              (0x0f)
#define     SCM_CLK_DIV_MP_BITS              (8)
#define     SCM_CLK_DIV_MP_MASK              (0x0f)
#define     SCM_CLK_DIV_QSPI_BITS            (12)
#define     SCM_CLK_DIV_QSPI_MASK            (0x0f)
#define     SCM_CLK_DIV_I2C_BITS             (16)
#define     SCM_CLK_DIV_I2C_MASK             (0x0f)
#define     SCM_CLK_DIV_UART6_BITS           (20)
#define     SCM_CLK_DIV_UART6_MASK           (0x3f)
#define     SCM_CLK_DIV_UART7_BITS           (26)
#define     SCM_CLK_DIV_UART7_MASK           (0x3f)

#define     SCM_CLK_DIV_SPI0_BITS           (0)
#define     SCM_CLK_DIV_SPI0_MASK           (0x0f)
#define     SCM_CLK_DIV_SPI1_BITS           (4)
#define     SCM_CLK_DIV_SPI1_MASK           (0x0f)
#define     SCM_CLK_DIV_SPI2_BITS           (8)
#define     SCM_CLK_DIV_SPI2_MASK           (0x0f)
#define     SCM_CLK_DIV_SPI3_BITS           (12)
#define     SCM_CLK_DIV_SPI3_MASK           (0x0f)
#define     SCM_CLK_DIV_SIM0_BITS           (16)
#define     SCM_CLK_DIV_SIM0_MASK           (0x0f)
#define     SCM_CLK_DIV_SIM1_BITS           (20)
#define     SCM_CLK_DIV_SIM1_MASK           (0x0f)
#define     SCM_CLK_DIV_SIM2_BITS           (24)
#define     SCM_CLK_DIV_SIM2_MASK           (0x0f)
#define     SCM_CLK_DIV_PWM_BITS            (28)
#define     SCM_CLK_DIV_PWM_MASK            (0x0f)

#define     SCM_CLK_DIV_RDSS_BITS           (16)
#define     SCM_CLK_DIV_RDSS_MASK           (0x0f)

//swrst_ctrl
#define     SCM_SWRST_CPU1                  (0)
#define     SCM_SWRST_DDR                   (1)
//#define     SCM_SWRST_62M                   (2)
#define     SCM_SWRST_ROM                   (3)
#define     SCM_SWRST_RAM                   (4)
#define     SCM_SWRST_BB                    (5)
#define     SCM_SWRST_MP                    (6)
#define     SCM_SWRST_DMAC0                 (7)
#define     SCM_SWRST_DMAC1                 (8)
#define     SCM_SWRST_QSPI                  (9)
#define     SCM_SWRST_SD                    (10)
#define     SCM_SWRST_UART0                 (11)
#define     SCM_SWRST_UART1                 (12)
#define     SCM_SWRST_UART2                 (13)
#define     SCM_SWRST_UART3                 (14)
#define     SCM_SWRST_UART4                 (15)
#define     SCM_SWRST_UART5                 (16)
#define     SCM_SWRST_UART6                 (17)
#define     SCM_SWRST_UART7                 (18)
#define     SCM_SWRST_I2C0                  (19)
#define     SCM_SWRST_I2C1                  (20)
#define     SCM_SWRST_SPI0                  (21)
#define     SCM_SWRST_SPI1                  (22)
#define     SCM_SWRST_SPI2                  (23)
#define     SCM_SWRST_SPI3                  (24)
#define     SCM_SWRST_SIM0                  (25)
#define     SCM_SWRST_SIM1                  (26)
#define     SCM_SWRST_SIM2                  (27)
#define     SCM_SWRST_PWM                   (28)
#define     SCM_SWRST_GPIO                  (29)
#define     SCM_SWRST_TIMER                 (30)

//iomux70:spi0/1/2 
//spi0
#define   SCM_IOMUX_70_SPI0_CLK_BIT             (0)
#define   SCM_IOMUX_70_SPI0_CLK_MASK            (3)
#define   SCM_IOMUX_70_SPI0_CLK_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI0_CLK_SIM1_CLK        (1)
#define   SCM_IOMUX_70_SPI0_CLK_SPI3_CLK        (2)
#define   SCM_IOMUX_70_SPI0_CLK_GPIOA26         (3)

#define   SCM_IOMUX_70_SPI0_CS_BIT              (2)
#define   SCM_IOMUX_70_SPI0_CS_MASK             (3)
#define   SCM_IOMUX_70_SPI0_CS_DEFAULT          (0)
#define   SCM_IOMUX_70_SPI0_CS_SPI3_CS          (2)
#define   SCM_IOMUX_70_SPI0_CS_GPIOA27          (3)

#define   SCM_IOMUX_70_SPI0_MISO_BIT            (4)
#define   SCM_IOMUX_70_SPI0_MISO_MASK           (3)
#define   SCM_IOMUX_70_SPI0_MISO_DEFAULT        (0)
#define   SCM_IOMUX_70_SPI0_MISO_SIM1_IO        (1)
#define   SCM_IOMUX_70_SPI0_MISO_SPI3_MOSI      (2)
#define   SCM_IOMUX_70_SPI0_MISO_GPIOA28        (3)

#define   SCM_IOMUX_70_SPI0_MOSI_BIT            (6)
#define   SCM_IOMUX_70_SPI0_MOSI_MASK           (3)
#define   SCM_IOMUX_70_SPI0_MOSI_DEFAULT        (0)
#define   SCM_IOMUX_70_SPI0_MOSI_SIM1_RST       (1)
#define   SCM_IOMUX_70_SPI0_MOSI_SPI3_MISO      (2)
#define   SCM_IOMUX_70_SPI0_MISO_GPIOA29        (3)

//spi1
#define   SCM_IOMUX_70_SPI1_CLK_BIT             (8)
#define   SCM_IOMUX_70_SPI1_CLK_MASK            (3)
#define   SCM_IOMUX_70_SPI1_CLK_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI1_CLK_GPIOA30         (3)

#define   SCM_IOMUX_70_SPI1_CS0_BIT             (10)
#define   SCM_IOMUX_70_SPI1_CS0_MASK            (3)
#define   SCM_IOMUX_70_SPI1_CS0_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI1_CS0_GPIOA31         (3)

#define   SCM_IOMUX_70_SPI1_CS1_BIT             (12)
#define   SCM_IOMUX_70_SPI1_CS1_MASK            (3)
#define   SCM_IOMUX_70_SPI1_CS1_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI1_CS1_GPIOB00         (3)

#define   SCM_IOMUX_70_SPI1_MISO_BIT            (14)
#define   SCM_IOMUX_70_SPI1_MISO_MASK           (3)
#define   SCM_IOMUX_70_SPI1_MISO_DEFAULT        (0)
#define   SCM_IOMUX_70_SPI1_MISO_GPIOB01        (3)

#define   SCM_IOMUX_70_SPI1_MOSI_BIT            (16)
#define   SCM_IOMUX_70_SPI1_MOSI_MASK           (3)
#define   SCM_IOMUX_70_SPI1_MOSI_DEFAULT        (0)
#define   SCM_IOMUX_70_SPI1_MISO_GPIOB02        (3)

//spi2
#define   SCM_IOMUX_70_SPI2_CLK_BIT             (18)
#define   SCM_IOMUX_70_SPI2_CLK_MASK            (3)
#define   SCM_IOMUX_70_SPI2_CLK_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI2_CLK_SIM0_CLK        (1)
#define   SCM_IOMUX_70_SPI2_CLK_SPI4_CLK        (2)
#define   SCM_IOMUX_70_SPI2_CLK_GPIOB03         (3)

#define   SCM_IOMUX_70_SPI2_CS0_BIT             (20)
#define   SCM_IOMUX_70_SPI2_CS0_MASK            (3)
#define   SCM_IOMUX_70_SPI2_CS0_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI2_CS0_SPI4_SEN0       (2)
#define   SCM_IOMUX_70_SPI2_CS0_GPIOB04         (3)

#define   SCM_IOMUX_70_SPI2_CS1_BIT             (22)
#define   SCM_IOMUX_70_SPI2_CS1_MASK            (3)
#define   SCM_IOMUX_70_SPI2_CS1_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI2_CS1_SPI4_SEN1       (2)
#define   SCM_IOMUX_70_SPI2_CS1_GPIOB05         (3)

#define   SCM_IOMUX_70_SPI2_CS2_BIT             (24)
#define   SCM_IOMUX_70_SPI2_CS2_MASK            (3)
#define   SCM_IOMUX_70_SPI2_CS2_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI2_CS2_SIM0_IO         (1)
#define   SCM_IOMUX_70_SPI2_CS2_GPIOB06         (3)

#define   SCM_IOMUX_70_SPI2_CS3_BIT             (26)
#define   SCM_IOMUX_70_SPI2_CS3_MASK            (3)
#define   SCM_IOMUX_70_SPI2_CS3_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI2_CS3_SIM0_RST        (1)
#define   SCM_IOMUX_70_SPI2_CS3_GPIOB07         (3)

#define   SCM_IOMUX_70_SPI2_CS4_BIT             (28)
#define   SCM_IOMUX_70_SPI2_CS4_MASK            (3)
#define   SCM_IOMUX_70_SPI2_CS4_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI2_CS4_ODO_PULSE       (1)
#define   SCM_IOMUX_70_SPI2_CS4_GPIOB08         (3)

#define   SCM_IOMUX_70_SPI2_CS5_BIT             (30)
#define   SCM_IOMUX_70_SPI2_CS5_MASK            (3)
#define   SCM_IOMUX_70_SPI2_CS5_DEFAULT         (0)
#define   SCM_IOMUX_70_SPI2_CS5_ODO_DIRECT      (1)
#define   SCM_IOMUX_70_SPI2_CS5_GPIOB09         (3)

//iomux74 spi2/qspi/uart0/uart1/uart2/
//spi2
#define   SCM_IOMUX_74_SPI2_CS6_BIT             (0)
#define   SCM_IOMUX_74_SPI2_CS6_MASK            (3)
#define   SCM_IOMUX_74_SPI2_CS6_DEFAULT         (0)
#define   SCM_IOMUX_74_SPI2_CS6_I2C1_SCL        (1)
#define   SCM_IOMUX_74_SPI2_CS6_GPIOB10         (3)

#define   SCM_IOMUX_74_SPI2_CS7_BIT             (2)
#define   SCM_IOMUX_74_SPI2_CS7_MASK            (3)
#define   SCM_IOMUX_74_SPI2_CS7_DEFAULT         (0)
#define   SCM_IOMUX_74_SPI2_CS7_I2C1_SDA        (1)
#define   SCM_IOMUX_74_SPI2_CS7_GPIOB11         (3)

#define   SCM_IOMUX_74_SPI2_MISO_BIT            (4)
#define   SCM_IOMUX_74_SPI2_MISO_MASK           (3)
#define   SCM_IOMUX_74_SPI2_MISO_DEFAULT        (0)
#define   SCM_IOMUX_74_SPI2_MISO_SPI4_IO        (2)
#define   SCM_IOMUX_74_SPI2_MISO_GPIOB12        (3)

#define   SCM_IOMUX_74_SPI2_MOSI_BIT            (6)
#define   SCM_IOMUX_74_SPI2_MOSI_MASK           (3)
#define   SCM_IOMUX_74_SPI2_MOSI_DEFAULT        (0)
#define   SCM_IOMUX_74_SPI2_MOSI_GPIOB13        (3)

//qspi: clk + cs + d0/1/2/3
#define   SCM_IOMUX_74_QSPI_IF_BIT              (8)
#define   SCM_IOMUX_74_QSPI_IF_MASK             (0xfff)
#define   SCM_IOMUX_74_QSPI_IF_DEFAULT          (0)

//uart0
#define   SCM_IOMUX_74_UART0_RX_BIT             (20)
#define   SCM_IOMUX_74_UART0_RX_MASK            (3)
#define   SCM_IOMUX_74_UART0_RX_DEFAULT         (0)
#define   SCM_IOMUX_74_UART0_RX_GPIOB14         (3)

#define   SCM_IOMUX_74_UART0_TX_BIT             (22)
#define   SCM_IOMUX_74_UART0_TX_MASK            (3)
#define   SCM_IOMUX_74_UART0_TX_DEFAULT         (0)
#define   SCM_IOMUX_74_UART0_TX_GPIOB15         (3)

//uart1
#define   SCM_IOMUX_74_UART1_RX_BIT             (24)
#define   SCM_IOMUX_74_UART1_RX_MASK            (3)
#define   SCM_IOMUX_74_UART1_RX_DEFAULT         (0)
#define   SCM_IOMUX_74_UART1_RX_GPIOB16         (3)

#define   SCM_IOMUX_74_UART1_TX_BIT             (26)
#define   SCM_IOMUX_74_UART1_TX_MASK            (3)
#define   SCM_IOMUX_74_UART1_TX_DEFAULT         (0)
#define   SCM_IOMUX_74_UART1_TX_GPIOB17         (3)

//uart2
#define   SCM_IOMUX_74_UART2_RX_BIT             (28)
#define   SCM_IOMUX_74_UART2_RX_MASK            (3)
#define   SCM_IOMUX_74_UART2_RX_DEFAULT         (0)
#define   SCM_IOMUX_74_UART2_RX_GPIOB18         (3)

#define   SCM_IOMUX_74_UART2_TX_BIT             (30)
#define   SCM_IOMUX_74_UART2_TX_MASK            (3)
#define   SCM_IOMUX_74_UART2_TX_DEFAULT         (0)
#define   SCM_IOMUX_74_UART2_TX_GPIOB19         (3)

//iomux78 uart3/uart4/uart5/uart6/uart7/gpio0~5
//uart3
#define   SCM_IOMUX_78_UART3_RX_BIT             (0)
#define   SCM_IOMUX_78_UART3_RX_MASK            (3)
#define   SCM_IOMUX_78_UART3_RX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART3_RX_GPIOB20         (3)

#define   SCM_IOMUX_78_UART3_TX_BIT             (2)
#define   SCM_IOMUX_78_UART3_TX_MASK            (3)
#define   SCM_IOMUX_78_UART3_TX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART3_TX_GPIOB21         (3)

//uart4
#define   SCM_IOMUX_78_UART4_RX_BIT             (4)
#define   SCM_IOMUX_78_UART4_RX_MASK            (3)
#define   SCM_IOMUX_78_UART4_RX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART4_RX_GPIOB22         (3)

#define   SCM_IOMUX_78_UART4_TX_BIT             (6)
#define   SCM_IOMUX_78_UART4_TX_MASK            (3)
#define   SCM_IOMUX_78_UART4_TX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART4_TX_GPIOB23         (3)

//uart5
#define   SCM_IOMUX_78_UART5_RX_BIT             (8)
#define   SCM_IOMUX_78_UART5_RX_MASK            (3)
#define   SCM_IOMUX_78_UART5_RX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART5_RX_GPIOB24         (3)

#define   SCM_IOMUX_78_UART5_TX_BIT             (10)
#define   SCM_IOMUX_78_UART5_TX_MASK            (3)
#define   SCM_IOMUX_78_UART5_TX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART5_TX_GPIOB25         (3)

//uart6
#define   SCM_IOMUX_78_UART6_RX_BIT             (12)
#define   SCM_IOMUX_78_UART6_RX_MASK            (3)
#define   SCM_IOMUX_78_UART6_RX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART6_RX_GPIOB26         (3)

#define   SCM_IOMUX_78_UART6_TX_BIT             (14)
#define   SCM_IOMUX_78_UART6_TX_MASK            (3)
#define   SCM_IOMUX_78_UART6_TX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART6_TX_GPIOB27         (3)

//uart7
#define   SCM_IOMUX_78_UART7_RX_BIT             (16)
#define   SCM_IOMUX_78_UART7_RX_MASK            (3)
#define   SCM_IOMUX_78_UART7_RX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART7_RX_GPIOB28         (3)

#define   SCM_IOMUX_78_UART7_TX_BIT             (18)
#define   SCM_IOMUX_78_UART7_TX_MASK            (3)
#define   SCM_IOMUX_78_UART7_TX_DEFAULT         (0)
#define   SCM_IOMUX_78_UART7_TX_GPIOB29         (3)

//gpioa00
#define   SCM_IOMUX_78_GPIOA00_BIT              (20)
#define   SCM_IOMUX_78_GPIOA00_MASK             (3)
#define   SCM_IOMUX_78_GPIOA00_DEFAULT          (0)

//gpioa01
#define   SCM_IOMUX_78_GPIOA01_BIT              (22)
#define   SCM_IOMUX_78_GPIOA01_MASK             (3)
#define   SCM_IOMUX_78_GPIOA01_DEFAULT          (0)
#define   SCM_IOMUX_78_GPIOA01_IRIGB_I          (1)

//gpioa02
#define   SCM_IOMUX_78_GPIOA02_BIT              (24)
#define   SCM_IOMUX_78_GPIOA02_MASK             (3)
#define   SCM_IOMUX_78_GPIOA02_DEFAULT          (0)

//gpioa03
#define   SCM_IOMUX_78_GPIOA03_BIT              (26)
#define   SCM_IOMUX_78_GPIOA03_MASK             (3)
#define   SCM_IOMUX_78_GPIOA03_DEFAULT          (0)
#define   SCM_IOMUX_78_GPIOA03_SIM2_RST         (1)

//gpioa04
#define   SCM_IOMUX_78_GPIOA04_BIT              (28)
#define   SCM_IOMUX_78_GPIOA04_MASK             (3)
#define   SCM_IOMUX_78_GPIOA04_DEFAULT          (0)
#define   SCM_IOMUX_78_GPIOA04_SIM2_IO          (1)

//gpioa05
#define   SCM_IOMUX_78_GPIOA05_BIT              (30)
#define   SCM_IOMUX_78_GPIOA05_MASK             (3)
#define   SCM_IOMUX_78_GPIOA05_DEFAULT          (0)

//iomux7c gpio6~7/prm0 d0~d13
#define   SCM_IOMUX_7C_GPIOA06_BIT              (0)
#define   SCM_IOMUX_7C_GPIOA06_MASK             (3)
#define   SCM_IOMUX_7C_GPIOA06_DEFAULT          (0)

#define   SCM_IOMUX_7C_GPIOA07_BIT              (2)
#define   SCM_IOMUX_7C_GPIOA07_MASK             (3)
#define   SCM_IOMUX_7C_GPIOA07_DEFAULT          (0)

#define   SCM_IOMUX_7C_PRM0_D0_BIT              (4)
#define   SCM_IOMUX_7C_PRM0_D0_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D0_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D0_GPIOB30          (3)

#define   SCM_IOMUX_7C_PRM0_D1_BIT              (6)
#define   SCM_IOMUX_7C_PRM0_D1_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D1_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D1_GPIOB31          (3)

#define   SCM_IOMUX_7C_PRM0_D2_BIT              (8)
#define   SCM_IOMUX_7C_PRM0_D2_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D2_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D2_GPIOC00          (3)

#define   SCM_IOMUX_7C_PRM0_D3_BIT              (10)
#define   SCM_IOMUX_7C_PRM0_D3_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D3_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D3_GPIOC01          (3)

#define   SCM_IOMUX_7C_PRM0_D4_BIT              (12)
#define   SCM_IOMUX_7C_PRM0_D4_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D4_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D4_GPIOC02          (3)

#define   SCM_IOMUX_7C_PRM0_D5_BIT              (14)
#define   SCM_IOMUX_7C_PRM0_D5_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D5_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D5_GPIOC03          (3)

#define   SCM_IOMUX_7C_PRM0_D6_BIT              (16)
#define   SCM_IOMUX_7C_PRM0_D6_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D6_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D6_GPIOC04          (3)

#define   SCM_IOMUX_7C_PRM0_D7_BIT              (18)
#define   SCM_IOMUX_7C_PRM0_D7_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D7_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D7_GPIOC05          (3)

#define   SCM_IOMUX_7C_PRM0_D8_BIT              (20)
#define   SCM_IOMUX_7C_PRM0_D8_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D8_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D8_GPIOC06          (3)

#define   SCM_IOMUX_7C_PRM0_D9_BIT              (22)
#define   SCM_IOMUX_7C_PRM0_D9_MASK             (3)
#define   SCM_IOMUX_7C_PRM0_D9_DEFAULT          (0)
#define   SCM_IOMUX_7C_PRM0_D9_GPIOC07          (3)

#define   SCM_IOMUX_7C_PRM0_D10_BIT             (24)
#define   SCM_IOMUX_7C_PRM0_D10_MASK            (3)
#define   SCM_IOMUX_7C_PRM0_D10_DEFAULT         (0)
#define   SCM_IOMUX_7C_PRM0_D10_GPIOC08         (3)

#define   SCM_IOMUX_7C_PRM0_D11_BIT             (26)
#define   SCM_IOMUX_7C_PRM0_D11_MASK            (3)
#define   SCM_IOMUX_7C_PRM0_D11_DEFAULT         (0)
#define   SCM_IOMUX_7C_PRM0_D11_GPIOC09         (3)

#define   SCM_IOMUX_7C_PRM0_D12_BIT             (28)
#define   SCM_IOMUX_7C_PRM0_D12_MASK            (3)
#define   SCM_IOMUX_7C_PRM0_D12_DEFAULT         (0)
#define   SCM_IOMUX_7C_PRM0_D12_GPIOC10         (3)

#define   SCM_IOMUX_7C_PRM0_D13_BIT             (30)
#define   SCM_IOMUX_7C_PRM0_D13_MASK            (3)
#define   SCM_IOMUX_7C_PRM0_D13_DEFAULT         (0)
#define   SCM_IOMUX_7C_PRM0_D13_GPIOC11         (3)

//iomux80 prm0 d14~15, prm0 clk0~13
#define   SCM_IOMUX_80_PRM0_D14_BIT             (0)
#define   SCM_IOMUX_80_PRM0_D14_MASK            (3)
#define   SCM_IOMUX_80_PRM0_D14_DEFAULT         (0)
#define   SCM_IOMUX_80_PRM0_D14_GPIOC12         (3)

#define   SCM_IOMUX_80_PRM0_D15_BIT             (2)
#define   SCM_IOMUX_80_PRM0_D15_MASK            (3)
#define   SCM_IOMUX_80_PRM0_D15_DEFAULT         (0)
#define   SCM_IOMUX_80_PRM0_D15_GPIOC13         (3)

#define   SCM_IOMUX_80_PRM0_CLK0_BIT            (4)
#define   SCM_IOMUX_80_PRM0_CLK0_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK0_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK0_GPIOC14        (3)

#define   SCM_IOMUX_80_PRM0_CLK1_BIT            (6)
#define   SCM_IOMUX_80_PRM0_CLK1_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK1_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK1_GPIOC15        (3)

#define   SCM_IOMUX_80_PRM0_CLK2_BIT            (8)
#define   SCM_IOMUX_80_PRM0_CLK2_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK2_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK2_GPIOC16        (3)

#define   SCM_IOMUX_80_PRM0_CLK3_BIT            (10)
#define   SCM_IOMUX_80_PRM0_CLK3_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK3_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK3_GPIOC17        (3)

#define   SCM_IOMUX_80_PRM0_CLK4_BIT            (12)
#define   SCM_IOMUX_80_PRM0_CLK4_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK4_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK4_GPIOC18        (3)

#define   SCM_IOMUX_80_PRM0_CLK5_BIT            (14)
#define   SCM_IOMUX_80_PRM0_CLK5_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK5_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK5_GPIOC19        (3)

#define   SCM_IOMUX_80_PRM0_CLK6_BIT            (16)
#define   SCM_IOMUX_80_PRM0_CLK6_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK6_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK6_GPIOC20        (3)

#define   SCM_IOMUX_80_PRM0_CLK7_BIT            (18)
#define   SCM_IOMUX_80_PRM0_CLK7_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK7_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK7_GPIOC21        (3)

#define   SCM_IOMUX_80_PRM0_CLK8_BIT            (20)
#define   SCM_IOMUX_80_PRM0_CLK8_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK8_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK8_GPIOC22        (3)

#define   SCM_IOMUX_80_PRM0_CLK9_BIT            (22)
#define   SCM_IOMUX_80_PRM0_CLK9_MASK           (3)
#define   SCM_IOMUX_80_PRM0_CLK9_DEFAULT        (0)
#define   SCM_IOMUX_80_PRM0_CLK9_GPIOC23        (3)

#define   SCM_IOMUX_80_PRM0_CLK10_BIT           (24)
#define   SCM_IOMUX_80_PRM0_CLK10_MASK          (3)
#define   SCM_IOMUX_80_PRM0_CLK10_DEFAULT       (0)
#define   SCM_IOMUX_80_PRM0_CLK10_GPIOC24       (3)

#define   SCM_IOMUX_80_PRM0_CLK11_BIT           (26)
#define   SCM_IOMUX_80_PRM0_CLK11_MASK          (3)
#define   SCM_IOMUX_80_PRM0_CLK11_DEFAULT       (0)
#define   SCM_IOMUX_80_PRM0_CLK11_GPIOC25       (3)

#define   SCM_IOMUX_80_PRM0_CLK12_BIT           (28)
#define   SCM_IOMUX_80_PRM0_CLK12_MASK          (3)
#define   SCM_IOMUX_80_PRM0_CLK12_DEFAULT       (0)
#define   SCM_IOMUX_80_PRM0_CLK12_GPIOC26       (3)

#define   SCM_IOMUX_80_PRM0_CLK13_BIT           (30)
#define   SCM_IOMUX_80_PRM0_CLK13_MASK          (3)
#define   SCM_IOMUX_80_PRM0_CLK13_DEFAULT       (0)
#define   SCM_IOMUX_80_PRM0_CLK13_GPIOC27       (3)
//iomux84 prm0 clk14~15 prm0 rst/prm0 sysclk/prm1 d0~11
#define   SCM_IOMUX_84_PRM0_CLK14_BIT           (0)
#define   SCM_IOMUX_84_PRM0_CLK14_MASK          (3)
#define   SCM_IOMUX_84_PRM0_CLK14_DEFAULT       (0)
#define   SCM_IOMUX_84_PRM0_CLK14_GPIOC28       (3)

#define   SCM_IOMUX_84_PRM0_CLK15_BIT           (2)
#define   SCM_IOMUX_84_PRM0_CLK15_MASK          (3)
#define   SCM_IOMUX_84_PRM0_CLK15_DEFAULT       (0)
#define   SCM_IOMUX_84_PRM0_CLK15_GPIOC29       (3)

#define   SCM_IOMUX_84_PRM0_RST_BIT             (4)
#define   SCM_IOMUX_84_PRM0_RST_MASK            (3)
#define   SCM_IOMUX_84_PRM0_RST_DEFAULT         (0)
#define   SCM_IOMUX_84_PRM0_RST_GPIOC30         (3)

#define   SCM_IOMUX_84_PRM_SYSCLK_BIT           (6)
#define   SCM_IOMUX_84_PRM_SYSCLK_MASK          (3)
#define   SCM_IOMUX_84_PRM_SYSCLK_DEFAULT       (0)
#define   SCM_IOMUX_84_PRM_SYSCLK_GPIOC31       (3)

#define   SCM_IOMUX_84_PRM1_D0_BIT              (8)
#define   SCM_IOMUX_84_PRM1_D0_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D0_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D0_IF3_0_IF5_8_IF9_12          (1)
#define   SCM_IOMUX_84_PRM1_D0_GPIOD00          (3)

#define   SCM_IOMUX_84_PRM1_D1_BIT              (10)
#define   SCM_IOMUX_84_PRM1_D1_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D1_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D1_IF3_1_IF5_9_IF9_13          (1)
#define   SCM_IOMUX_84_PRM1_D1_GPIOD01          (3)

#define   SCM_IOMUX_84_PRM1_D2_BIT              (12)
#define   SCM_IOMUX_84_PRM1_D2_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D2_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D2_IF3_2_IF5_10_IF9_14         (1)
#define   SCM_IOMUX_84_PRM1_D2_GPIOD02         (3)

#define   SCM_IOMUX_84_PRM1_D3_BIT              (14)
#define   SCM_IOMUX_84_PRM1_D3_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D3_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D3_IF3_3_IF5_11_IF9_15         (1)
#define   SCM_IOMUX_84_PRM1_D3_GPIOD03          (3)

#define   SCM_IOMUX_84_PRM1_D4_BIT              (16)
#define   SCM_IOMUX_84_PRM1_D4_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D4_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D4_IF3_4_IF5_6_IF7_8_IF8_12    (1)
#define   SCM_IOMUX_84_PRM1_D4_GPIOCD04         (3)

#define   SCM_IOMUX_84_PRM1_D5_BIT              (18)
#define   SCM_IOMUX_84_PRM1_D5_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D5_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D5_IF3_5_IF5_7_IF7_9_IF8_13    (1)
#define   SCM_IOMUX_84_PRM1_D5_GPIOD05          (3)

#define   SCM_IOMUX_84_PRM1_D6_BIT              (20)
#define   SCM_IOMUX_84_PRM1_D6_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D6_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D6_IF3_6_IF7_10_IF8_14         (1)
#define   SCM_IOMUX_84_PRM1_D6_GPIOD06          (3)

#define   SCM_IOMUX_84_PRM1_D7_BIT              (22)
#define   SCM_IOMUX_84_PRM1_D7_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D7_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D7_IF3_7_IF7_11_IF8_15         (1)
#define   SCM_IOMUX_84_PRM1_D7_GPIOD07          (3)

#define   SCM_IOMUX_84_PRM1_D8_BIT              (24)
#define   SCM_IOMUX_84_PRM1_D8_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D8_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D8_IF3_8_IF10_12               (1)

#define   SCM_IOMUX_84_PRM1_D9_BIT              (26)
#define   SCM_IOMUX_84_PRM1_D9_MASK             (3)
#define   SCM_IOMUX_84_PRM1_D9_DEFAULT          (0)
#define   SCM_IOMUX_84_PRM1_D9_IF3_9_IF10_13               (1)

#define   SCM_IOMUX_84_PRM1_D10_BIT             (28)
#define   SCM_IOMUX_84_PRM1_D10_MASK            (3)
#define   SCM_IOMUX_84_PRM1_D10_DEFAULT         (0)
#define   SCM_IOMUX_84_PRM1_D10_IF3_10_IF10_14             (1)

#define   SCM_IOMUX_84_PRM1_D11_BIT             (30)
#define   SCM_IOMUX_84_PRM1_D11_MASK            (3)
#define   SCM_IOMUX_84_PRM1_D11_DEFAULT         (0)
#define   SCM_IOMUX_84_PRM1_D11_IF3_11_IF10_15             (1)

//iomux88 prm1 d12~15 prm1 clk0~11
#define   SCM_IOMUX_88_PRM1_D12_BIT             (0)
#define   SCM_IOMUX_88_PRM1_D12_MASK            (3)
#define   SCM_IOMUX_88_PRM1_D12_DEFAULT         (0)
#define   SCM_IOMUX_88_PRM1_D12_IF3_12          (1)

#define   SCM_IOMUX_88_PRM1_D13_BIT             (2)
#define   SCM_IOMUX_88_PRM1_D13_MASK            (3)
#define   SCM_IOMUX_88_PRM1_D13_DEFAULT         (0)
#define   SCM_IOMUX_88_PRM1_D13_IF3_13          (1)

#define   SCM_IOMUX_88_PRM1_D14_BIT             (4)
#define   SCM_IOMUX_88_PRM1_D14_MASK            (3)
#define   SCM_IOMUX_88_PRM1_D14_DEFAULT         (0)
#define   SCM_IOMUX_88_PRM1_D14_IF3_14          (1)

#define   SCM_IOMUX_88_PRM1_D15_BIT             (6)
#define   SCM_IOMUX_88_PRM1_D15_MASK            (3)
#define   SCM_IOMUX_88_PRM1_D15_DEFAULT         (0)
#define   SCM_IOMUX_88_PRM1_D15_IF3_15          (1)

#define   SCM_IOMUX_88_PRM1_CLK0_BIT            (8)
#define   SCM_IOMUX_88_PRM1_CLK0_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK0_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK0_MMC0_CLK       (1)
#define   SCM_IOMUX_88_PRM1_CLK0_MMC1_CLK       (2)
#define   SCM_IOMUX_88_PRM1_CLK0_GPIOD08        (3)

#define   SCM_IOMUX_88_PRM1_CLK1_BIT            (10)
#define   SCM_IOMUX_88_PRM1_CLK1_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK1_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK1_MMC0_CMD       (1)
#define   SCM_IOMUX_88_PRM1_CLK1_MMC1_CMD       (2)
#define   SCM_IOMUX_88_PRM1_CLK1_GPIOD09        (3)

#define   SCM_IOMUX_88_PRM1_CLK2_BIT            (12)
#define   SCM_IOMUX_88_PRM1_CLK2_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK2_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK2_MMC0_D0        (1)
#define   SCM_IOMUX_88_PRM1_CLK2_MMC1_D0        (2)
#define   SCM_IOMUX_88_PRM1_CLK2_GPIOD10        (3)

#define   SCM_IOMUX_88_PRM1_CLK3_BIT            (14)
#define   SCM_IOMUX_88_PRM1_CLK3_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK3_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK3_MMC0_D1        (1)
#define   SCM_IOMUX_88_PRM1_CLK3_MMC1_D1        (2)
#define   SCM_IOMUX_88_PRM1_CLK3_GPIOD11        (3)

#define   SCM_IOMUX_88_PRM1_CLK4_BIT            (16)
#define   SCM_IOMUX_88_PRM1_CLK4_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK4_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK4_MMC0_D2        (1)
#define   SCM_IOMUX_88_PRM1_CLK4_MMC1_D2        (2)
#define   SCM_IOMUX_88_PRM1_CLK4_GPIOD12        (3)

#define   SCM_IOMUX_88_PRM1_CLK5_BIT            (18)
#define   SCM_IOMUX_88_PRM1_CLK5_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK5_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK5_MMC0_D3        (1)
#define   SCM_IOMUX_88_PRM1_CLK5_MMC1_D3        (2)
#define   SCM_IOMUX_88_PRM1_CLK5_GPIOD13        (3)

#define   SCM_IOMUX_88_PRM1_CLK6_BIT            (20)
#define   SCM_IOMUX_88_PRM1_CLK6_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK6_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK6_MMC0_WP        (1)
#define   SCM_IOMUX_88_PRM1_CLK6_MMC1_WP        (2)
#define   SCM_IOMUX_88_PRM1_CLK6_GPIOD14        (3)

#define   SCM_IOMUX_88_PRM1_CLK7_BIT            (22)
#define   SCM_IOMUX_88_PRM1_CLK7_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK7_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK7_MMC0_CD        (1)
#define   SCM_IOMUX_88_PRM1_CLK7_MMC1_CD        (2)
#define   SCM_IOMUX_88_PRM1_CLK7_GPIOD15        (3)

#define   SCM_IOMUX_88_PRM1_CLK8_BIT            (24)
#define   SCM_IOMUX_88_PRM1_CLK8_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK8_DEFAULT        (0)
#define   SCM_IOMUX_88_PRM1_CLK8_MMC1_PSW       (2)

#define   SCM_IOMUX_88_PRM1_CLK9_BIT            (26)
#define   SCM_IOMUX_88_PRM1_CLK9_MASK           (3)
#define   SCM_IOMUX_88_PRM1_CLK9_DEFAULT        (0)

#define   SCM_IOMUX_88_PRM1_CLK10_BIT           (28)
#define   SCM_IOMUX_88_PRM1_CLK10_MASK          (3)
#define   SCM_IOMUX_88_PRM1_CLK10_DEFAULT       (0)

#define   SCM_IOMUX_88_PRM1_CLK11_BIT           (30)
#define   SCM_IOMUX_88_PRM1_CLK11_MASK          (3)
#define   SCM_IOMUX_88_PRM1_CLK11_DEFAULT       (0)

//iomux8c prm1 clk12~15/prm1 rst/if din0~10

#define   SCM_IOMUX_8C_PRM1_CLK12_BIT           (0)
#define   SCM_IOMUX_8C_PRM1_CLK12_MASK          (3)
#define   SCM_IOMUX_8C_PRM1_CLK12_DEFAULT       (0)

#define   SCM_IOMUX_8C_PRM1_CLK13_BIT           (2)
#define   SCM_IOMUX_8C_PRM1_CLK13_MASK          (3)
#define   SCM_IOMUX_8C_PRM1_CLK13_DEFAULT       (0)

#define   SCM_IOMUX_8C_PRM1_CLK14_BIT           (4)
#define   SCM_IOMUX_8C_PRM1_CLK14_MASK          (3)
#define   SCM_IOMUX_8C_PRM1_CLK14_DEFAULT       (0)

#define   SCM_IOMUX_8C_PRM1_CLK15_BIT           (6)
#define   SCM_IOMUX_8C_PRM1_CLK15_MASK          (3)
#define   SCM_IOMUX_8C_PRM1_CLK15_DEFAULT       (0)

#define   SCM_IOMUX_8C_PRM1_RST_BIT             (8)
#define   SCM_IOMUX_8C_PRM1_RST_MASK            (3)
#define   SCM_IOMUX_8C_PRM1_RST_DEFAULT         (0)

#define   SCM_IOMUX_8C_PRM1_DIN0_BIT            (10)
#define   SCM_IOMUX_8C_PRM1_DIN0_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN0_IF0_0_IF4_12   (0)

#define   SCM_IOMUX_8C_PRM1_DIN1_BIT            (12)
#define   SCM_IOMUX_8C_PRM1_DIN1_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN1_IF0_1_IF4_13   (0)

#define   SCM_IOMUX_8C_PRM1_DIN2_BIT            (14)
#define   SCM_IOMUX_8C_PRM1_DIN2_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN2_IF0_2_IF4_14   (0)

#define   SCM_IOMUX_8C_PRM1_DIN3_BIT            (16)
#define   SCM_IOMUX_8C_PRM1_DIN3_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN3_IF0_3_IF4_15   (0)

#define   SCM_IOMUX_8C_PRM1_DIN4_BIT            (18)
#define   SCM_IOMUX_8C_PRM1_DIN4_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN4_IF0_4_IF6_12_IF4_6        (0)
#define   SCM_IOMUX_8C_PRM1_DIN4_SRAM_RD        (1)

#define   SCM_IOMUX_8C_PRM1_DIN5_BIT            (20)
#define   SCM_IOMUX_8C_PRM1_DIN5_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN5_IF0_5_IF6_13_IF4_7        (0)
#define   SCM_IOMUX_8C_PRM1_DIN5_SRAM_WR        (1)

#define   SCM_IOMUX_8C_PRM1_DIN6_BIT            (22)
#define   SCM_IOMUX_8C_PRM1_DIN6_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN6_IF0_6_IF6_14   (0)
#define   SCM_IOMUX_8C_PRM1_DIN6_SRAM_CS        (1)

#define   SCM_IOMUX_8C_PRM1_DIN7_BIT            (24)
#define   SCM_IOMUX_8C_PRM1_DIN7_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN7_IF0_7_IF6_15   (0)

#define   SCM_IOMUX_8C_PRM1_DIN8_BIT            (26)
#define   SCM_IOMUX_8C_PRM1_DIN8_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN8_IF0_8          (0)

#define   SCM_IOMUX_8C_PRM1_DIN9_BIT            (28)
#define   SCM_IOMUX_8C_PRM1_DIN9_MASK           (3)
#define   SCM_IOMUX_8C_PRM1_DIN9_IF0_9          (0)

#define   SCM_IOMUX_8C_PRM1_DIN10_BIT           (30)
#define   SCM_IOMUX_8C_PRM1_DIN10_MASK          (3)
#define   SCM_IOMUX_8C_PRM1_DIN10_IF0_10        (0)

//iomux90 if din11~26
#define   SCM_IOMUX_90_PRM1_DIN11_BIT           (0)
#define   SCM_IOMUX_90_PRM1_DIN11_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN11_IF0_11        (0)

#define   SCM_IOMUX_90_PRM1_DIN12_BIT           (2)
#define   SCM_IOMUX_90_PRM1_DIN12_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN12_IF0_12        (0)

#define   SCM_IOMUX_90_PRM1_DIN13_BIT           (4)
#define   SCM_IOMUX_90_PRM1_DIN13_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN13_IF0_13        (0)

#define   SCM_IOMUX_90_PRM1_DIN14_BIT           (6)
#define   SCM_IOMUX_90_PRM1_DIN14_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN14_IF0_14        (0)

#define   SCM_IOMUX_90_PRM1_DIN15_BIT           (8)
#define   SCM_IOMUX_90_PRM1_DIN15_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN15_IF0_15        (0)

#define   SCM_IOMUX_90_PRM1_DIN16_BIT           (10)
#define   SCM_IOMUX_90_PRM1_DIN16_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN16_IF1_0_IF4_8   (0)

#define   SCM_IOMUX_90_PRM1_DIN17_BIT           (12)
#define   SCM_IOMUX_90_PRM1_DIN17_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN17_IF1_1_IF4_9   (0)

#define   SCM_IOMUX_90_PRM1_DIN18_BIT           (14)
#define   SCM_IOMUX_90_PRM1_DIN18_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN18_IF1_2_IF4_10  (0)

#define   SCM_IOMUX_90_PRM1_DIN19_BIT           (16)
#define   SCM_IOMUX_90_PRM1_DIN19_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN19_IF1_3_IF4_11  (0)

#define   SCM_IOMUX_90_PRM1_DIN20_BIT           (18)
#define   SCM_IOMUX_90_PRM1_DIN20_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN20_IF1_4_IF6_8   (0)
#define   SCM_IOMUX_90_PRM1_DIN20_SRAM_A0       (1)

#define   SCM_IOMUX_90_PRM1_DIN21_BIT           (20)
#define   SCM_IOMUX_90_PRM1_DIN21_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN21_IF1_5_IF6_9   (0)
#define   SCM_IOMUX_90_PRM1_DIN21_SRAM_A1       (1)

#define   SCM_IOMUX_90_PRM1_DIN22_BIT           (22)
#define   SCM_IOMUX_90_PRM1_DIN22_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN22_IF1_6_IF6_10  (0)
#define   SCM_IOMUX_90_PRM1_DIN22_SRAM_A2       (1)

#define   SCM_IOMUX_90_PRM1_DIN23_BIT           (24)
#define   SCM_IOMUX_90_PRM1_DIN23_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN23_IF1_7_IF6_11  (0)
#define   SCM_IOMUX_90_PRM1_DIN23_SRAM_A3       (1)

#define   SCM_IOMUX_90_PRM1_DIN24_BIT           (26)
#define   SCM_IOMUX_90_PRM1_DIN24_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN24_IF1_8         (0)
#define   SCM_IOMUX_90_PRM1_DIN24_SRAM_D0       (1)

#define   SCM_IOMUX_90_PRM1_DIN25_BIT           (28)
#define   SCM_IOMUX_90_PRM1_DIN25_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN25_IF1_9         (0)
#define   SCM_IOMUX_90_PRM1_DIN25_SRAM_D1       (1)

#define   SCM_IOMUX_90_PRM1_DIN26_BIT           (30)
#define   SCM_IOMUX_90_PRM1_DIN26_MASK          (3)
#define   SCM_IOMUX_90_PRM1_DIN26_IF1_10        (0)
#define   SCM_IOMUX_90_PRM1_DIN26_SRAM_D2       (1)

//iomux94 if din27~42
#define   SCM_IOMUX_94_PRM1_DIN27_BIT           (0)
#define   SCM_IOMUX_94_PRM1_DIN27_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN27_IF1_11        (0)
#define   SCM_IOMUX_94_PRM1_DIN27_SRAM_D3       (1)

#define   SCM_IOMUX_94_PRM1_DIN28_BIT           (2)
#define   SCM_IOMUX_94_PRM1_DIN28_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN28_IF1_12        (0)
#define   SCM_IOMUX_94_PRM1_DIN28_SRAM_D4       (1)

#define   SCM_IOMUX_94_PRM1_DIN29_BIT           (4)
#define   SCM_IOMUX_94_PRM1_DIN29_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN29_IF1_13        (0)
#define   SCM_IOMUX_94_PRM1_DIN29_SRAM_D5       (1)

#define   SCM_IOMUX_94_PRM1_DIN30_BIT           (6)
#define   SCM_IOMUX_94_PRM1_DIN30_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN30_IF1_14        (0)
#define   SCM_IOMUX_94_PRM1_DIN30_SRAM_D6       (1)

#define   SCM_IOMUX_94_PRM1_DIN31_BIT           (8)
#define   SCM_IOMUX_94_PRM1_DIN31_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN31_IF1_15        (0)
#define   SCM_IOMUX_94_PRM1_DIN31_SRAM_D7       (1)

#define   SCM_IOMUX_94_PRM1_DIN32_BIT           (10)
#define   SCM_IOMUX_94_PRM1_DIN32_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN32_IF2_0_IF4_4_IF5_12        (0)

#define   SCM_IOMUX_94_PRM1_DIN33_BIT           (12)
#define   SCM_IOMUX_94_PRM1_DIN33_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN33_IF2_1_IF4_5_IF5_13        (0)

#define   SCM_IOMUX_94_PRM1_DIN34_BIT           (14)
#define   SCM_IOMUX_94_PRM1_DIN34_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN34_IF2_2_IF4_6_IF5_14        (0)

#define   SCM_IOMUX_94_PRM1_DIN35_BIT           (16)
#define   SCM_IOMUX_94_PRM1_DIN35_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN35_IF2_3_IF4_7_IF5_15        (0)

#define   SCM_IOMUX_94_PRM1_DIN36_BIT           (18)
#define   SCM_IOMUX_94_PRM1_DIN36_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN36_IF2_4_IF7_12              (0)

#define   SCM_IOMUX_94_PRM1_DIN37_BIT           (20)
#define   SCM_IOMUX_94_PRM1_DIN37_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN37_IF2_5_IF7_13              (0)

#define   SCM_IOMUX_94_PRM1_DIN38_BIT           (22)
#define   SCM_IOMUX_94_PRM1_DIN38_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN38_IF2_6_IF7_14              (0)

#define   SCM_IOMUX_94_PRM1_DIN39_BIT           (24)
#define   SCM_IOMUX_94_PRM1_DIN39_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN39_IF2_7_IF7_15              (0)

#define   SCM_IOMUX_94_PRM1_DIN40_BIT           (26)
#define   SCM_IOMUX_94_PRM1_DIN40_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN40_IF2_8_IF11_12              (0)
#define   SCM_IOMUX_94_PRM1_DIN40_GPIOD24       (3)

#define   SCM_IOMUX_94_PRM1_DIN41_BIT           (28)
#define   SCM_IOMUX_94_PRM1_DIN41_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN41_IF2_9_IF11_13              (0)
#define   SCM_IOMUX_94_PRM1_DIN41_GPIOD25       (3)

#define   SCM_IOMUX_94_PRM1_DIN42_BIT           (30)
#define   SCM_IOMUX_94_PRM1_DIN42_MASK          (3)
#define   SCM_IOMUX_94_PRM1_DIN42_IF2_10_IF11_14             (0)
#define   SCM_IOMUX_94_PRM1_DIN42_GPIOD26       (3)

//iomux98 if din43~47/hwpwm0~3/swpwm0/pps0~1/clk5m/i2c0/uart6 cts
#define   SCM_IOMUX_98_PRM1_DIN43_BIT           (0)
#define   SCM_IOMUX_98_PRM1_DIN43_MASK          (3)
#define   SCM_IOMUX_98_PRM1_DIN43_IF2_11_IF11_15             (0)
#define   SCM_IOMUX_98_PRM1_DIN43_GPIOD27       (3)

#define   SCM_IOMUX_98_PRM1_DIN44_BIT           (2)
#define   SCM_IOMUX_98_PRM1_DIN44_MASK          (3)
#define   SCM_IOMUX_98_PRM1_DIN44_IF2_12        (0)
#define   SCM_IOMUX_98_PRM1_DIN44_GPIOD28       (3)

#define   SCM_IOMUX_98_PRM1_DIN45_BIT           (4)
#define   SCM_IOMUX_98_PRM1_DIN45_MASK          (3)
#define   SCM_IOMUX_98_PRM1_DIN45_IF2_13        (0)
#define   SCM_IOMUX_98_PRM1_DIN45_GPIOD29       (3)

#define   SCM_IOMUX_98_PRM1_DIN46_BIT           (6)
#define   SCM_IOMUX_98_PRM1_DIN46_MASK          (3)
#define   SCM_IOMUX_98_PRM1_DIN46_IF2_14        (0)
#define   SCM_IOMUX_98_PRM1_DIN46_GPIOD30       (3)

#define   SCM_IOMUX_98_PRM1_DIN47_BIT           (8)
#define   SCM_IOMUX_98_PRM1_DIN47_MASK          (3)
#define   SCM_IOMUX_98_PRM1_DIN47_IF2_15        (0)
#define   SCM_IOMUX_98_PRM1_DIN47_GPIOD31       (3)

#define   SCM_IOMUX_98_HWPWM0_BIT              (10)
#define   SCM_IOMUX_98_HWPWM0_MASK             (3)
#define   SCM_IOMUX_98_HWPWM0_DEFAULT          (0)
#define   SCM_IOMUX_98_HWPWM0_GPIOD16          (3)

#define   SCM_IOMUX_98_HWPWM1_BIT              (12)
#define   SCM_IOMUX_98_HWPWM1_MASK             (3)
#define   SCM_IOMUX_98_HWPWM1_DEFAULT          (0)
#define   SCM_IOMUX_98_HWPWM1_GPIOD17          (3)

#define   SCM_IOMUX_98_HWPWM2_BIT              (14)
#define   SCM_IOMUX_98_HWPWM2_MASK             (3)
#define   SCM_IOMUX_98_HWPWM2_DEFAULT          (0)
#define   SCM_IOMUX_98_HWPWM2_GPIOD18          (3)

#define   SCM_IOMUX_98_HWPWM3_BIT              (16)
#define   SCM_IOMUX_98_HWPWM3_MASK             (3)
#define   SCM_IOMUX_98_HWPWM3_DEFAULT          (0)
#define   SCM_IOMUX_98_HWPWM3_GPIOD19          (3)

#define   SCM_IOMUX_98_SWPWM0_BIT              (18)
#define   SCM_IOMUX_98_SWPWM0_MASK             (3)
#define   SCM_IOMUX_98_SWPWM0_DEFAULT          (0)
#define   SCM_IOMUX_98_SWPWM0_GPIOD20          (3)

#define   SCM_IOMUX_98_PPS0_BIT                (20)
#define   SCM_IOMUX_98_PPS0_MASK               (3)
#define   SCM_IOMUX_98_PPS0_DEFAULT            (0)
#define   SCM_IOMUX_98_PPS0_GPIOD22            (3)

#define   SCM_IOMUX_98_PPS1_BIT                (22)
#define   SCM_IOMUX_98_PPS1_MASK               (3)
#define   SCM_IOMUX_98_PPS1_DEFAULT            (0)
//#define   SCM_IOMUX_98_PPS1_IRGB_O             (1)
#define   SCM_IOMUX_98_PPS1_GPIOD23            (3)

#define   SCM_IOMUX_98_CLK5M_BIT               (24)
#define   SCM_IOMUX_98_CLK5M_MASK              (3)
#define   SCM_IOMUX_98_CLK5M_DEFAULT           (0)
#define   SCM_IOMUX_98_CLK5M_SIM2_CLK          (1)

#define   SCM_IOMUX_98_I2C0_SCL_BIT            (26)
#define   SCM_IOMUX_98_I2C0_SCL_MASK           (3)
#define   SCM_IOMUX_98_I2C0_SCL_DEFAULT        (0)
#define   SCM_IOMUX_98_I2C0_SCL_GPIOA24        (3)

#define   SCM_IOMUX_98_I2C0_SDA_BIT            (28)
#define   SCM_IOMUX_98_I2C0_SDA_MASK           (3)
#define   SCM_IOMUX_98_I2C0_SDA_DEFAULT        (0)
#define   SCM_IOMUX_98_I2C0_SDA_GPIOA25        (3)

#define   SCM_IOMUX_98_UART6_CTS_BIT           (30)
#define   SCM_IOMUX_98_UART6_CTS_MASK          (3)
#define   SCM_IOMUX_98_UART6_CTS_DEFAULT       (0)

//iomux9c uart6 rts/gpio8~22
#define   SCM_IOMUX_9C_UART6_RTS_BIT            (0)
#define   SCM_IOMUX_9C_UART6_RTS_MASK           (3)
#define   SCM_IOMUX_9C_UART6_RTS_DEFAULT        (0)

#define   SCM_IOMUX_9C_GPIOA08_BIT              (2)
#define   SCM_IOMUX_9C_GPIOA08_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA08_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA09_BIT              (4)
#define   SCM_IOMUX_9C_GPIOA09_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA09_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA10_BIT              (6)
#define   SCM_IOMUX_9C_GPIOA10_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA10_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA11_BIT              (8)
#define   SCM_IOMUX_9C_GPIOA11_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA11_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA12_BIT              (10)
#define   SCM_IOMUX_9C_GPIOA12_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA12_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA13_BIT              (12)
#define   SCM_IOMUX_9C_GPIOA13_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA13_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA14_BIT              (14)
#define   SCM_IOMUX_9C_GPIOA14_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA14_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA15_BIT              (16)
#define   SCM_IOMUX_9C_GPIOA15_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA15_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA16_BIT              (18)
#define   SCM_IOMUX_9C_GPIOA16_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA16_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA17_BIT              (20)
#define   SCM_IOMUX_9C_GPIOA17_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA17_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA18_BIT              (22)
#define   SCM_IOMUX_9C_GPIOA18_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA18_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA19_BIT              (24)
#define   SCM_IOMUX_9C_GPIOA19_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA19_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA20_BIT              (26)
#define   SCM_IOMUX_9C_GPIOA20_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA20_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA21_BIT              (28)
#define   SCM_IOMUX_9C_GPIOA21_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA21_DEFAULT          (0)

#define   SCM_IOMUX_9C_GPIOA22_BIT              (30)
#define   SCM_IOMUX_9C_GPIOA22_MASK             (3)
#define   SCM_IOMUX_9C_GPIOA22_DEFAULT          (0)
#define   SCM_IOMUX_9C_GPIOA22_IRIGB_PPS        (1)

//iomuxa0 gpio23,swpwm1/
#define   SCM_IOMUX_A0_GPIOA23_BIT              (0)
#define   SCM_IOMUX_A0_GPIOA23_MASK             (3)
#define   SCM_IOMUX_A0_GPIOA23_DEFAULT          (0)
#define   SCM_IOMUX_A0_GPIOA23_IRIGB_GJB        (1)

#define   SCM_IOMUX_A0_SWPWM1_BIT               (2)
#define   SCM_IOMUX_A0_SWPWM1_MASK              (3)
#define   SCM_IOMUX_A0_SWPWM1_DEFAULT           (0)
#define   SCM_IOMUX_A0_SWPWM1_GPIOD21           (3)
#endif

