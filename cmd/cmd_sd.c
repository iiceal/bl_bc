#include <bl_common.h>

static BL_CMD_T sd_descriptor;

extern int sd_cmd_entry_funcing(u32 argc, const char **argv);

static u32 sd_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    return sd_cmd_entry_funcing(argc, argv);
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: sd read/write/erase/dma etc\n";
}

static u8 *long_description(void)
{
    return (u8 *) "The applicaition running command is as follow:\n\
                sd init\n\
                sd [read/write/auto], [addr], [length]\n\
                sd [dmar/dmaw], [addr], [length]\n\
                sd auto";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &sd_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "sd", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = sd_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
