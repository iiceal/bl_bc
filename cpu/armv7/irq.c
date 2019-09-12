#include "common.h"
#include "system_arm.h"
#include "gic400.h"
#include "irq.h"
#include "regs/scm.h"

#ifdef CONFIG_EXCEPTION_TEST
#define CONFIG_VERBOSE_EXCEPTIONS
#endif

static void bad_mode(void)
{
    serial_puts("Hang ...\n");
    while (1)
        ;
}

static void show_regs (const struct pt_regs *regs, const char *exception_str)
{
    serial_puts(exception_str);

#ifdef CONFIG_VERBOSE_EXCEPTIONS
    unsigned long flags;
#if 0
	const char *processor_modes[] = {
	"USER_26",	"FIQ_26",	"IRQ_26",	"SVC_26",
	"UK4_26",	"UK5_26",	"UK6_26",	"UK7_26",
	"UK8_26",	"UK9_26",	"UK10_26",	"UK11_26",
	"UK12_26",	"UK13_26",	"UK14_26",	"UK15_26",
	"USER_32",	"FIQ_32",	"IRQ_32",	"SVC_32",
	"UK4_32",	"UK5_32",	"UK6_32",	"ABT_32",
	"UK8_32",	"UK9_32",	"UK10_32",	"UND_32",
	"UK12_32",	"UK13_32",	"UK14_32",	"SYS_32",
	};
#endif
    flags = regs->ARM_cpsr;

    printf ("pc : %8x  lr : %8x  sp : %8x  r12: %8x\n",
            regs->ARM_pc, regs->ARM_lr,
            regs->ARM_sp, regs->ARM_ip);
    printf ("fp : %8x  r10: %8x  r9 : %8x  r8 : %8x\n",
            regs->ARM_fp, regs->ARM_r10, regs->ARM_r9, regs->ARM_r8);
    printf ("r7 : %8x  r6 : %8x  r5 : %8x  r4 : %8x\n",
            regs->ARM_r7, regs->ARM_r6, regs->ARM_r5, regs->ARM_r4);
    printf ("r3 : %8x  r2 : %8x  r1 : %8x  r0 : %8x\n",
            regs->ARM_r3, regs->ARM_r2, regs->ARM_r1, regs->ARM_r0);
    printf ("Flags: %c%c%c%c%c\n",
            flags & CC_N_BIT ? 'N' : 'n',
            flags & CC_Z_BIT ? 'Z' : 'z',
            flags & CC_C_BIT ? 'C' : 'c',
            flags & CC_V_BIT ? 'V' : 'v',
            flags & T_BIT    ? 'T' : 'A');
#if 1
    printf ("IRQ:%s FIQ:%s Mode:%2x\n",
            flags & I_BIT ? "off" : "on",
            flags & F_BIT ? "off" : "on",
            flags & MODE_MASK);
#endif
#endif
}

void cpu_abort_info(void)
{
#define MODE_MONITOR 0x16
    unsigned int mode;
    unsigned int world;
    unsigned int dfar;
    unsigned int dfsr;
    unsigned int ifar;
    unsigned int ifsr;

    __asm__ __volatile__("mrs %0, cpsr\n"    /* save mode */
                         "mrc p15, 0, %1, c1, c1, 0\n"
                         "orr %1, %1, #1\n"    /* set NS bit in SCR */
                         "cpsid aif, %6\n"    /* To monitor mode, disable interrupts */
                         "mcr p15, 0, %1, c1, c1, 0\n"
                         "mrc p15, 0, %2, c6, c0, 0\n"
                         "mrc p15, 0, %3, c5, c0, 0\n"
                         "mrc p15, 0, %4, c6, c0, 2\n"
                         "mrc p15, 0, %5, c5, c0, 1\n"
                         "bic %1, %1, #1\n"    /* clear NS bit in SCR */
                         "mcr p15, 0, %1, c1, c1, 0\n"
                         "isb\n"
                         "msr cpsr, %0\n"
                         : "=&r" (mode), "=&r" (world),
                           "=r" (dfar), "=r" (dfsr),
                           "=r" (ifar), "=r" (ifsr)
                         : "I" (MODE_MONITOR));
    printf("DFAR:0x%x, DFSR:0x%x, IFAR:0x%x, IFSR:0x%x\n", dfar, dfsr, ifar, ifsr);
}

void do_exception (struct pt_regs *pt_regs, int vector_number)
{
    const char *exception_name[] = {
        "reset",                    // 0
        "undefined instruction\n",  // 1
        "software interrupt\n",     // 2
        "prefetch abort\n",         // 3
        "data abort\n",             // 4
        "not used\n",               // 5
        "IRQ\n",                    // 6
        "FIQ\n",                    // 7
    };
    show_regs(pt_regs, exception_name[vector_number]);
    if ((3 == vector_number) || (4 == vector_number)) {
        cpu_abort_info();
    }
    bad_mode();
}

#ifdef CONFIG_USE_IRQ
/* enable IRQ interrupts */
void enable_interrupts (void)
{
    unsigned long temp;
    __asm__ __volatile__("mrs %0, cpsr\n"
			     "bic %0, %0, #0x80\n"
			     "msr cpsr_c, %0"
			     : "=r" (temp)
			     :
			     : "memory");
}

/*
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
int disable_interrupts (void)
{
    unsigned long old,temp;
    __asm__ __volatile__("mrs %0, cpsr\n"
			     "orr %1, %0, #0xc0\n"
			     "msr cpsr_c, %1"
			     : "=r" (old), "=r" (temp)
			     :
			     : "memory");
	return (old & 0x80) == 0;
}

int arch_interrupt_init(void)
{
    gic_init();
    enable_interrupts();
    return 0;
}

int interrupt_init (void)
{
	unsigned long cpsr, irq_stack_start;

	/*
	 * setup up stacks if necessary
	 */
	irq_stack_start = CONFIG_IRQ_STACK_START;

	__asm__ __volatile__("mrs %0, cpsr\n"
			     : "=r" (cpsr)
			     :
			     : "memory");

	__asm__ __volatile__("msr cpsr_c, %0\n"
			     "mov sp, %1\n"
			     :
			     : "r" (IRQ_MODE | I_BIT | F_BIT | (cpsr & ~FIQ_MODE)),
			       "r" (irq_stack_start)
			     : "memory");

	__asm__ __volatile__("msr cpsr_c, %0"
			     :
			     : "r" (cpsr)
			     : "memory");

	return arch_interrupt_init();
}

void do_fiq(void)
{
}

void do_irq(void)
{
    gic_handle_irq();
}
#endif

#ifdef CONFIG_EXCEPTION_TEST
void trigger_swi(void)
{
    __asm__ __volatile__("swi #4\n" ::: "memory");
}

void trigger_undefined(void)
{
    __asm__ __volatile__(".inst 0xdeff\n" ::: "memory");
}

void trigger_prefetch(void)
{
    __asm__ __volatile__("bkpt #3\n" ::: "memory");
}

long pointer_exception(long *p)
{
    return *p = 10;
}
#endif /* CONFIG_EXCEPTION_TEST */
