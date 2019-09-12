#include <bl_common.h>
#include "drivers/drv_sim.h"

static BL_CMD_T sim_descriptor;

static void dump_sim_buffer(void *buffer, u32 len)
{
    u32 i = 0;
    u8 *buf = (u8 *) buffer;

    printf("\n");
    for (i = 0; i < len; i++) {
        printf("%02x ", *(buf + i));
    }
    printf("\n");
}

static u8 buffer[1024];
static u8 selfcheck_txbuf[40] = {0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0x7d, 0x51,
    0x30, 0xb1, 0xc2, 0x2a, 0x7d, 0x5f, 0xa9, 0x62, 0x46, 0x4e,
    0x72, 0x0f, 0xbe, 0x5b, 0xdb, 0x1f, 0x7d, 0x98, 0x67, 0x58,
    0x26, 0x12, 0x5d, 0x81, 0x10, 0xac, 0x22, 0xa5, 0x29, 0x1f};
static u8 userinfo_txbuf[5] = {0x00, 0x03, 0x06, 0x20, 0x25};

static u8 cxor(u8 *pbuf, int length)
{
    u8 ret = 0;
    if (length <= 0)
        return -1;
    while (length > 0) {
        ret ^= *pbuf;
        length--;
        pbuf++;
    }
    return ret;
}

static int rdss_atr(u32 index, u32 etu)
{
    int res = 0;
    u32 timeout = 50;
    u32 len = 12;
    u8 parity = 0;
    res = sim_atr(index, buffer, &len, etu, 1, timeout);
    if (res == SIM_RET_FAIL)
        return RET_FAIL;
    parity = cxor(buffer, (len - 1));
    printf("\n%s parity = 0x%x, res = 0x%x, len = %d\n", __FUNCTION__, parity, res, len);
    dump_sim_buffer(buffer, len);
    return 0;
}

static int rdss_check(u32 index)
{
    int res = 0;
    u32 timeout = 50;
    u32 len = 40;
    u8 parity = 0;
    res = sim_write(index, selfcheck_txbuf, &len, timeout);
    if (res != 0)
        return res;
    timeout = 100;
    len = 0x6e;
    res = sim_read(index, buffer, &len, timeout);
    if (res == SIM_RET_FAIL)
        return RET_FAIL;
    parity = cxor(buffer, (len - 1));
    printf("\n%s parity = 0x%x, res = 0x%x, len = %d\n", __FUNCTION__, parity, res, len);
    dump_sim_buffer(buffer, len);
    return 0;
}

static int rdss_userinfo(u32 index)
{
    int res = 0;
    u32 timeout = 50;
    u32 len = 5;
    u8 parity = 0;
    res = sim_write(index, userinfo_txbuf, &len, timeout);
    if (res != 0)
        return res;
    timeout = 300;
    len = 0x25d;
    res = sim_read(index, buffer, &len, timeout);
    if (res == SIM_RET_FAIL)
        return RET_FAIL;
    parity = cxor(buffer, (len - 1));
    printf("\n%s parity = 0x%x, res = 0x%x, len = %d\n", __FUNCTION__, parity, res, len);
    dump_sim_buffer(buffer, len);
    return res;
}

int sim_rdss_test(int argc, const char **argv)
{
    u32 etu = 32;
    int res = RET_PASS;
    static u32 index = 1;

    if (argc > 1) {
        if (strcmp(argv[1], "i") == 0) {
            if (argc > 2) {
                index = simple_strtoul(argv[2], NULL, 10);
                printf("\nsim index = %d\n", index);
            }
        }

        if (strcmp(argv[1], "a") == 0) {
            if (argc > 2) {
                etu = simple_strtoul(argv[2], NULL, 10);
            } else {
                etu = 32;
            }
            printf("\nsim index = %d\n", index);
            res = rdss_atr(index, etu);
        }

        if (strcmp(argv[1], "c") == 0) {
            res = rdss_check(index);
        }

        if (strcmp(argv[1], "u") == 0) {
            res = rdss_userinfo(index);
        }

        if (strcmp(argv[1], "e") == 0) {
            if (argc > 2) {
                etu = simple_strtoul(argv[2], NULL, 10);
            } else {
                etu = 32;
            }
            res = sim_set_clk(1, etu);
        }
    }

    return res;
}
static u32 sim_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    return sim_rdss_test(argc, argv);
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: sim(rdss) atr/read/write etc\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified sim by printing a message from it\n\
                The applicaition running command is as follow:\n\
                sim i [index]: set index\n\
                sim a [baudrate]: get ATR\n\
                sim e [baudrate]: set etu\n\
                sim c: check\n\
                sim u: user info\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &sim_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "sim", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = sim_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
