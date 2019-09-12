#include <bl_common.h>
#include <common.h>
//#include "drivers/dma_imp.h"
#include "drivers/drv_dma.h"
#include "regs/uart.h"
#include "cp15.h"
#include <drivers/drv_qspi.h>

static BL_CMD_T dma_descriptor;

U32 dmac_id = 0;
__attribute__((aligned(64))) LLI_NODE_ST dma_lli[4] = {{0}};
// LLI_NODE_ST *dma_lli = (LLI_NODE_ST *) 0x2f0000;
__attribute__((aligned(64))) char dma_qspiflash_buf[320];
#ifdef CONFIG_UPV_QSPI
#include "flash.h"
extern int fl_dma_read(int rd_cmd, u32 fl_addr, u8 *buf, u32 len, u32 dump_regs);
#endif
#ifdef CONFIG_UPV_SERIAl
extern U32 uart_init(U32 port_id, U32 baut_rate, U8 int_enable);
extern int uart_send_buffer_dma(U32 ch, U32 port, void *buff, U32 len);
#endif

extern U32 dma_lli_node_addr_base;
// extern LLI_NODE_ST lli_48k_test[384];
__attribute__((aligned(64))) LLI_NODE_ST lli_48k_test[384];

#ifdef DMA_TIME_COUNT
extern DMA_TIME_COUNT_ST dma_time_record[2][HW_DMAC_CH_NUM_MAX];
#endif

#define BB_M2M_TEST_LOOP    10000

void bb_mem_test_cmp(U32 id, U32 ch_id, U32 loop, U32 src, U32 dst, U32 node_len, U32 dir)
{
    #ifdef DMA_TIME_COUNT
    DMA_TIME_COUNT_ST dma_time_record_st;
    U64 s_inv, e_inv;
    #endif
    U64 s, e, data_prepare_s, data_prepare_e;
    U32 memcpy_cache_total;
    U32 data_prepare_loop = 768; //48*1024/64;
    U32 data_prepare_total;
    // U64 s_clean, e_clean;
    // U32 memcpy_clean_total;
    U32 bbm2m_loop = loop, j = 0, default_node_len, default_node_total;
    U32 node_src, node_dst, k;
    char * curr_lli = NULL;
    char * last_lli = NULL;


    ////############ memcpy #############//
    default_node_len = node_len;
    if(dir == 0){
        printf("b-->s memcpy 48k test begin, inv src, loop_total:%d\n", bbm2m_loop);
    }
    if(dir == 1){
        printf("s-->b memcpy 48k test begin, clean dst, loop_total:%d\n", bbm2m_loop);
    }
    memcpy_cache_total = 0;
    data_prepare_total = 0;
    node_src = src;
    node_dst = dst;
    default_node_total = 12 * (4096/default_node_len);
    s = arch_counter_get_cntpct();
    for(k = 0; k < bbm2m_loop; k++)
    {
        data_prepare_s = arch_counter_get_cntpct();
        for(j = 0; j < data_prepare_loop; j++)
        {
            *(U32 *)node_src = k;
            node_src += 64;
        }
        data_prepare_e = arch_counter_get_cntpct();
        data_prepare_total += (U32)(data_prepare_e - data_prepare_s);

        node_src = src;
        node_dst = dst;
        // 0. invalidate src addr
        if(dir == 0) //bb ram---> sram, inv bb ram
        {
            #ifdef DMA_TIME_COUNT
            s_inv = arch_counter_get_cntpct();
            #endif
            // v7_dma_inv_range((U32)src, (U32)src + length);
            v7_dma_inv_range((U32)src, (U32)src + 48*1024);
            #ifdef DMA_TIME_COUNT
            e_inv = arch_counter_get_cntpct();
            memcpy_cache_total += (U32)(e_inv - s_inv);
            #endif
        }
        for(j = 0; j < default_node_total; j++)
        {
            // v7_dma_inv_range((U32)node_dst, (U32)node_dst + default_node_len);
            memcpy((char *)node_dst, (char *)node_src, default_node_len);
            *(U32 *)node_dst = k;
            node_src += default_node_len;
            node_dst += default_node_len;
        }
        if(dir == 1) // sram---> bb ram, clean bb ram
        {
            #ifdef DMA_TIME_COUNT
            s_inv = arch_counter_get_cntpct();
            #endif
            // v7_dma_clean_range((U32)dst, (U32)dst + length);
            v7_dma_clean_range((U32)dst, (U32)dst + 48*1024);
            #ifdef DMA_TIME_COUNT
            e_inv = arch_counter_get_cntpct();
            memcpy_cache_total += (U32)(e_inv - s_inv);
            #endif
        }
    }
    e = arch_counter_get_cntpct();
    printf("(%d bytes * %d) memcpy test done, loop:%d, %d us, cache %d us, data_prepare %d us!\n", 
    default_node_len, default_node_total, bbm2m_loop, ((U32)(e-s))/20, memcpy_cache_total/20, data_prepare_total/20);


    
    #ifdef DMA_TIME_COUNT
    dma_count_init(dmac_id, ch_id);
    #endif
    if(dir == 0) printf("bb ram-->sram lli 48k test begin, inv sram<dst>, loop_total:%d\n", bbm2m_loop);
    if(dir == 1) printf("sram-->bb ram lli 48k test begin, clean sram<src> loop_total:%d\n", bbm2m_loop);
    // // prepare lli node
    default_node_len = node_len;
    default_node_total = 12 * (4096/default_node_len);
    data_prepare_total = 0;
    node_src = src;
    node_dst = dst;

    s = arch_counter_get_cntpct();
    for(k = 0; k < bbm2m_loop; k++)
    {
        data_prepare_s = arch_counter_get_cntpct();
        for(j = 0; j < data_prepare_loop; j++)
        {
            *(U32 *)node_src = k;
            node_src += 64;
        }
        data_prepare_e = arch_counter_get_cntpct();
        data_prepare_total += (U32)(data_prepare_e - data_prepare_s);
        
        node_src = src;
        node_dst = dst;
        // curr_lli = (char *)dma_lli_node_addr_base;
        curr_lli = (char *)&lli_48k_test[0];
        last_lli = NULL;
        // 0. invalidate dst addr
        if(dir == 0){  // bb ram-->sram, inv sram first
            #ifdef DMA_TIME_COUNT
            s_inv = arch_counter_get_cntpct();
            #endif
            // v7_dma_inv_range((U32)dst, (U32)dst + length);
            v7_dma_inv_range((U32)dst, (U32)dst + 48*1024);
            #ifdef DMA_TIME_COUNT
            e_inv = arch_counter_get_cntpct();
            dma_time_record[id][ch_id].inv_total += (U32)(e_inv - s_inv);
            #endif
        } else if(dir == 1){  // sram-->bb ram, clean sram first
            #ifdef DMA_TIME_COUNT
            s_inv = arch_counter_get_cntpct();
            #endif
            // v7_dma_clean_range((U32)src, (U32)src + length);
            v7_dma_clean_range((U32)src, (U32)src + 48*1024);
            #ifdef DMA_TIME_COUNT
            e_inv = arch_counter_get_cntpct();
            dma_time_record[id][ch_id].clean_total += (U32)(e_inv - s_inv);
            #endif
        }

        // 1. set node len
        #ifdef DMA_TIME_COUNT
        dma_time_record[id][ch_id].config_s = arch_counter_get_cntpct();
        #endif
        dma_lli_node_len_set(dmac_id, ch_id, default_node_len);
        
        // set first node
        dma_ap_lli_node_set(dmac_id, ch_id, node_src, node_dst, curr_lli, last_lli, false);
        last_lli = curr_lli;
        curr_lli += sizeof(LLI_NODE_ST);
        node_src += default_node_len;
        node_dst += default_node_len;
        for(j = 1; j < default_node_total-1; j++)
        {
            // printf(" (%d) node set,curr:0x%x, last:0x%x!\n",j , (U32)curr_lli, (U32)last_lli);
            dma_ap_lli_node_set(dmac_id, ch_id, node_src, node_dst, curr_lli, last_lli, false);
            v7_dma_clean_range((U32)last_lli, (U32)last_lli + sizeof(LLI_NODE_ST));
            last_lli = curr_lli;
            curr_lli += sizeof(LLI_NODE_ST);
            node_src += default_node_len;
            node_dst += default_node_len;
        }
        dma_ap_lli_node_set(dmac_id, ch_id, node_src, node_dst, curr_lli, last_lli, true);
        v7_dma_clean_range((U32)last_lli, (U32)last_lli + sizeof(LLI_NODE_ST));
        v7_dma_clean_range((U32)curr_lli, (U32)curr_lli + sizeof(LLI_NODE_ST));

        dma_ap_lli_start(dmac_id, ch_id);
    }
    e = arch_counter_get_cntpct();
    printf("(%d bytes * %d) test done, %d us!\n", default_node_len, default_node_total, ((U32)(e-s))/20);
    #ifdef DMA_TIME_COUNT
    get_dma_count_info(dmac_id, ch_id, &dma_time_record_st);
    printf("config_total:%d us, trans_total:%d us, data_prepare_total:%d us ", dma_time_record_st.config_total/20, dma_time_record_st.trans_total/20, data_prepare_total/20);
    if(dir == 0) printf("cache :%d us\n", dma_time_record_st.inv_total/20);
    if(dir == 1) printf("cache :%d us\n", dma_time_record_st.clean_total/20);
    #endif
}

U32 dma_testing(u32 argc, const char **argv)
{
    #ifdef CONFIG_UPV_QSPI
    U32 i;
    #endif
    #ifdef DMA_TIME_COUNT
    DMA_TIME_COUNT_ST dma_time_record_st;
    #endif
    U32 ch_id = 2, src = 0x200000, dst = 0x280000, length = 0x100, tt_fc = -1;
    int err = 0;
    U64 s, e;
    // U32 default_node_len = 128;
    U32 j, bbm2m_loop = BB_M2M_TEST_LOOP;
#if 0
    U64 s_inv, e_inv, s_clean, e_clean;
    U32 memcpy_inv_total, memcpy_clean_total;
    U32 bbm2m_loop = BB_M2M_TEST_LOOP, j = 0, default_node_len = 128, default_node_total = 384;
    U32 node_src, node_dst, k;
    char * curr_lli = NULL;
    char * last_lli = NULL;
#endif
    dma_init(0);
    dma_init(1);

    // testing
    if (argc < 2) {
        printf(" para not enough! \n");
        dma_deinit(0);
        dma_deinit(1);
        return RET_FAIL;
    }

    if (strcmp(argv[1], "select") == 0) {
        if (argc > 2) {
            dmac_id = simple_strtoul(argv[2], NULL, 10);
            if(dmac_id >= 2){
                dmac_id = 0;
                printf("Only 0 or 1 can be selected, set default 0!\n");
            }else if(1 == dmac_id){
                printf("You must ensure the right had been set for cpu1 and dma1!\n");
            }
        }
    }
    if (strcmp(argv[1], "auto") == 0) {
        tt_fc = 0;
        printf("dma[%d],channel[%d],src(0x%x)->dst(0x%x),len(0x%x)!\n", dmac_id, ch_id, src, dst, length);
    }
    if (strcmp(argv[1], "m2m") == 0) {
        tt_fc = 0;
    }
    if (strcmp(argv[1], "m2mp") == 0) {
        tt_fc = 1;
    }
    if (strcmp(argv[1], "m2mlli") == 0) {
        tt_fc = 2;
    }
    if (strcmp(argv[1], "flashread") == 0) {
        tt_fc = 3;
    }
    if (strcmp(argv[1], "flashwrite") == 0) {
        tt_fc = 4;
    }
    if (strcmp(argv[1], "uartsend") == 0) {
        tt_fc = 5;
    }
    if (strcmp(argv[1], "bbm2m") == 0) {
        tt_fc = 6;
    }
    if (strcmp(argv[1], "cmps2b") == 0) {
        tt_fc = 7;
    }
    if (strcmp(argv[1], "cmpb2s") == 0) {
        tt_fc = 8;
    }
    if (strcmp(argv[1], "cmps2s") == 0) {
        tt_fc = 9;
    }
    if (strcmp(argv[1], "bb48k") == 0) {
        tt_fc = 10;
    }
    if (strcmp(argv[1], "lli48k0") == 0) {
        tt_fc = 11;
    }
    if (strcmp(argv[1], "lli48k1") == 0) {
        tt_fc = 12;
    }

    if (argc > 2) {
        ch_id = simple_strtoul(argv[2], NULL, 10);
        if (argc > 3) {
            src = simple_strtoul(argv[3], NULL, 16);
            if (argc > 4) {
                dst = simple_strtoul(argv[4], NULL, 16);
                if (argc > 5) {
                    length = simple_strtoul(argv[5], NULL, 16);
                    if (argc > 6) {
                        bbm2m_loop = simple_strtoul(argv[6], NULL, 10);
                        if (argc > 7) {
                            // default_node_len = simple_strtoul(argv[7], NULL, 10);
                        }
                    }
                }
            }
        }
    }

    printf("Use DMAC[%d] for testing---\n", dmac_id);

    v7_dma_clean_range(src, src + length);
    v7_dma_inv_range(dst, dst + length);

    if ((0 == tt_fc) || (1 == tt_fc)) {
        if (0 == tt_fc) {
            err = dma_m2m_single(dmac_id, ch_id, src, dst, length, true);
        } else {
            err = dma_m2m_single_performance(dmac_id, ch_id, src, dst, length, true);
        }
        if (0 == err) {
            printf(" dma_m2m ch(%d) test done#####\n", ch_id);
            if (memcmp((void *) dst, (void *) src, length)) {
                printf("\n data error!\n\n");
            } else {
                printf("\n data OK!\n\n");
            }
            // printf("\ndma_m2m channle(%d) print data done#####!\n\n", ch_id);
        } else {
            printf("dma_m2m channel(%d) test error(err:0x%x)!\n", ch_id, err);
        }
    } else if (2 == tt_fc) {
        // set node
        dma_m2m_lli_node_set(dmac_id, ch_id, src, dst, length / 2, &dma_lli[0], NULL, false);
        dma_m2m_lli_node_set(dmac_id, ch_id, src + length / 2, dst + length / 2, length / 2, &dma_lli[1],
                             &dma_lli[0], true);
        // clean node addr if necessary
        v7_dma_clean_range((U32)&dma_lli[0], (U32)&dma_lli[0]+sizeof(LLI_NODE_ST));
        v7_dma_clean_range((U32)&dma_lli[1], (U32)&dma_lli[1]+sizeof(LLI_NODE_ST));
        err = dma_m2m_lli_start(dmac_id, ch_id, true);
        printf("dma_m2m_lli channel(%d) test done!\n", ch_id);
        if (0 == err) {
            if (memcmp((void *) dst, (void *) src, length)) {
                printf("\n data error!\n\n");
            } else {
                printf("\n data OK!\n\n");
            }
        } else {
            printf("dma_m2m_lli channel(%d) test error(err:0x%x)!\n", ch_id, err);
        }
    }
    #ifdef CONFIG_UPV_QSPI
    else if (3 == tt_fc) {
        // qspiflash read config
        if (fl_rescan() == NULL)
            return RET_FAIL;
        if (0 == fl_dma_read(0x03, src, (U8 *) dma_qspiflash_buf, 256, 0)) {
            printf("dma_flashread channel(%d) test done!\n", ch_id);
            v7_dma_inv_range((U32) dma_qspiflash_buf, (U32) dma_qspiflash_buf + 256);
            for (i = 0; i < 64; i++) {
                if ((i != 0) && (i % 4 == 0))
                    printf("\n");
                printf("0x%x ", *(U32 *) (dma_qspiflash_buf + i * 4));
            }
            printf("\n data print done!\n\n");
        } else {
            printf("dma_flashread channel(%d) test error(err:0x%x)!\n", ch_id, err);
        }
    } else if (4 == tt_fc) {
        // qspiflash write config
        fl_dev = (struct qspi_fl_info *)fl_rescan();
        if (fl_dev == NULL)
            return RET_FAIL;
        if (0 == fl_dev->dma_write_page((u32)dst, (u8 *) src, (int)length, 32)) {
            printf("dma_flashwrite channel(%d) test done!\n", ch_id);
        } else {
            printf("dma_flashwrite channel(%d) test error(err:0x%x)!\n", ch_id, err);
        }
    }
    #endif
    #ifdef CONFIG_UPV_SERIAl
    else if (5 == tt_fc) {
        // prepare qspiflash config
        uart_init(1, 115200, 2); // for test only wwzz--uart1 with dma
        if (0 == uart_send_buffer_dma(ch_id, src, (U8 *) dma_qspiflash_buf, 64)) {
            printf("uart_send_buffer_dma channel(%d) test done!\n", ch_id);
        } else {
            printf("uart_send_buffer_dma channel(%d) test error(err:0x%x)!\n", ch_id, err);
        }
    }
    #endif
    else if (6 == tt_fc) {  // bb m2m test
        printf("bb m2m test begin, loop:%d\n", bbm2m_loop);
        s = arch_counter_get_cntpct();
        for(j = 0; j < bbm2m_loop; j++)
        {
            dma_m2m_single_performance(dmac_id, ch_id, src, dst, length, true);
        }
        e = arch_counter_get_cntpct();
        printf("bb m2m test done, loop:%d, consume %d ms!\n", bbm2m_loop, ((U32)(e-s))/20000);
    }
    else if (7 == tt_fc) {  // bb m2m test, sram-->bb ram, dma and cpu
        #ifdef DMA_TIME_COUNT
        dma_count_init(dmac_id, ch_id);
        #endif
        printf("sram-->bb mem test begin\n");
        s = arch_counter_get_cntpct();
        for(j = 0; j < bbm2m_loop; j++)
        {
            v7_dma_clean_range((U32)src, (U32)src + length);
            dma_m2m_single_performance(dmac_id, ch_id, src, dst, length, true);
        }
        e = arch_counter_get_cntpct();
        printf("sram-->bb mm dma test done, loop:%d, consume %d ms!\n", bbm2m_loop, ((U32)(e-s))/20000);
        s = arch_counter_get_cntpct();
        for(j = 0; j < bbm2m_loop; j++)
        {
            memcpy((char *)dst, (char *)src, length);
            v7_dma_clean_range((U32)dst, (U32)dst + length);
        }
        e = arch_counter_get_cntpct();
        printf("sram-->bb mm cpu memcpy test done, loop:%d, consume %d ms!\n", bbm2m_loop, ((U32)(e-s))/20000);
        #ifdef DMA_TIME_COUNT
        get_dma_count_info(dmac_id, ch_id, &dma_time_record_st);
        printf("loop_total:%d, config_total:%dms, trans_total:%dms\n", bbm2m_loop, dma_time_record_st.config_total/20000, dma_time_record_st.trans_total/20000);
        #endif
    }
    else if (8 == tt_fc) {  // bb m2m test, bb ram-->sram, dma and cpu
        #ifdef DMA_TIME_COUNT
        dma_count_init(dmac_id, ch_id);
        #endif
        printf("bb ram-->sram dma test begin\n");
        s = arch_counter_get_cntpct();
        for(j = 0; j < bbm2m_loop; j++)
        {
            dma_m2m_single_performance(dmac_id, ch_id, src, dst, length, true);
            v7_dma_inv_range((U32)dst, (U32)dst + length);
        }
        e = arch_counter_get_cntpct();
        printf("bb ram-->sram dma test done, loop:%d, consume %d ms!\n", bbm2m_loop, ((U32)(e-s))/20000);
        s = arch_counter_get_cntpct();
        for(j = 0; j < bbm2m_loop; j++)
        {
            v7_dma_inv_range((U32)src, (U32)src + length);
            memcpy((char *)dst, (char *)src, length);
        }
        e = arch_counter_get_cntpct();
        printf("bb ram-->sram cpu memcpy test done, loop:%d, consume %d ms!\n", bbm2m_loop, ((U32)(e-s))/20000);
        #ifdef DMA_TIME_COUNT
        get_dma_count_info(dmac_id, ch_id, &dma_time_record_st);
        printf("loop_total:%d, config_total:%dms, trans_total:%dms\n", bbm2m_loop, dma_time_record_st.config_total/20000, dma_time_record_st.trans_total/20000);
        #endif
    }
    else if (9 == tt_fc) {  // bb m2m test, sram-->sram, dma and cpu
        #ifdef DMA_TIME_COUNT
        dma_count_init(dmac_id, ch_id);
        #endif
        printf("sram-->sram test begin\n");
        s = arch_counter_get_cntpct();
        for(j = 0; j < bbm2m_loop; j++)
        {
            v7_dma_clean_range((U32)src, (U32)src + length);
            dma_m2m_single_performance(dmac_id, ch_id, src, dst, length, true);
            v7_dma_inv_range((U32)dst, (U32)dst + length);
        }
        e = arch_counter_get_cntpct();
        printf("sram-->sram dma test done, loop:%d, consume %d ms!\n", bbm2m_loop, ((U32)(e-s))/20000);
        s = arch_counter_get_cntpct();
        for(j = 0; j < bbm2m_loop; j++)
        {
            memcpy((char *)dst, (char *)src, length);
            v7_dma_clean_range((U32)dst, (U32)dst + length);
        }
        e = arch_counter_get_cntpct();
        printf("sram-->sram cpu memcpy test done, loop:%d, consume %d ms!\n", bbm2m_loop, ((U32)(e-s))/20000);
        #ifdef DMA_TIME_COUNT
        get_dma_count_info(dmac_id, ch_id, &dma_time_record_st);
        printf("loop_total:%d, config_total:%dms, trans_total:%dms\n", bbm2m_loop, dma_time_record_st.config_total/20000, dma_time_record_st.trans_total/20000);
        #endif
    }
    else if (11 == tt_fc) {  // bb ram to sram m2m test, lli 48k test
        bb_mem_test_cmp(dmac_id, ch_id, bbm2m_loop, src, dst, 128, 0);
        bb_mem_test_cmp(dmac_id, ch_id, bbm2m_loop, src, dst, 2048, 0);
        bb_mem_test_cmp(dmac_id, ch_id, bbm2m_loop, src, dst, 4096, 0);
    }
    else if (12 == tt_fc) {  // sram to bb ram m2m test, lli 48k test
        bb_mem_test_cmp(dmac_id, ch_id, bbm2m_loop, src, dst, 128, 1);
        bb_mem_test_cmp(dmac_id, ch_id, bbm2m_loop, src, dst, 2048, 1);
        bb_mem_test_cmp(dmac_id, ch_id, bbm2m_loop, src, dst, 4096, 1);
    }

    dma_deinit(0);
    dma_deinit(1);
    return RET_PASS;
}
static u32 dma_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    return dma_testing(argc, argv);
}

static u8 *short_description(void)
{
    return (u8 *) "Test the specified Dma \n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: Test the specified Dma by printing a message from it\n\
                The applicaition running command is as follow:\n\
                dma [auto]\n\
                dma [m2m/m2mp],[ch(2/3)],[src],[dst],[len]\n\
                dma [m2mlli],[ch(2/3)],[src],[dst],[len]\n\
                dma [uartsend],[ch(4/5/6/7<default 4>)],[port(1/6/7<1 for test>)],[addr],[len]\n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &dma_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "dma", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = dma_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
