#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

void scm_io_mux_70(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_70;

    iomux_debug("St:  io_mux_70 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_70 = reg_val;

    reg_val = hwp_apSCM->io_mux_70;
    iomux_debug("End: io_mux_70 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_74(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_74;

    iomux_debug("St:  io_mux_74 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_74 = reg_val;

    reg_val = hwp_apSCM->io_mux_74;
    iomux_debug("End: io_mux_74 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_78(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_78;
    iomux_debug("St:  io_mux_78 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_78 = reg_val;

    reg_val = hwp_apSCM->io_mux_78;
    iomux_debug("End: io_mux_78 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_7c(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_7c;
    iomux_debug("St:  io_mux_7c regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_7c = reg_val;

    reg_val = hwp_apSCM->io_mux_7c;
    iomux_debug("End: io_mux_7c regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_80(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_80;
    iomux_debug("St:  io_mux_80 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_80 = reg_val;

    reg_val = hwp_apSCM->io_mux_80;
    iomux_debug("End: io_mux_80 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_84(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_84;
    iomux_debug("St:  io_mux_84 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_84 = reg_val;

    reg_val = hwp_apSCM->io_mux_84;
    iomux_debug("End: io_mux_84 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_88(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_88;
    iomux_debug("St:  io_mux_88 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_88 = reg_val;

    reg_val = hwp_apSCM->io_mux_88;
    iomux_debug("End: io_mux_88 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_8c(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_8c;
    iomux_debug("St:  io_mux_8c regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_8c = reg_val;

    reg_val = hwp_apSCM->io_mux_8c;
    iomux_debug("End: io_mux_8c regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_90(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_90;
    iomux_debug("St:  io_mux_90 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_90 = reg_val;

    reg_val = hwp_apSCM->io_mux_90;
    iomux_debug("End: io_mux_90 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_94(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_94;
    iomux_debug("St:  io_mux_94 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_94 = reg_val;

    reg_val = hwp_apSCM->io_mux_94;
    iomux_debug("End: io_mux_94 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_98(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_98;
    iomux_debug("St:  io_mux_98 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_98 = reg_val;

    reg_val = hwp_apSCM->io_mux_98;
    iomux_debug("End: io_mux_98 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_9c(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_9c;
    iomux_debug("St:  io_mux_9c regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_9c = reg_val;

    reg_val = hwp_apSCM->io_mux_9c;
    iomux_debug("End: io_mux_9c regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}

void scm_io_mux_a0(int bit, u32 mask, u32 v)
{
    u32 reg_val = hwp_apSCM->io_mux_a0;
    iomux_debug("St:  io_mux_a0 regv: 0x%x, val %d, set to %d", reg_val, (reg_val >> bit) & mask,
                v);
    reg_val &= ~(mask << bit);
    reg_val |= ((v) << bit);
    hwp_apSCM->io_mux_a0 = reg_val;

    reg_val = hwp_apSCM->io_mux_a0;
    iomux_debug("End: io_mux_a0 regv: 0x%x pos %d, val %d", reg_val, bit, (reg_val >> bit) & mask);
}
