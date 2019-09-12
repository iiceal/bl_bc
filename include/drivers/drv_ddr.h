#ifndef __DDR_H_20180718__
#define __DDR_H_20180718__
#include <common.h>

#define REG_READ_ctrl( _r_ )		(*(volatile unsigned int*)(_r_))
#define REG_WRITE_ctrl( _r_, _v_) 	((*(volatile unsigned int*)(_r_)) = (unsigned int)(_v_))

void ddr_burst_write(u32 addr, u8 burst_len, u32 data);
void ddr_burst_read(u32 addr, u8 burst_len, u32 *buf);
int ddr_init(void);
int ddr3_init(void);
int lpddr_init(void);
int lpddr2_init(void);

void ddrc_init(void);
#endif //__DDR_H_20180718__
