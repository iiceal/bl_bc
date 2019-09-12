#include <bl_common.h>
#include <drivers/drv_reset.h>

static BL_CMD_T reset_descriptor;

static u32 reset_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
	if ((argc >= 2) && (strcmp(argv[1], "cpu1") == 0)) {
		swrst_cpu1_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "ddr") == 0)) {
		swrst_ddr_reset();
		return RET_PASS;
	}
#if 0
	if((argc >=2) && (strcmp(argv[1], "62m") == 0)) {
		swrst_62M_reset();
		return RET_PASS;
	}
#endif
	if((argc >=2) && (strcmp(argv[1], "rom") == 0)) {
		swrst_rom_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "ram") == 0)) {
		swrst_ram_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "bb") == 0)) {
		swrst_bb_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "mp") == 0)) {
		swrst_mp_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "dmac0") == 0)) {
		swrst_dmac0_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "dmac1") == 0)) {
		swrst_dmac1_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "qspi") == 0)) {
		swrst_qspi_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "sd") == 0)) {
		swrst_sd_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "uart0") == 0)) {
		swrst_uart0_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "uart1") == 0)) {
		swrst_uart1_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "uart2") == 0)) {
		swrst_uart2_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "uart3") == 0)) {
		swrst_uart3_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "uart4") == 0)) {
		swrst_uart4_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "uart5") == 0)) {
		swrst_uart5_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "uart6") == 0)) {
		swrst_uart6_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "uart7") == 0)) {
		swrst_uart7_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "i2c0") == 0)) {
		swrst_i2c0_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "i2c1") == 0)) {
		swrst_i2c1_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "spi0") == 0)) {
		swrst_spi0_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "spi1") == 0)) {
		swrst_spi1_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "spi2") == 0)) {
		swrst_spi2_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "spi3") == 0)) {
		swrst_spi3_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "sim0") == 0)) {
		swrst_sim0_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "sim1") == 0)) {
		swrst_sim1_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "sim2") == 0)) {
		swrst_sim2_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "pwm") == 0)) {
		swrst_pwm_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "gpio") == 0)) {
		swrst_gpio_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "timer") == 0)) {
		swrst_timer_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "greset") == 0)) {
		swrst_global_reset();
		return RET_PASS;
	}

	if((argc >=2) && (strcmp(argv[1], "gic") == 0)) {
		swrst_gic_reset();
		return RET_PASS;
	}

	return RET_NA; 
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: reset cpu1/ddr/ram etc.\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: The applicaition running command is as follow:\n\
                reset cpu1/ddr/rom/ram/bb/mp/dmac0/dmac1 \n\
                reset qspi/sd/uart[0~7]/i2c[0/1]/spi[0~3] \n\
                reset sim[0~2]/pwm/gpio/timer/greset/gic \n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &reset_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "reset", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = reset_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
