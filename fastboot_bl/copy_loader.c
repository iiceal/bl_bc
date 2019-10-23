
extern char input_data_bl[];
extern char input_data_bl_end[];

extern void v7_dma_flush_range(unsigned long, unsigned long);

static void * memcpy(void *dest, const void *src, unsigned long count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* while all data is aligned (common case), copy a word at a time */
	if ( (((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
		while (count >= sizeof(*dl)) {
			*dl++ = *sl++;
			count -= sizeof(*dl);
		}
	}
	/* copy the reset one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (count--)
		*d8++ = *s8++;

	return dest;
}

static unsigned long do_go_exec (unsigned long (*entry)(int), unsigned size)
{
    return entry(0);
}

#define CONFIG_SRAM_BL_START  (0x300000)
#define ROM_BOOT_LOAD_FLASH_MAGIC           0xfabbe000
#define ROM_BOOT_LOAD_FLASH_MAGIC_ADDR      0xc01d10f4

void boot_main(void)
{
    unsigned int bpfw_addr = CONFIG_SRAM_BL_START;
    int bpfw_len = input_data_bl_end - input_data_bl;
    *(volatile unsigned int *)(0xc01d10f0) = (unsigned int)__builtin_return_address(0);
    *(volatile unsigned int *)(ROM_BOOT_LOAD_FLASH_MAGIC_ADDR) = ROM_BOOT_LOAD_FLASH_MAGIC;

    memcpy((void*)bpfw_addr, input_data_bl, bpfw_len);
    
    v7_dma_flush_range(CONFIG_SRAM_BL_START, CONFIG_SRAM_BL_START + bpfw_len);

    do_go_exec( (void *)CONFIG_SRAM_BL_START, 0 );
}
