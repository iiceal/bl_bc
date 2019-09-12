#ifndef __DRV_CLK_H_
#define __DRV_CLK_H_

typedef enum {
    CLK_CPU0,       /*        divider*/
    CLK_CPU1,       /*gating, divider*/ 
    CLK_AXI,        /*        divider*/
    CLK_AHB,        /*        divider*/

    CLK_APB,        /*        divider*/
    CLK_DDR,        /*gating, divider*/ 
    CLK_62M,        /*gating, divider*/
    CLK_ROM,        /*gating, */

    CLK_RAM,        /*gating,  divider*/
    CLK_BB,         /*gating,  divider*/
    CLK_MP,         /*gating,  divider*/
    CLK_DMAC0,      /*gating, */
 
    CLK_DMAC1,      /*gating, */
    CLK_QSPI,       /*gating,  divider*/
    CLK_SD,         /*gating,  divider*/
    CLK_UART0,      /*gating, */

    CLK_UART1,      /*gating, */
    CLK_UART2,      /*gating, */
    CLK_UART3,      /*gating, */
    CLK_UART4,      /*gating, */

    CLK_UART5,      /*gating, */
    CLK_UART6,      /*gating,  divider*/
    CLK_UART7,      /*gating,  divider*/
    CLK_I2C0,       /*gating,  divider*/

    CLK_I2C1,       /*gating,  divider*/
    CLK_SPI0,       /*gating,  divider*/
    CLK_SPI1,       /*gating,  divider*/
    CLK_SPI2,       /*gating,  divider*/

    CLK_SPI3,       /*gating,  divider*/
    CLK_SIM0,       /*gating,  divider*/
    CLK_SIM1,       /*gating,  divider*/
    CLK_SIM2,       /*gating,  divider*/

    CLK_PWM,        /*gating,  divider*/
    CLK_GPIO,       /*gating, */
    CLK_TIMER,      /*gating, */
    CLK_EFUSE,      /*gating, */

    CLK_SYS32K,     /*gating, */ 
    CLK_5M,         /*gating, */
    CLK_RDSS,       /*gating, */

    CLK_ID_NUM,

} CLK_ID_TYPE_T;

#define     CLK_OP_SUCCESS                          (0)
#define     CLK_OP_FAILED                           (-1)
#define     CLK_OP_INV_FAILED                       (-2)
#define     CLK_OP_HW_INV_FAILED                    (-3)
#define     CLK_OP_HW_NOT_SUPPORT_FAILED            (-4)

int clk_get_rate_kHz(CLK_ID_TYPE_T id);
int clk_set_rate_kHz(CLK_ID_TYPE_T id, int rate_kHz);
int clk_disable(CLK_ID_TYPE_T id);
int clk_enable(CLK_ID_TYPE_T id);

#endif // __DRV_CLK_H_
