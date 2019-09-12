#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "chip_config.h"
#include "hg_types.h"
#include <stdarg.h>

struct tty {
	int initialized;
	int (*tstc)(void);
	void (*putc)(const char c);
	void (*puts)(const char *s);
	int (*getc)(void);
};

void dbg_serial_init(int baudrate);
void upv_serial_init(int baudrate);
void serial_enable_rtscts(void);
int  serial_getc(void);
int  serial_tstc(void);
void serial_putc(const char c);
void serial_puts(const char *s);
int ctrlc(void);

#endif /* _SERIAL_H_ */
