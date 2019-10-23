#ifndef __FASTBOOT_H__
#define __FASTBOOT_H__
#include "common.h"

#define FASTBOOT_MAX_SIZE        (0x200000 - 0x10000)
#define FASTBOOT_FW_RSV_ADDR     (0x100000)

#define EXEC_ADDR_IN_DDR(addr)   (((addr) >= CONFIG_DRAM_START) && ((addr) < CONFIG_DRAM_START + CONFIG_DRAM_SIZE))

typedef volatile struct{
    REG32  en;  // user set, api fastboot_reset         --0x0
    REG32  type;  // copy or not                        --0x4
    REG32  src;  // copy src                            --0x8
    REG32  dst;  // copy dst                            --0xc
    REG32  len;  // copy len(bin len)                   --0x10
    REG32  jump_addr;  // bin exec addr                 --0x14
    REG32  flash_addr;  // bin store addr               --0x18
    REG32  bak;  // main or bak                         --0x1c
    REG32  boot_cnt;  // count of fastboot              --0x20
    REG32  rsv[0x33];  // rsv data                      --0x24~0xec

    REG32  cpy_pc_pos;  // record cpy run addr          --0xf0
    REG32  rom_load_flash;  // rom read flash or not    --0xf4
    REG32  bl_pc_pos;  // count of fastboot             --0xf8
    REG32  fastboot_api_invoke;  // api reset or not    --0xfc
} FASTBOOT_ST;

#define REG_FASTBOOT_BASE    (SCM_BASE + 0x1000)
#define hwp_fastboot         ((FASTBOOT_ST*) (REG_FASTBOOT_BASE))

#define  FASTBOOT_RESET          (0x55AA)
#define  FASTBOOT_EN             (0x5A5A)
#define  FASTBOOT_NOT_SUPPORT    (0xAAAA)
#define  FASTBOOT_COPY           (0xAA55)
#define  FASTBOOT_DIRECT         (0xA5A5)

void fast_boot(void);
void fastboot_save_para(U32 exec_addr, U32 bin_size, U32 flash_addr, U32 bak);
int fastboot_pll1_set(unsigned int val);

#endif //__FASTBOOT_H__
