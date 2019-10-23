#include <common.h>
#include <regs/scm.h>
#include <pll.h>
#include <drivers/drv_clk.h>

#ifdef CONFIG_ENABLE_TRACE
#define clk_debug(fmt, args...)  printf(fmt"\n", ##args);
//#define clk_debug(fmt, args...)  printf("%d "fmt"\n", __LINE__, ##args);
#else
#define clk_debug(fmt, args...)
#endif

/*
 *  clock gating interface
 */
static int control_clken0_regs(int bit, int en)
{
   u32 clken = hwp_apSCM->clken_ctrl0;
   if(en > 0)
        clken |= (1 << bit);
   else
        clken &= ~(1 << bit);
    hwp_apSCM->clken_ctrl0 = clken;
    clk_debug("clken_ctrl0 regv: 0x%x", hwp_apSCM->clken_ctrl0);
    return CLK_OP_SUCCESS;
}

static int control_clken1_regs(int bit, int en)
{
   u32 clken = hwp_apSCM->clken_ctrl1;
   if(en > 0)
        clken |= (1 << bit);
   else
        clken &= ~(1 << bit);
    hwp_apSCM->clken_ctrl1 = clken;
    clk_debug("clken_ctrl1 regv: 0x%x", hwp_apSCM->clken_ctrl1);
    return CLK_OP_SUCCESS;
}

int clk_disable(CLK_ID_TYPE_T id)
{
    switch(id) {
    case CLK_CPU1:
        return control_clken0_regs(SCM_CLK_EN_CPU1, 0);  
    case CLK_DDR:
        return control_clken0_regs(SCM_CLK_EN_DDR, 0);  
    case CLK_62M:
        return control_clken0_regs(SCM_CLK_EN_62M, 0);  
    case CLK_ROM:
        return control_clken0_regs(SCM_CLK_EN_ROM, 0);  
    case CLK_RAM:
        return control_clken0_regs(SCM_CLK_EN_RAM, 0);  
    case CLK_BB:             
        return control_clken0_regs(SCM_CLK_EN_BB, 0);  
    case CLK_MP:             
        return control_clken0_regs(SCM_CLK_EN_MP, 0);  
    case CLK_DMAC0:
        return control_clken0_regs(SCM_CLK_EN_DMAC0, 0);  
    case CLK_DMAC1:
        return control_clken0_regs(SCM_CLK_EN_DMAC1, 0);  
    case CLK_QSPI:
        return control_clken0_regs(SCM_CLK_EN_QSPI, 0);  
    case CLK_SD:
        return control_clken0_regs(SCM_CLK_EN_SD, 0);  
    case CLK_UART0:
        return control_clken0_regs(SCM_CLK_EN_UART0, 0);  
    case CLK_UART1:
        return control_clken0_regs(SCM_CLK_EN_UART1, 0);  
    case CLK_UART2:
        return control_clken0_regs(SCM_CLK_EN_UART2, 0);  
    case CLK_UART3:
        return control_clken0_regs(SCM_CLK_EN_UART3, 0);  
    case CLK_UART4:
        return control_clken0_regs(SCM_CLK_EN_UART4, 0);  
    case CLK_UART5:
        return control_clken0_regs(SCM_CLK_EN_UART5, 0);  
    case CLK_UART6:
        return control_clken0_regs(SCM_CLK_EN_UART6, 0);  
    case CLK_UART7:
        return control_clken0_regs(SCM_CLK_EN_UART7, 0);  
    case CLK_I2C0:
        return control_clken0_regs(SCM_CLK_EN_I2C0, 0);  
    case CLK_I2C1:
        return control_clken0_regs(SCM_CLK_EN_I2C1, 0);  
    case CLK_SPI0:
        return control_clken0_regs(SCM_CLK_EN_SPI0, 0);  
    case CLK_SPI1:
        return control_clken0_regs(SCM_CLK_EN_SPI1, 0);  
    case CLK_SPI2:
        return control_clken0_regs(SCM_CLK_EN_SPI2, 0);  
    case CLK_SPI3:
        return control_clken0_regs(SCM_CLK_EN_SPI3, 0);  
    case CLK_SIM0:
        return control_clken0_regs(SCM_CLK_EN_SIM0, 0);  
    case CLK_SIM1:
        return control_clken0_regs(SCM_CLK_EN_SIM1, 0);  
    case CLK_SIM2:
        return control_clken0_regs(SCM_CLK_EN_SIM2, 0);  
    case CLK_PWM:
        return control_clken0_regs(SCM_CLK_EN_PWM, 0);  
    case CLK_GPIO:
        return control_clken0_regs(SCM_CLK_EN_GPIO, 0);  
    case CLK_TIMER:
        return control_clken0_regs(SCM_CLK_EN_TIMER, 0);  
    case CLK_EFUSE:
        return control_clken0_regs(SCM_CLK_EN_EFUSE, 0);  
    case CLK_SYS32K:     
        return control_clken1_regs(SCM_CLK_EN_SYS32K, 0);  
    case CLK_5M:         
        return control_clken1_regs(SCM_CLK_EN_5M, 0);  
    case CLK_RDSS:       
        return control_clken1_regs(SCM_CLK_EN_RDSS, 0);  
    default:
        clk_debug("%d ID is error!", id);
        return CLK_OP_INV_FAILED;
    }

    return CLK_OP_INV_FAILED;
}

int clk_enable(CLK_ID_TYPE_T id)
{
    switch(id) {
    case CLK_CPU1:
        return control_clken0_regs(SCM_CLK_EN_CPU1, 1);  
    case CLK_DDR:
        return control_clken0_regs(SCM_CLK_EN_DDR, 1);  
    case CLK_62M:
        return control_clken0_regs(SCM_CLK_EN_62M, 1);  
    case CLK_ROM:
        return control_clken0_regs(SCM_CLK_EN_ROM, 1);  
    case CLK_RAM:
        return control_clken0_regs(SCM_CLK_EN_RAM, 1);  
    case CLK_BB:             
        return control_clken0_regs(SCM_CLK_EN_BB, 1);  
    case CLK_MP:             
        return control_clken0_regs(SCM_CLK_EN_MP, 1);  
    case CLK_DMAC0:
        return control_clken0_regs(SCM_CLK_EN_DMAC0, 1);  
    case CLK_DMAC1:
        return control_clken0_regs(SCM_CLK_EN_DMAC1, 1);  
    case CLK_QSPI:
        return control_clken0_regs(SCM_CLK_EN_QSPI, 1);  
    case CLK_SD:
        return control_clken0_regs(SCM_CLK_EN_SD, 1);  
    case CLK_UART0:
        return control_clken0_regs(SCM_CLK_EN_UART0, 1);  
    case CLK_UART1:
        return control_clken0_regs(SCM_CLK_EN_UART1, 1);  
    case CLK_UART2:
        return control_clken0_regs(SCM_CLK_EN_UART2, 1);  
    case CLK_UART3:
        return control_clken0_regs(SCM_CLK_EN_UART3, 1);  
    case CLK_UART4:
        return control_clken0_regs(SCM_CLK_EN_UART4, 1);  
    case CLK_UART5:
        return control_clken0_regs(SCM_CLK_EN_UART5, 1);  
    case CLK_UART6:
        return control_clken0_regs(SCM_CLK_EN_UART6, 1);  
    case CLK_UART7:
        return control_clken0_regs(SCM_CLK_EN_UART7, 1);  
    case CLK_I2C0:
        return control_clken0_regs(SCM_CLK_EN_I2C0, 1);  
    case CLK_I2C1:
        return control_clken0_regs(SCM_CLK_EN_I2C1, 1);  
    case CLK_SPI0:
        return control_clken0_regs(SCM_CLK_EN_SPI0, 1);  
    case CLK_SPI1:
        return control_clken0_regs(SCM_CLK_EN_SPI1, 1);  
    case CLK_SPI2:
        return control_clken0_regs(SCM_CLK_EN_SPI2, 1);  
    case CLK_SPI3:
        return control_clken0_regs(SCM_CLK_EN_SPI3, 1);  
    case CLK_SIM0:
        return control_clken0_regs(SCM_CLK_EN_SIM0, 1);  
    case CLK_SIM1:
        return control_clken0_regs(SCM_CLK_EN_SIM1, 1);  
    case CLK_SIM2:
        return control_clken0_regs(SCM_CLK_EN_SIM2, 1);  
    case CLK_PWM:
        return control_clken0_regs(SCM_CLK_EN_PWM, 1);  
    case CLK_GPIO:
        return control_clken0_regs(SCM_CLK_EN_GPIO, 1);  
    case CLK_TIMER:
        return control_clken0_regs(SCM_CLK_EN_TIMER, 1);  
    case CLK_EFUSE:
        return control_clken0_regs(SCM_CLK_EN_EFUSE, 1);  
    case CLK_SYS32K:     
        return control_clken1_regs(SCM_CLK_EN_SYS32K, 1);  
    case CLK_5M:         
        return control_clken1_regs(SCM_CLK_EN_5M, 1);  
    case CLK_RDSS:       
        return control_clken1_regs(SCM_CLK_EN_RDSS, 1);  
    default:
        return CLK_OP_INV_FAILED;
    }
    return CLK_OP_INV_FAILED;
}
/*
 *  clock divider interface
 */

static int get_strip_refclk_rate_kHz(void)
{
    u32 stap = hwp_apSCM->strappingstatus;
    clk_debug("strappingstatus regv: 0x%x", stap);
    switch(stap & SCM_STRAP_REFCLK_MASK) {
    case SCM_STRAP_REFCLK_10M:
        return 10000;
    case SCM_STRAP_REFCLK_20M:
        return 20000;
    case SCM_STRAP_REFCLK_25M:
        return 25000;
    default:
        return CLK_OP_HW_INV_FAILED;
    }
    return CLK_OP_HW_INV_FAILED;
}

static int control_clk_div0_regs(int bit, u32 mask, u32 *v, int set)
{
    u32 clk_div = hwp_apSCM->clk_div0;
    clk_debug("St:  clk_div0 regv: 0x%x", clk_div);
    if(set > 0) {
        clk_div &= ~(mask << bit);
        clk_div |= ((*v) << bit);
        hwp_apSCM->clk_div0 = clk_div;
    } else {
        clk_div = clk_div >> bit;
        *v = clk_div & mask;
    }
    clk_debug("End: clk_div0 regv: 0x%x", hwp_apSCM->clk_div0);
    return CLK_OP_SUCCESS;
}

static int control_clk_div1_regs(int bit, u32 mask, u32 *v, int set)
{
    u32 clk_div = hwp_apSCM->clk_div1;
    clk_debug("St:  clk_div1 regv: 0x%x", clk_div);
    if(set > 0) {
        clk_div &= ~(mask << bit);
        clk_div |= ((*v) << bit);
        hwp_apSCM->clk_div1 = clk_div;
    } else {
        clk_div = clk_div >> bit;
        *v = clk_div & mask;
    }
    clk_debug("End: clk_div1 regv: 0x%x", hwp_apSCM->clk_div1);
    return CLK_OP_SUCCESS;
}

static int control_clk_div2_regs(int bit, u32 mask, u32 *v, int set)
{
    u32 clk_div = hwp_apSCM->clk_div2;
    clk_debug("St:  clk_div2 regv: 0x%x", clk_div);
    if(set > 0) {
        clk_div &= ~(mask << bit);
        clk_div |= ((*v) << bit);
        hwp_apSCM->clk_div2 = clk_div;
    } else {
        clk_div = clk_div >> bit;
        *v = clk_div & mask;
    }
    clk_debug("End: clk_div2 regv: 0x%x", hwp_apSCM->clk_div2);
    return CLK_OP_SUCCESS;
}

static int control_clk_pll4_ctrl_regs(int bit, u32 mask, u32 *v, int set)
{
    u32 clk_div = hwp_apSCM->pll4_ctrl;
    clk_debug("St:  pll4_ctrl regv: 0x%x", clk_div);
    if(set > 0) {
        clk_div &= ~(mask << bit);
        clk_div |= ((*v) << bit);
        hwp_apSCM->pll4_ctrl = clk_div;
    } else {
        clk_div = clk_div >> bit;
        *v = clk_div & mask;
    }
    clk_debug("End: pll4_ctrl regv: 0x%x", hwp_apSCM->pll4_ctrl);
    return CLK_OP_SUCCESS;
}

static int clk_get_invalid_div_value(CLK_ID_TYPE_T id, u32 v, u32 *ret_v)
{
    u32 mask_v = 0; 

    if(v == 0)
        return CLK_OP_INV_FAILED;

    switch(id) {
    case CLK_CPU0:
        mask_v = (SCM_CLK_DIV_CPU0_MASK & v);
        break;
    case CLK_CPU1:
        mask_v = (SCM_CLK_DIV_CPU1_MASK & v);
        break;
    case CLK_AXI:
        mask_v = (SCM_CLK_DIV_AXI_MASK & v);
        break;
    case CLK_AHB:
        mask_v = (SCM_CLK_DIV_AHB_MASK & v);
        break;
    case CLK_APB:
        mask_v = (SCM_CLK_DIV_APB_MASK & v);
        break;
    case CLK_DDR:
        mask_v = (SCM_CLK_DIV_DDR_MASK & v);
        break;
    case CLK_62M:
        mask_v = (SCM_CLK_DIV_62M_MASK & v);
        break;
    case CLK_RAM:
        mask_v = (SCM_CLK_DIV_RAM_MASK & v);
        break;
    case CLK_SD:
        mask_v = (SCM_CLK_DIV_SD_MASK & v);
        break;
    case CLK_BB:
        mask_v = (SCM_CLK_DIV_BB_MASK & v);
        break;
    case CLK_MP:
        mask_v = (SCM_CLK_DIV_MP_MASK & v);
        break;
    case CLK_QSPI:
        mask_v = (SCM_CLK_DIV_QSPI_MASK & v);
        break;
    case CLK_I2C0:
    case CLK_I2C1:
        mask_v = (SCM_CLK_DIV_I2C_MASK & v);
        break;
    case CLK_UART6:
        mask_v = (SCM_CLK_DIV_UART6_MASK & v);
        break;
    case CLK_UART7:
        mask_v = (SCM_CLK_DIV_UART7_MASK & v);
        break;
    case CLK_SPI0:
        mask_v = (SCM_CLK_DIV_SPI0_MASK & v);
        break;
    case CLK_SPI1:
        mask_v = (SCM_CLK_DIV_SPI1_MASK & v);
        break;
    case CLK_SPI2:
        mask_v = (SCM_CLK_DIV_SPI2_MASK & v);
        break;
    case CLK_SPI3:
        mask_v = (SCM_CLK_DIV_SPI3_MASK & v);
        break;
    case CLK_SIM0:
        mask_v = (SCM_CLK_DIV_SIM0_MASK & v);
        break;
    case CLK_SIM1:
        mask_v = (SCM_CLK_DIV_SIM1_MASK & v);
        break;
    case CLK_SIM2:
        mask_v = (SCM_CLK_DIV_SIM2_MASK & v);
        break;
    case CLK_PWM:
        mask_v = (SCM_CLK_DIV_PWM_MASK & v);
        break;
    case CLK_RDSS:
        mask_v = (SCM_CLK_DIV_RDSS_MASK & v);
        break;
    default:
        return CLK_OP_INV_FAILED;
    }

    if(mask_v != v)
        return CLK_OP_HW_NOT_SUPPORT_FAILED;

    *ret_v = mask_v;

    return CLK_OP_SUCCESS;
}

static int set_clk_div_value(CLK_ID_TYPE_T id, u32 v)
{
    u32 div;
    int ret = clk_get_invalid_div_value(id, v, &div);

    if(ret != CLK_OP_SUCCESS)
        return ret;

    switch(id) {
    case CLK_CPU0:
        control_clk_div0_regs(SCM_CLK_DIV_CPU0_BITS, SCM_CLK_DIV_CPU0_MASK, &div, 1);
        break;
    case CLK_CPU1:
        control_clk_div0_regs(SCM_CLK_DIV_CPU1_BITS, SCM_CLK_DIV_CPU1_MASK, &div, 1);
        break;
    case CLK_AXI:
        control_clk_div0_regs(SCM_CLK_DIV_AXI_BITS, SCM_CLK_DIV_AXI_MASK, &div, 1);
        break;
    case CLK_AHB:
        control_clk_div0_regs(SCM_CLK_DIV_AHB_BITS, SCM_CLK_DIV_AHB_MASK, &div, 1);
        break;
    case CLK_APB:
        control_clk_div0_regs(SCM_CLK_DIV_APB_BITS, SCM_CLK_DIV_APB_MASK, &div, 1);
        break;
    case CLK_DDR:
        control_clk_div0_regs(SCM_CLK_DIV_DDR_BITS, SCM_CLK_DIV_DDR_MASK, &div, 1);
        break;
    case CLK_62M:
        control_clk_div0_regs(SCM_CLK_DIV_62M_BITS, SCM_CLK_DIV_62M_MASK, &div, 1);
        break;
    case CLK_RAM:
        control_clk_div0_regs(SCM_CLK_DIV_RAM_BITS, SCM_CLK_DIV_RAM_MASK, &div, 1);
        break;
    case CLK_SD:
        control_clk_div1_regs(SCM_CLK_DIV_SD_BITS, SCM_CLK_DIV_SD_MASK, &div, 1);
        break;
    case CLK_BB:
        control_clk_div1_regs(SCM_CLK_DIV_BB_BITS, SCM_CLK_DIV_BB_MASK, &div, 1);
        break;
    case CLK_MP:
        control_clk_div1_regs(SCM_CLK_DIV_MP_BITS, SCM_CLK_DIV_MP_MASK, &div, 1);
        break;
    case CLK_QSPI:
        control_clk_div1_regs(SCM_CLK_DIV_QSPI_BITS, SCM_CLK_DIV_QSPI_MASK, &div, 1);
        break;
    case CLK_I2C0:
    case CLK_I2C1:
        control_clk_div1_regs(SCM_CLK_DIV_I2C_BITS, SCM_CLK_DIV_I2C_MASK, &div, 1);
        break;
    case CLK_UART6:
        control_clk_div1_regs(SCM_CLK_DIV_UART6_BITS, SCM_CLK_DIV_UART6_MASK, &div, 1);
        break;
    case CLK_UART7:
        control_clk_div1_regs(SCM_CLK_DIV_UART7_BITS, SCM_CLK_DIV_UART7_MASK, &div, 1);
        break;
    case CLK_SPI0:
        control_clk_div2_regs(SCM_CLK_DIV_SPI0_BITS, SCM_CLK_DIV_SPI0_MASK, &div, 1);
        break;
    case CLK_SPI1:
        control_clk_div2_regs(SCM_CLK_DIV_SPI1_BITS, SCM_CLK_DIV_SPI1_MASK, &div, 1);
        break;
    case CLK_SPI2:
        control_clk_div2_regs(SCM_CLK_DIV_SPI2_BITS, SCM_CLK_DIV_SPI2_MASK, &div, 1);
        break;
    case CLK_SPI3:
        control_clk_div2_regs(SCM_CLK_DIV_SPI3_BITS, SCM_CLK_DIV_SPI3_MASK, &div, 1);
        break;
    case CLK_SIM0:
        control_clk_div2_regs(SCM_CLK_DIV_SIM0_BITS, SCM_CLK_DIV_SIM0_MASK, &div, 1);
        break;
    case CLK_SIM1:
        control_clk_div2_regs(SCM_CLK_DIV_SIM1_BITS, SCM_CLK_DIV_SIM1_MASK, &div, 1);
        break;
    case CLK_SIM2:
        control_clk_div2_regs(SCM_CLK_DIV_SIM2_BITS, SCM_CLK_DIV_SIM2_MASK, &div, 1);
        break;
    case CLK_PWM:
        control_clk_div2_regs(SCM_CLK_DIV_PWM_BITS, SCM_CLK_DIV_PWM_MASK, &div, 1);
        break;
    case CLK_RDSS:
        control_clk_pll4_ctrl_regs(SCM_CLK_DIV_RDSS_BITS, SCM_CLK_DIV_RDSS_MASK, &div, 1);
        break;
    default:
        return CLK_OP_INV_FAILED;
    }

    return CLK_OP_SUCCESS;
}

static int get_clk_div_value(CLK_ID_TYPE_T id, u32 *v)
{
    u32 div = 0;
    switch(id) {
    case CLK_CPU0:
        control_clk_div0_regs(SCM_CLK_DIV_CPU0_BITS, SCM_CLK_DIV_CPU0_MASK, &div, 0);
        break;
    case CLK_CPU1:
        control_clk_div0_regs(SCM_CLK_DIV_CPU1_BITS, SCM_CLK_DIV_CPU1_MASK, &div, 0);
        break;
    case CLK_AXI:
        control_clk_div0_regs(SCM_CLK_DIV_AXI_BITS, SCM_CLK_DIV_AXI_MASK, &div, 0);
        break;
    case CLK_AHB:
        control_clk_div0_regs(SCM_CLK_DIV_AHB_BITS, SCM_CLK_DIV_AHB_MASK, &div, 0);
        break;
    case CLK_APB:
        control_clk_div0_regs(SCM_CLK_DIV_APB_BITS, SCM_CLK_DIV_APB_MASK, &div, 0);
        break;
    case CLK_DDR:
        control_clk_div0_regs(SCM_CLK_DIV_DDR_BITS, SCM_CLK_DIV_DDR_MASK, &div, 0);
        break;
    case CLK_62M:
        control_clk_div0_regs(SCM_CLK_DIV_62M_BITS, SCM_CLK_DIV_62M_MASK, &div, 0);
        break;
    case CLK_RAM:
        control_clk_div0_regs(SCM_CLK_DIV_RAM_BITS, SCM_CLK_DIV_RAM_MASK, &div, 0);
        break;
    case CLK_SD:
        control_clk_div1_regs(SCM_CLK_DIV_SD_BITS, SCM_CLK_DIV_SD_MASK, &div, 0);
        break;
    case CLK_BB:
        control_clk_div1_regs(SCM_CLK_DIV_BB_BITS, SCM_CLK_DIV_BB_MASK, &div, 0);
        break;
    case CLK_MP:
        control_clk_div1_regs(SCM_CLK_DIV_MP_BITS, SCM_CLK_DIV_MP_MASK, &div, 0);
        break;
    case CLK_QSPI:
        control_clk_div1_regs(SCM_CLK_DIV_QSPI_BITS, SCM_CLK_DIV_QSPI_MASK, &div, 0);
        break;
    case CLK_I2C0:
    case CLK_I2C1:
        control_clk_div1_regs(SCM_CLK_DIV_I2C_BITS, SCM_CLK_DIV_I2C_MASK, &div, 0);
        break;
    case CLK_UART6:
        control_clk_div1_regs(SCM_CLK_DIV_UART6_BITS, SCM_CLK_DIV_UART6_MASK, &div, 0);
        break;
    case CLK_UART7:
        control_clk_div1_regs(SCM_CLK_DIV_UART7_BITS, SCM_CLK_DIV_UART7_MASK, &div, 0);
        break;
    case CLK_SPI0:
        control_clk_div2_regs(SCM_CLK_DIV_SPI0_BITS, SCM_CLK_DIV_SPI0_MASK, &div, 0);
        break;
    case CLK_SPI1:
        control_clk_div2_regs(SCM_CLK_DIV_SPI1_BITS, SCM_CLK_DIV_SPI1_MASK, &div, 0);
        break;
    case CLK_SPI2:
        control_clk_div2_regs(SCM_CLK_DIV_SPI2_BITS, SCM_CLK_DIV_SPI2_MASK, &div, 0);
        break;
    case CLK_SPI3:
        control_clk_div2_regs(SCM_CLK_DIV_SPI3_BITS, SCM_CLK_DIV_SPI3_MASK, &div, 0);
        break;
    case CLK_SIM0:
        control_clk_div2_regs(SCM_CLK_DIV_SIM0_BITS, SCM_CLK_DIV_SIM0_MASK, &div, 0);
        break;
    case CLK_SIM1:
        control_clk_div2_regs(SCM_CLK_DIV_SIM1_BITS, SCM_CLK_DIV_SIM1_MASK, &div, 0);
        break;
    case CLK_SIM2:
        control_clk_div2_regs(SCM_CLK_DIV_SIM2_BITS, SCM_CLK_DIV_SIM2_MASK, &div, 0);
        break;
    case CLK_PWM:
        control_clk_div2_regs(SCM_CLK_DIV_PWM_BITS, SCM_CLK_DIV_PWM_MASK, &div, 0);
        break;
    case CLK_RDSS:
        control_clk_pll4_ctrl_regs(SCM_CLK_DIV_RDSS_BITS, SCM_CLK_DIV_RDSS_MASK, &div, 0);
        break;
    default:
        return CLK_OP_INV_FAILED;
    }

    if(div == 0)
        return CLK_OP_HW_INV_FAILED;

    *v = div;

    return CLK_OP_SUCCESS;
}

inline static u32 calc_div_value(u32 pll_o, u32 set)
{
      u32 tmp = pll_o * 10;
      tmp = tmp / set;
      tmp += 5;			//round up
      tmp /= 10;
      if(tmp == 0)
        tmp = 1;
      return tmp;
}

/*cpu0/cpu1*/
static int clk_get_cpu_rate_kHz(CLK_ID_TYPE_T id)
{
    u32 div = 0;
    int ret = get_clk_div_value(id, &div);
        
    if(ret != CLK_OP_SUCCESS) 
        return ret;

    return pll0_get_rate_kHz()/div;
}

static int clk_set_cpu_rate_kHz(CLK_ID_TYPE_T id, int khz)
{
    u32 div = 0;
    if(khz <=0)
        return CLK_OP_INV_FAILED;

    div = calc_div_value(pll0_get_rate_kHz(), khz);
    return set_clk_div_value(id, div);
}

/*ddr/62M*/
static int clk_get_ddr_rate_kHz(CLK_ID_TYPE_T id)
{
    u32 div = 0;
    int ret = get_clk_div_value(id, &div);
        
    if(ret != CLK_OP_SUCCESS) 
        return ret;

    return pll1_get_rate_kHz()/div;
}

static int clk_set_ddr_rate_kHz(CLK_ID_TYPE_T id, int khz)
{
    u32 div = 0;
    if(khz <=0)
        return CLK_OP_INV_FAILED;

    div = calc_div_value(pll1_get_rate_kHz(), khz);
    return set_clk_div_value(id, div);
}

/*axi/ahb/apb/ram/sd/mp/qspi/i2c/uart6/uar7/pwm*/
static int clk_get_bus_rate_kHz(CLK_ID_TYPE_T id)
{
    u32 div = 0;
    int ret = get_clk_div_value(id, &div);
        
    if(ret != CLK_OP_SUCCESS) 
        return ret;
    return pll2_get_rate_kHz()/div;
}

static int clk_set_bus_rate_kHz(CLK_ID_TYPE_T id, int khz)
{
    u32 div = 0;
    if(khz <=0)
        return CLK_OP_INV_FAILED;

    div = calc_div_value(pll2_get_rate_kHz(), khz);
    return set_clk_div_value(id, div);
}

/*bb*/
static int clk_get_bb_rate_kHz(void)
{
    u32 div = 0;
    int ret = get_clk_div_value(CLK_BB, &div);
        
    if(ret != CLK_OP_SUCCESS) 
        return ret;
 
    return pll3_get_rate_kHz()/div;
}

static int clk_set_bb_rate_kHz(int khz)
{
    u32 div = 0;
    if(khz <=0)
        return CLK_OP_INV_FAILED;
  
   div = calc_div_value(pll3_get_rate_kHz(), khz);
   return set_clk_div_value(CLK_BB, div);
}

/*rdss*/
static int clk_get_rdss_rate_kHz(void)
{
    u32 div = 0;
    int ret = get_clk_div_value(CLK_RDSS, &div);
        
    if(ret != CLK_OP_SUCCESS) 
        return ret;
 
    return pll3_get_rate_kHz()/div;
}

static int clk_set_rdss_rate_kHz(int khz)
{
    u32 div = 0;
    if(khz <=0)
        return CLK_OP_INV_FAILED;
  
   div = calc_div_value(pll3_get_rate_kHz(), khz);
   return set_clk_div_value(CLK_RDSS, div);
}

/*sim0/sim1/sim2*/
static int clk_get_sim_rate_kHz(CLK_ID_TYPE_T id)
{
    u32 div = 0;
    int pll_refclk;
    int ret = get_clk_div_value(id, &div);
        
    if(ret != CLK_OP_SUCCESS) 
        return ret;

    pll_refclk = get_strip_refclk_rate_kHz();
    if(pll_refclk < 0)  
        return CLK_OP_INV_FAILED;

    return pll_refclk/div;
}

static int clk_set_sim_rate_kHz(CLK_ID_TYPE_T id, int khz)
{
    u32 div = 0, pll_refclk;
    if(khz <=0)
        return CLK_OP_INV_FAILED;

    pll_refclk = get_strip_refclk_rate_kHz();
    if(pll_refclk < 0)
        return CLK_OP_INV_FAILED;

    div = calc_div_value(pll_refclk, khz);
    return set_clk_div_value(id, div);
}

int clk_get_rate_kHz(CLK_ID_TYPE_T id)
{
    switch(id) {
    case CLK_CPU0:
    case CLK_CPU1:
         return clk_get_cpu_rate_kHz(id);
    case CLK_DDR:
    case CLK_62M:
         return clk_get_ddr_rate_kHz(id);
    case CLK_AXI:
    case CLK_AHB:
    case CLK_APB:
    case CLK_RAM:
    case CLK_SD:
    case CLK_MP:
    case CLK_QSPI:
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SPI2:
    case CLK_SPI3:
    case CLK_PWM:
        return clk_get_bus_rate_kHz(id);
    case CLK_BB:
        return clk_get_bb_rate_kHz();
    case CLK_RDSS:
        return clk_get_rdss_rate_kHz();
    case CLK_SIM0:
    case CLK_SIM1:
    case CLK_SIM2:
        return clk_get_sim_rate_kHz(id); 
    default:
         break;
    }
    return CLK_OP_INV_FAILED; 
}

int clk_set_rate_kHz(CLK_ID_TYPE_T id, int rate_kHz)
{
    switch(id) {
    case CLK_CPU0:
    case CLK_CPU1:
         return clk_set_cpu_rate_kHz(id, rate_kHz);
    case CLK_DDR:
    case CLK_62M:
         return clk_set_ddr_rate_kHz(id, rate_kHz);
    case CLK_AXI:
    case CLK_AHB:
    case CLK_APB:
    case CLK_RAM:
    case CLK_SD:
    case CLK_MP:
    case CLK_QSPI:
    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SPI2:
    case CLK_SPI3:
    case CLK_PWM:
        return clk_set_bus_rate_kHz(id, rate_kHz);
    case CLK_BB:
        return clk_set_bb_rate_kHz(rate_kHz);
    case CLK_RDSS:
        return clk_set_rdss_rate_kHz(rate_kHz);
    case CLK_SIM0:
    case CLK_SIM1:
    case CLK_SIM2:
        return clk_set_sim_rate_kHz(id, rate_kHz); 
    default:
         break;
    }
    return CLK_OP_INV_FAILED; 
}
