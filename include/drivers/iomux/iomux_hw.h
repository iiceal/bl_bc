#ifndef __IOMUX_HW_20171218_H__
#define __IOMUX_HW_20171218_H__
#include <common.h>

#define iomux_printf(fmt, args...) printf("Iomux: " fmt "\n", ##args);

//#define IOMUX_DEBUG

#ifdef IOMUX_DEBUG
#define iomux_debug(fmt, args...) printf(fmt "\n", ##args);
#else
#define iomux_debug(fmt, args...)
#endif

#define DRV_OP_SUCCESS (0)
#define DRV_OP_FAILED (-1)
#define DRV_OP_INV_FAILED (-2)
#define DRV_OP_HW_INV_FAILED (-3)
#define DRV_OP_HW_NOT_SUPPORT_FAILED (-4)

/*
 * Func:        Set SIM  pin mux
 * Arguments:   sim_index: sim index value is 0/1/2
 * Notice:
 */
int scm_sim_en(int sim_index);

/*
 * Func:        Set mmc pin mux
 * Arguments:   index: mmc index value is 0/1
 * Notice:
 */
int scm_mmc_en(int index);

/*
 * Func:        Set Uart pin mux
 * Arguments:   index: uart index, value is 0~7
 *              ts_flag: stream control flag for uart6, 1: do as stream port
 * Notice:      ts_flag is only for uart6, Its usless for the other uart ports
 */
int scm_uart_en(int index, int ts_flag);

/*
 * Func:        Set PRM  pin mux
 * Arguments:
 * Notice:
 */
void scm_prm0_en(void);
void scm_prm1_en(void);

/*
 * Func:        Set Sram pin mux
 * Arguments:
 * Notice:
 */
void scm_asram_en(void);

void scm_pps0(void);
void scm_pps1(void);
void scm_clk5m(void);
void scm_i2c0_en(void);
void scm_i2c1_en(void);
void scm_odo_en(void);
void scm_irigb_en(void);

int scm_if_en(int index);

#include "iomux_hw_spi.h"
#include "iomux_hw_pwm.h"
#include "iomux_hw_gpio.h"
#include "iomux_hw_if.h"

#endif //__IOMUX_HW_20171218_H__
