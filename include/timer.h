#ifndef _SYS_TIMER_H_
#define _SYS_TIMER_H_

#include "chip_config.h"
#include "hg_types.h"

typedef union
{
    u64 timer;
    struct
    {
        u32 timer_l     :32;
        u32 timer_h     :32;
    } fields;
} TIMER;

#define tick_to_second(tick)	((tick)/TICKS_PER_S) 
#define second_to_tick(second)	((second) * TICKS_PER_S)

#define tick_to_ms(tick)	((tick)/TICKS_PER_MS) 
#define ms_to_tick(ms)		((ms) * TICKS_PER_MS)

#define tick_to_us(tick)	((tick)/TICKS_PER_US)
#define us_to_tick(us)		((us) * TICKS_PER_US)

u64 timer_get_tick(void);
u32 timer_get_tick32(void);

void timeout_setup_ms(TIMER *timer, u32 ms);
void timeout_setup_us(TIMER *timer, u32 us);
BOOL timeout_check(TIMER *timer);

void mdelay(u32 ms);
void udelay(u32 us);

extern int arch_timer_starting_cpu(void);
extern inline u64 arch_counter_get_cntpct(void);
extern u64 arch_counter_get_current(void);


#endif /* _SYS_TIMER_H_ */
