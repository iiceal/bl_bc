export CROSS_COMPILE
export PLATFORM_LIBGCC
export OBJ_FILES
export PLATFORM_BASE

ifndef CROSS_COMPILE
CROSS_COMPILE    = arm-eabi-
endif

include ./cpu.mk
CC               = $(CROSS_COMPILE)gcc
PP               = $(CROSS_COMPILE)gcc -P -E 
CPP 		 = $(CROSS_COMPILE)g++
OBJDUMP          = $(CROSS_COMPILE)objdump
OBJCOPY          = $(CROSS_COMPILE)objcopy
LD 		 = $(CROSS_COMPILE)ld
AR		 = $(CROSS_COMPILE)ar


TARGET_BASE      = fastboot

TARGET_BIN       = $(TARGET_BASE).bin
TARGET_ELF       = $(TARGET_BASE).elf
TARGET_ASCII_BIN = $(TARGET_BASE).binary
TARGET_ASCII_HEX = $(TARGET_BASE).hex
TARGET_LST       = $(TARGET_BASE).lst
MAP_FILE         = $(TARGET_BASE).map
TARGET_LINKFILE  = $(TARGET_BASE).ld


include ld.mk
include srcs.mk

define print_info
	printf "\nFastboot Size:\n"
	size $1
	printf "\nBiggest object files:\n"
	$(OBJDUMP) -t $1 | egrep '\.text|\.rodata' \
	   | sed 's/^[^.]*\([.][^ \t]*\)[ \t]*\([^ \t]*\)/\2 \1 /' \
	   | sort \
	   | awk '{a=strtonum("0x"$$1);sum+=a;printf("%-35s(%-8s) %5d (%d)\n",$$3,$$2,a,sum);}' \
	| tail
endef


all: $(TARGET_LINKFILE) $(TARGET_BIN) $(TARGET_LST) 

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

$(TARGET_LST): $(TARGET_ELF)
	$(OBJDUMP) -D $(TARGET_ELF) > $@
 
$(TARGET_ELF): $(OBJ_FILES)
	@echo "cflags************: $(CFLAGS)\n"
	$(LD) -o $@ $^ $(LDFLAGS)
	-@$(call print_info, $@)

$(TARGET_LINKFILE):
	$(PP) $(CFLAGS) $(PPASFLAGS) -c -o $(TARGET_LINKFILE) $(LD_INPUT_S)

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@ 

%.o: %.S
	$(CC) $(AFLAGS) -g -c $< -o $@ 


clean:
	rm -f $(TARGET_LINKFILE)
	rm -f $(OBJ_FILES)
	rm -f *.elf *.bin *.map *.img *.hex* *.lst

