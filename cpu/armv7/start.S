#include <chip_config.h>

.globl _start
_start: 
	b	_reset_handler
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq
_undefined_instruction: .word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word data_abort
_irq:			.word irq
_fiq:			.word fiq


	.balignl 16,0xdeadbeef
/*************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************/

.globl _TEXT_BASE
_TEXT_BASE:
	.word _start

/*
 * the actual reset code
 */

_reset_handler:
	/*
	 * set the cpu to SYS32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f // clear CPSR.M
	orr	r0,r0,#0xd3 // set  SVC(0x13) and disable IRQ+FIQ
//	orr	r0,r0,#0xdf // set  SYS(0x1F) and disable IRQ+FIQ
	msr	cpsr,r0

	/* the mask ROM code should have PLL and others stable */
	bl	cpu_init_cp15
	bl	cpu_smp_init

copy_data:
	ldr	r0, =__code_end__	/* src */
	ldr	r1, =__data_start	/* dst */
	ldr	r2, =__data_end__	/* dst end */

cpdata_1:				/* block copy function */
	cmp	r1, r2
	ldrlo	r3, [r0], #4
	strlo	r3, [r1], #4
	blo	cpdata_1

	mov	r2, #0x00000000		/* clear			    */
	mov	r3,r2
	mov	r4,r2
	mov	r5,r2
	mov	r6,r2
	mov	r7,r2
	mov	r8,r2
	mov	r9,r2
	mov	r10,r2
	mov	r11,r2
	mov	r12,r2

clear_bss:
	ldr	r0, =__bss_start
	ldr	r1, =__bss_end__
clbss_l:
	cmp	r0, r1
	strlo	r2, [r0], #4
	blo	clbss_l


/* Set stackpointer in internal RAM to call board_init_f */
call_board_init_f:
	ldr	sp, =(CONFIG_SYS_INIT_SP_ADDR)
	bic	sp, sp, #7 /* 8-byte alignment for ABI compliance */
	ldr	r0, =0x00000000
	bl	boot_main

/*------------------------------------------------------------------------------*/

/*************************************************************************
 *
 * cpu_init_cp15
 *
 * Setup CP15 registers (cache, MMU, TLBs, btb). 
 *
 *************************************************************************/
cpu_init_cp15:
	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	bic	r0, r0, #0x00001800	@ clear bit 11(Z---) BTB, bit 12 (I) I-cache 
	orr	r0, r0, #0x00001000	@ set bit bit 12 I-Cache
	orr	r0, r0, #0x00000002	@ set bit 1 (--A-) Align
	mcr	p15, 0, r0, c1, c0, 0
    /*
	 * Enable actlr.SMP so we can use MMU and D$
	 */
	mrc	p15, 0, r0, c1, c0, 1
	tst	r0, #(1 << 6)		@ SMP/nAMP mode enabled?
	orreq	r0, r0, #(1 << 6)	@ Enable SMP/nAMP mode
	mcreq	p15, 0, r0, c1, c0, 1

    isb
    dsb

    /*
	 * Invalidate L1 I/D
	 */
	mov	r0, #0			@ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
	mcr	p15, 0, r0, c7, c5, 6	@ invalidate BP array
	mcr p15, 0, r0, c7, c10, 4	@ DSB
	mcr p15, 0, r0, c7, c5, 4	@ ISB

    mov sp, lr
    bl    v7_inv_l1_l2_dcache
    mov lr, sp

	mrc p15, 0, r0, c1, c0, 0   @ Read CP15 SCTLR Register
	bic r0, #(1 << 13) 	/* V = 1 */
	mcr p15, 0, r0, c1, c0, 0   @ Write CP15 SCTLR Register

	/* Set vector address in CP15 VBAR register */
	ldr r0, =_start
	mcr p15, 0, r0, c12, c0, 0  @Set VBAR

	@Enable VFP / NEON.
   	mrc p15, 0, r0, c1, c0, 2 @ Read CP Access register
   	orr r0, r0, #0x00f00000   @ Enable full access to NEON/VFP (Coprocessors 10 and 11)
   	mcr p15, 0, r0, c1, c0, 2 @ Write CP Access register
   	isb
   	mov r0, #0x40000000       @ Switch on the VFP and NEON hardware
   	vmsr fpexc, r0            @ Set EN bit in FPEXC
	isb


	mov	pc, lr			@ back to my caller


/*************************************************************************
 *
 * CPU_smp_init
 *
 * Register usage:
 *   - R0-R3 - temp / arguments
 *   - R4    - CPU number
 *   - R5    - Jump address valid magic number
 *   - R6    - Wakeup address/flag structure for current CPU
 *************************************************************************/
cpu_smp_init:
	mrc	p15, 0, r4, c0, c0, 5	/* get cpu id */
	and	r4, r4, #0xf

	cmp	r4, #0			/* check if PRIMARY */
	beq	primary_check_wakeup_jump

/*************************************************************************
 *
 * Initialize Secondary CPU, and enter wfe state
 *
 *************************************************************************/
check_jump_s:
	wfe
	b	check_jump_s

/*************************************************************************
 *
 * Initialize Primary CPU
 *   - Check Wakeup Flag and execute if valid
 *
 *************************************************************************/
primary_check_wakeup_jump:
	mov	pc, lr		/* return if magic doesn't match */


/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */
@
@ IRQ stack frame.
@
#define S_FRAME_SIZE	72

#define S_OLD_R0	68
#define S_PSR		64
#define S_PC		60
#define S_LR		56
#define S_SP		52

#define S_IP		48
#define S_FP		44
#define S_R10		40
#define S_R9		36
#define S_R8		32
#define S_R7		28
#define S_R6		24
#define S_R5		20
#define S_R4		16
#define S_R3		12
#define S_R2		8
#define S_R1		4
#define S_R0		0

#define MODE_SVC 0x13
#define I_BIT	 0x80


	.macro	save_exception_regs exception
	ldr	sp, =(CONFIG_EXCEPTION_STACK_START-S_FRAME_SIZE) @ setup our mode stack
	stmia	sp, {r0-r12}			@ Save user registers
	mov     r1,#\exception			@ Save exception number in R1
	.endm

/*
 * exception handlers
 */
undefined_instruction:
	save_exception_regs 1
	b	common_exception_handler

software_interrupt:
	save_exception_regs 2
	b	common_exception_handler

prefetch_abort:
	save_exception_regs 3
	b	common_exception_handler

data_abort:
	save_exception_regs 4

	// fall through...
common_exception_handler:
	add	r8, sp, #S_PC
	stmdb	r8, {sp, lr}^			@ System mode SP, LR
	str	lr, [r8, #0]			@ Save calling PC
	mrs	r6, spsr
	str	r6, [r8, #4]			@ Save CPSR

	// R0: saved stack frame, R1: exception vector number
	mov     r0, sp
	b	do_exception

fiq:
	save_exception_regs 7
	b	common_exception_handler

#ifndef CONFIG_USE_IRQ

irq:
	save_exception_regs 6
	b	common_exception_handler

#else
/*
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */

	.macro	irq_save_user_regs stack
	ldr	sp, =(\stack - S_FRAME_SIZE)
	stmia	sp, {r0-r3, r12}		@ Save r0-r3, r12
	str	lr, [sp, #S_PC]			@ Save calling PC
	mrs	r0, spsr
	str	r0, [sp, #S_PSR]		@ Save CPSR
	.endm


	.macro	irq_restore_user_regs
	ldmia	sp, {r0-r3, r12}		@ Calling r0-r3, r12
	ldr	lr, [sp, #S_PC]			@ Get PC
	subs	pc, lr, #4			@ return
	.endm

irq:
	irq_save_user_regs CONFIG_IRQ_STACK_START
	bl	do_irq
	irq_restore_user_regs
#endif /* CONFIG_USE_IRQ */
