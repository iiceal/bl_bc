#ifndef _REGS_EFUSE_H_
#define _REGS_EFUSE_H_

#define REG_AP_EFUSE0_BASE          (APB2_BASE+0xd00b0)

typedef volatile struct
{
    REG32 chipid;     		   /* 0x00000000*/
    REG32 sn;			   /* 0x00000004*/
    REG32 se;    		   /* 0x00000008*/
    REG32 pll;    		   /* 0x0000000c*/

    //external aes 128K, which only used by bp_firmware
    REG32 aes_key_d0;     	   /* 0x00000010*/
    REG32 aes_key_d1;     	   /* 0x00000014*/
    REG32 aes_key_d2;     	   /* 0x00000018*/
    REG32 aes_key_d3;     	   /* 0x0000001c*/
} HWP_EFUSE_T;


#define hwp_apEfuse                ((HWP_EFUSE_T*) (REG_AP_EFUSE0_BASE))

//se

#define		SE_BB_JTAG_DISABLE		(1 << 0) 	
#define		SE_AP_JTAG_DISABLE		(1 << 1) 	
#define		SE_BL_VERF_DISABLE		(1 << 2) 	
#define		SE_FW_VERF_DISABLE		(1 << 3) 

#define		SE_KEY_INDEX(n)			((n & 0xf) << 4) 
#define		SE_KEY_INDEX_MASK		(0xf << 4) 
#define		SE_KEY_INDEX_SHIFT		(4) 

#define		SE_AP_KEY_ENABLE		(1 << 8) 
#define		SE_TRACE_DISABLE		(1 << 9) 


#endif

