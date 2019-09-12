#include <chip_config.h>

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
#ifdef CONFIG_BUILD_STATIC_LIBS
GROUP( libtarget.a libgcc.a)  /*for build libtarget.a*/
#endif

MEMORY
{
    sram (rwx):  ORIGIN = CONFIG_SRAM_START, LENGTH = CONFIG_SRAM_BL_START - CONFIG_SRAM_START
}

REGION_ALIAS("REGION_TEXT", sram);
REGION_ALIAS("REGION_DATA", sram);
REGION_ALIAS("REGION_BSS", sram);
SECTIONS
{
	.text :	{
		*head-in.o	(.text)
		 __cmd_descriptor_handle_begin = .;
		   KEEP(*(func.get_case_descriptor))
		 __cmd_descriptor_handle_end = .;
		. = ALIGN(4);
		KEEP(*(.spiflash_input_data))
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
		. = ALIGN(4);
		*(.text*)
		. = ALIGN(8);
	} > REGION_TEXT

	__code_end__ = .;

	.data : {
		__data_start = .;
		*(.data*)
		__data_end__ = .;
		. = ALIGN(4);
	} > REGION_DATA

	.rel.dyn : {
		__rel_dyn_start = .;
		*(.rel*)
		__rel_dyn_end = .;
	} > REGION_DATA

	.dynsym : {
		__dynsym_start = .;
		*(.dynsym)
	} > REGION_DATA

#ifdef CONFIG_BUILD_ENABLE_UNWIND
	. = ALIGN(8);
    .ARM.exidx : {
    __exidx_start = .; 
        *(.ARM.exidx)
        *(.gnu.linkonce.armexidx)
    __exidx_end = .; 
     } > sram
    .ARM.extab : {
    __extab_start = .; 
        *(.ARM.extab)
        *(.gnu.linkonce.armextab)
    __extab_end = .; 
     } > sram
#endif

	_end = .;

	.bss _end (OVERLAY) : {
		__bss_start = .;
		*(.bss*)
		*(COMMON)
		 . = ALIGN(4);
		__bss_end__ = .;
	} > REGION_BSS
	ASSERT(!((__bss_end__ - ORIGIN(sram)) >= LENGTH(sram)), "program memory overrun error!!, please reduce BLcase number")
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
}
