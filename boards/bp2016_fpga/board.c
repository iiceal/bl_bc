#include <common.h>
#ifdef CONFIG_DRV_DDR
#include <drivers/drv_ddr.h>
#endif
#include "flash.h"

void global_iomux_set(void)
{
    // 1. find iomux configuration info in MBR

    // 2. read iomux configuration in norflash

    // 3. parse para and set iomux
}

int board_init_f(void)
{
#ifdef CONFIG_BOOT_FROM_QSPI
    cpu_qspi_init();
#endif
    // wwzz add iomux setting here
    // wwzz add 20181031
    init_spinor();
    //global_iomux_set();


	return 0;
}

#ifdef CONFIG_BOOT_FROM_QSPI
int cpu_qspi_init()
{

}
#endif
