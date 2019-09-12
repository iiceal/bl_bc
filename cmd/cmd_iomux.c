#include <bl_common.h>
#include <drivers/iomux/iomux_hw.h>

static BL_CMD_T iomux_descriptor;

static u32 iomux_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    int index = 0;

    if (argc < 2) {
        printf("Iomux: command error!, too few arguments\n");
        return RET_NA;
    }

    if (strcmp(argv[1], "sim") == 0) {
        index = simple_strtoul(argv[2], NULL, 10);
        printf("sim %d\n", index);

        if (DRV_OP_SUCCESS != scm_sim_en(index)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "mmc") == 0) {
        index = simple_strtoul(argv[2], NULL, 10);
        if (DRV_OP_SUCCESS != scm_mmc_en(index)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "uart") == 0) {
        index = simple_strtoul(argv[2], NULL, 10);
        if (DRV_OP_SUCCESS != scm_uart_en(index, 1)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "prm0") == 0) {
        scm_prm0_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "prm1") == 0) {
        scm_prm1_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "asram") == 0) {
        scm_asram_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "pps0") == 0) {
        scm_pps0();
        return RET_PASS;
    }

    if (strcmp(argv[1], "pps1") == 0) {
        scm_pps1();
        return RET_PASS;
    }

    if (strcmp(argv[1], "clk5m") == 0) {
        scm_clk5m();
        return RET_PASS;
    }

    if (strcmp(argv[1], "i2c0") == 0) {
        scm_i2c0_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "i2c1") == 0) {
        scm_i2c1_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "odo") == 0) {
        scm_odo_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "irigb") == 0) {
        scm_irigb_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "qspi") == 0) {
        scm_qspi_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "spi0") == 0) {
        scm_spi0_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "spi1") == 0) {
        if (DRV_OP_SUCCESS != scm_spi1_en(1, 1)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "spi2") == 0) {
        if (DRV_OP_SUCCESS != scm_spi2_en(1, 1, 1, 1, 1, 1, 1, 1)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "spi3") == 0) {
        scm_spi3_en();
        return RET_PASS;
    }

    if (strcmp(argv[1], "spi4") == 0) {
        if (DRV_OP_SUCCESS != scm_spi4_en(1, 1)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "pwm0") == 0) {
        scm_hwpwm0();
        return RET_PASS;
    }

    if (strcmp(argv[1], "pwm1") == 0) {
        scm_hwpwm1();
        return RET_PASS;
    }

    if (strcmp(argv[1], "pwm2") == 0) {
        scm_hwpwm2();
        return RET_PASS;
    }

    if (strcmp(argv[1], "pwm3") == 0) {
        scm_hwpwm3();
        return RET_PASS;
    }

    if (strcmp(argv[1], "spwm0") == 0) {
        scm_swpwm0();
        return RET_PASS;
    }

    if (strcmp(argv[1], "spwm1") == 0) {
        scm_swpwm1();
        return RET_PASS;
    }

    if (strcmp(argv[1], "gpioa") == 0) {
        index = simple_strtoul(argv[2], NULL, 10);
        if (DRV_OP_SUCCESS != scm_gpioa_en(index)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "gpiob") == 0) {
        index = simple_strtoul(argv[2], NULL, 10);
        if (DRV_OP_SUCCESS != scm_gpiob_en(index)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "gpioc") == 0) {
        index = simple_strtoul(argv[2], NULL, 10);
        if (DRV_OP_SUCCESS != scm_gpioc_en(index)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "gpiod") == 0) {
        index = simple_strtoul(argv[2], NULL, 10);
        if (DRV_OP_SUCCESS != scm_gpiod_en(index)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    if (strcmp(argv[1], "if") == 0) {
        index = simple_strtoul(argv[2], NULL, 10);
        if (DRV_OP_SUCCESS != scm_if_en(index)) {
            return RET_FAIL;
        } else
            return RET_PASS;
    }

    return RET_NA;
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: iomux sim[0~2] mmc[0/1] uart[0~7] prm0 prm1 asram etc.\n\
                              iomux pwm0 pwm1 pwm2 pwm3 spwm0 spwm1 etc. \n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: The applicaition running command is as follow:\n\
                iomux sim [0~2] mmc [0/1] uart [0~7] prm0 prm1 asram  \n\
                iomux pps0 pps1 clk5m i2c0 i2c1 odo irigb\n\
                iomux spi0 spi1 spi2 spi3 spi4 qspi\n\
                iomux gpioa [0~31] gpiob [0~31] gpioc [0~31] gpiod [0~31]\n\
                iomux pwm0 pwm1 pwm2 pwm3 spwm0 spwm1 if [0~3]\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &iomux_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "iomux", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = iomux_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
