#include <common.h>

extern char input_data_bl[];
extern char input_data_bl_end[];

extern int chip_early_init_f(void);
extern void early_system_init(void);
extern void v7_dma_flush_range(u32, u32);
extern unsigned long do_go_exec (unsigned long (*entry)(int), unsigned size);

#ifdef CPBL_DEBUG
#define cpbl_debug(fmt, args...)  printf("CPBL: " fmt "\n", ##args);
#else
#define cpbl_debug(fmt, args...)
#endif

void boot_main(void)
{
#if 0
    u32 i;
#endif
   
    printf("Bink: BL start. copy bl.\n");
    printf("Bink: BL start. copy bl.\n");
    u32 bpfw_addr = CONFIG_SRAM_BL_START;
    int bpfw_len = input_data_bl_end - input_data_bl;

    chip_early_init_f();
    early_system_init();

#if 0
	cpbl_debug("bpfw_addr = 0x%x, bpfw_len = %d\n", bpfw_addr, bpfw_len);
    for(i = 0; i < 0x40000; i++){
        if(i%16 == 0){
            cpbl_debug("\n");
        }
        cpbl_debug("%02x ", *(volatile u8 *)(0x140000 + i));
    }
	cpbl_debug("\n\n print end, while 1 .....\n");
    while(1);
#endif
	cpbl_debug("bpfw_addr = 0x%x, bpfw_len = %d\n", bpfw_addr, bpfw_len);

    printf("Bink: copy bl to 0x290000.\n");
    memcpy((void*)bpfw_addr, input_data_bl, bpfw_len);
    
    cpbl_debug("flush 0x%x~3M Memory\n", CONFIG_SRAM_BL_START);
    v7_dma_flush_range(CONFIG_SRAM_BL_START, CONFIG_SRAM_BL_START + bpfw_len);
    cpbl_debug("flush 0x%x~3M Memory end!\n", CONFIG_SRAM_BL_START);

    printf("Bink: copy bl done. go to bl.\n");
    do_go_exec( (void *)CONFIG_SRAM_BL_START, 0 );
}
