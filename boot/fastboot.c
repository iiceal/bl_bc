#include "common.h"
#include "fastboot.h"
#include "regs/scm.h"
#include <pll.h>

#ifdef CONFIG_ENABLE_TRACE
#define fastboot_debug(fmt, args...)  printf("%s: %d " fmt, __func__, __LINE__, ##args)
#else
#define fastboot_debug(fmt, args...)
#endif

#if 0
void fastboot_get_para(FASTBOOT_ST *st)
{
#if 0
    st->en = FASTBOOT_EN;
    st->type = FASTBOOT_COPY;
    st->src = 0x100000;
    st->dst = 0x20000000;
    st->len = 0x200000;
    st->jump_addr = 0x20000000;
#else
    st->en = hwp_fastboot->en;
    st->type = hwp_fastboot->type;
    st->src = hwp_fastboot->src;
    st->dst = hwp_fastboot->dst;
    st->len = hwp_fastboot->len;
    st->jump_addr = hwp_fastboot->jump_addr;
#endif
}
#endif

extern unsigned long do_go_exec (unsigned long (*entry)(int), unsigned size);
extern void v7_dma_inv_range(U32, U32);
extern void v7_dma_clean_range(U32, U32);
extern void v7_dma_flush_range(u32, u32);

#define ROM_BOOT_LOAD_FLASH_MAGIC           0xfabbe000
#define FASTBOOT_API_INVOKED_MAGIC          0xfabbe000
#define FW_FASTBOOT_MAGIC                   0xfabbe000

void fast_boot(void)
{
    U32 cpy_src = 0, cpy_dst = 0, cpy_len = 0, boot_type, boot_en, addr;

    boot_en = hwp_fastboot->en;

    // check fastboot flag
    if((boot_en != FASTBOOT_EN) || (hwp_apSCM->sw_bp_jaddr == 0) || (ROM_BOOT_LOAD_FLASH_MAGIC == hwp_fastboot->rom_load_flash)) {
        //printf("\nen(0x%x)not support...\n", boot_en);
        return ;
    }

    boot_type = hwp_fastboot->type;
    cpy_src = hwp_fastboot->src;
    cpy_dst = hwp_fastboot->dst;
    cpy_len = hwp_fastboot->len;
    addr = hwp_fastboot->jump_addr;

    // check support or not
    if(boot_type == FASTBOOT_NOT_SUPPORT){
        //printf("type(0x%x)not support...\n", boot_type);
        *(volatile U32 *)0xc0000000 = 'N';
        *(volatile U32 *)0xc0000000 = 0x0a;
        *(volatile U32 *)0xc0000000 = 0x0d;
        return ;
    }

    // memcpy codes to dst ddr and go
    if(boot_type == FASTBOOT_COPY){
        //printf("\ntype(0x%x) copy ...\n", boot_type);
        *(volatile U32 *)0xc0000000 = 'C';
        *(volatile U32 *)0xc0000000 = 0x0a;
        *(volatile U32 *)0xc0000000 = 0x0d;
        v7_dma_inv_range(cpy_src, cpy_src + cpy_len);
        memcpy((void *)cpy_dst, (void *)cpy_src, cpy_len);
        v7_dma_clean_range(cpy_dst, cpy_dst + cpy_len);
        //printf("cpy and clean done, ready to jump\n\n");
    }

    //printf("ready to go ...\n", boot_type);
    hwp_fastboot->boot_cnt++;
    hwp_apSCM->sw_bp_magic = FW_FASTBOOT_MAGIC;
    hwp_apSCM->sw_bp_jaddr = CONFIG_SRAM_BL_START;
    do_go_exec((void *)addr, 0);
    //printf("jump err, return~~~\n");

    while(1){
        printf("jump err, return~~~\n");
        ;
    }
}

int fastboot_pll1_set(unsigned int val)
{
    int ret;
    // pll1 set flag, use sw_ap_magic area to store
    // this flag reset to 0 only when power, watchdog and glob reset
    if(hwp_apSCM->sw_ap_magic == 0xfabbe000){
        return RET_OK;
    }
    
    ret = pll1_set_rate_kHz(val);
    hwp_apSCM->sw_ap_magic = 0xfabbe000;

    return ret;
}

void fastboot_save_para(U32 exec_addr, U32 bin_size, U32 flash_addr, U32 bak)
{
    hwp_fastboot->en = FASTBOOT_EN;
    hwp_fastboot->bak = bak;

    if(bin_size>= FASTBOOT_MAX_SIZE){
        hwp_fastboot->type = FASTBOOT_NOT_SUPPORT;
        return;
    }

    if(EXEC_ADDR_IN_DDR(exec_addr)){
        // copy data to iram and flush.
        memcpy((void *)FASTBOOT_FW_RSV_ADDR, (void *)exec_addr, bin_size);
        v7_dma_flush_range(FASTBOOT_FW_RSV_ADDR, FASTBOOT_FW_RSV_ADDR + bin_size);
        hwp_fastboot->type = FASTBOOT_COPY;
        hwp_fastboot->src = FASTBOOT_FW_RSV_ADDR;
        hwp_fastboot->dst = exec_addr;
        hwp_fastboot->len = bin_size;        
    }
    hwp_fastboot->jump_addr = exec_addr;
    hwp_apSCM->sw_bp_magic = FW_FASTBOOT_MAGIC;
    hwp_fastboot->rom_load_flash = 0x0;
    hwp_apSCM->sw_bp_jaddr = CONFIG_SRAM_BL_START;

    return;
}

