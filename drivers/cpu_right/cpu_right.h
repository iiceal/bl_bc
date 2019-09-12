#ifndef _CPU_RIGHT_H_
#define _CPU_RIGHT_H_

#include "common.h"
#include "regs/scm.h"
#include "regs/right.h"

#define RAM_ROM_ACCESS_ALIGN                      (0x40)    // must be integer times of 2
#define RAM_ROM_ACCESS_ALIGN_SHIFT                (0x6)

#define CPU1_ROM_START                            (0xFC00)      // 60K
#define CPU1_ROM_END                              (0x10000)     // 64K
#define CPU1_RAM_START                            (0x200000)    // 2M
#define CPU1_RAM_END                              (0x300000)    // 3M
#define CPU1_QSPI_START                           (_SPIFLASH_D_BASE + 0xb00000)   // 16M
#define CPU1_QSPI_END                             (_SPIFLASH_D_BASE + 0x1000000)   // 32M
#define CPU1_DDR_START                            (0x60000000)  // 
#define CPU1_DDR_END                              (0xa0000000)  // 

#define ROM_ADDR_VALID(x)                         ((CONFIG_ROM_START <= (x))\
                                                   && (CONFIG_ROM_START + CONFIG_ROM_SIZE >= (x)))
#define SRAM_ADDR_VALID(x)                        ((CONFIG_SRAM_START <= (x))\
                                                   && (CONFIG_SRAM_START + CONFIG_SRAM_SIZE >= (x)))
#define QSPI_ADDR_VALID(x)                        ((SPIFLASH_D_BASE <= (x))\
                                                   && (SPIFLASH_D_BASE + 0x1000000 >= (x)))
#define DDR_ADDR_VALID(x)                         ((CONFIG_DRAM_START <= (x))\
                                                   && (CONFIG_DRAM_START + CONFIG_DRAM_SIZE >= (x)))

#endif
