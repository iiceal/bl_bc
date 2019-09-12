#include <bl_common.h>
#include <ping/ping.h>
#include "netconf.h"
#include "udp_echoclient/udp_echoclient.h"
static BL_CMD_T net_descriptor;

int do_ping(int argc, const char **argv)
{
    // bsp_init();
    // lwip_init_stack();
    // ping_init();
    // ping_send_now();

    return 0;
}

int do_udp_send(int argc, const char **argv)
{
    lwip_init_stack();
    udp_echoclient_connect();
    return 0;
}

extern int do_w5500(int argc, const char **argv);

static u32 net_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    // return do_ping(argc, argv);
    // return do_udp_send(argc, argv);
    return do_w5500(argc, argv);
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: net(rdss) atr/read/write etc\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified net by printing a message from it\n\
                The applicaition running command is as follow:\n\
                ping 192.168.1.111\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &net_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "net", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = net_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
