#include <common.h>
#include "cpu_right.h"
#include <drivers/drv_cpu_right.h>

// 0---rom; 1---ram
static inline void cpu1_area_set(AREA_TYPE_E type, U32 start, U32 end)
{
    U32 set_start = ((start >> RAM_ROM_ACCESS_ALIGN_SHIFT) << RAM_ROM_ACCESS_ALIGN_SHIFT);
    U32 set_end = ((end >> RAM_ROM_ACCESS_ALIGN_SHIFT) << RAM_ROM_ACCESS_ALIGN_SHIFT);

    if ((set_start != start) || (set_end != end)) {
        printf(" want (0x%x-->0x%x), actual set(0x%x-->0x%x) \n", start, end, set_start, set_end);
        printf(" area set must be aligned with cache line(64 bytes), given not, return \n");
        return;
    }

    printf(" area set(0x%x-->0x%x) \n", set_start, set_end);
    switch (type) {
    case AREA_TYPE_E_ROM:
        hwp_apSCM->cpu1rom_baddr = set_start;
        hwp_apSCM->cpu1rom_eaddr = set_end;
        hwp_apSCM->rsv_1160_11ec[0] = set_start;
        hwp_apSCM->rsv_1160_11ec[1] = set_end;
        break;
    case AREA_TYPE_E_RAM:
        hwp_apSCM->cpu1ram_baddr = set_start;
        hwp_apSCM->cpu1ram_eaddr = set_end;
        hwp_apSCM->rsv_1160_11ec[2] = set_start;
        hwp_apSCM->rsv_1160_11ec[3] = set_end;
        break;
    case AREA_TYPE_E_QSPI:
        hwp_apSCM->cpu1qspi_baddr = set_start;
        hwp_apSCM->cpu1qspi_eaddr = set_end;
        hwp_apSCM->rsv_1160_11ec[4] = set_start;
        hwp_apSCM->rsv_1160_11ec[5] = set_end;
        break;
    case AREA_TYPE_E_DDR:
        hwp_apSCM->cpu1ddr_baddr = set_start;
        hwp_apSCM->cpu1ddr_eaddr = set_end;
        hwp_apSCM->rsv_1160_11ec[6] = set_start;
        hwp_apSCM->rsv_1160_11ec[7] = set_end;
        break;

    default:
        break;
    }
}

static inline CPU_RIGTH_SHIFT_E get_right_shift(U32 type)
{
    return type;
}

void cpu_right_test_init(void)
{
    static U32 right_init = 0;
    if (0 == right_init) {
        right_init = 1;
        // writel_relaxed(0xFFFFFFFF, SCM_BASE + 0x34);	//trap addr set
        // 0xFFFFFFFF, cause data abort
        hwp_apSCM->cpu1trap_addr = 0xFFFFFFFF;
        hwp_apSCM->cpu1rom_baddr = CPU1_ROM_START;
        hwp_apSCM->cpu1rom_eaddr = CPU1_ROM_END;
        hwp_apSCM->cpu1ram_baddr = CPU1_RAM_START;
        hwp_apSCM->cpu1ram_eaddr = CPU1_RAM_END;
        hwp_apSCM->cpu1qspi_baddr = CPU1_QSPI_START;
        hwp_apSCM->cpu1qspi_eaddr = CPU1_QSPI_END;
        hwp_apSCM->cpu1ddr_baddr = CPU1_DDR_START;
        hwp_apSCM->cpu1ddr_eaddr = CPU1_DDR_END;
        hwp_apSCM->cpu1right_ctrl = 0x0FFFFFFF; // disable "ALL"
        // copy area data to reg where cpu1 can read
        hwp_apSCM->rsv_1160_11ec[0] = hwp_apSCM->cpu1rom_baddr;
        hwp_apSCM->rsv_1160_11ec[1] = hwp_apSCM->cpu1rom_eaddr;
        hwp_apSCM->rsv_1160_11ec[2] = hwp_apSCM->cpu1ram_baddr;
        hwp_apSCM->rsv_1160_11ec[3] = hwp_apSCM->cpu1ram_eaddr;
        hwp_apSCM->rsv_1160_11ec[4] = hwp_apSCM->cpu1qspi_baddr;
        hwp_apSCM->rsv_1160_11ec[5] = hwp_apSCM->cpu1qspi_eaddr;
        hwp_apSCM->rsv_1160_11ec[6] = hwp_apSCM->cpu1ddr_baddr;
        hwp_apSCM->rsv_1160_11ec[7] = hwp_apSCM->cpu1ddr_eaddr;
        printf("cpu0:cpu_right_test_init...trap addr is 0xFFFFFFFF\n");
    }
}

void cpu1_rom_area_set(U32 start, U32 end)
{
    if (ROM_ADDR_VALID(start) && ROM_ADDR_VALID(end) && (end > start)) {
        cpu1_area_set(AREA_TYPE_E_ROM, start, end);
    } else {
        printf("para invalid! \n");
    }
}

void cpu1_ram_area_set(U32 start, U32 end)
{
    if (SRAM_ADDR_VALID(start) && SRAM_ADDR_VALID(end) && (end > start)) {
        cpu1_area_set(AREA_TYPE_E_RAM, start, end);
    } else {
        printf("para invalid! \n");
    }
}

void cpu1_qspi_area_set(U32 start, U32 end)
{
    if (QSPI_ADDR_VALID(start) && QSPI_ADDR_VALID(end) && (end > start)) {
        cpu1_area_set(AREA_TYPE_E_QSPI, start, end);
    } else {
        printf("para invalid! \n");
    }
}

void cpu1_ddr_area_set(U32 start, U32 end)
{
    if (DDR_ADDR_VALID(start) && DDR_ADDR_VALID(end) && (end > start)) {
        cpu1_area_set(AREA_TYPE_E_DDR, start, end);
    } else {
        printf("para invalid! \n");
    }
}

U32 cpu1_right_ctl_set(CPU_RIGTH_E right_type, BOOL en)
{
    U32 reg_val, right_shift;
    CPU_RIGTH_E right_max = CPU_RIGTH_E_ALL;
    if (right_type > right_max) {
        printf(" err right type, input(%d) \n", right_type);
        return -1;
    }

    reg_val = hwp_apSCM->cpu1right_ctrl;
    right_shift = get_right_shift(right_type);
    if (en) {
        reg_val |= (0x1 << right_shift);
    } else {
        reg_val &= ~(0x1 << right_shift);
    }

    // always set uart 1 and sram right
    reg_val |= ((0x1 << 1) | (0x1 << 14));
    hwp_apSCM->cpu1right_ctrl = reg_val;
    printf("------right ctl value:0x%x\n", reg_val);
    printf("#####-for test, uart1 and sram always open for cpu1-#####\n");

    return 0;
}

U32 cpu1_area_ctl_set(AREA_TYPE_E area_type, U32 start, U32 end)
{
    if (area_type >= AREA_TYPE_E_END) {
        printf(" area type err,area type max(%d), input(%d) \n", AREA_TYPE_E_END - 1, area_type);
        return -1;
    }

    switch (area_type) {
    case AREA_TYPE_E_ROM:
        cpu1_rom_area_set(start, end);
        break;

    case AREA_TYPE_E_RAM:
        cpu1_ram_area_set(start, end);
        break;

    case AREA_TYPE_E_QSPI:
        cpu1_qspi_area_set(start, end);
        break;

    case AREA_TYPE_E_DDR:
        cpu1_ddr_area_set(start, end);
        break;

    default:
        break;
    }

    return 0;
}

#define writel_relaxed(v,c)	((*(volatile u32 *) (c)) = (v))
#define readl_relaxed(c)	(*(volatile u32 *) (c))

void print_right_ctl_addr(BOOL list_en)
{
    U32 i;
    printf("cpu1 rom valid range:(0x%x--0x%x)\n", hwp_apSCM->cpu1rom_baddr,
           hwp_apSCM->cpu1rom_eaddr);
    printf("cpu1 sram valid range:(0x%x--0x%x)\n", hwp_apSCM->cpu1ram_baddr,
           hwp_apSCM->cpu1ram_eaddr);
    printf("cpu1 qspi valid range:(0x%x--0x%x)\n", hwp_apSCM->cpu1qspi_baddr,
           hwp_apSCM->cpu1qspi_eaddr);
    printf("cpu1 ddr valid range:(0x%x--0x%x)\n", hwp_apSCM->cpu1ddr_baddr,
           hwp_apSCM->cpu1ddr_eaddr);

    if (!list_en)
        return;
    printf("module list:\n");
    printf("index:\tname<range>");
    for (i = 0; i < 28; i++) {
        printf("\n%2d:\t%s", i, right_desc[i]);
    }
    printf("\n\n");
    return;
}
