#ifndef GIC400_H_
#define	GIC400_H_
extern void gic_init(void);
extern void gic_mask_irq(int irq);
extern void gic_unmask_irq(int irq);
extern void gic_eoi_irq(int irq);
extern void gic_set_type(int irq, int type);
extern int gic_ack_irq(void);
#endif
