LD_INPUT_S    = ./in_iram.ld.S

LDFLAGS         = $(PLATFORM_LIBGCC) -nostdlib --script $(TARGET_LINKFILE) -Map $(MAP_FILE)
PPASFLAGS       = -P -E -D__LINKER__


