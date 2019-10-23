cmd_drivers/ddr/ddr_burst_op.o := arm-eabi-gcc -Wp,-MD,drivers/ddr/.ddr_burst_op.o.d  -nostdinc -isystem /opt/gcc/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include   -D__ASSEMBLY__  -march=armv7-a -mno-thumb-interwork -fno-stack-protector -fno-builtin -ffreestanding -fno-exceptions -fno-non-call-exceptions -Wall -Werror -Wstrict-prototypes -include include/generated/autoconf.h  -mfloat-abi=hard -mfpu=vfpv4-d16 -fno-unwind-tables -Os  -I./chip/bp2016/include/ -I./include/ -I./include/asm/         -c -o drivers/ddr/ddr_burst_op.o drivers/ddr/ddr_burst_op.S

source_drivers/ddr/ddr_burst_op.o := drivers/ddr/ddr_burst_op.S

deps_drivers/ddr/ddr_burst_op.o := \

drivers/ddr/ddr_burst_op.o: $(deps_drivers/ddr/ddr_burst_op.o)

$(deps_drivers/ddr/ddr_burst_op.o):
