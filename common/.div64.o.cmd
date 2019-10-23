cmd_common/div64.o := arm-eabi-gcc -Wp,-MD,common/.div64.o.d  -nostdinc -isystem /opt/gcc/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include  -march=armv7-a -mthumb -mno-thumb-interwork -fno-stack-protector -fno-builtin -ffreestanding -fno-exceptions -fno-non-call-exceptions -Wall -Werror -Wstrict-prototypes -include include/generated/autoconf.h -DHGVERSION=\"2ff606f7\" -DHGBUILDDATE=\"2019-10-23\" -mfloat-abi=hard -mfpu=vfpv4-d16 -fno-unwind-tables -Os -I./chip/bp2016/include/ -I./include/ -I./include/asm/    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(div64)"  -D"KBUILD_MODNAME=KBUILD_STR(div64)" -c -o common/div64.o common/div64.c

source_common/div64.o := common/div64.c

deps_common/div64.o := \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/use/stdint.h) \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/asm/posix_types.h \
  include/asm/types.h \
    $(wildcard include/config/arm64.h) \
  /opt/gcc/arm-eabi-4.8/lib/gcc/arm-eabi/4.8/include/stdbool.h \

common/div64.o: $(deps_common/div64.o)

$(deps_common/div64.o):
