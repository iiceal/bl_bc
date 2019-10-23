#!/bin/sh -x

export PATH=/opt/gcc/arm-eabi-4.8/bin/:$PATH

make bp2016_oem_fast_defconfig
make clean
make -j4 V=1

cd ./fastboot_bl/ && make -j4 V=1
mv fastboot.*  ../
make clean
cd ..
