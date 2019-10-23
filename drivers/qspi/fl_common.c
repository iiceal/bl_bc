#include <common.h>
#include <drivers/drv_qspi.h>
#include "fl_common_cmd.h"

extern int qspi_read_xx_data(u8 cmd, u32 addr, int addr_vld, u8 *rx_dbuf, int rx_len);
extern int qspi_read_page_data(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len, int dfs);

int qspi_fl_read_rdid(u8 rdid_cmd, int rdid_len, u8 *dbuf)
{
    return qspi_read_xx_data(rdid_cmd, 0, 0, dbuf, rdid_len);
}

int qspi_fl_read_devid(u8 devid_cmd, int devid_len, u8 *dbuf)
{
    return qspi_read_xx_data(devid_cmd, 0, 1, dbuf, devid_len);
}

int qspi_fl_read_page(u8 cmd, u32 addr, u8 *rbuf, int len, int dfs)
{
    return qspi_read_page_data(cmd, addr, rbuf, len, dfs);
}

u8 qspi_fl_read_status(u8 cmd)
{
    u8 data;
    qspi_read_xx_data(cmd, 0, 0, &data, 1);
    return data;
}

int qspi_fl_wait_wel(void)
{
#ifdef CONFIG_QSPI_WAIT_WEL 
    u8 st1 = 0;
    int count=10000;  //100ms

    while(count--){
        st1 = qspi_fl_read_status(SPINOR_OP_RDSR1);
        if(st1 & SR_WEL)
            return QSPI_OP_SUCCESS;
		udelay(1);
    }

    return QSPI_OP_SUCCESS;
#else
    return QSPI_OP_SUCCESS;
#endif
}

