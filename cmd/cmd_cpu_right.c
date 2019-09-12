#include <bl_common.h>
#include <drivers/drv_cpu_right.h>

static BL_CMD_T cpu_right_descriptor;

U32 right_testing(u32 argc, const char **argv)
{
    U32 right_max = CPU_RIGTH_E_ALL;
    U32 right_type = 28;
    U32 right_en = 0;
    U32 start, end;
    BOOL en = false;

    cpu_right_test_init();

    if (argc < 2) {
        printf(" need more para \n");
        return RET_FAIL;
    }

    if (strcmp(argv[1], "print") == 0) {
        print_right_ctl_addr(true);
        return RET_PASS;
    }
    if (strcmp(argv[1], "area") == 0) {
        print_right_ctl_addr(false);
        return RET_PASS;
    }
    else if (strcmp(argv[1], "set") == 0) {
        if (argc > 2) {
            right_type = simple_strtoul(argv[2], NULL, 10);
            if (right_type > right_max) {
                printf(" para err,right type max(%d), input(%d) \n", right_max, right_type);
                return RET_FAIL;
            }
            if (argc > 3) {
                right_en = simple_strtoul(argv[3], NULL, 10);
                if (right_en) {
                    en = true;
                }
            }
        }
        printf("cpu0 set right(module-%s-<%s>) for cpu1...\n", right_desc[right_type],
               ((en == true) ? "enable" : "disable"));
        cpu1_right_ctl_set(right_type, en);
    } else if (strcmp(argv[1], "areaset") == 0) {
        if (argc < 5) {
            printf(" need 5 para at last, but given(%d) \n", argc);
            return RET_FAIL;
        }
        if (argc > 2) {
            right_type = simple_strtoul(argv[2], NULL, 10);
            if (right_type >= AREA_TYPE_E_END) {
                printf(" area type err,area type max(%d), input(%d) \n", AREA_TYPE_E_END - 1,
                       right_type);
                return RET_FAIL;
            }
            if (argc > 3) {
                start = simple_strtoul(argv[3], NULL, 16);
                if (argc > 4) {
                    end = simple_strtoul(argv[4], NULL, 16);
                    if (start >= end) {
                        printf(" end(0x%x) must bigger than start(0x%x) \n", end, start);
                        return RET_FAIL;
                    }
                }
            }
        }
        cpu1_area_ctl_set(right_type, start, end);
    }

    return 0;

}

static u32 right_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    return right_testing(argc, argv);
}

static u8 *short_description(void)
{
    return (u8 *) "Cpu right module-cpu0 set rights for cpu1\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified wakeup ap by printing a message from it\n\
                The applicaition running command is as follow:\n\
                right [print/area]\n\
                right [set],[module<0~28>],[en<0/1>]\n\
                right [areaset],[0-rom/1-sram/2-qspiflash/3-ddr>],[start],[end]\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &cpu_right_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "right", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = right_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
