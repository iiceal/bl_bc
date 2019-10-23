ifndef CROSS_COMPILE
CROSS_COMPILE   = arm-eabi-
endif

PLATFORM_LIBGCC  = -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc 
PLATFORM_CFLAGS  = -march=armv7-a $(call cc-option,-march=armv7-a)
#PLATFORM_CFLAGS += -nostdlib -fno-common -ffixed-r8 -msoft-float
#PLATFORM_CFLAGS += -mfloat-abi=hard -mfpu=neon 
ifeq ($(CONFIG_ENABLE_UNWIND_FUNCTION), true)
PLATFORM_CFLAGS += -msoft-float
else
#PLATFORM_CFLAGS += -mfloat-abi=hard -mfpu=vfpv4-d16  
PLATFORM_CFLAGS += -mfloat-abi=hard -mfpu=neon  
endif
PLATFORM_CFLAGS += -mno-thumb-interwork -fno-stack-protector -fno-builtin -ffreestanding
PLATFORM_CFLAGS += -fno-exceptions -fno-non-call-exceptions -Wall -Werror -Wstrict-prototypes

ifeq ($(CONFIG_ENABLE_UNWIND_FUNCTION), true)
PLATFORM_CFLAGS +=  -funwind-tables -O0 -DCONFIG_BUILD_ENABLE_UNWIND
else
PLATFORM_CFLAGS +=  -fno-unwind-tables -Os
endif

