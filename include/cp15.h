#ifndef __CP15__H__
#define	__CP15__H__

#define	dsb() asm volatile("dsb" : : : "memory");
#define	dmb() asm volatile("dmb" : : : "memory");
#define	isb() asm volatile("isb" : : : "memory");

extern void mmu_setup(void);
extern BOOL is_mmu_enable(void);

extern void v7_dma_inv_range(U32, U32);
extern void v7_dma_clean_range(U32, U32);
extern void v7_dma_flush_range(U32, U32);

// init func for timer in cpu
extern int arch_timer_starting_cpu(void);
extern inline u64 arch_counter_get_cntpct(void);

#endif
