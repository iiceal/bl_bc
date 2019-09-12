#ifndef __EXPORTS_H__
#define __EXPORTS_H__

unsigned long get_version(void);
int  getc(void);
int  tstc(void);
void putc(const char);
void puts(const char*);
int printf(const char* fmt, ...);
void *malloc(size_t);
void free(void*);
void __udelay(unsigned long);
int vprintf(const char *, va_list);
unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
int strict_strtoul(const char *cp, unsigned int base, unsigned long *res);
long simple_strtol(const char *cp,char **endp,unsigned int base);
int strcmp(const char * cs,const char * ct);
unsigned long ustrtoul(const char *cp, char **endp, unsigned int base);
void __assert_fail(const char *assertion, const char *file, unsigned line,
                   const char *function);

int interrupt_init (void);
#endif	/* __EXPORTS_H__ */
