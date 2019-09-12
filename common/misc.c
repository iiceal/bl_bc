#include <common.h>

static u32 randomnum = 1;
#define RESET_SEED(s)  randomnum = (s)
#define a 16807         /* multiplier */
#define m 0xFFFFFFFFL   /* 2**31 - 1 */
#define q 127773L       /* m div a */
#define r 2836          /* m mod a */

void set_random(void)
{
    randomnum = arch_counter_get_cntpct();
}

static u32 hg_rand_imp(const u32 seed)
{      
	u32 lo, hi;      
	lo = a * (long)(seed & 0xFFFF);     
	hi = a * (long)((u32)seed >> 16);      
	lo += (hi & 0xFFFF) << 16;     
	if (lo > m)      
	{            
		lo &= m;           
		++lo;      
	}      
	lo += hi >> 16;      
	if (lo > m)      
	{            
		lo &= m;            
		++lo;      
	}      
	return (u32)lo;
}

u32 hg_pseudo_random_sequence(void)
{
	randomnum = hg_rand_imp(randomnum);      
	return randomnum;
}

void printf_buf_fmt_32bit(u32 * buf, u32 addr, int len)
{
	int i = 0;
	for(i = 0; i < len; i++) {
		if(i % 4 == 0) 
			printf("\r\n%08x: ", addr + i * 4);
		printf("%08x ", *buf ++); 
	}
	printf("\r\n");
}

void printf_buf(u8 * buf,  int len)
{
	int i = 0;
	for(i = 0; i < len; i++) {
		if(i%16 == 0)
			printf("\r\n%08x: ", i);
		printf("%02x ", *buf ++);
	}
	printf("\r\n");
}
