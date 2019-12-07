#include "common.h"


#if 0
extern void register_common_cmd(DYN_SHELL_ITEM_T *p_common_shell);
static DYN_SHELL_ITEM_T *p_root_shell;
static DYN_SHELL_ITEM_T *p_common_shell;

BL_CMD_INFO_T g_cmd_list[BL_MAX_CMDS_NUM];
unsigned int g_cmd_num = 0;

int command_entry(shell_cmd_t *cptr, const unsigned int argc, const char **argv)
{
    int i, ret;
    BL_CMD_T *cur;

    for (i = 0; i < g_cmd_num; i++) {
        cur = g_cmd_list[i].p_cmd_descriptor;
        if (!strcmp(argv[0], (const char *) (cur->name))) {

            ret = cur->cmd_entry_func(NULL, argc, argv);

            g_cmd_list[i].result = ret;
            switch (ret) {
            case RET_PASS:
                printf("<<<PASS>>>\r\n");
                break;
            case RET_FAIL:
                printf("<<<FAIL>>>\r\n");
                break;
            case RET_NA:
                printf("<<NA>>\r\n");
                break;
            case RET_BLOCK:
                printf("<<<BLOCK(not ready. code:%d)>>>\r\n", ret);
                break;
            default:
                printf("<<<UNKNOWN, code:%d>>>\r\n", ret);
            }
            return ret;
        }
    }
    printf("The cmd is not available.");
    return 0;
}

void bl_cmd_init(void);

void register_bl_cmds(void)
{
    unsigned int i, len, pos;
    unsigned char *p_desc, *p_long_desc;
    char desc_buff[BL_MAX_STRING];
    BL_CMD_T *cur;

    bl_cmd_init();

    for (i = 0; i < g_cmd_num; i++) {
        cur = g_cmd_list[i].p_cmd_descriptor;
        if (cur->cmd_entry_func) {
            if (NULL == cur->cmd_description) {
                pos = 0;
                len = strlen((const char *) cur->name);
                memcpy(&desc_buff[pos], cur->name, len);
                pos += len;

                len = strlen("\t\t - run the cmd of ");
                memcpy(&desc_buff[pos], "\t\t - run the cmd of ", len);
                pos += len;

                len = strlen((const char *) cur->name);
                memcpy(&desc_buff[pos], cur->name, len);
                pos += len;
                desc_buff[pos] = 0;
                p_desc = (unsigned char *) desc_buff;
            } else
                p_desc = cur->cmd_description();

            if (cur->cmd_long_description)
                p_long_desc = cur->cmd_long_description();
            else
                p_long_desc = NULL;

            BL_SHELL_ADD_CMD(p_root_shell,
					cur->name,0,
					p_desc,
					p_long_desc,
					command_entry);
		}

    }
}

void bl_cmd_init(void)
{
    BL_GET_CMD_DESCRIPTOR_T *fn, *begin, *end;
    BL_CMD_INFO_T *list;

    begin = __cmd_descriptor_handle_begin;
    end = __cmd_descriptor_handle_end;
    list = g_cmd_list;

    for (fn = begin; fn != end; fn++) {
        printf("[bl_cmd_init] No.%d,", g_cmd_num);
        list[g_cmd_num].p_cmd_descriptor = (*fn)(NULL);
        list[g_cmd_num++].result = RET_NOTTEST;
        printf("%s\r\n", list[g_cmd_num - 1].p_cmd_descriptor->name);
    }
}

int SHELL_INIT(void)
{
    p_root_shell = NULL;
    p_common_shell = NULL;

    dyn_shell_init(&p_root_shell, (unsigned char *) "shell");
    dyn_shell_init(&p_common_shell, (unsigned char *) "");

    printf("shell init end\n");
    register_common_cmd(p_common_shell);
    register_bl_cmds();

    dyn_shell_load(p_root_shell, p_common_shell);
    printf("shell test done\r\n");
    return 0;
}
#endif

extern int chip_early_init_f(void);
extern void early_system_init(void);
extern int board_init_f(void);
#include "regs/uart.h"
#include "upgrade.h"
#include "drivers/drv_serial.h"
extern inline int __serial_getc_direct(HWP_UART_T *hw_uart);
extern int __serial_tstc(HWP_UART_T *hw_uart);
#ifdef CONFIG_UPV_SERIAl
//extern int select_id;   // upgrade uart port_id
extern void upgrade_processing(void);
#endif
extern void boot_parse_init(void);

bool uart_get_input(U32 *port, u8 *c)
{
    if(__serial_tstc(hwp_dbgUart))
    {
        *c = __serial_getc_direct(hwp_dbgUart);
        if(PK_HEADER == *c)
        {
            *port = 0;
        }
        return true;
    }

    if(__serial_tstc(hwp_upvUart))
    {
        *c = __serial_getc_direct(hwp_upvUart);
        if(PK_HEADER == *c)
        {
            *port = 1;
        }
        return true;
    }

    return false;
}

#ifdef CONFIG_BOOT_COUNT_DOWN
void counting_down(void)
{
    U64 s, e;
    U8 c_input;
    U32 port = 0, skip_s = 0, delay_ms = 0;
    U32 time_out = CONFIG_BOOT_COUNT_DOWN_SEC * 1000;
    if(0 == time_out){
        printf("boot counting down bypass...\n");
        return;
    }
    s = arch_counter_get_current();
    while(1){
        if(true == uart_get_input(&port, &c_input)){
                SHELL_INIT();
        }
        e = arch_counter_get_current();
        delay_ms = (((U32)(e-s))/TICKS_PER_MS) + 1;
        if(delay_ms > time_out){  // 3s
            printf("no input in %dS, continue...\n", CONFIG_BOOT_COUNT_DOWN_SEC);
            break;
        }else{
            if((delay_ms % 1000) == 0){
                printf("skip %d s...\n", ++skip_s);
                mdelay(1);
            }
        }

        if(delay_ms > time_out){  // 3s
            printf("no upgrade tool detected in %dms, continue...\n", time_out);
            break;
        }
    }
}
#endif

#ifdef CONFIG_AUTO_UPGRADE
int auto_upgrade(void)
{
    U64 s, e;
    U8 c_input;
    U32 port = 0, delay_ms = 0;
    U32 time_out = CONFIG_AUTO_UPGRADE_DELAY_MS;
    U32 ret = 0;
    if(0 == time_out){
        printf("auto_upgrade bypass...\n");
        return 0;
    }
    s = arch_counter_get_current();
    while(1){
        if(true == uart_get_input(&port, &c_input)){
            if(PK_HEADER == c_input){
#ifdef CONFIG_UPV_SERIAl
                upgrade_start();
#else
//                printf("upgrade tool in pc detected, but not support in this version, start shell...\n");
//                SHELL_INIT();
#endif
            }
            if((KEY_SPACE == c_input) || (KEY_ESC == c_input))
            {
                printf("Get User Stop To Bootloader Command,\nStart bootloader shell.\n\n");
//                SHELL_INIT();
                ret = 1;
            }
        }
        e = arch_counter_get_current();
        delay_ms = (((U32)(e-s))/TICKS_PER_MS) + 1;
        if(delay_ms > time_out){  // 3s
            printf("no upgrade tool detected in %dms, continue...\n", time_out);
            break;
        }
    }

    return ret;
}
#endif

extern void print_boot_par(void);
extern void copy_rom_run_paramter_to_dst(void);

extern void upgrade_start(void);
extern void init_spinor(void);
void boot_main(void)
{
    printf("Bink: BL start.\n ");
    chip_early_init_f();
    early_system_init();
//    printf("\nboard %s BL_ASIC(%s) ver(%s) start...\n", CONFIG_BOARD_NAME, HGBUILDDATE, HGVERSION);
    //print_boot_par();
//    copy_rom_run_paramter_to_dst();

//    mem_malloc_init(CONFIG_SYS_MALLOC_START, CONFIG_SYS_MALLOC_LEN);
    set_random();

//    init_spinor();
//    board_init_f();

#ifdef CONFIG_AUTO_UPGRADE
    int ret;
   ret =  auto_upgrade();
   if(ret == 1)
       goto UPGRADE;
#endif

#ifdef CONFIG_BOOT_COUNT_DOWN
    counting_down();
#endif

    // try to start ecos or linux
//    printf("try to start BP...\n");
    boot_parse_init();
//    printf("start BP fail, start shell...\n");
//    SHELL_INIT();
//UPGRADE:

    while (1)
    {
#ifdef CONFIG_AUTO_UPGRADE
        auto_upgrade();
#endif
        mdelay(1000);
    }
}

#ifdef CONFIG_BUILD_ENABLE_UNWIND
void abort( void )
{
    printf("abort ...\n");
    while(1);
}
#endif
