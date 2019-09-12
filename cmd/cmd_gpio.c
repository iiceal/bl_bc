#include <bl_common.h>
#include "drivers/drv_gpio.h"
#include <regs/gpio.h>

static BL_CMD_T gpio_descriptor;
int gpio_test(int gpio_num, int input);

static u32 gpio_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    int gpio_num, input;

    gpio_num = 0;
    input = 0;

    if (argc >= 2)
        gpio_num = simple_strtoul(argv[1], NULL, 10);
    if (argc >= 3)
        input = simple_strtoul(argv[2], NULL, 10);

    printf("gpio num: %d, mode %s\n", gpio_num, input > 0 ? "input" : "output");

    gpio_test(gpio_num, input);
    return 0;
}

int gpio1_irq_handler(int irq, void *dev_id)
{
    u32 st = gpio_interupt_get_intstatus();
    if (st & (1 << (irq - SYS_IRQ_ID_GPIO0))) {
        gpio_interrupt_clr(1 << (irq - SYS_IRQ_ID_GPIO0));
        printf("clr gpio%d interrupt\n", (irq - SYS_IRQ_ID_GPIO0));
    }
    return 0;
}

int gpio_test(int gpio_num, int input)
{
    int i;
    u32 v;
    if (input == 0) {
        printf("gpio %d, output testing .\n", gpio_num);
        gpio_enable(gpio_num);
        gpio_disable_interrupt(gpio_num);
        gpio_set_output_direction(gpio_num);

        for (i = 0; i < 60; i++) {
            gpio_set_output_high(gpio_num);
            mdelay(100);
            gpio_set_output_low(gpio_num);
            mdelay(100);
        }
        gpio_disable(gpio_num);

        printf_buf_fmt_32bit((u32 *) hwp_apGpio, (u32) hwp_apGpio, 64);
        return 0;
    }

    printf("gpio %d, input testing .\n", gpio_num);
    gpio_enable(gpio_num);
    gpio_disable_interrupt(gpio_num);
    gpio_set_input_direction(gpio_num);

    for (i = 0; i < 60; i++) {
        gpio_get_input_data(gpio_num, &v);
        printf("gpio %d, input v = %d\n", gpio_num, v);
        mdelay(100);
    }
    gpio_disable(gpio_num);

    printf_buf_fmt_32bit((u32 *) hwp_apGpio, (u32) hwp_apGpio, 64);

    gpio_enable(gpio_num);
    gpio_enable_interrupt(gpio_num);
    gpio_set_input_direction(gpio_num);
    gpio_interrupt_type_set_level(gpio_num);
    gpio_interrupt_polarity_set_lowlevel(gpio_num);
    gpio_interrupt_unmask(gpio_num);

    request_irq(SYS_IRQ_ID_GPIO0 + gpio_num, gpio1_irq_handler, NULL, (void *) hwp_apGpio);

    printf("gpio %d, interrupt testing .\n", gpio_num);
    return 0;
}

static u8 *short_description(void)
{
    return (u8 *) "Case list:output/intput/interrupt etc\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified Spi by printing a message from it\n\
                The applicaition running command is as follow:\n\
                gpio num output[o =0, i = 1]\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &gpio_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "gpio", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = gpio_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
