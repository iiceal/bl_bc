#ifndef __DDR_H_20180718__
#define __DDR_H_20180718__
#include <common.h>

#define REG_READ_ctrl( _r_ )		(*(volatile unsigned int*)(_r_))
#define REG_WRITE_ctrl( _r_, _v_) 	((*(volatile unsigned int*)(_r_)) = (unsigned int)(_v_))

#ifdef CONFIG_ENABLE_TRACE
#define ddr_printf(fmt, args...)  printf("%s: %d " fmt, __func__, __LINE__, ##args)
#else
#define ddr_printf(fmt, args...)
#endif

void ddr_burst_write(u32 addr, u8 burst_len, u32 data);
void ddr_burst_read(u32 addr, u8 burst_len, u32 *buf);
int ddr_init(void);
int ddr3_init(void);
int lpddr_init(void);
int lpddr2_init(void);

void ddrc_init(void);
#endif //__DDR_H_20180718__
