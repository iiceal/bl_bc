#include <common.h>
#include "system_arm.h"

static u32 arch_timer_init = 0;

#define ARCH_TIMER_CTRL_ENABLE          (1 << 0)
#define ARCH_TIMER_CTRL_IT_MASK         (1 << 1)
#define ARCH_TIMER_CTRL_IT_STAT         (1 << 2)

#define CNTHCTL_EL1PCTEN                (1 << 0)
#define CNTHCTL_EL1PCEN                 (1 << 1)
#define CNTHCTL_EVNTEN                  (1 << 2)
#define CNTHCTL_EVNTDIR                 (1 << 3)
#define CNTHCTL_EVNTI                   (0xF << 4)

#define ARCH_TIMER_USR_PCT_ACCESS_EN    (1 << 0) /* physical counter */
#define ARCH_TIMER_USR_VCT_ACCESS_EN    (1 << 1) /* virtual counter */
#define ARCH_TIMER_VIRT_EVT_EN          (1 << 2)
#define ARCH_TIMER_EVT_TRIGGER_SHIFT    (4)
#define ARCH_TIMER_EVT_TRIGGER_MASK     (0xF << ARCH_TIMER_EVT_TRIGGER_SHIFT)
#define ARCH_TIMER_USR_VT_ACCESS_EN     (1 << 8) /* virtual timer registers */
#define ARCH_TIMER_USR_PT_ACCESS_EN     (1 << 9) /* physical timer registers */

#define ARCH_TIMER_EVT_STREAM_FREQ      10000 /* 100us */

/*
 * On ARMv5 and above those functions can be implemented around the
 * clz instruction for much better code efficiency.  __clz returns
 * the number of leading zeros, zero input will return 32, and
 * 0x80000000 will return 0.
 */
static inline unsigned int __clz(unsigned int x)
{
    unsigned int ret;

    asm("clz\t%0, %1" : "=r"(ret) : "r"(x));

    return ret;
}

/*
 * fls() returns zero if the input is zero, otherwise returns the bit
 * position of the last set bit, where the LSB is 1 and MSB is 32.
 */
static inline int fls(int x)
{
    return 32 - __clz(x);
}

static inline void arch_timer_set_cntfrq(u32 val)
{
    asm volatile("mcr p15, 0, %0, c14, c0, 0 @ set cntfreq" : : "r"(val) : "cc");
}

inline u32 arch_timer_get_cntfrq(void)
{
    u32 val;
    asm volatile("mrc p15, 0, %0, c14, c0, 0" : "=r"(val));
    return val;
}

inline u64 arch_counter_get_cntpct(void)
{
    u64 cval;

    isb();
    asm volatile("mrrc p15, 0, %Q0, %R0, c14" : "=r"(cval));
    return cval;
}

inline u64 arch_counter_get_cntvct(void)
{
    u64 cval;

    isb();
    asm volatile("mrrc p15, 1, %Q0, %R0, c14" : "=r"(cval));
    return cval;
}

static inline u32 arch_timer_get_cntkctl(void)
{
    u32 cntkctl;
    asm volatile("mrc p15, 0, %0, c14, c1, 0" : "=r"(cntkctl));
    return cntkctl;
}

static inline void arch_timer_set_cntkctl(u32 cntkctl)
{
    asm volatile("mcr p15, 0, %0, c14, c1, 0" : : "r"(cntkctl));
}

static void arch_timer_evtstrm_enable(int divider)
{
    u32 cntkctl = arch_timer_get_cntkctl();

    cntkctl &= ~ARCH_TIMER_EVT_TRIGGER_MASK;
    /* Set the divider and enable virtual event stream */
    cntkctl |= (divider << ARCH_TIMER_EVT_TRIGGER_SHIFT) | ARCH_TIMER_VIRT_EVT_EN;
    arch_timer_set_cntkctl(cntkctl);
}

static void arch_timer_configure_evtstream(void)
{
    int evt_stream_div, pos;

    /* Find the closest power of two to the divisor */
    evt_stream_div = CONFIG_COUNTER_FREQ / ARCH_TIMER_EVT_STREAM_FREQ;
    pos = fls(evt_stream_div);
    if (pos > 1 && !(evt_stream_div & (1 << (pos - 2))))
        pos--;
    /* enable event stream */
    if (pos > 15)
        pos = 15;
    arch_timer_evtstrm_enable(pos);
}

static void arch_counter_set_user_access(void)
{
    u32 cntkctl = arch_timer_get_cntkctl();

    /* Disable user access to the timers and both counters */
	/* Also disable virtual event stream */
	cntkctl &= ~(ARCH_TIMER_USR_PT_ACCESS_EN
			| ARCH_TIMER_USR_VT_ACCESS_EN
		        | ARCH_TIMER_USR_VCT_ACCESS_EN
			| ARCH_TIMER_VIRT_EVT_EN
			| ARCH_TIMER_USR_PCT_ACCESS_EN);

	/*
	 * Enable user access to the virtual counter if it doesn't
	 * need to be workaround. The vdso may have been already
	 * disabled though.
	 */
    cntkctl |= ARCH_TIMER_USR_VCT_ACCESS_EN | ARCH_TIMER_USR_PCT_ACCESS_EN;

    arch_timer_set_cntkctl(cntkctl);
}

int arch_timer_starting_cpu(void)
{
    if (arch_timer_init == 1)
        return 0;

    arch_timer_init = 1;
#if 0
	struct clock_event_device *clk = this_cpu_ptr(arch_timer_evt);
	u32 flags;

	__arch_timer_setup(ARCH_TIMER_TYPE_CP15, clk);

	flags = check_ppi_trigger(arch_timer_ppi[arch_timer_uses_ppi]);
	enable_percpu_irq(arch_timer_ppi[arch_timer_uses_ppi], flags);

	if (arch_timer_has_nonsecure_ppi()) {
		flags = check_ppi_trigger(arch_timer_ppi[ARCH_TIMER_PHYS_NONSECURE_PPI]);
		enable_percpu_irq(arch_timer_ppi[ARCH_TIMER_PHYS_NONSECURE_PPI],
				  flags);
	}
#endif
    if (arch_timer_get_cntfrq() == 0)
        arch_timer_set_cntfrq(CONFIG_COUNTER_FREQ);

    arch_counter_set_user_access();
    arch_timer_configure_evtstream();

    return 0;
}

// wwzz add new interface for archtimer cnt
u64 arch_counter_get_current(void)
{
    // fix: BP2016 use async clk to syscounter, the func arch_counter_get_cntpct 
    // might return incorrect data when read cp15 in special point of time,
    // discard low 4bit to reduce the accuracy(from 50ns to 800ns)
    // consecutive 3 times of read, if t0==t1, means it's the correct value
    // if not equal, means one of t0 and t1 is incorrect, then the t2 will be correct value
    // (The wrong data would not exist consecutively)
    u64 t0, t1, t2;
    t0 = arch_counter_get_cntpct()&0xFFFFFFFFFFFFFFF0;
    t1 = arch_counter_get_cntpct()&0xFFFFFFFFFFFFFFF0;
    t2 = arch_counter_get_cntpct()&0xFFFFFFFFFFFFFFF0;

    if(t0 == t1){
        return t0;
    }

    return t2;
}
