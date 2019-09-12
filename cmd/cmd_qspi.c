#include <bl_common.h>

static BL_CMD_T qspi_descriptor;
extern int fl_cmd_entry_funcing(u32 argc, const char **argv);
static u32 qspi_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    return fl_cmd_entry_funcing(argc, argv);
}

static u8 *short_description(void)
{
    return (u8 *) "Case list:read/program/erase/protect/readid etc\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: The applicaition running command is as follow:\n\
                qspi init [irq], [log], [rd_thr:0, 1]\n\
                qspi readid\n\
                qspi read/readfast/readdual/readquad, [addr], [len], [dump], [dfs]\n\
                qspi ahb[read/readfast/readdual/readquad], [addr], [len], [dump], [dfs]\n\
                qspi dma[read/readfast/readdual/readquad], [addr], [len], [dump], [dfs]\n\
                qspi program, [addr], [len], [dump], [wdata]\n\
                qspi dmaprogram, [addr], [len], [dump], [wdata]\n\
                qspi ahbprogram, [addr], [len], [dump], [wdata]\n\
                qspi erase, [addr]\n\
                qspi eraseall\n\
                qspi protect, [disable/enable], region[0-15]\n\
				qspi prs, presure test \n\
				qspi lock addr, block lock \n\
				qspi lockstatus addr\n\
				qspi unlock addr, block unlock \n\
				qspi glock \n\
				qspi gunlock \n\
				qspi reset \n\
				qspi pdown, power down test \n\
                qspi upv, [flash_addr], [src_data_len], [dump], [src_data_addr]";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &qspi_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "qspi", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = qspi_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
