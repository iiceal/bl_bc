#include <drivers/drv_ddr.h>

#ifdef CONFIG_DDR_FPGA
void ddr_phy_init(void)
{
    REG_WRITE_ctrl(0xC01A0008, 0x078c2e04);
    REG_WRITE_ctrl(0xC01A0030, 0x0000008b);
    REG_WRITE_ctrl(0xC01A0018, 0x00000010);
    REG_WRITE_ctrl(0xC01A001c, 0x00800080);
    REG_WRITE_ctrl(0xC01A0020, 0x00080421);
    REG_WRITE_ctrl(0xC01A0040, 0x00000911);
    REG_WRITE_ctrl(0xC01A0044, 0x00000000);
    REG_WRITE_ctrl(0xC01A0048, 0x00000000);
    REG_WRITE_ctrl(0xC01A004c, 0x00000000);
    REG_WRITE_ctrl(0xC01A0034, 0x38d48890);
    REG_WRITE_ctrl(0xC01A0038, 0x388b00d8);
    REG_WRITE_ctrl(0xC01A003c, 0x10019004);
    REG_WRITE_ctrl(0xC01A002c, 0xfa00001f);
    REG_WRITE_ctrl(0xC01A001c, 0x06004e28);
    REG_WRITE_ctrl(0xC01A0020, 0x042a006e);
    REG_WRITE_ctrl(0xC01A01d4, 0x3db0a009);
    REG_WRITE_ctrl(0xC01A0214, 0x3db0a009);
}

void ddr_controller_init(void)
{
    REG_WRITE_ctrl(0xC0190000, 0x03040001);
    REG_WRITE_ctrl(0xC0190000, 0x03040001);
    REG_WRITE_ctrl(0xC0190000, 0x03040001);
    REG_WRITE_ctrl(0xC0190000, 0x03040201);
    REG_WRITE_ctrl(0xC0190000, 0x03040201);
    REG_WRITE_ctrl(0xC0190000, 0x03040201);
    REG_WRITE_ctrl(0xC0190000, 0x03040201);
    REG_WRITE_ctrl(0xC0190000, 0x03040201);
    REG_WRITE_ctrl(0xC0190000, 0x03040201);
    REG_WRITE_ctrl(0xC0190010, 0x00000030);
    REG_WRITE_ctrl(0xC0190010, 0x00000030);
    REG_WRITE_ctrl(0xC0190010, 0x00003030);
    REG_WRITE_ctrl(0xC0190010, 0x00003030);
    REG_WRITE_ctrl(0xC0190014, 0x000063e5);
    REG_WRITE_ctrl(0xC0190020, 0x00000000);
    REG_WRITE_ctrl(0xC0190020, 0x00000000);
    REG_WRITE_ctrl(0xC0190024, 0x602952c0);
    REG_WRITE_ctrl(0xC0190030, 0x00000000);
    REG_WRITE_ctrl(0xC0190030, 0x00000000);
    REG_WRITE_ctrl(0xC0190030, 0x00000000);
    REG_WRITE_ctrl(0xC0190030, 0x00000000);
    REG_WRITE_ctrl(0xC0190030, 0x00000000);
    REG_WRITE_ctrl(0xC0190034, 0x00402002);
    REG_WRITE_ctrl(0xC0190034, 0x0040c902);
    REG_WRITE_ctrl(0xC0190034, 0x000ec902);
    REG_WRITE_ctrl(0xC0190038, 0x00000002);
    REG_WRITE_ctrl(0xC0190038, 0x00000000);
    REG_WRITE_ctrl(0xC0190038, 0x00ee0000);
    REG_WRITE_ctrl(0xC0190050, 0x00210004);
    REG_WRITE_ctrl(0xC0190050, 0x00210024);
    REG_WRITE_ctrl(0xC0190050, 0x0021b024);
    REG_WRITE_ctrl(0xC0190054, 0x00000002);
    REG_WRITE_ctrl(0xC0190054, 0x00010002);
    REG_WRITE_ctrl(0xC0190060, 0x00000000);
    REG_WRITE_ctrl(0xC0190060, 0x00000000);
    REG_WRITE_ctrl(0xC0190064, 0x00620046);
    REG_WRITE_ctrl(0xC0190064, 0x00040007);
    REG_WRITE_ctrl(0xC01900c0, 0x00000000);
    REG_WRITE_ctrl(0xC01900c0, 0x00000000);
    REG_WRITE_ctrl(0xC01900c0, 0x00000000);
    REG_WRITE_ctrl(0xC01900d0, 0x00020001);
    REG_WRITE_ctrl(0xC01900d0, 0x00020001);
    REG_WRITE_ctrl(0xC01900d0, 0x40020001);
    REG_WRITE_ctrl(0xC01900d4, 0x0000000c);
    REG_WRITE_ctrl(0xC01900d4, 0x0000030c);
    REG_WRITE_ctrl(0xC01900d4, 0x0001030c);
    REG_WRITE_ctrl(0xC01900d8, 0x00000d06);
    REG_WRITE_ctrl(0xC01900d8, 0x00000206);
    REG_WRITE_ctrl(0xC01900dc, 0x00000000);
    REG_WRITE_ctrl(0xC01900dc, 0x09410000);
    REG_WRITE_ctrl(0xC01900e0, 0x00000000);
    REG_WRITE_ctrl(0xC01900e0, 0x00080000);
    REG_WRITE_ctrl(0xC01900e4, 0x00100007);
    REG_WRITE_ctrl(0xC01900e4, 0x00090007);
    REG_WRITE_ctrl(0xC01900f0, 0x00000000);
    REG_WRITE_ctrl(0xC01900f0, 0x00000000);
    REG_WRITE_ctrl(0xC01900f4, 0x00000667);
    REG_WRITE_ctrl(0xC01900f4, 0x000006e7);
    REG_WRITE_ctrl(0xC01900f4, 0x000008e7);
    REG_WRITE_ctrl(0xC0190100, 0x0f101b0a);
    REG_WRITE_ctrl(0xC0190100, 0x0f10110a);
    REG_WRITE_ctrl(0xC0190100, 0x0f0e110a);
    REG_WRITE_ctrl(0xC0190100, 0x090e110a);
    REG_WRITE_ctrl(0xC0190104, 0x0008040e);
    REG_WRITE_ctrl(0xC0190104, 0x0008020e);
    REG_WRITE_ctrl(0xC0190104, 0x0007020e);
    REG_WRITE_ctrl(0xC0190108, 0x03050607);
    REG_WRITE_ctrl(0xC0190108, 0x03050407);
    REG_WRITE_ctrl(0xC0190108, 0x03040407);
    REG_WRITE_ctrl(0xC0190108, 0x03040407);
    REG_WRITE_ctrl(0xC019010c, 0x00504008);
    REG_WRITE_ctrl(0xC019010c, 0x00502008);
    REG_WRITE_ctrl(0xC019010c, 0x00002008);
    REG_WRITE_ctrl(0xC0190110, 0x05040405);
    REG_WRITE_ctrl(0xC0190110, 0x05040305);
    REG_WRITE_ctrl(0xC0190110, 0x05020305);
    REG_WRITE_ctrl(0xC0190110, 0x04020305);
    REG_WRITE_ctrl(0xC0190114, 0x05050402);
    REG_WRITE_ctrl(0xC0190114, 0x05050202);
    REG_WRITE_ctrl(0xC0190114, 0x050c0202);
    REG_WRITE_ctrl(0xC0190114, 0x0e0c0202);
    REG_WRITE_ctrl(0xC0190118, 0x02020000);
    REG_WRITE_ctrl(0xC0190118, 0x020e0000);
    REG_WRITE_ctrl(0xC0190118, 0x0c0e0000);
    REG_WRITE_ctrl(0xC019011c, 0x00000204);
    REG_WRITE_ctrl(0xC019011c, 0x00000204);
    REG_WRITE_ctrl(0xC0190120, 0x00004408);
    REG_WRITE_ctrl(0xC0190120, 0x00000a08);
    REG_WRITE_ctrl(0xC0190180, 0x02000020);
    REG_WRITE_ctrl(0xC0190180, 0x00800020);
    REG_WRITE_ctrl(0xC0190180, 0x20800020);
    REG_WRITE_ctrl(0xC0190180, 0x20800020);
    REG_WRITE_ctrl(0xC0190180, 0x20800020);
    REG_WRITE_ctrl(0xC0190184, 0x02000070);
    REG_WRITE_ctrl(0xC0190184, 0x00000070);
    REG_WRITE_ctrl(0xC0190188, 0x00000000);
    REG_WRITE_ctrl(0xC0190190, 0x07020002);
    REG_WRITE_ctrl(0xC0190190, 0x07020102);
    REG_WRITE_ctrl(0xC0190190, 0x07020102);
    REG_WRITE_ctrl(0xC0190190, 0x07030102);
    REG_WRITE_ctrl(0xC0190190, 0x07030102);
    REG_WRITE_ctrl(0xC0190190, 0x02000102);
    REG_WRITE_ctrl(0xC0190194, 0x00000402);
    REG_WRITE_ctrl(0xC0190194, 0x00000102);
    REG_WRITE_ctrl(0xC0190194, 0x00030102);
    REG_WRITE_ctrl(0xC0190198, 0x07000000);
    REG_WRITE_ctrl(0xC0190198, 0x07000030);
    REG_WRITE_ctrl(0xC0190198, 0x07000030);
    REG_WRITE_ctrl(0xC0190198, 0x07003030);
    REG_WRITE_ctrl(0xC0190198, 0x07003030);
    REG_WRITE_ctrl(0xC0190198, 0x07503030);
    REG_WRITE_ctrl(0xC0190198, 0x07503030);
    REG_WRITE_ctrl(0xC01901a0, 0x00400003);
    REG_WRITE_ctrl(0xC01901a0, 0x00400003);
    REG_WRITE_ctrl(0xC01901a0, 0x00400003);
    REG_WRITE_ctrl(0xC01901a4, 0x000000d7);
    REG_WRITE_ctrl(0xC01901a4, 0x00ae00d7);
    REG_WRITE_ctrl(0xC01901a8, 0x8010015d);
    REG_WRITE_ctrl(0xC01901a8, 0x8e68015d);
    REG_WRITE_ctrl(0xC01901a8, 0x8e68015d);
    REG_WRITE_ctrl(0xC01901ac, 0x00100aed);
    REG_WRITE_ctrl(0xC01901ac, 0x00310aed);
    REG_WRITE_ctrl(0xC01901b0, 0x00000001);
    REG_WRITE_ctrl(0xC01901b4, 0x00000203);
    REG_WRITE_ctrl(0xC01901b4, 0x00000303);
    REG_WRITE_ctrl(0xC01901b8, 0x01010101);
    REG_WRITE_ctrl(0xC01901b8, 0x01010101);
    REG_WRITE_ctrl(0xC01901b8, 0x01010101);
    REG_WRITE_ctrl(0xC01901b8, 0x01010101);
    REG_WRITE_ctrl(0xC01901d0, 0x00280000);
    REG_WRITE_ctrl(0xC01901d0, 0x00280000);
    REG_WRITE_ctrl(0xC01901d0, 0x00280000);
    REG_WRITE_ctrl(0xC01901d0, 0x03c80000);
    REG_WRITE_ctrl(0xC01901d4, 0x00000fff);
    REG_WRITE_ctrl(0xC01901d4, 0x0fff0fff);
    REG_WRITE_ctrl(0xC01901d8, 0x00270078);
    REG_WRITE_ctrl(0xC01901d8, 0x00970078);
    REG_WRITE_ctrl(0xC0190200, 0x00000007);
    REG_WRITE_ctrl(0xC0190204, 0x00000000);
    REG_WRITE_ctrl(0xC0190204, 0x00000000);
    REG_WRITE_ctrl(0xC0190204, 0x00010000);
    REG_WRITE_ctrl(0xC0190208, 0x00000002);
    REG_WRITE_ctrl(0xC0190208, 0x00000502);
    REG_WRITE_ctrl(0xC0190208, 0x00020502);
    REG_WRITE_ctrl(0xC0190208, 0x04020502);
    REG_WRITE_ctrl(0xC019020c, 0x00000006);
    REG_WRITE_ctrl(0xC019020c, 0x00000406);
    REG_WRITE_ctrl(0xC019020c, 0x00060406);
    REG_WRITE_ctrl(0xC019020c, 0x0f060406);
    REG_WRITE_ctrl(0xC0190210, 0x0000000f);
    REG_WRITE_ctrl(0xC0190210, 0x00000f0f);
    REG_WRITE_ctrl(0xC0190214, 0x00000001);
    REG_WRITE_ctrl(0xC0190214, 0x00000301);
    REG_WRITE_ctrl(0xC0190214, 0x00070301);
    REG_WRITE_ctrl(0xC0190214, 0x07070301);
    REG_WRITE_ctrl(0xC0190218, 0x00000008);
    REG_WRITE_ctrl(0xC0190218, 0x00000808);
    REG_WRITE_ctrl(0xC0190218, 0x00050808);
    REG_WRITE_ctrl(0xC0190218, 0x0f050808);
    REG_WRITE_ctrl(0xC0190240, 0x04000424);
    REG_WRITE_ctrl(0xC0190240, 0x04000c24);
    REG_WRITE_ctrl(0xC0190240, 0x040e0c24);
    REG_WRITE_ctrl(0xC0190240, 0x080e0c24);
    REG_WRITE_ctrl(0xC0190244, 0x00002210);
    REG_WRITE_ctrl(0xC0190244, 0x00002200);
    REG_WRITE_ctrl(0xC0190244, 0x00002300);
    REG_WRITE_ctrl(0xC0190244, 0x00003300);
    REG_WRITE_ctrl(0xC0190250, 0x00000305);
    REG_WRITE_ctrl(0xC0190250, 0x00000305);
    REG_WRITE_ctrl(0xC0190250, 0x00000305);
    REG_WRITE_ctrl(0xC0190250, 0x00000305);
    REG_WRITE_ctrl(0xC0190250, 0x00f90305);
    REG_WRITE_ctrl(0xC0190250, 0x62f90305);
    REG_WRITE_ctrl(0xC0190254, 0x0000000a);
    REG_WRITE_ctrl(0xC019025c, 0x0f001b37);
    REG_WRITE_ctrl(0xC019025c, 0x5b001b37);
    REG_WRITE_ctrl(0xC0190264, 0x0f0001dd);
    REG_WRITE_ctrl(0xC0190264, 0x300001dd);
    REG_WRITE_ctrl(0xC019026c, 0x0f007be1);
    REG_WRITE_ctrl(0xC019026c, 0xe3007be1);
    REG_WRITE_ctrl(0xC0190300, 0x00000001);
    REG_WRITE_ctrl(0xC0190300, 0x00000001);
    REG_WRITE_ctrl(0xC0190300, 0x00000001);
    REG_WRITE_ctrl(0xC0190300, 0x00000001);
    REG_WRITE_ctrl(0xC0190304, 0x00000000);
    REG_WRITE_ctrl(0xC0190304, 0x00000000);
    REG_WRITE_ctrl(0xC019030c, 0x00000000);
    REG_WRITE_ctrl(0xC019030c, 0x00000000);
    REG_WRITE_ctrl(0xC019030c, 0x00000000);
    REG_WRITE_ctrl(0xC019030c, 0x00000000);
    REG_WRITE_ctrl(0xC0190320, 0x00000001);
    REG_WRITE_ctrl(0xC0190400, 0x00000001);
    REG_WRITE_ctrl(0xC0190404, 0x00005045);
    REG_WRITE_ctrl(0xC0190408, 0x0000309b);
    REG_WRITE_ctrl(0xC0190490, 0x00000001);
    REG_WRITE_ctrl(0xC0190304, 0x00000000);
}

int ddr_init(void)
{
    volatile u32 ddr_rd_st, pgcr, pgsr, swstat;
    /*Setp 1*/
    REG_WRITE_ctrl(0xC0190304, 0x00000001); /*disable pop queue*/
    REG_WRITE_ctrl(0xC0190030, 0x00000001); /*enable self_refresh*/

    /*Setp 2*/
    ddr_rd_st = REG_READ_ctrl(0xC0190004); /*read and reset value?*/
    ddr_printf("2. ddr rd status: 0x%x\n", ddr_rd_st);
    while (ddr_rd_st != 0) {
        ddr_rd_st = REG_READ_ctrl(0xC0190004);
    }
    ddr_printf("2. ddr rd status: 0x%x\n", ddr_rd_st);

    /*Setp 3*/
    ddr_controller_init();
    ddr_printf("3. ddr controller init done\n");

    /*Setp 4*/
    REG_READ_ctrl(0xC0190030);
    REG_WRITE_ctrl(0xC0190030, 0x00000000);
    REG_READ_ctrl(0xC0190030);
    REG_WRITE_ctrl(0xC0190030, 0x00000000);

    /*Setp 5*/
    REG_WRITE_ctrl(0xC0190320, 0x00000000);
    REG_WRITE_ctrl(0xC01901b0, 0x00000000);

    /*phy init */
    /*Setp 6*/
    pgcr = REG_READ_ctrl(0xC01A0008);
    printf("6. phy pgcr = 0x%x\n", pgcr);

    /*Setp 7*/
    ddr_phy_init();
    ddr_printf("7. ddr phy init done\n");

    /*Setp 8*/
    pgsr = REG_READ_ctrl(0xC01A000c);
    ddr_printf("8. ddr phy pgsr: 0x%x\n", pgsr);
    while (pgsr != 0x7) {
        pgsr = REG_READ_ctrl(0xC01A000c);
        ddr_printf("8. ddr phy pgsr: 0x%x\n", pgsr);
    }

    /*Step 9*/
    REG_WRITE_ctrl(0xC01A0004, 0x00000061);

    /*Step 10*/
    pgsr = REG_READ_ctrl(0xC01A000c);
    ddr_printf("ddr phy pgsr: 0x%x\n", pgsr);
    while (pgsr != 0xf) {
        pgsr = REG_READ_ctrl(0xC01A000c);
        ddr_printf("ddr phy pgsr: 0x%x\n", pgsr);
    }

    /*Step 11*/
    REG_WRITE_ctrl(0xC01901b0, 0x00000001);
    REG_WRITE_ctrl(0xC0190320, 0x00000001);

    /*Step 13*/
    swstat = REG_READ_ctrl(0xC0190324);
    while (swstat != 0x1)
        swstat = REG_READ_ctrl(0xC0190324);
    ddr_printf("ddr swstat: 0x%x\n", swstat);

    /*Step 14*/
    ddr_rd_st = REG_READ_ctrl(0xC0190004);
    ddr_printf("ddr rd status: 0x%x\n", ddr_rd_st);
    while (ddr_rd_st != 1) {
        ddr_rd_st = REG_READ_ctrl(0xC0190004);
    }
    ddr_printf("ddr rd status: 0x%x\n", ddr_rd_st);

    /*Step 15*/
    REG_WRITE_ctrl(0xC0190030, 0x00000000);

    return 0;
}
#endif

// ddr controller init func
#include <pll.h>
#include <drivers/drv_clk.h>
#include <drivers/drv_reset.h>
#include "fastboot.h"
void ddrc_init(void)
{
#ifdef CONFIG_LPDDR
  #ifdef CONFIG_FASTBOOT
    // for fastboot, (bp2016)if fw call go 0 api, set pll1 maybe hang.
    // fastboot not set pll1 again, only re-config ddr_printf
    if(RET_FAILED == fastboot_pll1_set(800000)){
        ddr_printf("fastboot pll1 set 800M error!\n");
        return;
    }
  #else
    if(RET_FAILED == pll1_set_rate_kHz(800000)){
        ddr_printf("pll1 set 800M error!\n");
        return;
    }
  #endif

    if(CLK_OP_SUCCESS != clk_set_rate_kHz(CLK_DDR, 200000)){
        ddr_printf("lpddr clk set 200M error!\n");
        return;
    }
    swrst_ddr_reset();
    ddr_printf("\nlpddr init~~~!\n");
    lpddr_init();
#elif CONFIG_LPDDR2
    if(RET_FAILED == pll1_set_rate_kHz(800000)){
        ddr_printf("pll1 set 800M error!\n");
        return;
    }
    if(CLK_OP_SUCCESS != clk_set_rate_kHz(CLK_DDR, 200000)){
        ddr_printf("ddr clk set 200M error!\n");
        return;
    }
    swrst_ddr_reset();
    ddr_printf("\nlpddr2 init~~~!\n");
    lpddr2_init();
#elif CONFIG_DDR3
    if(RET_FAILED == pll1_set_rate_kHz(1060000)){
        ddr_printf("pll1 set 1060M error!\n");
        return;
    }
    if(CLK_OP_SUCCESS != clk_set_rate_kHz(CLK_DDR, 530000)){
        ddr_printf("ddr clk set 530M error!\n");
        return;
    }
    swrst_ddr_reset();
    ddr_printf("\nddr3 init~~~!\n");
    ddr3_init();
#elif CONFIG_DDR_FPGA
    swrst_ddr_reset();
    ddr_printf("\nddr-fpga init~~~!\n");
    ddr_init();
#endif
}
