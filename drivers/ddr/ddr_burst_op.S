.globl  ddr_burst_write
ddr_burst_write:
    push {r4-r12}

    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
    mov r9, r2
    mov r10, r2
    mov r11, r2
    mov r12, r2

    stmia r0, {r3-r12}

    pop {r4-r12}
	mov pc,	lr

.type ddr_burst_write, %function; 

.globl  ddr_burst_read
ddr_burst_read:
    push {r4-r12}
    ldmia  r0, {r3-r12}
    stmia r2, {r3-r12}
    pop {r4-r12}
	mov pc,	lr

.type ddr_burst_read, %function; 


