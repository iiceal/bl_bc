#include <bl_common.h>
#include "drivers/drv_spi.h"

static BL_CMD_T spi_descriptor;

#define SPI_TEST_BUF_SIZE 0x200
static void * handler = NULL;
__attribute__((aligned(64))) u8 txbuf[SPI_TEST_BUF_SIZE];
__attribute__((aligned(64))) u8 rxbuf[SPI_TEST_BUF_SIZE];

int spi_flash_test(int argc, const char **argv)
{
    u32 addr = 0;
    u32 txlen = 0x100, rxlen = 0x100;
    int i = 0;

    if (argc > 2)
        addr = simple_strtoul(argv[2], NULL, 16);
    if (argc > 3) {
        txlen = simple_strtoul(argv[3], NULL, 16);
        rxlen = simple_strtoul(argv[3], NULL, 16);
    }

    if (argc > 1 && strcmp(argv[1], "init") == 0) {
        handler = spi_init_bus(0, 0);
    }

    if (argc > 2 && strcmp(argv[1], "read") == 0) {
        printf("spi read addr 0x%x, len = %d\n", addr, rxlen);
        txbuf[0] = 0x03;    // read
        txbuf[1] = (addr >> 16) & 0xff;
        txbuf[2] = (addr >> 8) & 0xff;
        txbuf[3] = addr & 0xff;
        txlen = 4;
        rxlen = 0x100;
        spi_eeprom_data(handler, txbuf, txlen, rxbuf, rxlen);
        printf_buf_fmt_32bit((u32 *) rxbuf, (u32) addr, rxlen / 4);
    }

    if (argc > 2 && strcmp(argv[1], "program") == 0) {
        printf("spi program addr 0x%x, len = %d\n", addr, txlen);
        txbuf[0] = 0x06;    // wren
        txlen = 1;
        spi_write_data(handler, txbuf, txlen);

        txbuf[0] = 0x02;    // program
        txbuf[1] = (addr >> 16) & 0xff;
        txbuf[2] = (addr >> 8) & 0xff;
        txbuf[3] = addr & 0xff;
        txlen = 0x100 + 4;
        for (i = 0; i < 0x100; i++) {
            txbuf[i + 4] = 0x66 + i;
        }

        spi_write_data(handler, txbuf, txlen);
    }

    return 0;
}

static u32 spi_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    return spi_flash_test(argc, argv);
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: spi flash/xn112/loop m-s\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified Spi by printing a message from it\n\
                The applicaition running command is as follow:\n\
                ----------flash----------------------\n\
                spi init clk_value[KHz] [spi_index]\n\
                spi st_read\n\
                spi prob_disabe\n\
                spi [program / read] addr length\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &spi_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "spi", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = spi_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
