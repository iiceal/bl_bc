OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
MEMORY
{
    sram (rwx): ORIGIN = 0x300000, LENGTH = ((0x00100000) + 0x00200000 - 0x300000 - (8 * 1024) - (8*1024)) - (16*1024) - ((8*1024) + (4*1024)) - (0)
}
REGION_ALIAS("REGION_TEXT", sram);
REGION_ALIAS("REGION_DATA", sram);
REGION_ALIAS("REGION_BSS", sram);
SECTIONS
{
 .text : {
  *head-in.o (.text)
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
