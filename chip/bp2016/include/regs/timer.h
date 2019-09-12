#ifndef _REGS_TIMER_H_
#define _REGS_TIMER_H_

#define REG_AP_TIMER0_BASE         (APB2_BASE+0x70000)
#define REG_AP_TIMER1_BASE         (APB2_BASE+0x70100)
#define REG_AP_TIMER2_BASE         (APB2_BASE+0x70200)
#define REG_AP_TIMER3_BASE         (APB2_BASE+0x70300)

typedef volatile struct
{
    REG32                          ctrl;               //0x00000000
    REG32                          loadval;            //0x00000004
    REG32                          curval;             //0x00000008
    REG32                          irq_enable;         //0x0000000C

    REG32                          _pad_0x10;          //0x00000010
    REG32                          irq_eoi;            //0x00000014
    REG32                          irq_cause;          //0x00000018
    REG32                          _pad_0x1c;          //0x0000001c

    REG32                          lockval_l;          //0x00000020
    REG32                          lockval_h;          //0x00000024
} HWP_TIMER_AP_T;

#define hwp_apTimer0               ((HWP_TIMER_AP_T*) (REG_AP_TIMER0_BASE))
#define hwp_apTimer1               ((HWP_TIMER_AP_T*) (REG_AP_TIMER1_BASE))
#define hwp_apTimer2               ((HWP_TIMER_AP_T*) (REG_AP_TIMER2_BASE))
#define hwp_apTimer3               ((HWP_TIMER_AP_T*) (REG_AP_TIMER3_BASE))


//ctrl
#define TIMER_AP_ENABLE            (1<<0)
#define TIMER_AP_REPEAT            (1<<3)
#define TIMER_AP_LOAD              (1<<4)

//irq enable
#define TIMER_AP_IRQ_ENABLE      (1<<0)

//irq_cause
#define TIMER_AP_IRQ_CAUSE	 (1<<0)

//lockval_h
#define TIMER_AP_OS_LOCKVAL_H(n)   (((n)&0xFFFFFF)<<0)
#define TIMER_AP_OS_LOCKVAL_H_MASK (0xFFFFFF<<0)
#define TIMER_AP_OS_LOCKVAL_H_SHIFT (0)

#endif

