#include <common.h>

extern char input_data_bl[];
extern char input_data_bl_end[];

extern int chip_early_init_f(void);
extern void early_system_init(void);
extern void v7_dma_flush_range(u32, u32);
extern unsigned long do_go_exec (unsigned long (*entry)(int), unsigned size);

void boot_main(void)
{
#if 0
    u32 i;
#endif
    u32 bpfw_addr = CONFIG_SRAM_BL_START;
    int bpfw_len = input_data_bl_end - input_data_bl;

    chip_early_init_f();
    early_system_init();
#if 0
	printf("bpfw_addr = 0x%x, bpfw_len = %d\n", bpfw_addr, bpfw_len);
    for(i = 0; i < 0x40000; i++){
        if(i%16 == 0){
            printf("\n");
        }
        printf("%02x ", *(volatile u8 *)(0x140000 + i));
    }
	printf("\n\n print end, while 1 .....\n");
    while(1);
#endif
	printf("bpfw_addr = 0x%x, bpfw_len = %d\n", bpfw_addr, bpfw_len);

    memcpy((void*)bpfw_addr, input_data_bl, bpfw_len);
    
    printf("flush 0x%x~3M Memory\n", CONFIG_SRAM_BL_START);
    v7_dma_flush_range(CONFIG_SRAM_BL_START, CONFIG_SRAM_BL_START + bpfw_len);
    printf("flush 0x%x~3M Memory end!\n", CONFIG_SRAM_BL_START);

    do_go_exec( (void *)CONFIG_SRAM_BL_START, 0 );
}
