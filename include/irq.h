#ifndef IRQ__H__
#define	IRQ__H__
typedef int (*irq_handler_t)(int, void *);
struct irq_desc {
    int           irq;
    void *        dev_id;
    irq_handler_t thread_fn;
    U32           irq_count;
    const char *  name;
    u32           cur_time;
    u32           max_time;
    u32           total_time;
};
extern void gic_handle_irq(void);
extern int request_irq(unsigned int irq, irq_handler_t handler,
			 const char *devname, void *dev_id);
extern int unrequest_irq(unsigned int irq);
#endif
