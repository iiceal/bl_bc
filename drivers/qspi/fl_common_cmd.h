#ifndef __FL_WINBOND_GD_H__
#define __FL_WINBOND_GD_H__

/*
 * Note on opcode nomenclature: some opcodes have a format like
 * SPINOR_OP_FUNCTION{4,}_x_y_z. The numbers x, y, and z stand for the number
 * of I/O lines used for the opcode, address, and data (respectively). The
 * FUNCTION has an optional suffix of '4', to represent an opcode which
 * requires a 4-byte (32-bit) address.
 */

/* 4-byte address opcodes - used on Spansion and some Macronix flashes. */
#define SPINOR_OP_READ4		0x13	/* Read data bytes (low frequency) */
#define SPINOR_OP_READ4_FAST	0x0c	/* Read data bytes (high frequency) */
#define SPINOR_OP_READ4_1_1_2	0x3c	/* Read data bytes (Dual SPI) */
#define SPINOR_OP_READ4_1_1_4	0x6c	/* Read data bytes (Quad SPI) */
#define SPINOR_OP_PP_4B		0x12	/* Page program (up to 256 bytes) */
#define SPINOR_OP_SE_4B		0xdc	/* Sector erase (usually 64KiB) */

/* Used for SST flashes only. */
#define SPINOR_OP_BP		0x02	/* Byte program */
#define SPINOR_OP_WRDI		0x04	/* Write disable */
#define SPINOR_OP_AAI_WP	0xad	/* Auto address increment word program */

/* Used for Macronix and Winbond flashes. */
#define SPINOR_OP_EN4B		0xb7	/* Enter 4-byte mode */
#define SPINOR_OP_EX4B		0xe9	/* Exit 4-byte mode */

/* Used for Spansion flashes only. */
#define SPINOR_OP_BRWR		0x17	/* Bank register write */
#define SPINOR_OP_BRRD		0x16	/* Bank register read*/
#define	SPINOR_OP_BR_BA25	0x02    /* Bank register: Address 25-bit enable*/
#define	SPINOR_OP_BR_BA24	0x01    /* Bank register: Address 24-bit enable*/


/* Status Register bits. */
#define SR_WIP			1	/* Write in progress */
#define SR_WEL			2	/* Write enable latch */
/* meaning of other SR_* bits may differ between vendors */
#define SR_BP0			4	/* Block protect 0 */
#define SR_BP1			8	/* Block protect 1 */
#define SR_BP2			0x10	/* Block protect 2 */
#define SR_SRWD			0x80	/* SR write protect */

#define SR_QUAD_EN_MX		0x40	/* Macronix Quad I/O */

/* Flag Status Register bits */
#define FSR_READY		0x80

/* Configuration Register bits. */
#define CR_QUAD_EN_SPAN		0x2	/* Spansion Quad I/O */
#define CR_BPNV_EN_SPAN		0x8	/* Spansion BPNV I/O: 1 = BP2-0 volatile*/
#define CR_TBPROT_EN_SPAN	0x20	/* Spansion TBProt I/O: 1=low address */

#define SR_WIP_SPAN		0x0	/* Spansion Quad I/O */
#define SR_E_ERR_SPAN		0x20	/* Spansion BPNV I/O: 1 = BP2-0 volatile*/
#define SR_P_ERR_SPAN		0x40	/* Spansion TBProt I/O: 1=low address */
#define	SR_BP_BITS_MASK		0x1C
#define	SR_BP_BITS_SHIFT(v)	(v << 2)



#define SPINOR_OP_BE_64K 0xd8
#define SPINOR_OP_BE_32K 0x52
#define SPINOR_OP_BE_SECTOR 0x20
#define SPINOR_OP_CHIP_ERASE 0xc7
#define SPINOR_OP_BE_MAX_MS (2000)
#define SPINOR_OP_CE_MAX_MS (400 * 1000)
#define SPINOR_OP_PP_MAX_MS (500)

#define SPINOR_OP_WR_EXTADDR 0xc5
#define SPINOR_OP_RD_EXTADDR 0xc8
#define SPINOR_OP_EXTR_A24 0x01
#define SPINOR_OP_EXTR_A25 0x02
#define SPINOR_OP_EXTR_A26 0x04

#define SPINOR_OP_RDSR1 0x05
#define SPINOR_OP_RDSR2 0x35
#define SPINOR_OP_RDSR3 0x15

#define SPINOR_OP_WRSR1 0x01
#define SPINOR_OP_WRSR2 0x31
#define SPINOR_OP_WRSR3 0x11

#define WPS_BLOCK_LOCK			(0x36)
#define WPS_BLOCK_UNLOCK		(0x39)
#define WPS_BLOCK_READ_LOCK		(0x3d)
#define WPS_GLOBAL_BLOCK_LOCK	(0x7e)
#define WPS_GLOBAL_BLOCK_UNLOCK	(0x98)

#define SPINOR_OP_DEEP_POWER_DOWN	(0xb9)

#define BLOCK_PROTECTED     (1)
#define BLOCK_UNPROTECTED   (0)

#endif //__FL_WINBOND_GD_H__
