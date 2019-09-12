#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

void scm_hwpwm0(void)
{
    scm_io_mux_98(SCM_IOMUX_98_HWPWM0_BIT, SCM_IOMUX_98_HWPWM0_MASK, SCM_IOMUX_98_HWPWM0_DEFAULT);
}

void scm_hwpwm1(void)
{
    scm_io_mux_98(SCM_IOMUX_98_HWPWM1_BIT, SCM_IOMUX_98_HWPWM1_MASK, SCM_IOMUX_98_HWPWM1_DEFAULT);
}

void scm_hwpwm2(void)
{
    scm_io_mux_98(SCM_IOMUX_98_HWPWM2_BIT, SCM_IOMUX_98_HWPWM2_MASK, SCM_IOMUX_98_HWPWM2_DEFAULT);
}

void scm_hwpwm3(void)
{
    scm_io_mux_98(SCM_IOMUX_98_HWPWM3_BIT, SCM_IOMUX_98_HWPWM3_MASK, SCM_IOMUX_98_HWPWM3_DEFAULT);
}

void scm_swpwm0(void)
{
    scm_io_mux_98(SCM_IOMUX_98_SWPWM0_BIT, SCM_IOMUX_98_SWPWM0_MASK, SCM_IOMUX_98_SWPWM0_DEFAULT);
}

void scm_swpwm1(void)
{
    scm_io_mux_a0(SCM_IOMUX_A0_SWPWM1_BIT, SCM_IOMUX_A0_SWPWM1_MASK, SCM_IOMUX_A0_SWPWM1_DEFAULT);
}
