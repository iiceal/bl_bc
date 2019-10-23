#include "common.h"

#ifdef CONFIG_ENABLE_TRACE
#define main_debug(fmt, args...)  printf("%s: %d " fmt, __func__, __LINE__, ##args)
#else
#define main_debug(fmt, args...)
#endif

#ifdef CONFIG_CMD_SHELL
extern int SHELL_INIT(void);
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
#ifdef CONFIG_DRV_UART
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
#endif
    return false;
}

#ifdef CONFIG_BOOT_COUNT_DOWN
U32 counting_down(void)
{
    U64 s, e;
    U8 c_input;
    U32 port = 0, skip_s = 0, delay_ms = 0,ret = 0;
    U32 time_out = CONFIG_BOOT_COUNT_DOWN_SEC * 1000;
    if(0 == time_out){
        printf("boot counting down bypass...\n");
        return ret;
    }
    s = arch_counter_get_current();
    while(1){
        if(true == uart_get_input(&port, &c_input)){
#ifdef CONFIG_CMD_SHELL
            SHELL_INIT();
#else
            printf("no shell, go on start...\n");
#endif
            ret = 1;
            break;
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
    }
    return ret;
}
#endif

#ifdef CONFIG_AUTO_UPGRADE
U32 auto_upgrade(void)
{
    U32 ret = 0;
    U64 s, e;
    U8 c_input;
    U32 port = 0, delay_ms = 0;
    U32 time_out = CONFIG_AUTO_UPGRADE_DELAY_MS;
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
  #ifdef CONFIG_CMD_SHELL
                printf("upgrade tool in pc detected, but not support in this version, start shell...\n");
                SHELL_INIT();
  #else
                printf("recv char %c, no shell, go on start...\n", &c_input);
  #endif
                break;
#endif
            }
            if((KEY_SPACE == c_input) || (KEY_ESC == c_input))
            {
  #ifdef CONFIG_CMD_SHELL
                printf("user stop in pc detected, but not support in this version, start shell...\n");
                SHELL_INIT();
  #else
                printf("recv char %c, user stop the progress, continue...\n", &c_input);
  #endif
                ret = 1;
                return ret;
            }
        }
        e = arch_counter_get_current();
        delay_ms = (((U32)(e-s))/TICKS_PER_MS) + 1;
        if(delay_ms > time_out){  // 3s
            printf("no upgrade tool detected in %dms, continue...\n", time_out);
            break;
        }
    }
    return 0;

}
#endif

#ifdef CONFIG_FASTBOOT
extern void fast_boot(void);
#endif

void boot_main(void)
{
    int ret;
    chip_early_init_f();
    //printf("\nboard %s BL_ASIC(%s) ver(%s) start...\n", CONFIG_BOARD_NAME, HGBUILDDATE, HGVERSION);
    printf("\n%s BL(%s) ver(%s)\n", CONFIG_BOARD_NAME, HGBUILDDATE, HGVERSION);
    early_system_init();

    // check fastboot
#ifdef CONFIG_FASTBOOT
    fast_boot();
#endif
    printf("no fastboot booting.Normal booting...\n");

#ifdef CONFIG_COMMON_MALLOC
    main_debug("Init memalloc: 0x%x~0x%x\n", 
		CONFIG_SYS_MALLOC_START, 
		CONFIG_SYS_MALLOC_START + CONFIG_SYS_MALLOC_LELN );
    mem_malloc_init(CONFIG_SYS_MALLOC_START, CONFIG_SYS_MALLOC_LEN);
#endif

    set_random();

    board_init_f();

#ifdef CONFIG_AUTO_UPGRADE
    ret = auto_upgrade();
    if(ret == 1) goto AUTO_UPGRADE;
#endif

#ifdef CONFIG_BOOT_COUNT_DOWN
    ret = counting_down();
    if(ret == 1) goto AUTO_UPGRADE;
#endif

    // try to start ecos or linux
    boot_parse_init();
#ifdef CONFIG_CMD_SHELL
    SHELL_INIT();
#else
    printf("no shell, while(1) upgrade...\n");
#endif
AUTO_UPGRADE:
    while (1){
#ifdef CONFIG_UPV_SERIAl
        auto_upgrade();
#endif
        ;
    }
}

#ifdef CONFIG_BUILD_ENABLE_UNWIND
void abort( void )
{
    printf("abort ...\n");
    while(1);
}
#endif
