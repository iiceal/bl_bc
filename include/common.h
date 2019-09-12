#ifndef COMMON__H__
#define	COMMON__H__

#include <stdarg.h>
#include <linux/ctype.h>
#include <errno.h>
#include "hg_types.h"
#include <exports.h>
#include <malloc.h>
#include <vsprintf.h>
#include <bl_shell.h>
#include <bl_common.h>
#include <gic400.h>
#include <irq.h>
#include <irq_defs.h>
#include <dump_stack.h>
#include <misc.h>

#include "global_macros.h"
#include "string.h"
#include "chip_config.h"
#include "serial.h"
#include "timer.h"
#include "crc.h"

#ifdef CONFIG_LOAD_XMODEM
#include "xyzModem.h"
#endif // CONFIG_LOAD_XMODEM 

typedef unsigned char		uchar;
typedef unsigned long 		ulong;
#endif
