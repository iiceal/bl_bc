cmd_boards/bp2016_oem/board.o := arm-eabi-gcc -Wp,-MD,boards/bp2016_oem/.board.o.d  -nostdinc -isystem /opt/gcc/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include  -march=armv7-a -mthumb -mno-thumb-interwork -fno-stack-protector -fno-builtin -ffreestanding -fno-exceptions -fno-non-call-exceptions -Wall -Werror -Wstrict-prototypes -include include/generated/autoconf.h -DHGVERSION=\"2ff606f7\" -DHGBUILDDATE=\"2019-10-23\" -mfloat-abi=hard -mfpu=vfpv4-d16 -fno-unwind-tables -Os -I./chip/bp2016/include/ -I./include/ -I./include/asm/    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(board)"  -D"KBUILD_MODNAME=KBUILD_STR(board)" -c -o boards/bp2016_oem/board.o boards/bp2016_oem/board.c

source_boards/bp2016_oem/board.o := boards/bp2016_oem/board.c

deps_boards/bp2016_oem/board.o := \
    $(wildcard include/config/drv/ddr.h) \
    $(wildcard include/config/boot/from/qspi.h) \
  include/common.h \
    $(wildcard include/config/load/xmodem.h) \
  /opt/gcc/arm-eabi-4.8/lib/gcc/arm-eabi/4.8/include/stdarg.h \
  include/linux/ctype.h \
  include/errno.h \
    $(wildcard include/config/errno/str.h) \
  include/asm/errno.h \
  include/hg_types.h \
  include/exports.h \
  include/malloc.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/use/stdint.h) \
  include/linux/posix_types.h \
  include/asm/posix_types.h \
  include/asm/types.h \
    $(wildcard include/config/arm64.h) \
  /opt/gcc/arm-eabi-4.8/lib/gcc/arm-eabi/4.8/include/stdbool.h \
  include/vsprintf.h \
    $(wildcard include/config/sys/vsnprintf.h) \
  include/bl_shell.h \
  include/bl_common.h \
  include/gic400.h \
  include/irq.h \
  chip/bp2016/include/irq_defs.h \
  include/dump_stack.h \
  include/misc.h \
  include/global_macros.h \
  include/string.h \
  chip/bp2016/include/chip_config.h \
    $(wildcard include/config/rom/h//.h) \
    $(wildcard include/config/timer/freq.h) \
    $(wildcard include/config/counter/freq.h) \
    $(wildcard include/config/apb/clock.h) \
    $(wildcard include/config/serial/baudrate.h) \
    $(wildcard include/config/rom/start.h) \
    $(wildcard include/config/rom/size.h) \
    $(wildcard include/config/sram/start.h) \
    $(wildcard include/config/sram/size.h) \
    $(wildcard include/config/bb/mem/start.h) \
    $(wildcard include/config/bb/mem/size.h) \
    $(wildcard include/config/dram/start.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/boot/rsvaddr/size.h) \
    $(wildcard include/config/boot/rsvaddr/start.h) \
    $(wildcard include/config/rom/run/range.h) \
    $(wildcard include/config/rom/rsvaddr.h) \
    $(wildcard include/config/pgtable/size.h) \
    $(wildcard include/config/ttb/base.h) \
    $(wildcard include/config/sys/init/sp/addr.h) \
    $(wildcard include/config/stacksize/svc.h) \
    $(wildcard include/config/stacksize/irq.h) \
    $(wildcard include/config/irq/stack/start.h) \
    $(wildcard include/config/exception/stack/start.h) \
    $(wildcard include/config/sp/max/size.h) \
    $(wildcard include/config/common/malloc.h) \
    $(wildcard include/config/sys/malloc/len.h) \
    $(wildcard include/config/malloc/len/kb.h) \
    $(wildcard include/config/sys/malloc/start.h) \
    $(wildcard include/config/sram/bl/start.h) \
    $(wildcard include/config/sram/bl/size.h) \
    $(wildcard include/config/sys/pbsize.h) \
  chip/bp2016/include/iomap.h \
    $(wildcard include/config/dbg/serial/num.h) \
    $(wildcard include/config/upv/serial/num.h) \
  include/serial.h \
  include/timer.h \
  include/crc.h \
  include/drivers/drv_ddr.h \
    $(wildcard include/config/enable/trace.h) \
  include/flash.h \
    $(wildcard include/config/sys/max/flash/sect.h) \
    $(wildcard include/config/sys/flash/cfi.h) \
    $(wildcard include/config/mtd.h) \
    $(wildcard include/config/cfi/flash.h) \
    $(wildcard include/config/flash/cfi/mtd.h) \
    $(wildcard include/config/sys/flash/protection.h) \
    $(wildcard include/config/flash/cfi/legacy.h) \

boards/bp2016_oem/board.o: $(deps_boards/bp2016_oem/board.o)

$(deps_boards/bp2016_oem/board.o):
