cmd_common/ctype.o := arm-eabi-gcc -Wp,-MD,common/.ctype.o.d  -nostdinc -isystem /opt/gcc/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include  -march=armv7-a -mthumb -mno-thumb-interwork -fno-stack-protector -fno-builtin -ffreestanding -fno-exceptions -fno-non-call-exceptions -Wall -Werror -Wstrict-prototypes -include include/generated/autoconf.h -DHGVERSION=\"2ff606f7\" -DHGBUILDDATE=\"2019-10-23\" -mfloat-abi=hard -mfpu=vfpv4-d16 -fno-unwind-tables -Os -I./chip/bp2016/include/ -I./include/ -I./include/asm/    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ctype)"  -D"KBUILD_MODNAME=KBUILD_STR(ctype)" -c -o common/ctype.o common/ctype.c

source_common/ctype.o := common/ctype.c

deps_common/ctype.o := \
  include/linux/ctype.h \

common/ctype.o: $(deps_common/ctype.o)

$(deps_common/ctype.o):
