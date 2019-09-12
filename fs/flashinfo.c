#include <flash.h>
#include <board_cfg.h>
#include <drivers/drv_qspi.h>

#define PARTITION_SEC_OFF	(0x800000)
#define PARTITION_SEC_0		(100)	//offset of the first partiton
#define PARTITION_SEC_1		(100)
#define PARTITION_SEC_2		(200)

flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS];

unsigned long flash_init(void)
{
	unsigned long total_size = 0;
	u8 i, j;

	flash_info[0].size = CONFIG_JFFS2_PART_SIZE; //fl_dev->blk_len * PARTITION_SEC_0;
	//flash_info[1].size = fl_dev->blk_len * PARTITION_SEC_1;
	//flash_info[2].size = fl_dev->blk_len * PARTITION_SEC_2;

    printf("%s: size 0x%x \n",__func__,  flash_info[0].size);
	
	for (i = 0; i < CONFIG_SYS_MAX_FLASH_BANKS; i++) {
		flash_info[i].flash_id = fl_dev->jedec;
		flash_info[i].sector_count = flash_info[i].size/fl_dev->blk_len;
		if(0==i)
		{
			flash_info[i].start[0] = CONFIG_SYS_FLASH0_BASE; //+ PARTITION_SEC_OFF;
		}else{
			flash_info[i].start[0] = flash_info[i-1].start[0] + total_size;
		}

		for (j = 1; j < flash_info[i].sector_count; j++) {
			flash_info[i].start[j] = flash_info[i].start[j - 1]
				+ fl_dev->blk_len;
		}

		total_size += flash_info[i].size;
	}

    printf("%s: flash_info[0].Size 0x%x flash_info[0].start 0x%x sector_count %d \n",
            __func__ , flash_info[0].size, flash_info[0].start[0], flash_info[0].sector_count);

	return total_size;
}

void flash_print_info(flash_info_t *info)
{
	int i;

	if (info->flash_id == FLASH_UNKNOWN) {
		printf("missing or unknown FLASH type\n");
		return;
	} else if (info->flash_id == FLASH_STM32) {
		printf("stm32 Embedded Flash\n");
	}

	printf("  Size: %ld MB in %d Sectors\n",
	       info->size >> 20, info->sector_count);

	printf("  Sector Start Addresses:");
	for (i = 0; i < info->sector_count; ++i) {
		if ((i % 5) == 0)
			printf("\n   ");
		printf(" %08lX%s",
		       info->start[i],
			info->protect[i] ? " (RO)" : "     ");
	}
	printf("\n");
	return;
}
