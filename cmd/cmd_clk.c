#include <bl_common.h>
#include <drivers/drv_clk.h>

static BL_CMD_T clk_descriptor;

static u32 clk_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
	CLK_ID_TYPE_T cur_id=CLK_ID_NUM;
	int rate_kHz=0;
	int ret=0;
	char name[10];

	if(argc < 3)
	{
		printf("command error!, too few arguments\n");
		return RET_NA; 
	}

	memset(name, 0, 10);
	if(strcmp(argv[2], "cpu0") == 0)
	{
		cur_id = CLK_CPU0;
		memcpy(name, "cpu0", 4);
	}

	if(strcmp(argv[2], "cpu1") == 0)
	{
		cur_id = CLK_CPU1;
		memcpy(name, "cpu1", 4);
	}

	if(strcmp(argv[2], "ddr") == 0)
	{
		cur_id = CLK_DDR;
		memcpy(name, "ddr", 3);
	}

	if(strcmp(argv[2], "62m") == 0)
	{
		cur_id = CLK_62M;
		memcpy(name, "62m", 3);
	}

	if(strcmp(argv[2], "rom") == 0)
	{
		cur_id = CLK_ROM;
		memcpy(name, "rom", 3);
	}

	if(strcmp(argv[2], "ram") == 0)
	{
		cur_id = CLK_RAM;
		memcpy(name, "ram", 3);
	}

	if(strcmp(argv[2], "dmac0") == 0)
	{
		cur_id = CLK_DMAC0;
		memcpy(name, "dmac0", 5);
	}

	if(strcmp(argv[2], "dmac1") == 0)
	{
		cur_id = CLK_DMAC1;
		memcpy(name, "dmac1", 5);
	}

	if(strcmp(argv[2], "axi") == 0)
	{
		cur_id = CLK_AXI;
		memcpy(name, "axi", 3);
	}

	if(strcmp(argv[2], "ahb") == 0)
	{
		cur_id = CLK_AHB;
		memcpy(name, "ahb", 3);
	}

	if(strcmp(argv[2], "apb") == 0)
	{
		cur_id = CLK_APB;
		memcpy(name, "apb", 3);
	}

	if(strcmp(argv[2], "ram") == 0)
	{
		cur_id = CLK_RAM;
		memcpy(name, "ram", 3);
	}

	if(strcmp(argv[2], "sd") == 0)
	{
		cur_id = CLK_SD;
		memcpy(name, "sd", 2);
	}

	if(strcmp(argv[2], "mp") == 0)
	{
		cur_id = CLK_MP;
		memcpy(name, "mp", 2);
	}

	if(strcmp(argv[2], "qspi") == 0)
	{
		cur_id = CLK_QSPI;
		memcpy(name, "qspi", 4);
	}

	if(strcmp(argv[2], "i2c0") == 0)
	{
		cur_id = CLK_I2C0;
		memcpy(name, "i2c0", 4);
	}

	if(strcmp(argv[2], "i2c1") == 0)
	{
		cur_id = CLK_I2C1;
		memcpy(name, "i2c1", 4);
	}

	if(strcmp(argv[2], "uart0") == 0)
	{
		cur_id = CLK_UART0;
		memcpy(name, "uart0", 5);
	}

	if(strcmp(argv[2], "uart1") == 0)
	{
		cur_id = CLK_UART1;
		memcpy(name, "uart1", 5);
	}

	if(strcmp(argv[2], "uart2") == 0)
	{
		cur_id = CLK_UART2;
		memcpy(name, "uart2", 5);
	}

	if(strcmp(argv[2], "uart3") == 0)
	{
		cur_id = CLK_UART3;
		memcpy(name, "uart3", 5);
	}

	if(strcmp(argv[2], "uart4") == 0)
	{
		cur_id = CLK_UART4;
		memcpy(name, "uart4", 5);
	}

	if(strcmp(argv[2], "uart5") == 0)
	{
		cur_id = CLK_UART5;
		memcpy(name, "uart5", 5);
	}

	if(strcmp(argv[2], "uart6") == 0)
	{
		cur_id = CLK_UART6;
		memcpy(name, "uart6", 5);
	}

	if(strcmp(argv[2], "uart7") == 0)
	{
		cur_id = CLK_UART7;
		memcpy(name, "uart7", 5);
	}

	if(strcmp(argv[2], "spi0") == 0)
	{
		cur_id = CLK_SPI0;
		memcpy(name, "spi0", 4);
	}

	if(strcmp(argv[2], "spi1") == 0)
	{
		cur_id = CLK_SPI1;
		memcpy(name, "spi1", 4);
	}

	if(strcmp(argv[2], "spi2") == 0)
	{
		cur_id = CLK_SPI2;
		memcpy(name, "spi2", 4);
	}

	if(strcmp(argv[2], "spi3") == 0)
	{
		cur_id = CLK_SPI3;
		memcpy(name, "spi3", 4);
	}

	if(strcmp(argv[2], "pwm") == 0)
	{
		cur_id = CLK_PWM;
		memcpy(name, "pwm", 3);
	}

	if(strcmp(argv[2], "bb") == 0)
	{
		cur_id = CLK_BB;
		memcpy(name, "bb", 2);
	}

	if(strcmp(argv[2], "rdss") == 0)
	{
		cur_id = CLK_RDSS;
		memcpy(name, "RDSS", 4);
	}

	if(strcmp(argv[2], "sim0") == 0)
	{
		cur_id = CLK_SIM0;
		memcpy(name, "sim0", 4);
	}

	if(strcmp(argv[2], "sim1") == 0)
	{
		cur_id = CLK_SIM1;
		memcpy(name, "sim1", 4);
	}

	if(strcmp(argv[2], "sim2") == 0)
	{
		cur_id = CLK_SIM2;
		memcpy(name, "sim2", 4);
	}

	if(strcmp(argv[2], "gpio") == 0)
	{
		cur_id = CLK_GPIO;
		memcpy(name, "gpio", 4);
	}

	if(strcmp(argv[2], "timer") == 0)
	{
		cur_id = CLK_TIMER;
		memcpy(name, "timer", 5);
	}

	if(strcmp(argv[2], "efuse") == 0)
	{
		cur_id = CLK_EFUSE;
		memcpy(name, "efuse", 5);
	}

	if(strcmp(argv[2], "sys32k") == 0)
	{
		cur_id = CLK_SYS32K;
		memcpy(name, "sys32k", 6);
	}

	if(strcmp(argv[2], "5m") == 0)
	{
		cur_id = CLK_5M;
		memcpy(name, "5M", 2);
	}

	if(cur_id == CLK_ID_NUM)
	{
		printf("Clock ID ERROR! exit... \n");
		return RET_FAIL; 
	}

	if ((argc >= 4) && (strcmp(argv[1], "set") == 0)) {
        rate_kHz = simple_strtoul(argv[3], NULL, 10);
		ret = clk_set_rate_kHz(cur_id, rate_kHz);
		if(CLK_OP_SUCCESS != ret)
		{
			printf("set %s clock rate to %d kHz failed!\n", name, rate_kHz);
			return RET_FAIL;
		}
		return RET_PASS;
	}

	if ((argc >= 3) && (strcmp(argv[1], "get") == 0)) {

		ret = clk_get_rate_kHz(cur_id);

		if(CLK_OP_INV_FAILED != ret)
		{
			printf("%s: %d kHz\n", name, ret);
			return RET_PASS;
		}else{
			printf("get %s clock rate failed!\n", name);
			return RET_FAIL;
		}
	}

	if ((argc >= 3) && (strcmp(argv[1], "en") == 0)) {
		ret = clk_enable(cur_id);
		if(CLK_OP_SUCCESS != ret)
		{
			printf("%s: enable failed!\n", name);
			return RET_FAIL;
		}else{
			printf("%s: enable OK!\n", name);
			return RET_PASS;
		}
	}

	if ((argc >= 3) && (strcmp(argv[1], "dis") == 0)) {
		ret = clk_disable(cur_id);
		if(CLK_OP_SUCCESS != ret)
		{
			printf("%s: disable failed!\n", name);
			return RET_FAIL;
		}else{
			printf("%s: disable OK!\n", name);
			return RET_PASS;
		}
	}

    return RET_NA; 
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: clk set/get rate/gating etc.\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: The applicaition running command is as follow:\n\
                clk set cpu0/cpu1/ddr/62m/ram/bb/mp kHz \n\
                clk set qspi/sd/uart[6/7]/i2c[0/1]/spi[0~3] kHz\n\
                clk set rdss/sim[0~2]/pwm/axi/ahb/apb kHz\n\
                clk get cpu0/cpu1/ddr/62m/ram/bb/mp \n\
                clk get qspi/sd/uart[6/7]/i2c[0/1]/spi[0~3] \n\
                clk get rdss/sim[0~2]/pwm/axi/ahb/apb \n\
                clk en/dis cpu1/ddr/62m/rom/ram/bb/mp/dmac[0/1] \n\
                clk en/dis qspi/sd/uart[0~7]/i2c[0/1]/spi[0~3] \n\
                clk en/dis sim[0~2]/pwm/gpio/timer/efuse/sys32k/5m/rdss \n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &clk_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "clk", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = clk_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
