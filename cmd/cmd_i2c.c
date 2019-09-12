#include <bl_common.h>
#include <drivers/drv_i2c.h>

static BL_CMD_T i2c_descriptor;
extern bool gpio_set_output_direction(int gpio_num);
extern bool gpio_set_output_high(int gpio_num);
extern bool gpio_set_output_low(int gpio_num);

static unsigned char wbuf[256], rdbuf[1024];

extern int i2c_irq_handler(int irq, void *dev_id);

void i2c_irq_testing(HWP_I2C_AP_T *hwp_i2c, int irq, int argc, const char **argv)
{
    u32 w_addr = 0;
    u32 rw_len = 0;
    static int initflag = 0;

    printf("\nrequest i2c irq test case\n");
    if (initflag == 0) {
        request_irq(irq, i2c_irq_handler, NULL, (void *) hwp_i2c);
        initflag++;
    }

    if (strcmp(argv[2], "write") == 0) {
        if (argc > 3) {
            w_addr = simple_strtoul(argv[3], NULL, 16);
            wbuf[1] = (u8) w_addr;
            wbuf[0] = (u8)(w_addr >> 8);
        } else {
            wbuf[0] = 0x00;
            wbuf[1] = 0x00;
        }
        wbuf[3] = (u8) w_addr;
        wbuf[2] = (u8)(w_addr >> 8);
        if (argc > 4) {
            rw_len = simple_strtoul(argv[4], NULL, 16);
            if (rw_len > DATA_SIZE)
                rw_len = DATA_SIZE;
        } else {
            rw_len = DATA_SIZE;
        }

        i2c_dw_xfer_msg(hwp_i2c, wbuf, (rw_len + 2));
    }

    if (strcmp(argv[2], "read") == 0) {
        if (argc > 3) {
            w_addr = simple_strtoul(argv[3], NULL, 16);
            wbuf[1] = (u8) w_addr;
            wbuf[0] = (u8)(w_addr >> 8);
        } else {
            wbuf[0] = 0x00;
            wbuf[1] = 0x00;
        }

        if (argc > 4) {
            rw_len = simple_strtoul(argv[4], NULL, 16);
        } else {
            rw_len = DATA_SIZE;
        }

        i2c_dw_xfer_msg(hwp_i2c, wbuf, 2);
        i2c_dw_read(hwp_i2c, rdbuf, rw_len);
        printf("\nread ok");
    }
}

int test_i2c_dw(u32 argc, const char **argv)
{
    int res = 0;
    u32 i;
    u32 w_addr = 0, r_addr = 0;
    u32 w_len, r_len;
    static u32 initflag = 0;
    static u32 i2c_index = 0;
    HWP_I2C_AP_T *hwp_i2c = get_i2c_hwp(i2c_index);
    if (hwp_i2c == NULL) {
        return RET_FAIL;
    }

    for (i = 0; i < DATA_SIZE; i++) {
        wbuf[i + 2] = i;
        rdbuf[i] = 0xaa;
    }

    if (initflag == 0) {
        dw_i2c_master_init(hwp_i2c, SLV_ADDR, DW_IC_CON_SPEED_STD);
        initflag = 1;
    }
    if (argc < 2)
        return (-1);

    if (strcmp(argv[1], "init") == 0) {
        i2c_index = simple_strtoul(argv[2], NULL, 16);
        hwp_i2c = get_i2c_hwp(i2c_index);
        printf("\ni2c_index = %d, hwp_i2c = 0x%x\n", i2c_index, hwp_i2c);
    }

    if (strcmp(argv[1], "mode") == 0) {
        if (argc > 2) {
            if (strcmp(argv[2], "s") == 0) {
                dw_i2c_master_init(hwp_i2c, SLV_ADDR, DW_IC_CON_SPEED_STD);
            } else if (strcmp(argv[2], "f") == 0) {
                dw_i2c_master_init(hwp_i2c, SLV_ADDR, DW_IC_CON_SPEED_FAST);
            } else if (strcmp(argv[2], "h") == 0) {
                dw_i2c_master_init(hwp_i2c, SLV_ADDR, DW_IC_CON_SPEED_HIGH);
            }
        } else {
            dw_i2c_master_init(hwp_i2c, SLV_ADDR, DW_IC_CON_SPEED_STD);
        }
    }

    if (strcmp(argv[1], "read") == 0) {
        if (argc > 2) {
            r_addr = simple_strtoul(argv[2], NULL, 16);
            wbuf[1] = (u8) r_addr;
            wbuf[0] = (u8)(r_addr >> 8);
        } else {
            wbuf[0] = 0x00;
            wbuf[1] = 0x00;
        }

        if (argc > 3) {
            r_len = simple_strtoul(argv[3], NULL, 16);
        } else {
            r_len = DATA_SIZE;
        }

        res = dw_i2c_send_bytes(hwp_i2c, wbuf, 2);
        if (res != 0) {
            return RET_FAIL;
        }
        res = dw_i2c_read(hwp_i2c, rdbuf, r_len);
        if (res != 0) {
            return RET_FAIL;
        }
        printf("DATA_SIZE:%d i2c_base:0x%x\n", r_len, hwp_i2c);
    }

    if (strcmp(argv[1], "byte") == 0) {
        if (argc > 2) {
            w_addr = simple_strtoul(argv[2], NULL, 16);
            wbuf[1] = (u8) w_addr;
            wbuf[0] = (u8)(w_addr >> 8);
        } else {
            wbuf[0] = 0x00;
            wbuf[1] = 0x00;
        }
        if (argc > 3) {
            wbuf[2] = simple_strtoul(argv[3], NULL, 16);
        }
        res = dw_i2c_send_bytes(hwp_i2c, wbuf, 3);
        if (res != 0) {
            return RET_FAIL;
        }
        printf("DATA: %d i2c_base:0x%x\n", wbuf[2], hwp_i2c);
    }

    if (strcmp(argv[1], "write") == 0) {
        for (i = 0; i < DATA_SIZE; i++) {
            wbuf[i + 2] = i;
        }
        if (argc > 2) {
            w_addr = simple_strtoul(argv[2], NULL, 16);
            wbuf[1] = (u8) w_addr;
            wbuf[0] = (u8)(w_addr >> 8);
        } else {
            wbuf[0] = 0x00;
            wbuf[1] = 0x00;
        }

        if (argc > 3) {
            w_len = simple_strtoul(argv[3], NULL, 16);
            if (w_len > DATA_SIZE)
                w_len = DATA_SIZE;
        } else {
            w_len = DATA_SIZE;
        }
        wbuf[3] = (u8) w_addr;
        wbuf[2] = (u8)(w_addr >> 8);

        res = dw_i2c_send_bytes(hwp_i2c, wbuf, w_len + 2);
        if (res != 0) {
            return RET_FAIL;
        }
        printf("DATA_SIZE:%d i2c_base:0x%x\n", w_len, hwp_i2c);
    }

    if (strcmp(argv[1], "auto") == 0) {
        w_len = DATA_SIZE;
        r_len = DATA_SIZE;
        if (argc > 2) {
            w_addr = simple_strtoul(argv[2], NULL, 16);
            wbuf[0] = (u8)(w_addr >> 8);
            wbuf[1] = (u8) w_addr;
        } else {
            wbuf[0] = 0x00;
            wbuf[1] = 0x00;
        }
        dw_i2c_master_init(hwp_i2c, SLV_ADDR, DW_IC_CON_SPEED_FAST);
        res = dw_i2c_send_bytes(hwp_i2c, wbuf, w_len + 2);
        if (res != 0) {
            return RET_FAIL;
        }
        mdelay(10);
        res = dw_i2c_send_bytes(hwp_i2c, wbuf, 2);
        if (res != 0) {
            return RET_FAIL;
        }
        res = dw_i2c_read(hwp_i2c, rdbuf, r_len);
        if (res != 0) {
            return RET_FAIL;
        }
        res = memcmp(rdbuf, (wbuf + 2), DATA_SIZE);
        if (res != 0) {
            return RET_FAIL;
        }
    }

    if (strcmp(argv[1], "smbus") == 0) {
        if (argc > 2) {
            r_addr = simple_strtoul(argv[2], NULL, 16);
            wbuf[1] = (u8) r_addr;
            wbuf[0] = (u8)(r_addr >> 8);
        } else {
            wbuf[0] = 0x00;
            wbuf[1] = 0x00;
        }

        if (argc > 3) {
            r_len = simple_strtoul(argv[3], NULL, 16);
        } else {
            r_len = DATA_SIZE;
        }

        res = dw_i2c_smbus_read(hwp_i2c, wbuf, 2, rdbuf, r_len);
        if (res != 0) {
            return RET_FAIL;
        }
        printf("DATA_SIZE:%d i2c_base:0x%x\n", r_len, hwp_i2c);
    }

    if (strcmp(argv[1], "irq") == 0) {
        i2c_irq_testing(hwp_i2c, SYS_IRQ_ID_I2C0, argc, argv);
    }

#if 0
    if (strcmp(argv[1], "rfrd") == 0) {

        dw_i2c_master_init(hwp_i2c, 0x60, DW_IC_CON_SPEED_FAST);
        gpio_set_output_low(simple_strtoul(argv[4], NULL, 16));
        mdelay(2);

        memset(wbuf, 0 , 200);
        memset(rdbuf, 0 , 200);
        if (argc > 2) {
            r_addr = simple_strtoul(argv[2], NULL, 16);
            wbuf[0] = (u8) r_addr;
            //wbuf[0] = (u8)(r_addr >> 8);
        } else {
            wbuf[0] = 0x00;
            //wbuf[1] = 0x00;
        }

        if (argc > 3) {
            r_len = simple_strtoul(argv[3], NULL, 16);
        } else {
            //r_len = DATA_SIZE;
            r_len = 14;
        }

        res = dw_i2c_smbus_read(hwp_i2c, wbuf, 1, rdbuf, r_len);
        printf("rfrd: addr %d len %d\n", r_addr, r_len);
        if (res != 0) {
        //    gpio_set_output_high(4);
            return RET_FAIL;
        }
        printf("DATA_SIZE:%d i2c_base:0x%x\n", r_len, hwp_i2c);
        //gpio_set_output_high(4);

    }

    if (strcmp(argv[1], "gpioen") == 0) {

        if (argc > 2) {
            r_addr = simple_strtoul(argv[2], NULL, 16);
        }
        else
            r_addr = 4;

        printf("gpio en %d\n", r_addr);
        gpio_set_output_direction(r_addr);
        gpio_set_output_low(r_addr);
        mdelay(2);
    }

    if (strcmp(argv[1], "gpiods") == 0) {

        if (argc > 2) {
            r_addr = simple_strtoul(argv[2], NULL, 16);
        }
        else
            r_addr = 4;

        printf("gpio disable %d\n", r_addr);
        gpio_set_output_direction(r_addr);
        gpio_set_output_high(r_addr);
        mdelay(2);
    }

    if (strcmp(argv[1], "rfwr") == 0) {

        dw_i2c_master_init(hwp_i2c, 0x60, DW_IC_CON_SPEED_FAST);
        gpio_set_output_low(4);
        mdelay(2);
        if (argc > 3) {
			//register addr
            r_addr = simple_strtoul(argv[2], NULL, 16);
            wbuf[0] = (u8) r_addr;
			//value
            r_addr = simple_strtoul(argv[3], NULL, 16);
            wbuf[1] = (u8) r_addr;
        } else {
            wbuf[0] = 0x00;
            wbuf[1] = 0x00;
        }

        printf("rfwr: addr %d valud 0x%x\n", wbuf[0], wbuf[1]);

        dw_i2c_master_init(hwp_i2c, 0x60, DW_IC_CON_SPEED_STD);
        res = dw_i2c_send_bytes(hwp_i2c, wbuf, 2);
        if (res != 0) {
            return RET_FAIL;
        }

        gpio_set_output_high(4);
    }
#endif
    return res;
}

static u32 i2c_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    return test_i2c_dw(argc, argv);
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: i2c eepromm test\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified Timer by printing a message from it\n\
                The applicaition running command is as follow:\n\
                i2c init [index]\n\
                i2c mode [s/f/h]\n\
                i2c write / read / smbus [addr] [num]\n\
                i2c auto\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &i2c_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "i2c", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = i2c_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
