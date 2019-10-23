#include <bl_common.h>

static BL_CMD_T go_descriptor;
static u32 go_exec_addr(u32 (*entry)(void))
{
	printf("jump to addr:0x%x \n", (u32)(entry));
    return entry();
}

extern void boot_parse_init(void);
static u32 go_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    u32 (*addr_go)(void);
    if(1 == argc){
        printf("Trying to start BBFW...\n");
        boot_parse_init();
        return RET_FAIL;
    } else if(2 == argc){
        addr_go = (u32 (*)(void))simple_strtoul(argv[1], NULL, 16);
        go_exec_addr(addr_go);
        printf("\n$$#$#$#$&& return from go_exec_addr &&&&&&&&&&\n\n\n");
    } else {
        return RET_FAIL;
    }
    return RET_FAIL;
}

static u8 *short_description(void)
{
    return (u8 *) "Test the go cmd\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified go by printing a message from it\n\
                The applicaition running command is as follow:\n\
                go (try to start BBFW) \n\
                go [address](example:go 0x100000) \n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &go_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "go", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = go_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
