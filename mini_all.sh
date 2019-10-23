#!/bin/sh -x

export PATH=/opt/gcc/arm-eabi-4.8/bin/:$PATH

make bp2016_oem_beta_defconfig
make clean
make -j4 V=1
#find . -name "bloader_bp2016.bin" |xargs -i cp {} ./copy_bl/

cd ./fastboot_bl/ && make -j4 V=1
mv fastboot.*  ../
make clean
cd ..
#make -C copy_bl/ -j4 V=1
#make clean
#mv copy_bl/fastboot* ./
