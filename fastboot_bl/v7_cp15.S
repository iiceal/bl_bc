
.macro	dcache_line_size, reg, tmp
    mrc	p15, 0, \tmp, c0, c0, 1		/* read ctr */
    lsr	\tmp, \tmp, #16
    and	\tmp, \tmp, #0xf		    /* cache line size encoding*/
    mov	\reg, #4		        	/* bytes per wor d*/
    mov	\reg, \reg, lsl \tmp		/* actual cache line size  */
.endm

.globl  v7_dma_inv_range
v7_dma_inv_range:
	dcache_line_size r2, r3
	sub	r3, r2, #1
	tst	r0, r3
	bic	r0, r0, r3
	mcrne	p15, 0, r0, c7, c14, 1		/* clean & invalidate D / U line */

	tst	r1, r3
	bic	r1, r1, r3
	mcrne	p15, 0, r1, c7, c14, 1		/* clean & invalidate D / U line */
1:
	mcr	p15, 0, r0, c7, c6, 1		    /*  invalidate D / U line */
	add	r0, r0, r2
	cmp	r0, r1
	blo	1b
	dsb	st
	mov pc,	lr

.type v7_dma_inv_range, %function; 

.globl  v7_dma_clean_range
v7_dma_clean_range:
	dcache_line_size r2, r3
	sub	r3, r2, #1
	bic	r0, r0, r3
1:
	mcr	p15, 0, r0, c7, c10, 1		  /* clean D / U line */
	add	r0, r0, r2
	cmp	r0, r1
	blo	1b
	dsb	st
	mov pc,	lr

.type v7_dma_clean_range, %function; 


.globl v7_dma_flush_range

v7_dma_flush_range:
	dcache_line_size r2, r3
	sub	r3, r2, #1
	bic	r0, r0, r3
1:
	mcr	p15, 0, r0, c7, c14, 1		/* clean & invalidate D / U line */
	add	r0, r0, r2
	cmp	r0, r1
	blo	1b
	dsb	st
	mov pc,	lr
.type v7_dma_flush_range, %function; 


.globl v7_inv_l1_l2_dcache
v7_inv_l1_l2_dcache:
        /* Invalidate Data/Unified Caches */
        
        MRC     p15, 1, r0, c0, c0, 1      /* Read CLIDR r0 = 0x0a200023 */
        ANDS    r3, r0, #0x07000000        /* Extract coherency level    */
        MOV     r3, r3, LSR #23            /* Total cache levels << 1    */
        BEQ     Finished                   /* If 0, no need to clean     */
    
        MOV     r10, #0                    /* R10 holds current cache level << 1 */
Loop1:
        ADD     r2, r10, r10, LSR #1       /* R2 holds cache "Set" position      */
        MOV     r1, r0, LSR r2             /* Bottom 3 bits are the Cache-type for this level */
        AND     r1, r1, #7                 /* Isolate those lower 3 bits */
        CMP     r1, #2
        BLT     Skip                       /* No cache or only instruction cache at this level */
        
        MCR     p15, 2, r10, c0, c0, 0     /* Write the Cache Size selection register */
        ISB                                /* ISB to sync the change to the CacheSizeID reg */
        MRC     p15, 1, r1, c0, c0, 0      /* Reads current Cache Size ID register*/
        AND     r2, r1, #7                 /* Extract the line length field */
        ADD     r2, r2, #4                 /* Add 4 for the line length offset (log2 16 bytes)*/
        LDR     r4, =0x3FF
        ANDS    r4, r4, r1, LSR #3         /* R4 is the max number on the way size (right aligned)*/
        CLZ     r5, r4                     /* R5 is the bit position of the way size increment */
        LDR     r7, =0x7FFF
        ANDS    r7, r7, r1, LSR #13        /* R7 is the max number of the index size (right aligned) */

Loop2:
        MOV     r9, r4                     /* R9 working copy of the max way size (right aligned)*/

Loop3:
        ORR     r11, r10, r9, LSL r5       /* Factor in the Way number and cache number into R11*/
        ORR     r11, r11, r7, LSL r2       /* Factor in the Set number */
        MCR     p15, 0, r11, c7, c6, 2     /* Invalidate by Set/Way */
        SUBS    r9, r9, #1                 /* Decrement the Way number */
        BGE     Loop3
        SUBS    r7, r7, #1                 /* Decrement the Set number */
        BGE     Loop2
Skip:
        ADD     r10, r10, #2               /* increment the cache number */
        CMP     r3, r10
        BGT     Loop1

Finished:
        mov     pc, lr

.type v7_inv_l1_l2_dcache, %function

