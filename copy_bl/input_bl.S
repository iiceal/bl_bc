.section .spiflash_input_data,#alloc
.globl  input_data_bl
input_data_bl:
.incbin "./copy_bl/bloader_bp2016.bin"
.globl  input_data_bl_end
input_data_bl_end: 
