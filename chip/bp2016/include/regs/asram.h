#ifndef _REGS_ASRAM_H_
#define _REGS_ASRAM_H_

#define REG_AP_ASRAM_BASE          (APB0_BASE+0x10000)

typedef volatile struct
{
    REG32 cfg_r;        		   /* 0x00000000*/
    REG32 timing_r0;			   /* 0x00000004*/
    REG32 timing_r1;    		   /* 0x00000008*/
    REG32 sdram_ctrl_r;     	   /* 0x0000000c*/

    //external aes 128K, which only used by bp_firmware
    REG32 ref_interval_r;     	   /* 0x00000010*/
    REG32 cs_r0_low;         	   /* 0x00000014-base address for chip select 0*/
    REG32 cs_r1_low;         	   /* 0x00000018-base address for chip select 1*/
    REG32 cs_r2_low;         	   /* 0x0000001c-base address for chip select 2*/
    REG32 cs_r3_low;         	   /* 0x00000020-base address for chip select 3*/
    REG32 cs_r4_low;         	   /* 0x00000024-base address for chip select 4*/
    REG32 cs_r5_low;         	   /* 0x00000028-base address for chip select 5*/
    REG32 cs_r6_low;         	   /* 0x0000002c-base address for chip select 6*/
    REG32 cs_r7_low;         	   /* 0x00000030-base address for chip select 7*/
    REG32 rsv_34_50[8];       	   /* 0x34~0x50-reserved 8*/

    REG32 mask_r0;           	   /* 0x00000054*/
    REG32 mask_r1;           	   /* 0x00000058*/
    REG32 mask_r2;           	   /* 0x0000005c*/
    REG32 mask_r3;           	   /* 0x00000060*/
    REG32 mask_r4;           	   /* 0x00000064*/
    REG32 mask_r5;           	   /* 0x00000068*/
    REG32 mask_r6;           	   /* 0x0000006c*/
    REG32 mask_r7;           	   /* 0x00000070*/

    REG32 alias_s0;           	   /* 0x00000074*/
    REG32 alias_s1;           	   /* 0x00000078*/
    REG32 rsv_7c_80[2];       	   /* 0x7c~0x80-reserved 2*/

    REG32 remap_s0;           	   /* 0x00000084*/
    REG32 remap_s1;           	   /* 0x00000088*/
    REG32 rsv_8c_90[2];       	   /* 0x8c~0x90-reserved 2*/

    REG32 timing_r_set0;       	   /* 0x00000094*/
    REG32 timing_r_set1;       	   /* 0x00000098*/
    REG32 timing_r_set2;       	   /* 0x0000009c*/
    REG32 flash_trpd_timing_r; 	   /* 0x000000A0*/
    REG32 static_mem_ctrl_r;  	   /* 0x000000A4*/
    REG32 rsv_A8;       	       /* 0xA8-reserved*/

    REG32 extension_mode_r; 	   /* 0x000000AC*/
    REG32 rsv_B0_EC[16];       	   /* 0xB0~0xEC-reserved 12*/

    REG32 comp_para_cfg_r2; 	   /* 0x000000F0*/
    REG32 comp_para_cfg_r1; 	   /* 0x000000F4*/
    REG32 comp_ver;    	    	   /* 0x000000F8*/
    REG32 comp_type;        	   /* 0x000000FC*/
} HWP_ASRAM_T;

#define hwp_apAsram                ((HWP_ASRAM_T*) (REG_AP_ASRAM_BASE))

#endif

