#include <bl_common.h>
#include "regs/uart.h"
#include "monitor.h"
#include "common.h"

static BL_CMD_T uart_descriptor;
// static u32 isr_save = 0;
// static u32 isr_oe = 0;
// static u32 isr_err = 0;
// static u32 dbg_addr = 0x200000;
// static u32 xmodem_error;
// static u32 xmodem_cancel;
volatile static u32 dbg_write_size = 0;
#ifdef CONFIG_LOAD_XMODEM
extern int load_xmodem(unsigned int load_addr, int load_size);
#endif //CONFIG_LOAD_XMODEM 

extern int serial_getc(void);
extern void serial_putc(const char c);

#define SYS_SRAM_ADDR_BASE       0x00100000
#define SYS_SRAM_LENGTH          0x00200000

#define SYS_RAM_START_ADDR       SYS_SRAM_ADDR_BASE
#define SYS_RAM_LENGTH           SYS_SRAM_LENGTH

U32 loadversion_start_data_valid(U32 addr, U32 len)
{
    if ((SYS_RAM_START_ADDR <= addr) &&
        (CONFIG_SRAM_BL_START > (addr + len))) {
        return 1;
    }
    return 0;
}

#include <drivers/drv_serial.h>
extern void upgrade_start(void);

#define DEFAULT_BAUD    (9600)
int loadx_total_len = 0;
static u32 uart_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
#ifdef CONFIG_LOAD_XMODEM
    int length = 0;
#endif
    int port_id = 0xFF;
    if (argc > 1) {
        if (strcmp(argv[1], "init") == 0) {
            port_id = simple_strtoul(argv[2], NULL, 10);
            if(port_id > 7) {
                printf("error: uart port invalid\n");
                return RET_FAIL;
            }
            printf("uart init %d \n", port_id);
            uart_init(port_id, DEFAULT_BAUD, 0); 
            return RET_PASS;
        }

#ifdef CONFIG_LOAD_XMODEM
        int addr = 0;
        if (strcmp(argv[1], "loadx") == 0){
            load_xmodem((u32)0x100000, 0x20000);
            return RET_PASS;
        }

        if (strcmp(argv[1], "loadv") == 0) {
            if (argc > 2) {
                addr = simple_strtoul(argv[2], NULL, 16);
                if (argc > 3) {
                    length = simple_strtoul(argv[3], NULL, 16);
                } else {
                    length = 0x40000;
                }
            } else {
                addr = 0x140000;
                length = 0x40000;
            }

            //if (!loadversion_start_data_valid(addr, length)) {
            //    printf("para invalid! addr=0x%x, length=0x%x\n", addr, length);
            //    return RET_FAIL;
            //}

            loadx_total_len = load_xmodem(addr, length);
            printf("load done! addr=0x%x, file length=0x%x\n", addr, loadx_total_len);
            return RET_PASS;
        }
#endif

//#ifdef CONFIG_UPV_SERIAl
#ifdef CONFIG_AUTO_UPGRADE
        if (strcmp(argv[1], "upv") == 0) {
            // upgrade_processing();
            upgrade_start();
            return RET_PASS;
        }
#endif
    }

    // printf("uart testing end, error %d, cancel = %d\n", xmodem_error, xmodem_cancel);
    return 0;
}

static u8 *short_description(void)
{
    return (u8 *) "Test the specified Uart port\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified Uart by printing a message from it\n\
                The applicaition running command is as follow:\n\
                uart [init] [port_id] \n\
                uart [loadx/loadb], [length]\n\
                uart [loadv], [ram_addr], [length]\n\
                uart [upv]\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &uart_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "uart", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = uart_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
