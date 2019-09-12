#include <common.h>
#include "regs/asram.h"
#include "regs/scm.h"

#define  VALID_ADDR_START    0x400000
#define  VALID_ADDR_RANGE    16

static BL_CMD_T asram_descriptor;

/*
 * Perform a memory test. A more complete alternative test can be
 * configured using CONFIG_SYS_ALT_MEMTEST. The complete test loops until
 * interrupted by ctrl-c or by a failure of one of the sub-tests.
 */
void v7_dma_inv_range(u32 s, u32 e);
void v7_dma_clean_range(u32 s, u32 e);

void asram_init(void)
{
    hwp_apSCM->io_mux_8c = 0x00540000;  // sram_cs, sram_wr, sram_rd
    hwp_apSCM->io_mux_90 = 0x55540000;  // sram_data2~data0, sram_a3~a0
    hwp_apSCM->io_mux_94 = 0x00000155;  // sram_data7~data3
    // set SMTMGR register
    // t_prc(22:19)--(b'000)
    // t_bta(18:16)--(b'001)
    // t_wp(15:10)--(b'0000 01)
    // t_wr(9:8)--(b'01)
    // t_as(7:6)--(b'01)
    // t_rc(5:0)--(b'0000 01)
    hwp_apAsram->timing_r_set0 = 0x00010541;
    printf("Asram init OK!\n");
}

static u32 asram_rw_test(void)
{
    U32 addr;
    U8  w_u8, r_u8;
    U16 w_u16, r_u16;
    U32 w_u32, r_u32;
    U32 i;

    printf("U8 test begin!\n");
    for(i = 0; i < VALID_ADDR_RANGE/sizeof(U8); i++){
        addr = (VALID_ADDR_START + i);
        w_u8 = i;
        *(volatile u8 *)addr = w_u8;
        r_u8 = *(volatile u8 *)addr;
        if(r_u8 != w_u8){
            printf("U8 err! addr:0x%x, w:0x%x, r:0x%x!\n", addr, w_u8, r_u8);
            return RET_FAIL;
        }
    }
    printf("U8 test end-pass!\n");

    printf("U16 test begin!\n");
    for(i = 0; i < VALID_ADDR_RANGE/sizeof(U16); i++){
        addr = (VALID_ADDR_START + i*2);
        w_u16 = i;
        *(volatile u16 *)addr = w_u16;
        r_u16 = *(volatile u16 *)addr;
        if(r_u16 != w_u16){
            printf("U16 err! addr:0x%x, w:0x%x, r:0x%x!\n", addr, w_u16, r_u16);
            return RET_FAIL;
        }
    }
    printf("U16 test end-pass!\n");

    printf("U32 test begin!\n");
    for(i = 0; i < VALID_ADDR_RANGE/sizeof(U32); i++){
        addr = (VALID_ADDR_START + i*4);
        w_u32 = i;
        *(volatile u32 *)addr = w_u32;
        r_u32 = *(volatile u32 *)addr;
        if(r_u32 != w_u32){
            printf("U32 err! addr:0x%x, w:0x%x, r:0x%x!\n", addr, w_u32, r_u32);
            return RET_FAIL;
        }
    }
    printf("U32 test end-pass!\n");
    return RET_PASS;
}

static U32 random_u8_cnt = 0, random_u16_cnt = 0, random_u32_cnt = 0;

static inline u32 asram_stress(void)
{
    U32 addr, offset;
    U8  random_u8, r_u8;
    U16 random_u16, r_u16;
    U32 random_u32, r_u32;

    offset = hg_pseudo_random_sequence() & 0x0F;
    addr = VALID_ADDR_START + offset;
    if(addr % 4 == 0){
        random_u32_cnt++;
        random_u32 = hg_pseudo_random_sequence();
        *(volatile U32 *)addr = random_u32;
        r_u32 = *(volatile U32 *)addr;
        if(r_u32 != random_u32){
            printf("Stress, U32 err! addr:0x%x, random_u32:0x%x, r:0x%x!\n", addr, random_u32, r_u32);
            return RET_FAIL;
        }
    } else if(addr % 2 == 0){
        random_u16_cnt++;
        random_u16 = hg_pseudo_random_sequence() & 0xFFFF;
        *(volatile U16 *)addr = random_u16;
        r_u16 = *(volatile U16 *)addr;
        if(r_u16 != random_u16){
            printf("Stress, U16 err! addr:0x%x, random_u16:0x%x, r:0x%x!\n", addr, random_u16, r_u16);
            return RET_FAIL;
        }
    } else{
        random_u8_cnt++;
        random_u8 = hg_pseudo_random_sequence() & 0xFF;
        *(volatile U8 *)addr = random_u8;
        r_u8 = *(volatile U8 *)addr;
        if(r_u8 != random_u8){
            printf("Stress, U8 err! addr:0x%x, random_u8:0x%x, r:0x%x!\n", addr, random_u8, r_u8);
            return RET_FAIL;
        }
    }

    return RET_PASS;
}

static u32 asram_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    U64 s, e;
    U32 pre_time = 60000, test_time, last = 0, ret = 0;  // ms
    if (strcmp(argv[1], "auto") == 0) {
        asram_init();
        ret = asram_rw_test();
    }

    if (strcmp(argv[1], "init") == 0) {
        asram_init();
        ret = RET_PASS;
    }

    if (strcmp(argv[1], "rw") == 0) {
        ret = asram_rw_test();
    }

    if (strcmp(argv[1], "stress") == 0) {
        random_u32_cnt = 0;
        random_u16_cnt = 0;
        random_u8_cnt = 0;
        if(argc > 2){
            pre_time = simple_strtoul(argv[2], NULL, 10);
        }

        s = arch_counter_get_cntpct();
        while(1){
            e = arch_counter_get_cntpct();
            test_time = ((U32)(e-s))/(CONFIG_COUNTER_FREQ/1000);
            if(test_time - last >= 1000){
                last = test_time;
                printf("Stress, U32 total %d, U16 total %d, U8 total %d!\n", random_u32_cnt, random_u16_cnt, random_u8_cnt);
            }
            if(test_time >= pre_time){
                ret = RET_PASS;
                break;
            }
            if(RET_PASS != asram_stress()){
                ret = RET_FAIL;
                break;
            }
        }
        printf("Stress testing random total %d ms end!\n", test_time);
        printf("Stress tesing end, U32 total %d, U16 total %d, U8 total %d!\n", random_u32_cnt, random_u16_cnt, random_u8_cnt);
    }

    return ret;
}

static u8 *short_description(void)
{
    return (u8 *) "asram test, write/read/stress testing\n";
}

static u8 *long_description(void)
{
    return (u8 *) "asram [init]--init controller for asram\n\
                   asram [auto]--init and rw testing\n\
                   asram [rw]--write and readback testing\n\
                   asram [stress],[time<ms>]--random write and readback testing\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &asram_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "asram", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = asram_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
