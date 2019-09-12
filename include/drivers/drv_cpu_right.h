#ifndef _CPU_RIGHT_IF_H_
#define _CPU_RIGHT_IF_H_

typedef enum
{
    CPU_RIGTH_E_ROM                        = 0,
    CPU_RIGTH_E_SRAM                       = 1,
    CPU_RIGTH_E_DDR                        = 2,
    CPU_RIGTH_E_ASRAM                      = 3,
    CPU_RIGTH_E_SD                         = 4,
    CPU_RIGTH_E_DMA1                       = 5,
    CPU_RIGTH_E_QSPI                       = 6,
    CPU_RIGTH_E_SPI0                       = 7,
    CPU_RIGTH_E_SPI1                       = 8,
    CPU_RIGTH_E_SPI2                       = 9,
    CPU_RIGTH_E_SPI3                       = 10,
    CPU_RIGTH_E_I2C0                       = 11,
    CPU_RIGTH_E_I2C1                       = 12,
    CPU_RIGTH_E_UART0                      = 13,
    CPU_RIGTH_E_UART1                      = 14,
    CPU_RIGTH_E_UART2                      = 15,
    CPU_RIGTH_E_UART3                      = 16,
    CPU_RIGTH_E_UART4                      = 17,
    CPU_RIGTH_E_UART5                      = 18,
    CPU_RIGTH_E_UART6                      = 19,
    CPU_RIGTH_E_UART7                      = 20,
    CPU_RIGTH_E_GPIO                       = 21,
    CPU_RIGTH_E_TIMER                      = 22,
    CPU_RIGTH_E_PWM                        = 23,
    CPU_RIGTH_E_SIM0                       = 24,
    CPU_RIGTH_E_SIM1                       = 25,
    CPU_RIGTH_E_SIM2                       = 26,
    CPU_RIGTH_E_BB                         = 27,
    CPU_RIGTH_E_ALL                        = 28,
} CPU_RIGTH_E;

typedef enum
{
    AREA_TYPE_E_ROM = 0,
    AREA_TYPE_E_RAM,
    AREA_TYPE_E_QSPI = 2,
    AREA_TYPE_E_DDR, 
    AREA_TYPE_E_END
}AREA_TYPE_E;

const char *right_desc[] = {
		"rom<0x0--0x10000>",                            // 0
		"sram<0x200000--0x300000>",                     // 1
		"ddr<0x60000000--0xa0000000>",                  // 2
		"asram<0xb0010000--0xb0020000>",                // 3
		"sd<0xb0020000--0xb0030000>",                   // 4
		"dma1<0xb0040000--0xb0050000>",                 // 5
		"qspi<0xb0000000--0xb0010000>",                 // 6
		"spi0<0xc0080000--0xc0090000>",                 // 7
		"spi1<0xc0090000--0xc00a0000>",                 // 8
		"spi2<0xc00a0000--0xc00b0000>",                 // 9
		"spi3<0xc00b0000--0xc00c0000>",                 // 10
		"i2c0<0xc00c0000--0xc00d0000>",                 // 11
		"i2c1<0xc00d0000--0xc00e0000>",                 // 12
		"uart0<0xc0000000--0xc0010000>",                // 13
		"uart1<0xc0010000--0xc0020000>",                // 14
		"uart2<0xc0020000--0xc0030000>",                // 15
		"uart3<0xc0030000--0xc0040000>",                // 16
		"uart4<0xc0040000--0xc0050000>",                // 17
		"uart5<0xc0050000--0xc0060000>",                // 18
		"uart6<0xc0060000--0xc0070000>",                // 19
		"uart7<0xc0070000--0xc0080000>",                // 20
		"gpio<0xc0130000--0xc0170000>",                 // 21
		"timer<0xc0170000--0xc0180000>",                // 22
		"pwm<0xc0180000--0xc0190000>",                  // 23
		"sim0<0xc0100000--0xc0110000>",                 // 24
		"sim1<0xc0110000--0xc0120000>",                 // 25
		"sim2<0xc0120000--0xc0130000>",                 // 26
		"bb<0x90000000--0x91000000>",                   // 27
		"all",                      					// 28
};

void cpu_right_test_init(void);
U32 cpu1_right_ctl_set(CPU_RIGTH_E right_type, BOOL en);
U32 cpu1_area_ctl_set(AREA_TYPE_E area_type, U32 start, U32 end);
void cpu1_rom_area_set(U32 start, U32 end);
void cpu1_ram_area_set(U32 start, U32 end);
void cpu1_qspi_area_set(U32 start, U32 end);
void cpu1_ddr_area_set(U32 start, U32 end);
void print_right_ctl_addr(BOOL list_en);

#endif
