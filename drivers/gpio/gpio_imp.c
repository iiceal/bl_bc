#include <common.h>
#include <regs/gpio.h>

static bool gpio_get_portx_dr(int gpio_num, u32 *v)
{
    if (gpio_num >= 0 && gpio_num <= 31) {
        *v = hwp_apGpio->porta_dr;
        return true;
    }
    if (gpio_num >= 32 && gpio_num <= 63) {
        *v = hwp_apGpio->portb_dr;
        return true;
    }

    if (gpio_num >= 64 && gpio_num <= 95) {
        *v = hwp_apGpio->portc_dr;
        return true;
    }

    if (gpio_num >= 96 && gpio_num <= 127) {
        *v = hwp_apGpio->portd_dr;
        return true;
    }
    return false;
}

static bool gpio_set_portx_dr(int gpio_num, u32 v)
{
    if (gpio_num >= 0 && gpio_num <= 31) {
        hwp_apGpio->porta_dr = v;
        return true;
    }
    if (gpio_num >= 32 && gpio_num <= 63) {
        hwp_apGpio->portb_dr = v;
        return true;
    }

    if (gpio_num >= 64 && gpio_num <= 95) {
        hwp_apGpio->portc_dr = v;
        return true;
    }

    if (gpio_num >= 96 && gpio_num <= 127) {
        hwp_apGpio->portd_dr = v;
        return true;
    }
    return false;
}

static bool gpio_get_portx_ddr(int gpio_num, u32 *v)
{
    if (gpio_num >= 0 && gpio_num <= 31) {
        *v = hwp_apGpio->porta_ddr;
        return true;
    }
    if (gpio_num >= 32 && gpio_num <= 63) {
        *v = hwp_apGpio->portb_ddr;
        return true;
    }

    if (gpio_num >= 64 && gpio_num <= 95) {
        *v = hwp_apGpio->portc_ddr;
        return true;
    }

    if (gpio_num >= 96 && gpio_num <= 127) {
        *v = hwp_apGpio->portd_ddr;
        return true;
    }
    return false;
}

static bool gpio_set_portx_ddr(int gpio_num, u32 v)
{
    if (gpio_num >= 0 && gpio_num <= 31) {
        hwp_apGpio->porta_ddr = v;
        return true;
    }
    if (gpio_num >= 32 && gpio_num <= 63) {
        hwp_apGpio->portb_ddr = v;
        return true;
    }

    if (gpio_num >= 64 && gpio_num <= 95) {
        hwp_apGpio->portc_ddr = v;
        return true;
    }

    if (gpio_num >= 96 && gpio_num <= 127) {
        hwp_apGpio->portd_ddr = v;
        return true;
    }
    return false;
}

static bool gpio_get_portx_ctl(int gpio_num, u32 *v)
{
    if (gpio_num >= 0 && gpio_num <= 31) {
        *v = hwp_apGpio->porta_ctl;
        return true;
    }
    if (gpio_num >= 32 && gpio_num <= 63) {
        *v = hwp_apGpio->portb_ctl;
        return true;
    }

    if (gpio_num >= 64 && gpio_num <= 95) {
        *v = hwp_apGpio->portc_ctl;
        return true;
    }

    if (gpio_num >= 96 && gpio_num <= 127) {
        *v = hwp_apGpio->portd_ctl;
        return true;
    }
    return false;
}

static bool gpio_set_portx_ctl(int gpio_num, u32 v)
{
    if (gpio_num >= 0 && gpio_num <= 31) {
        hwp_apGpio->porta_ctl = v;
        return true;
    }
    if (gpio_num >= 32 && gpio_num <= 63) {
        hwp_apGpio->portb_ctl = v;
        return true;
    }

    if (gpio_num >= 64 && gpio_num <= 95) {
        hwp_apGpio->portc_ctl = v;
        return true;
    }

    if (gpio_num >= 96 && gpio_num <= 127) {
        hwp_apGpio->portd_ctl = v;
        return true;
    }
    return false;
}

static inline u32 gpio_get_porta_inten(void)
{
    return hwp_apGpio->inten;
}

static inline void gpio_set_porta_inten(u32 v)
{
    hwp_apGpio->inten = v;
}

static inline u32 gpio_get_porta_intmask(void)
{
    return hwp_apGpio->intmask;
}

static inline void gpio_set_porta_intmask(u32 v)
{
    hwp_apGpio->intmask = v;
}

static inline u32 gpio_get_porta_inttype_level(void)
{
    return hwp_apGpio->inttype_level;
}

static inline void gpio_set_porta_inttype_level(u32 v)
{
    hwp_apGpio->inttype_level = v;
}

static inline u32 gpio_get_porta_int_polarity(void)
{
    return hwp_apGpio->int_polarity;
}

static inline void gpio_set_porta_int_polarity(u32 v)
{
    hwp_apGpio->int_polarity = v;
}

static inline u32 gpio_get_porta_intstatus(void)
{
    return hwp_apGpio->intstatus;
}

static inline u32 gpio_get_porta_raw_intstatus(void)
{
    return hwp_apGpio->raw_intstatus;
}

static inline void gpio_set_debounce(u32 v)
{
    hwp_apGpio->debounce = v;
}

static inline void gpio_set_porta_eoi(u32 v)
{
    hwp_apGpio->porta_eoi = v;
}

static bool gpio_get_portx_input_data(int gpio_num, u32 *v)
{
    if (gpio_num >= 0 && gpio_num <= 31) {
        *v = hwp_apGpio->ext_porta;
        return true;
    }
    if (gpio_num >= 32 && gpio_num <= 63) {
        *v = hwp_apGpio->ext_portb;
        return true;
    }

    if (gpio_num >= 64 && gpio_num <= 95) {
        *v = hwp_apGpio->ext_portc;
        return true;
    }

    if (gpio_num >= 96 && gpio_num <= 127) {
        *v = hwp_apGpio->ext_portd;
        return true;
    }
    return false;
}

/*************************************************************/
static bool gpio_set_ctl(int gpio_num, u32 v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift;
    u32 s_v;

    if (gpio_get_portx_ctl(gpio_num, &s_v) == false)
        return false;

    s_v &= ~mask;
    s_v |= (v << shift);

    return gpio_set_portx_ctl(gpio_num, s_v);
}

static bool gpio_set_direction(int gpio_num, u32 v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift;
    u32 s_v;

    if (gpio_get_portx_ddr(gpio_num, &s_v) == false)
        return false;

    s_v &= ~mask;
    s_v |= (v << shift);

    return gpio_set_portx_ddr(gpio_num, s_v);
}

static bool gpio_set_inten(int gpio_num, u32 v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift;
    u32 s_v;

    if (gpio_num >= 0 && gpio_num <= 31) {
        s_v = gpio_get_porta_inten();
        s_v &= ~mask;
        s_v |= (v << shift);

        gpio_set_porta_inten(s_v);
        return true;
    }

    return false;
}

static bool gpio_set_intmask(int gpio_num, u32 v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift;
    u32 s_v;

    if (gpio_num >= 0 && gpio_num <= 31) {
        s_v = gpio_get_porta_intmask();
        s_v &= ~mask;
        s_v |= (v << shift);

        gpio_set_porta_intmask(s_v);
        return true;
    }
    return false;
}

static bool gpio_set_inttype_level(int gpio_num, u32 v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift;
    u32 s_v;

    if (gpio_num >= 0 && gpio_num <= 31) {
        s_v = gpio_get_porta_inttype_level();
        s_v &= ~mask;
        s_v |= (v << shift);

        gpio_set_porta_inttype_level(s_v);
        return true;
    }
    return false;
}

static bool gpio_set_int_polarity(int gpio_num, u32 v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift;
    u32 s_v;

    if (gpio_num >= 0 && gpio_num <= 31) {
        s_v = gpio_get_porta_int_polarity();
        s_v &= ~mask;
        s_v |= (v << shift);

        gpio_set_porta_int_polarity(s_v);
        return true;
    }
    return false;
}

static bool gpio_get_direction(int gpio_num, u32 *v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift;

    if (gpio_get_portx_ddr(gpio_num, v) == true) {
        *v = (*v & mask) >> shift;
        return true;
    }

    return false;
}

static bool gpio_set_output_data(int gpio_num, int v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift, dir_v;
    u32 s_v;

    if (gpio_get_direction(gpio_num, &dir_v) == false)
        return false;

    if (dir_v == GPIO_DDR_INPUT)
        return false;

    if (gpio_get_portx_dr(gpio_num, &s_v) == false)
        return false;

    s_v &= ~mask;
    s_v |= v << shift;

    return gpio_set_portx_dr(gpio_num, s_v);
}

/***#################################################**/
bool gpio_enable(int gpio_num)
{
    return gpio_set_ctl(gpio_num, GPIO_CTL_SOFT);
}

bool gpio_disable(int gpio_num)
{
    return gpio_set_ctl(gpio_num, GPIO_CTL_HW);
}

bool gpio_enable_interrupt(int gpio_num) // enable this pin to interupt, nor gpio function
{
    return gpio_set_inten(gpio_num, GPIO_PORTA_INTEN);
}

bool gpio_disable_interrupt(int gpio_num)
{
    return gpio_set_inten(gpio_num, GPIO_PORTA_NORMAL);
}

bool gpio_interrupt_mask(int gpio_num)
{
    return gpio_set_intmask(gpio_num, GPIO_INT_MASK);
}

bool gpio_interrupt_unmask(int gpio_num)
{
    return gpio_set_intmask(gpio_num, GPIO_INT_UNMASK);
}

bool gpio_interrupt_type_set_level(int gpio_num)
{
    return gpio_set_inttype_level(gpio_num, GPIO_INTTYPE_LEVEL);
}

bool gpio_interrupt_type_set_edge(int gpio_num)
{
    return gpio_set_inttype_level(gpio_num, GPIO_INTTYPE_EDGE);
}

bool gpio_interrupt_polarity_set_lowlevel(int gpio_num)
{
    return gpio_set_int_polarity(gpio_num, GPIO_INT_POLARITY_LOW);
}

bool gpio_interrupt_polarity_set_highlevel(int gpio_num)
{
    return gpio_set_int_polarity(gpio_num, GPIO_INT_POLARITY_HIGH);
}

bool gpio_interrupt_polarity_set_falling_edge(int gpio_num)
{
    return gpio_set_int_polarity(gpio_num, GPIO_INT_POLARITY_LOW);
}

bool gpio_interrupt_polarity_set_rising_edge(int gpio_num)
{
    return gpio_set_int_polarity(gpio_num, GPIO_INT_POLARITY_HIGH);
}

u32 gpio_interupt_get_intstatus(void)
{
    return gpio_get_porta_intstatus();
}

u32 gpio_interupt_get_raw_intstatus(void)
{
    return gpio_get_porta_raw_intstatus();
}

void gpio_debounce_enable(void)
{
    gpio_set_debounce(GPIO_DEBOUNCE_EN);
}

void gpio_debounce_disable(void)
{
    gpio_set_debounce(0);
}

bool gpio_set_output_direction(int gpio_num)
{
    return gpio_set_direction(gpio_num, GPIO_DDR_OUTPUT);
}

bool gpio_set_output_high(int gpio_num)
{
    return gpio_set_output_data(gpio_num, 1);
}

bool gpio_set_output_low(int gpio_num)
{
    return gpio_set_output_data(gpio_num, 0);
}

bool gpio_set_input_direction(int gpio_num)
{
    return gpio_set_direction(gpio_num, GPIO_DDR_INPUT);
}

void gpio_interrupt_clr(int irq)
{
    gpio_set_porta_eoi(irq);
}

bool gpio_get_input_data(int gpio_num, u32 *v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift, dir_v;

    if (gpio_get_direction(gpio_num, &dir_v) == false)
        return false;

    if (dir_v == GPIO_DDR_OUTPUT)
        return false;

    if (gpio_get_portx_input_data(gpio_num, v) == true) {
        *v = ((*v & mask) >> shift);
        return true;
    }
    return false;
}

bool gpio_get_output_data(int gpio_num, u32 *v)
{
    int shift = gpio_num % 32;
    u32 mask = 1 << shift, dir_v;

    if (gpio_get_direction(gpio_num, &dir_v) == false)
        return false;

    if (dir_v == GPIO_DDR_INPUT)
        return false;

    if (gpio_get_portx_dr(gpio_num, v) == true) {
        *v = ((*v & mask) >> shift);
        return true;
    }
    return false;
}
