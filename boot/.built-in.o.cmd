cmd_boot/built-in.o :=  arm-eabi-ld    -r -o boot/built-in.o boot/serial.o boot/timer.o boot/fastboot.o boot/main.o 
