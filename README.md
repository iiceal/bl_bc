# this is hg bp2016 bootloader
- TEST wujinying push permission

- The config system according to the Linux Kernel Kconfig/Kbuild/Makefile

## HOW to configuration
```
make bp2016_asic_defconfig
make menuconfig
```

## HOW to compile
1. use script like this
```
./make.sh
```

2. use make
```
make clean
make
```

## HOW to add new driver in drivers directory
1. add you config in drivers/Kconfig like I2C
2. add you config in drivers/Kbuild like I2C
3. add a Kbuild in you driver directory
