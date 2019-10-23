
AOBJ_FILES   = ./start.o
AOBJ_FILES  += ./v7_cp15.o
AOBJ_FILES  += ./input_bl.o
COBJ_FILES   = ./copy_loader.o 


PLATFORM_INCLUDE := ./include
CFLAGS      = $(PLATFORM_CFLAGS) -I$(PLATFORM_INCLUDE)
AFLAGS      = $(PLATFORM_CFLAGS) -Iinclude -I$(PLATFORM_INCLUDE)

OBJ_FILES   += $(COBJ_FILES) $(AOBJ_FILES)

