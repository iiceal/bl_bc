#include <drivers/drv_ddr.h>

#ifdef CONFIG_DDR3
void ddr3_phy_init(void)
{
    //181202

REG_WRITE_ctrl(0xC01A0008,0x078c2e04);//41466778.0 ps
REG_WRITE_ctrl(0xC01A0008,0x07842e04);//41519458.0 ps
REG_WRITE_ctrl(0xC01A0030,0x0000048b);//41572138.0 ps
REG_WRITE_ctrl(0xC01A0018,0x10000010);//41782858.0 ps
REG_WRITE_ctrl(0xC01A001c,0x00800080);//41993578.0 ps
REG_WRITE_ctrl(0xC01A0020,0x00080421);//42204298.0 ps
REG_WRITE_ctrl(0xC01A0040,0x00000d61);//42256978.0 ps
REG_WRITE_ctrl(0xC01A0040,0x00000d41);//42309658.0 ps
REG_WRITE_ctrl(0xC01A0044,0x00000044);//42362338.0 ps
REG_WRITE_ctrl(0xC01A0048,0x00000408);//42415018.0 ps
REG_WRITE_ctrl(0xC01A004c,0x00000000);//42467698.0 ps
REG_WRITE_ctrl(0xC01A0034,0x38d48890);//42678418.0 ps
REG_WRITE_ctrl(0xC01A0038,0x388b80d8);//42878602.0 ps
REG_WRITE_ctrl(0xC01A003c,0x10019004);//43078786.0 ps
REG_WRITE_ctrl(0xC01A002c,0xfa00001f);//43426474.0 ps
REG_WRITE_ctrl(0xC01A001c,0x07a52eda);//43847914.0 ps
REG_WRITE_ctrl(0xC01A0020,0x042ab6ad);//44332570.0 ps
    REG_WRITE_ctrl(0xC01A01d4, 0x3db0a009);
    REG_WRITE_ctrl(0xC01A0214, 0x3db0a009);
    ddr_printf("0xc01a0008 = %x\n", REG_READ_ctrl(0xc01a0008));
}

void ddr3_controller_init(void)
{
//181202
REG_WRITE_ctrl(0xC0190000,0x03040001);//12819394.0 ps
REG_WRITE_ctrl(0xC0190000,0x03040001);//12893146.0 ps
REG_WRITE_ctrl(0xC0190000,0x03040001);//12966898.0 ps
REG_WRITE_ctrl(0xC0190000,0x03040201);//13040650.0 ps
REG_WRITE_ctrl(0xC0190000,0x03040201);//13114402.0 ps
REG_WRITE_ctrl(0xC0190000,0x03040201);//13188154.0 ps
REG_WRITE_ctrl(0xC0190000,0x03040201);//13261906.0 ps
REG_WRITE_ctrl(0xC0190000,0x03040201);//13335658.0 ps
REG_WRITE_ctrl(0xC0190000,0x01040201);//13409410.0 ps
REG_WRITE_ctrl(0xC0190010,0x00000030);//13820314.0 ps
REG_WRITE_ctrl(0xC0190010,0x00000030);//14231218.0 ps
REG_WRITE_ctrl(0xC0190010,0x00003030);//14642122.0 ps
REG_WRITE_ctrl(0xC0190010,0x00003030);//15053026.0 ps
REG_WRITE_ctrl(0xC0190014,0x000063e5);//15463930.0 ps
REG_WRITE_ctrl(0xC0190020,0x00000000);//15874834.0 ps
REG_WRITE_ctrl(0xC0190020,0x00000000);//16285738.0 ps
REG_WRITE_ctrl(0xC0190024,0x602952c0);//16517530.0 ps
REG_WRITE_ctrl(0xC0190030,0x00000000);//16928434.0 ps
REG_WRITE_ctrl(0xC0190030,0x00000000);//17339338.0 ps
REG_WRITE_ctrl(0xC0190030,0x00000000);//17750242.0 ps
REG_WRITE_ctrl(0xC0190030,0x00000000);//18161146.0 ps
REG_WRITE_ctrl(0xC0190030,0x00000000);//18572050.0 ps
REG_WRITE_ctrl(0xC0190034,0x00402002);//18803842.0 ps
REG_WRITE_ctrl(0xC0190034,0x0040c902);//18877594.0 ps
REG_WRITE_ctrl(0xC0190034,0x000ec902);//18951346.0 ps
REG_WRITE_ctrl(0xC0190038,0x00000002);//19025098.0 ps
REG_WRITE_ctrl(0xC0190038,0x00000000);//19098850.0 ps
REG_WRITE_ctrl(0xC0190038,0x00ee0000);//19172602.0 ps
REG_WRITE_ctrl(0xC0190050,0x00210004);//19741546.0 ps
REG_WRITE_ctrl(0xC0190050,0x00210024);//20152450.0 ps
REG_WRITE_ctrl(0xC0190050,0x0021b024);//20331562.0 ps
REG_WRITE_ctrl(0xC0190050,0x0021b024);//20436922.0 ps
REG_WRITE_ctrl(0xC0190054,0x00000002);//20552818.0 ps
REG_WRITE_ctrl(0xC0190054,0x00020001);//20816218.0 ps
REG_WRITE_ctrl(0xC0190060,0x00000000);//20932114.0 ps
REG_WRITE_ctrl(0xC0190060,0x00000000);//21048010.0 ps
REG_WRITE_ctrl(0xC0190064,0x00620046);//21311410.0 ps
REG_WRITE_ctrl(0xC0190064,0x0041005e);//21574810.0 ps
REG_WRITE_ctrl(0xC01900c0,0x00000000);//21690706.0 ps
REG_WRITE_ctrl(0xC01900c0,0x00000000);//21806602.0 ps
REG_WRITE_ctrl(0xC01900c0,0x00000000);//21922498.0 ps
REG_WRITE_ctrl(0xC01900d0,0x00020001);//22143754.0 ps
REG_WRITE_ctrl(0xC01900d0,0x00020001);//22217506.0 ps
REG_WRITE_ctrl(0xC01900d0,0x40020001);//22291258.0 ps
REG_WRITE_ctrl(0xC01900d4,0x0000000c);//22365010.0 ps
REG_WRITE_ctrl(0xC01900d4,0x0000030c);//22438762.0 ps
REG_WRITE_ctrl(0xC01900d4,0x0001030c);//22512514.0 ps
REG_WRITE_ctrl(0xC01900d8,0x00000d06);//22586266.0 ps
REG_WRITE_ctrl(0xC01900d8,0x00000206);//22660018.0 ps
REG_WRITE_ctrl(0xC01900dc,0x00000000);//22733770.0 ps
REG_WRITE_ctrl(0xC01900dc,0x09410000);//22965562.0 ps
REG_WRITE_ctrl(0xC01900e0,0x00000000);//23039314.0 ps
REG_WRITE_ctrl(0xC01900e0,0x00080000);//23113066.0 ps
REG_WRITE_ctrl(0xC01900e4,0x00100007);//23334322.0 ps
REG_WRITE_ctrl(0xC01900e4,0x00090007);//23408074.0 ps
REG_WRITE_ctrl(0xC01900f0,0x00000000);//23481826.0 ps
REG_WRITE_ctrl(0xC01900f0,0x00000000);//23555578.0 ps
REG_WRITE_ctrl(0xC01900f4,0x00000667);//23629330.0 ps
REG_WRITE_ctrl(0xC01900f4,0x000006e7);//23703082.0 ps
REG_WRITE_ctrl(0xC01900f4,0x000008e7);//23776834.0 ps
REG_WRITE_ctrl(0xC0190100,0x0f101b0a);//23998090.0 ps
REG_WRITE_ctrl(0xC0190100,0x0f10110a);//24071842.0 ps
REG_WRITE_ctrl(0xC0190100,0x0f0e110a);//24145594.0 ps
REG_WRITE_ctrl(0xC0190100,0x0b0e110c);//24219346.0 ps
REG_WRITE_ctrl(0xC0190104,0x0008040e);//24451138.0 ps
REG_WRITE_ctrl(0xC0190104,0x0008020e);//24524890.0 ps
REG_WRITE_ctrl(0xC0190104,0x0007020e);//24598642.0 ps
REG_WRITE_ctrl(0xC0190108,0x03050607);//24830434.0 ps
REG_WRITE_ctrl(0xC0190108,0x03050407);//24904186.0 ps
REG_WRITE_ctrl(0xC0190108,0x03040407);//24977938.0 ps
REG_WRITE_ctrl(0xC0190108,0x03040409);//25051690.0 ps
REG_WRITE_ctrl(0xC019010c,0x00504008);//25283482.0 ps
REG_WRITE_ctrl(0xC019010c,0x00502008);//25357234.0 ps
REG_WRITE_ctrl(0xC019010c,0x00002008);//25430986.0 ps
REG_WRITE_ctrl(0xC0190110,0x05040405);//25652242.0 ps
REG_WRITE_ctrl(0xC0190110,0x05040305);//25725994.0 ps
REG_WRITE_ctrl(0xC0190110,0x05020305);//25799746.0 ps
REG_WRITE_ctrl(0xC0190110,0x04020305);//25873498.0 ps
REG_WRITE_ctrl(0xC0190114,0x05050402);//26105290.0 ps
REG_WRITE_ctrl(0xC0190114,0x05050202);//26179042.0 ps
REG_WRITE_ctrl(0xC0190114,0x050c0202);//26252794.0 ps
REG_WRITE_ctrl(0xC0190114,0x0e0c0202);//26326546.0 ps
REG_WRITE_ctrl(0xC0190118,0x02020000);//26400298.0 ps
REG_WRITE_ctrl(0xC0190118,0x020e0000);//26474050.0 ps
REG_WRITE_ctrl(0xC0190118,0x0c0e0000);//26547802.0 ps
REG_WRITE_ctrl(0xC019011c,0x00000204);//26621554.0 ps
REG_WRITE_ctrl(0xC019011c,0x00000204);//26695306.0 ps
REG_WRITE_ctrl(0xC0190120,0x00004408);//26769058.0 ps
REG_WRITE_ctrl(0xC0190120,0x00000a08);//26842810.0 ps
REG_WRITE_ctrl(0xC0190180,0x02000020);//27064066.0 ps
REG_WRITE_ctrl(0xC0190180,0x00800020);//27137818.0 ps
REG_WRITE_ctrl(0xC0190180,0x20800020);//27211570.0 ps
REG_WRITE_ctrl(0xC0190180,0x20800020);//27285322.0 ps
REG_WRITE_ctrl(0xC0190180,0x20800020);//27359074.0 ps
REG_WRITE_ctrl(0xC0190184,0x02000070);//27590866.0 ps
REG_WRITE_ctrl(0xC0190184,0x00000070);//27664618.0 ps
REG_WRITE_ctrl(0xC0190188,0x00000000);//28075522.0 ps
REG_WRITE_ctrl(0xC0190190,0x07020002);//28307314.0 ps
REG_WRITE_ctrl(0xC0190190,0x07020102);//28381066.0 ps
REG_WRITE_ctrl(0xC0190190,0x07020102);//28454818.0 ps
REG_WRITE_ctrl(0xC0190190,0x07030102);//28528570.0 ps
REG_WRITE_ctrl(0xC0190190,0x07030102);//28602322.0 ps
REG_WRITE_ctrl(0xC0190190,0x02030102);//28676074.0 ps
REG_WRITE_ctrl(0xC0190194,0x00000402);//28749826.0 ps
REG_WRITE_ctrl(0xC0190194,0x00000102);//28823578.0 ps
REG_WRITE_ctrl(0xC0190194,0x00030102);//28897330.0 ps
REG_WRITE_ctrl(0xC0190198,0x07000000);//28971082.0 ps
REG_WRITE_ctrl(0xC0190198,0x07000030);//29044834.0 ps
REG_WRITE_ctrl(0xC0190198,0x07000030);//29118586.0 ps
REG_WRITE_ctrl(0xC0190198,0x07003030);//29192338.0 ps
REG_WRITE_ctrl(0xC0190198,0x07003030);//29266090.0 ps
REG_WRITE_ctrl(0xC0190198,0x07503030);//29339842.0 ps
REG_WRITE_ctrl(0xC0190198,0x07503030);//29413594.0 ps
REG_WRITE_ctrl(0xC01901a0,0x00400003);//29645386.0 ps
REG_WRITE_ctrl(0xC01901a0,0x00400003);//29719138.0 ps
REG_WRITE_ctrl(0xC01901a0,0x00400003);//29792890.0 ps
REG_WRITE_ctrl(0xC01901a4,0x000000d7);//29866642.0 ps
REG_WRITE_ctrl(0xC01901a4,0x00ae00d7);//29940394.0 ps
REG_WRITE_ctrl(0xC01901a8,0x8010015d);//30172186.0 ps
REG_WRITE_ctrl(0xC01901a8,0x8e68015d);//30393442.0 ps
REG_WRITE_ctrl(0xC01901a8,0x8e68015d);//30467194.0 ps
REG_WRITE_ctrl(0xC01901ac,0x00100aed);//30698986.0 ps
REG_WRITE_ctrl(0xC01901ac,0x00310aed);//30772738.0 ps
REG_WRITE_ctrl(0xC01901b0,0x00000001);//30846490.0 ps
REG_WRITE_ctrl(0xC01901b4,0x00000203);//30920242.0 ps
REG_WRITE_ctrl(0xC01901b4,0x00000303);//30993994.0 ps
REG_WRITE_ctrl(0xC01901b8,0x01010101);//31225786.0 ps
REG_WRITE_ctrl(0xC01901b8,0x01010101);//31299538.0 ps
REG_WRITE_ctrl(0xC01901b8,0x01010101);//31373290.0 ps
REG_WRITE_ctrl(0xC01901b8,0x01010101);//31447042.0 ps
REG_WRITE_ctrl(0xC01901d0,0x00280000);//31520794.0 ps
REG_WRITE_ctrl(0xC01901d0,0x00280000);//31594546.0 ps
REG_WRITE_ctrl(0xC01901d0,0x00280000);//31668298.0 ps
REG_WRITE_ctrl(0xC01901d0,0x03c80000);//31742050.0 ps
REG_WRITE_ctrl(0xC01901d4,0x00000fff);//31815802.0 ps
REG_WRITE_ctrl(0xC01901d4,0x0fff0fff);//31889554.0 ps
REG_WRITE_ctrl(0xC01901d8,0x00270078);//32110810.0 ps
REG_WRITE_ctrl(0xC01901d8,0x00970078);//32184562.0 ps
REG_WRITE_ctrl(0xC0190200,0x0000001f);//32258314.0 ps
REG_WRITE_ctrl(0xC0190204,0x00000000);//32332066.0 ps
REG_WRITE_ctrl(0xC0190204,0x00000000);//32405818.0 ps
REG_WRITE_ctrl(0xC0190204,0x00000300);//32479570.0 ps
REG_WRITE_ctrl(0xC0190208,0x00000002);//32553322.0 ps
REG_WRITE_ctrl(0xC0190208,0x00000502);//32627074.0 ps
REG_WRITE_ctrl(0xC0190208,0x00020502);//32700826.0 ps
REG_WRITE_ctrl(0xC0190208,0x02040003);//32774578.0 ps
REG_WRITE_ctrl(0xC019020c,0x00000006);//32848330.0 ps
REG_WRITE_ctrl(0xC019020c,0x00000406);//32922082.0 ps
REG_WRITE_ctrl(0xC019020c,0x00060406);//32995834.0 ps
REG_WRITE_ctrl(0xC019020c,0x0f020407);//33069586.0 ps
REG_WRITE_ctrl(0xC0190210,0x0000000f);//33143338.0 ps
REG_WRITE_ctrl(0xC0190210,0x00000f0f);//33217090.0 ps
REG_WRITE_ctrl(0xC0190214,0x00000001);//33290842.0 ps
REG_WRITE_ctrl(0xC0190214,0x00000301);//33364594.0 ps
REG_WRITE_ctrl(0xC0190214,0x00070301);//33438346.0 ps
REG_WRITE_ctrl(0xC0190214,0x06060206);//33512098.0 ps
REG_WRITE_ctrl(0xC0190218,0x00000008);//33585850.0 ps
REG_WRITE_ctrl(0xC0190218,0x00000808);//33659602.0 ps
REG_WRITE_ctrl(0xC0190218,0x00050808);//33733354.0 ps
REG_WRITE_ctrl(0xC0190218,0x06040707);//33807106.0 ps
REG_WRITE_ctrl(0xC0190240,0x04000424);//34038898.0 ps
REG_WRITE_ctrl(0xC0190240,0x04000c24);//34112650.0 ps
REG_WRITE_ctrl(0xC0190240,0x040e0c24);//34186402.0 ps
REG_WRITE_ctrl(0xC0190240,0x0706081c);//34418194.0 ps
REG_WRITE_ctrl(0xC0190244,0x00002210);//34491946.0 ps
REG_WRITE_ctrl(0xC0190244,0x00002200);//34565698.0 ps
REG_WRITE_ctrl(0xC0190244,0x00002300);//34639450.0 ps
REG_WRITE_ctrl(0xC0190244,0x00000102);//34713202.0 ps
REG_WRITE_ctrl(0xC0190250,0x00000305);//34786954.0 ps
REG_WRITE_ctrl(0xC0190250,0x00000305);//34860706.0 ps
REG_WRITE_ctrl(0xC0190250,0x00000305);//34934458.0 ps
REG_WRITE_ctrl(0xC0190250,0x00000305);//35008210.0 ps
REG_WRITE_ctrl(0xC0190250,0x00f90305);//35081962.0 ps
REG_WRITE_ctrl(0xC0190250,0x62f90305);//35155714.0 ps
REG_WRITE_ctrl(0xC0190250,0x62f90301);//35229466.0 ps
REG_WRITE_ctrl(0xC0190254,0x0000000a);//35303218.0 ps
REG_WRITE_ctrl(0xC019025c,0x0f001b37);//35535010.0 ps
REG_WRITE_ctrl(0xC019025c,0x5b001b37);//35608762.0 ps
REG_WRITE_ctrl(0xC0190264,0x0f0001dd);//35840554.0 ps
REG_WRITE_ctrl(0xC0190264,0x300001dd);//35914306.0 ps
REG_WRITE_ctrl(0xC019026c,0x0f007be1);//36146098.0 ps
REG_WRITE_ctrl(0xC019026c,0xe3007be1);//36219850.0 ps
REG_WRITE_ctrl(0xC0190300,0x00000001);//36293602.0 ps
REG_WRITE_ctrl(0xC0190300,0x00000001);//36367354.0 ps
REG_WRITE_ctrl(0xC0190300,0x00000001);//36441106.0 ps
REG_WRITE_ctrl(0xC0190300,0x00000001);//36514858.0 ps
REG_WRITE_ctrl(0xC0190304,0x00000000);//36620218.0 ps
REG_WRITE_ctrl(0xC0190304,0x00000000);//36736114.0 ps
REG_WRITE_ctrl(0xC019030c,0x00000000);//37147018.0 ps
REG_WRITE_ctrl(0xC019030c,0x00000000);//37557922.0 ps
REG_WRITE_ctrl(0xC019030c,0x00000000);//37968826.0 ps
REG_WRITE_ctrl(0xC019030c,0x00000000);//38379730.0 ps
REG_WRITE_ctrl(0xC0190320,0x00000001);//38453482.0 ps
REG_WRITE_ctrl(0xC0190400,0x00000001);//38685274.0 ps
REG_WRITE_ctrl(0xC0190404,0x00005045);//38759026.0 ps
REG_WRITE_ctrl(0xC0190408,0x0000309b);//38832778.0 ps
REG_WRITE_ctrl(0xC0190494,0x02100100);//39064570.0 ps
REG_WRITE_ctrl(0xC0190490,0x00000001);//39180466.0 ps
REG_WRITE_ctrl(0xC0190304,0x00000000);//39612442.0 ps


}

int ddr3_init(void)
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
    ddr3_controller_init();
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
    ddr3_phy_init();
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

	mdelay(100);
    REG_WRITE_ctrl(0xC01a0004, 0x00000181);  //enable data trainning
	//mdelay(10);
    //REG_WRITE_ctrl(0xC01a0004, 0x00000183);  //enable data trainning + dll reset
	//mdelay(10);
	ddr_printf("0xC01A000c = %x\n", REG_READ_ctrl(0xC01A000c));
    //REG_WRITE_ctrl(0xC01a01d4, 0x3db0800a); // lichunlei 20181207 , ddr3l , 510m~540m
    //REG_WRITE_ctrl(0xC01a0214, 0x3db0800a);
    return 0;
}
#endif
