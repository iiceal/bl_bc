#include <common.h>
#include <irq.h>
#include "gic400.h"

static struct irq_desc irq_descs[NB_SYS_IRQ];
u64                    e_time = 0, s_time = 0;

static void irq_enter(int irq)
{
    s_time = arch_counter_get_cntpct();
}

static void irq_exit(int irq)
{
    struct irq_desc *d = NULL;
    e_time = arch_counter_get_cntpct();

    d           = &irq_descs[irq - 32];
    d->cur_time = tick_to_us((unsigned long) (e_time - s_time));
    d->max_time = ((d->cur_time) > (d->max_time)) ? (d->cur_time) : (d->max_time);
    d->total_time += d->cur_time;
}

struct irq_desc *get_irq_desc_addr(void)
{
    return irq_descs;
}

static void generic_handle_ppi(int irqnr)
{

}

static void generic_handle_irq(int irqnr)
{
	struct irq_desc  * d = NULL;
	int irq = irqnr - 32;
	if(irq < NB_SYS_IRQ) {
		d = &irq_descs[irq];
		if((d->irq == irq) && (d->thread_fn != NULL)) {
			d->thread_fn(irq, d->dev_id);
			d->irq_count ++;
		}
	}else {
		printf("irq = %d(%d), invalid interrupt number!\n", irqnr, irq);
	}
}

void gic_handle_irq(void)
{
	int intack = gic_ack_irq();
	int irq = intack;
	//printf("gic_handle_irq :%d\n", intack);
	if(intack < 32 || intack > 1020) {
		printf("bad irq, irq = %d\n", irq);
		return;
	}
	if(irq >= 32 && irq < 1021) {
        irq_enter(irq);
		generic_handle_irq(irq);
        irq_exit(irq);
	} else if(irq < 32 && irq > 15) {
		generic_handle_ppi(irq);
	} else {
		//ipi
	}
	gic_eoi_irq(intack);
}

int request_irq(unsigned int irq, irq_handler_t handler,
			 const char *devname, void *dev_id)
{
	struct irq_desc  * d = NULL;
	if(irq >= NB_SYS_IRQ) {
		printf("invalid interrupt number:%d, must be < %d\n",
				irq, NB_SYS_IRQ);
		return -1;
	}
	if(handler == NULL) {
		printf("Handler must be !NULL\n");
		return -1;
	}
	d = &irq_descs[irq];
	d->irq_count  = 0;
	d->thread_fn = handler;
	d->dev_id = dev_id;
	d->name = devname;
	d->irq = irq;
	d->cur_time = 0;
	d->max_time = 0;
    d->total_time = 0;
	gic_unmask_irq(irq + 32);
	return 0;
}

int unrequest_irq(unsigned int irq)
{
	struct irq_desc  * d = NULL;
	if(irq >= NB_SYS_IRQ) {
		printf("invalid interrupt number:%d, must be < %d\n",
				irq, NB_SYS_IRQ);
		return -1;
	}
	gic_mask_irq(irq + 32);
	d = &irq_descs[irq];
	d->irq_count  = 0;
	d->thread_fn = NULL;
	d->dev_id = NULL;
	d->cur_time = 0;
	d->max_time = 0;
    d->total_time = 0;
	d->name = NULL;
	d->irq = 0;
	return 0;
}
