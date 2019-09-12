#ifndef __MISC_H__
#define __MISC_H__

void set_random(void);
u32 hg_pseudo_random_sequence(void);
void printf_buf_fmt_32bit(u32 * buf, u32 addr, int len);
void printf_buf(u8 * buf,  int len);

#endif //__MISC_H__
