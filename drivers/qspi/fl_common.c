#include <common.h>
#include <drivers/drv_qspi.h>

extern int qspi_read_xx_data(u8 cmd, u32 addr, int addr_vld, u8 *rx_dbuf, int rx_len);
extern int qspi_read_page_data(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len, int dfs);
extern int qspi_dma_read_page(u8 cmd, u32 addr, u8 *rx_dbuf, int rx_len);

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
