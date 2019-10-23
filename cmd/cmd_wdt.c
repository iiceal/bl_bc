#include <bl_common.h>
#include <regs/wdt.h>
#include <drivers/drv_wdt.h>

static BL_CMD_T wdt_descriptor;

static volatile int wdt_int_count = 0;

static u32 wdt_get_hw_base(int index)
{
    switch(index) {
        case 0:
            return (u32)hwp_apWdt0;
        case 1:
            return (u32)hwp_apWdt1;
        default:
            return 0;
    }
    return 0;
}

#ifdef CONFIG_USE_IRQ
static u32 wdt_get_hw_irqnum(int index)
{
    switch(index) {
        case 0:
            return SYS_IRQ_ID_WDT0;
        case 1:
            return SYS_IRQ_ID_WDT1;
        default:
            return 0;
    }
    return 0;
}
#endif

int wdt_test(int index, int torr_index)
{
    int i = 0;
    u32 ccvr = 0;
    u32 total_reset_ms, wait_ms, loop_count;
    HWP_WDT_AP_T * hw_wdt;

    hw_wdt = (HWP_WDT_AP_T*)wdt_get_hw_base(index);
    if(hw_wdt == NULL)
        return RET_FAIL;

    total_reset_ms = 
        (get_torr_range_max_v(torr_index) + 1) / (CONFIG_APB_CLOCK / 1000);
    total_reset_ms ++;

    if (total_reset_ms > 1000)
        wait_ms = 1000;
    else if (total_reset_ms > 100)
        wait_ms = 100;
    else if (total_reset_ms > 10)
        wait_ms = 10;
    else if (total_reset_ms >= 1)
        wait_ms = 1;
    else {
        printf("parameter error, total_ms = %d, index = %d, 0x%x\n", 
                total_reset_ms, torr_index,
                get_torr_range_max_v(torr_index));
        return -1;
    }
    loop_count = total_reset_ms / wait_ms + 2;

    printf("total reset ms = %d, wait ms = %d, loop = %d\n", 
                total_reset_ms, wait_ms, loop_count);

    wdt_torr(hw_wdt, torr_index, torr_index);
    wdt_cr(hw_wdt, 1, 0, 6);//wdt enable + disable resp mode
    for (i = 0; i < loop_count; i++) {
        mdelay(wait_ms);
        ccvr = wdt_ccvr(hw_wdt);
        printf("%d: wdt_ccvr = 0x%x (%d)\n", i, ccvr, 
                get_torr_range_max_v(torr_index) - ccvr);
        wdt_crr(hw_wdt, 0x76);
    }

    wdt_crr(hw_wdt, 0x76);

    if(index == 0) {
        printf("Wait %d ms, and system will restart\n", total_reset_ms);
        while (1) {
            mdelay(1000);
            printf(".");
        }
    } else {
        printf("Please check that cpu1 had restarted\n");
        mdelay(total_reset_ms);
    }

    return 0;
}

#ifdef CONFIG_USE_IRQ
static int wdt_irq_handler(int irq, void *dev_id)
{
    u32 time_count = timer_get_tick32();
    u32 int_flags = wdt_stat(dev_id);
    if (int_flags > 0) {
        wdt_int_count += wdt_eoi(dev_id);
        wdt_int_count++;
    }

    printf("0x%x:  %d, wdt_ccvr = 0x%x\n", 
        time_count, wdt_int_count, wdt_ccvr(dev_id));
    return 0;
}

int wdt_rsp_mode_test(int index, int torr_index)
{
    int total_reset_ms;
    u32 irq_num;
    HWP_WDT_AP_T * hw_wdt;
    wdt_int_count = 0;
    hw_wdt = (HWP_WDT_AP_T*)wdt_get_hw_base(index);
    if(hw_wdt == NULL)
        return RET_FAIL;
    irq_num = wdt_get_hw_irqnum(index);

    printf("Enable reponse mode testing:\n");

    total_reset_ms = (get_torr_range_max_v(torr_index) + 1) / (CONFIG_APB_CLOCK / 1000);
    total_reset_ms ++;
    printf("total reset ms = %d\n", total_reset_ms);
    request_irq(irq_num, wdt_irq_handler, NULL, (void*)hw_wdt);

    wdt_torr(hw_wdt, torr_index, torr_index);
    wdt_cr(hw_wdt, 1, 1, 6);
    while (wdt_int_count < 5)
        ;
    unrequest_irq(irq_num);
    if(index == 0) {
        printf("Wait %d ms, and system will restart\n", total_reset_ms * 2);

        while (1) {
            mdelay(1000);
            printf(".");
        }
    } else {
        printf("Please check that cpu1 had restarted\n");
        mdelay(total_reset_ms *2);
    }
    return 0;
}
#endif

static u32 wdt_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    u32 rsp_mode;
    int torr_index , index;

    index = 0;
    rsp_mode = 0;
    torr_index = 4;

    if (argc >= 2)
        index = simple_strtoul(argv[1], NULL, 10);
    if (argc >= 3)
        rsp_mode = simple_strtoul(argv[2], NULL, 10);
    if (argc >= 4)
        torr_index = simple_strtoul(argv[3], NULL, 10);

    if (torr_index < 0 || torr_index > 15)
        torr_index = 4;

    printf("wdt%d, torr_index = %d, rsp_mode = %d\n",
            index, torr_index, rsp_mode);

    if (rsp_mode == 0)
        return wdt_test(index, torr_index);
        
#ifdef CONFIG_USE_IRQ
    return wdt_rsp_mode_test(index, torr_index);
#else
    printf("no irq, can not test resp mode...\n");
    return 0;
#endif
}

static u8 *short_description(void)
{
    return (u8 *) "Case List:Reset system/restart wdt(or timer)\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: The applicaition running command is as follow:\n\
                wdt index[0,1], rsp_mode[0,1], torr_index[0~15]\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &wdt_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "wdt", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = wdt_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
