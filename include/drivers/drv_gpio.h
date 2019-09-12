#ifndef __DRV_GPIO_H_
#define __DRV_GPIO_H_

bool gpio_enable(int gpio_num);
bool gpio_disable(int gpio_num);
bool gpio_enable_interrupt(int gpio_num);
bool gpio_disable_interrupt(int gpio_num);
bool gpio_interrupt_mask(int gpio_num);
bool gpio_interrupt_unmask(int gpio_num);
bool gpio_interrupt_type_set_level(int gpio_num);
bool gpio_interrupt_type_set_edge(int gpio_num);
bool gpio_interrupt_polarity_set_lowlevel(int gpio_num);
bool gpio_interrupt_polarity_set_highlevel(int gpio_num);
bool gpio_interrupt_polarity_set_falling_edge(int gpio_num);
bool gpio_interrupt_polarity_set_rising_edge(int gpio_num);
u32 gpio_interupt_get_intstatus(void);
u32 gpio_interupt_get_raw_intstatus(void);
void gpio_debounce_enable(void);
void gpio_debounce_disable(void);
bool gpio_set_output_direction(int gpio_num);
bool gpio_set_output_high(int gpio_num);
bool gpio_set_output_low(int gpio_num);
bool gpio_set_input_direction(int gpio_num);
bool gpio_get_input_data(int gpio_num, u32 *v);
bool gpio_get_output_data(int gpio_num, u32 *v);

void gpio_interrupt_clr(int irq);
#endif  /*__DRV_GPIO_H_*/
