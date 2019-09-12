#ifndef _RIGHT_H_
#define _RIGHT_H_

#include "scm.h"

#define REG_AP_SCM_RIGHT_BASE         (REG_AP_SCM_BASE+0x2C)

typedef volatile struct
{
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
} HWP_SCM_RIGHT_ST;

#define hwp_apSCM_Right               ((HWP_SCM_RIGHT_ST*) (REG_AP_SCM_RIGHT_BASE))

typedef enum
{
    CPU_RIGTH_SHIFT_E_ROM                        = 0,
    CPU_RIGTH_SHIFT_E_SRAM                       = 1,
    CPU_RIGTH_SHIFT_E_DDR                        = 2,
    CPU_RIGTH_SHIFT_E_ASRAM                      = 3,
    CPU_RIGTH_SHIFT_E_SD                         = 4,
    CPU_RIGTH_SHIFT_E_DMA1                       = 5,
    CPU_RIGTH_SHIFT_E_QSPI                       = 6,
    CPU_RIGTH_SHIFT_E_SPI0                       = 7,
    CPU_RIGTH_SHIFT_E_SPI1                       = 8,
    CPU_RIGTH_SHIFT_E_SPI2                       = 9,
    CPU_RIGTH_SHIFT_E_SPI3                       = 10,
    CPU_RIGTH_SHIFT_E_I2C0                       = 11,
    CPU_RIGTH_SHIFT_E_I2C1                       = 12,
    CPU_RIGTH_SHIFT_E_UART0                      = 13,
    CPU_RIGTH_SHIFT_E_UART1                      = 14,
    CPU_RIGTH_SHIFT_E_UART2                      = 15,
    CPU_RIGTH_SHIFT_E_UART3                      = 16,
    CPU_RIGTH_SHIFT_E_UART4                      = 17,
    CPU_RIGTH_SHIFT_E_UART5                      = 18,
    CPU_RIGTH_SHIFT_E_UART6                      = 19,
    CPU_RIGTH_SHIFT_E_UART7                      = 20,
    CPU_RIGTH_SHIFT_E_GPIO                       = 21,
    CPU_RIGTH_SHIFT_E_TIMER                      = 22,
    CPU_RIGTH_SHIFT_E_PWM                        = 23,
    CPU_RIGTH_SHIFT_E_SIM0                       = 24,
    CPU_RIGTH_SHIFT_E_SIM1                       = 25,
    CPU_RIGTH_SHIFT_E_SIM2                       = 26,
    CPU_RIGTH_SHIFT_E_BB                         = 27,
    CPU_RIGTH_SHIFT_E_ALL                        = 28,
} CPU_RIGTH_SHIFT_E;

#endif

