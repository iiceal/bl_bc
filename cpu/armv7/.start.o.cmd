cmd_cpu/armv7/start.o := arm-eabi-gcc -Wp,-MD,cpu/armv7/.start.o.d  -nostdinc -isystem /opt/gcc/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include   -D__ASSEMBLY__  -march=armv7-a -mno-thumb-interwork -fno-stack-protector -fno-builtin -ffreestanding -fno-exceptions -fno-non-call-exceptions -Wall -Werror -Wstrict-prototypes -include include/generated/autoconf.h  -mfloat-abi=hard -mfpu=vfpv4-d16 -fno-unwind-tables -Os  -I./chip/bp2016/include/ -I./include/ -I./include/asm/         -c -o cpu/armv7/start.o cpu/armv7/start.S

source_cpu/armv7/start.o := cpu/armv7/start.S

deps_cpu/armv7/start.o := \
    $(wildcard include/config/sys/init/sp/addr.h) \
    $(wildcard include/config/exception/stack/start.h) \
    $(wildcard include/config/use/irq.h) \
    $(wildcard include/config/irq/stack/start.h) \
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
    $(wildcard include/config/stacksize/svc.h) \
    $(wildcard include/config/stacksize/irq.h) \
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

cpu/armv7/start.o: $(deps_cpu/armv7/start.o)

$(deps_cpu/armv7/start.o):
