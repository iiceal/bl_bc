deps_config := \
	cmd/Kconfig \
	net/lwip/Kconfig \
	net/lwip/port/Kconfig \
	net/Kconfig \
	fs/jffs2/Kconfig \
	fs/Kconfig \
	drivers/net/Kconfig \
	drivers/sd/Kconfig \
	drivers/ddr/Kconfig \
	drivers/gpio/Kconfig \
	drivers/cpu_right/Kconfig \
	drivers/clk/Kconfig \
	drivers/iomux/Kconfig \
	drivers/wdt/Kconfig \
	drivers/reset/Kconfig \
	drivers/uart/Kconfig \
	drivers/dma/Kconfig \
	drivers/spi/Kconfig \
	drivers/i2c/Kconfig \
	drivers/sim/Kconfig \
	drivers/qspi/Kconfig \
	drivers/Kconfig \
	common/Kconfig \
	boot/Kconfig \
	cpu/Kconfig \
	copy_bl/Kconfig \
	chip/Kconfig \
	boards/Kconfig \
	Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(HG_BL_VERSION)" "1"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
