#include "common.h"

/*use arch timer*/
u32 timer_get_tick32(void)
{
    return (u32)arch_counter_get_cntpct();
}

u64 timer_get_tick(void)
{
    return (u64)arch_counter_get_cntpct();
}

void timeout_setup_ms(TIMER *timer, u32 ms)
{
	timer->timer = timer_get_tick() + ms_to_tick((u64)ms);
}

void timeout_setup_us(TIMER *timer, u32 us)
{
	timer->timer = timer_get_tick() + us_to_tick((u64)us);
}

BOOL timeout_check(TIMER *timer)
{
	return (timer_get_tick() > timer->timer);
}

void mdelay(u32 ms)
{
	TIMER timer;

	timeout_setup_ms(&timer, ms);
	while(!timeout_check(&timer))
		;
}

void udelay(u32 us)
{
	TIMER timer;

	timeout_setup_us(&timer, us);
	while(!timeout_check(&timer))
		;
}
