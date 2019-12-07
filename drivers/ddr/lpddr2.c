#include <drivers/drv_ddr.h>

#ifdef CONFIG_LPDDR2

static void lpddr2_phy_init(void)
{
    REG_WRITE_ctrl(0xC01A0008,0x0b842e02);//at:156796.658 ns
    REG_WRITE_ctrl(0xC01A01c0,0x00090881);//at:157818.650 ns
    REG_WRITE_ctrl(0xC01A0200,0x00090881);//at:158577.242 ns
    REG_WRITE_ctrl(0xC01A0028,0x00000c40);//at:159335.834 ns
    REG_WRITE_ctrl(0xC01A002c,0xfa00025f);//at:160357.826 ns
    REG_WRITE_ctrl(0xC01A0030,0x0000040c);//at:160410.506 ns
    REG_WRITE_ctrl(0xC01A0018,0x10000010);//at:161063.738 ns
    REG_WRITE_ctrl(0xC01A001c,0x00800080);//at:161716.970 ns
    REG_WRITE_ctrl(0xC01A0020,0x00080421);//at:162359.666 ns
    REG_WRITE_ctrl(0xC01A0040,0x00000082);//at:162412.346 ns
    REG_WRITE_ctrl(0xC01A0044,0x00000082);//at:162465.026 ns
    REG_WRITE_ctrl(0xC01A0048,0x00000004);//at:162517.706 ns
    REG_WRITE_ctrl(0xC01A004c,0x00000002);//at:162570.386 ns
    REG_WRITE_ctrl(0xC01A0034,0x3491886d);//at:163223.618 ns
    REG_WRITE_ctrl(0xC01A0038,0x193480a0);//at:163876.850 ns
    REG_WRITE_ctrl(0xC01A003c,0x10030c03);//at:164519.546 ns
    REG_WRITE_ctrl(0xC01A002c,0xfa00125f);//at:165541.538 ns
    REG_WRITE_ctrl(0xC01A001c,0x060138f0);//at:166574.066 ns
    REG_WRITE_ctrl(0xC01A0020,0x06402260);//at:167617.130 ns
}


static void lpddr2_controller_init(void)
{
    REG_WRITE_ctrl(0xC0190000,0x03040000);//at:106455.650 ns
    REG_WRITE_ctrl(0xC0190000,0x03040000);//at:106529.402 ns
    REG_WRITE_ctrl(0xC0190000,0x03040004);//at:106603.154 ns
    REG_WRITE_ctrl(0xC0190000,0x03040004);//at:106676.906 ns
    REG_WRITE_ctrl(0xC0190000,0x03040004);//at:106750.658 ns
    REG_WRITE_ctrl(0xC0190000,0x03040004);//at:106824.410 ns
    REG_WRITE_ctrl(0xC0190000,0x03040004);//at:106898.162 ns
    REG_WRITE_ctrl(0xC0190000,0x03020004);//at:106971.914 ns
    REG_WRITE_ctrl(0xC0190000,0x01020004);//at:107045.666 ns
    REG_WRITE_ctrl(0xC0190010,0x00000030);//at:107456.570 ns
    REG_WRITE_ctrl(0xC0190010,0x00000030);//at:107867.474 ns
    REG_WRITE_ctrl(0xC0190010,0x00006030);//at:108278.378 ns
    REG_WRITE_ctrl(0xC0190010,0x00006030);//at:108689.282 ns
    REG_WRITE_ctrl(0xC0190014,0x00003398);//at:109100.186 ns
    REG_WRITE_ctrl(0xC0190020,0x00000000);//at:109511.090 ns
    REG_WRITE_ctrl(0xC0190020,0x00000002);//at:109921.994 ns
    REG_WRITE_ctrl(0xC0190024,0x2c771004);//at:110596.298 ns
    REG_WRITE_ctrl(0xC0190030,0x00000000);//at:111007.202 ns
    REG_WRITE_ctrl(0xC0190030,0x00000002);//at:111418.106 ns
    REG_WRITE_ctrl(0xC0190030,0x00000002);//at:111829.010 ns
    REG_WRITE_ctrl(0xC0190030,0x0000000a);//at:112239.914 ns
    REG_WRITE_ctrl(0xC0190030,0x0000000a);//at:112650.818 ns
    REG_WRITE_ctrl(0xC0190034,0x00402003);//at:113314.586 ns
    REG_WRITE_ctrl(0xC0190034,0x00401a03);//at:113388.338 ns
    REG_WRITE_ctrl(0xC0190034,0x002b1a03);//at:113462.090 ns
    REG_WRITE_ctrl(0xC0190038,0x00000002);//at:113535.842 ns
    REG_WRITE_ctrl(0xC0190038,0x00000002);//at:113609.594 ns
    REG_WRITE_ctrl(0xC0190038,0x00090002);//at:113683.346 ns
    REG_WRITE_ctrl(0xC0190050,0x00210000);//at:114094.250 ns
    REG_WRITE_ctrl(0xC0190050,0x00210040);//at:114505.154 ns
    REG_WRITE_ctrl(0xC0190050,0x00217040);//at:114916.058 ns
    REG_WRITE_ctrl(0xC0190050,0x00808040);//at:115916.978 ns
    REG_WRITE_ctrl(0xC0190054,0x00000012);//at:116327.882 ns
    REG_WRITE_ctrl(0xC0190054,0x00030012);//at:116738.786 ns
    REG_WRITE_ctrl(0xC0190060,0x00000000);//at:117149.690 ns
    REG_WRITE_ctrl(0xC0190060,0x00000000);//at:117560.594 ns
    REG_WRITE_ctrl(0xC0190064,0x0062001a);//at:118424.546 ns
    REG_WRITE_ctrl(0xC0190064,0x0030001a);//at:118540.442 ns
    REG_WRITE_ctrl(0xC01900c0,0x00000000);//at:118656.338 ns
    REG_WRITE_ctrl(0xC01900c0,0x00000000);//at:118772.234 ns
    REG_WRITE_ctrl(0xC01900c0,0x00000000);//at:118888.130 ns
    REG_WRITE_ctrl(0xC01900d0,0x00020003);//at:119541.362 ns
    REG_WRITE_ctrl(0xC01900d0,0x00020003);//at:119615.114 ns
    REG_WRITE_ctrl(0xC01900d0,0x40020003);//at:119688.866 ns
    REG_WRITE_ctrl(0xC01900d4,0x00000009);//at:119762.618 ns
    REG_WRITE_ctrl(0xC01900d4,0x00000009);//at:119836.370 ns
    REG_WRITE_ctrl(0xC01900d4,0x00010009);//at:119910.122 ns
    REG_WRITE_ctrl(0xC01900d8,0x00000d06);//at:119983.874 ns
    REG_WRITE_ctrl(0xC01900d8,0x00000e06);//at:120057.626 ns
    REG_WRITE_ctrl(0xC01900dc,0x00000004);//at:120131.378 ns
    REG_WRITE_ctrl(0xC01900dc,0x00820004);//at:120205.130 ns
    REG_WRITE_ctrl(0xC01900e0,0x00000000);//at:120278.882 ns
    REG_WRITE_ctrl(0xC01900e0,0x00020000);//at:120352.634 ns
    REG_WRITE_ctrl(0xC01900e4,0x00100005);//at:121037.474 ns
    REG_WRITE_ctrl(0xC01900e4,0x00080005);//at:121111.226 ns
    REG_WRITE_ctrl(0xC01900f0,0x00000000);//at:121184.978 ns
    REG_WRITE_ctrl(0xC01900f0,0x00000000);//at:121258.730 ns
    REG_WRITE_ctrl(0xC01900f4,0x0000066f);//at:121332.482 ns
    REG_WRITE_ctrl(0xC01900f4,0x0000061f);//at:121406.234 ns
    REG_WRITE_ctrl(0xC01900f4,0x00000c1f);//at:121479.986 ns
    REG_WRITE_ctrl(0xC0190100,0x0f101b08);//at:122164.826 ns
    REG_WRITE_ctrl(0xC0190100,0x0f100c08);//at:122238.578 ns
    REG_WRITE_ctrl(0xC0190100,0x0f0a0c08);//at:122312.330 ns
    REG_WRITE_ctrl(0xC0190100,0x060a0c08);//at:122386.082 ns
    REG_WRITE_ctrl(0xC0190104,0x0008040d);//at:123039.314 ns
    REG_WRITE_ctrl(0xC0190104,0x0008010d);//at:123113.066 ns
    REG_WRITE_ctrl(0xC0190104,0x0002010d);//at:123186.818 ns
    REG_WRITE_ctrl(0xC0190108,0x03050605);//at:123840.050 ns
    REG_WRITE_ctrl(0xC0190108,0x03050505);//at:123913.802 ns
    REG_WRITE_ctrl(0xC0190108,0x03030505);//at:123987.554 ns
    REG_WRITE_ctrl(0xC0190108,0x02030505);//at:124061.306 ns
    REG_WRITE_ctrl(0xC019010c,0x00504006);//at:124735.610 ns
    REG_WRITE_ctrl(0xC019010c,0x00501006);//at:124809.362 ns
    REG_WRITE_ctrl(0xC019010c,0x00301006);//at:124883.114 ns
    REG_WRITE_ctrl(0xC0190110,0x05040405);//at:125557.418 ns
    REG_WRITE_ctrl(0xC0190110,0x05040205);//at:125631.170 ns
    REG_WRITE_ctrl(0xC0190110,0x05010205);//at:125704.922 ns
    REG_WRITE_ctrl(0xC0190110,0x04010205);//at:125778.674 ns
    REG_WRITE_ctrl(0xC0190114,0x05050403);//at:126442.442 ns
    REG_WRITE_ctrl(0xC0190114,0x05050303);//at:126516.194 ns
    REG_WRITE_ctrl(0xC0190114,0x050e0303);//at:126589.946 ns
    REG_WRITE_ctrl(0xC0190114,0x030e0303);//at:126663.698 ns
    REG_WRITE_ctrl(0xC0190118,0x02020005);//at:127338.002 ns
    REG_WRITE_ctrl(0xC0190118,0x02020005);//at:127411.754 ns
    REG_WRITE_ctrl(0xC0190118,0x06020005);//at:127485.506 ns
    REG_WRITE_ctrl(0xC019011c,0x00000206);//at:127559.258 ns
    REG_WRITE_ctrl(0xC019011c,0x00000706);//at:127633.010 ns
    REG_WRITE_ctrl(0xC0190120,0x0000440a);//at:127706.762 ns
    REG_WRITE_ctrl(0xC0190120,0x00000a0a);//at:127780.514 ns
    REG_WRITE_ctrl(0xC0190180,0x02000012);//at:128454.818 ns
    REG_WRITE_ctrl(0xC0190180,0x00480012);//at:128528.570 ns
    REG_WRITE_ctrl(0xC0190180,0x20480012);//at:128602.322 ns
    REG_WRITE_ctrl(0xC0190180,0x20480012);//at:128676.074 ns
    REG_WRITE_ctrl(0xC0190180,0x20480012);//at:128749.826 ns
    REG_WRITE_ctrl(0xC0190184,0x02000070);//at:129424.130 ns
    REG_WRITE_ctrl(0xC0190184,0x00a00070);//at:129497.882 ns
    REG_WRITE_ctrl(0xC0190188,0x00000000);//at:129908.786 ns
    REG_WRITE_ctrl(0xC0190190,0x07020001);//at:130562.018 ns
    REG_WRITE_ctrl(0xC0190190,0x07020101);//at:130635.770 ns
    REG_WRITE_ctrl(0xC0190190,0x07020101);//at:130709.522 ns
    REG_WRITE_ctrl(0xC0190190,0x07020101);//at:130783.274 ns
    REG_WRITE_ctrl(0xC0190190,0x07020101);//at:130857.026 ns
    REG_WRITE_ctrl(0xC0190190,0x02020101);//at:130930.778 ns
    REG_WRITE_ctrl(0xC0190194,0x00000402);//at:131004.530 ns
    REG_WRITE_ctrl(0xC0190194,0x00000102);//at:131078.282 ns
    REG_WRITE_ctrl(0xC0190194,0x00040102);//at:131152.034 ns
    REG_WRITE_ctrl(0xC0190198,0x07000000);//at:131225.786 ns
    REG_WRITE_ctrl(0xC0190198,0x07000030);//at:131299.538 ns
    REG_WRITE_ctrl(0xC0190198,0x07000030);//at:131373.290 ns
    REG_WRITE_ctrl(0xC0190198,0x07008030);//at:131447.042 ns
    REG_WRITE_ctrl(0xC0190198,0x07008030);//at:131520.794 ns
    REG_WRITE_ctrl(0xC0190198,0x07608030);//at:131594.546 ns
    REG_WRITE_ctrl(0xC0190198,0x07608030);//at:131668.298 ns
    REG_WRITE_ctrl(0xC01901a0,0x00400003);//at:132321.530 ns
    REG_WRITE_ctrl(0xC01901a0,0x00400003);//at:132395.282 ns
    REG_WRITE_ctrl(0xC01901a0,0x00400003);//at:132469.034 ns
    REG_WRITE_ctrl(0xC01901a4,0x000000ca);//at:132542.786 ns
    REG_WRITE_ctrl(0xC01901a4,0x000400ca);//at:132616.538 ns
    REG_WRITE_ctrl(0xC01901a8,0x8010015d);//at:133280.306 ns
    REG_WRITE_ctrl(0xC01901a8,0x8e68015d);//at:133954.610 ns
    REG_WRITE_ctrl(0xC01901a8,0x0e68015d);//at:134028.362 ns
    REG_WRITE_ctrl(0xC01901ac,0x00100aed);//at:134681.594 ns
    REG_WRITE_ctrl(0xC01901ac,0x00310aed);//at:134755.346 ns
    REG_WRITE_ctrl(0xC01901b0,0x00000001);//at:134829.098 ns
    REG_WRITE_ctrl(0xC01901b4,0x00000203);//at:134902.850 ns
    REG_WRITE_ctrl(0xC01901b4,0x00000003);//at:134976.602 ns
    REG_WRITE_ctrl(0xC01901b8,0x01010101);//at:135640.370 ns
    REG_WRITE_ctrl(0xC01901b8,0x01010101);//at:135714.122 ns
    REG_WRITE_ctrl(0xC01901b8,0x01010101);//at:135787.874 ns
    REG_WRITE_ctrl(0xC01901b8,0x01010101);//at:135861.626 ns
    REG_WRITE_ctrl(0xC01901d0,0x00280000);//at:135935.378 ns
    REG_WRITE_ctrl(0xC01901d0,0x00280000);//at:136009.130 ns
    REG_WRITE_ctrl(0xC01901d0,0x00280000);//at:136082.882 ns
    REG_WRITE_ctrl(0xC01901d0,0x03ea0000);//at:136757.186 ns
    REG_WRITE_ctrl(0xC01901d4,0x00000fff);//at:136830.938 ns
    REG_WRITE_ctrl(0xC01901d4,0x0fff0fff);//at:136904.690 ns
    REG_WRITE_ctrl(0xC01901d8,0x002700a1);//at:137578.994 ns
    REG_WRITE_ctrl(0xC01901d8,0x004200a1);//at:137652.746 ns
    REG_WRITE_ctrl(0xC0190200,0x0000001f);//at:137726.498 ns
    REG_WRITE_ctrl(0xC0190204,0x00000000);//at:137800.250 ns
    REG_WRITE_ctrl(0xC0190204,0x00000100);//at:137874.002 ns
    REG_WRITE_ctrl(0xC0190204,0x00120100);//at:137947.754 ns
    REG_WRITE_ctrl(0xC0190208,0x00000006);//at:138021.506 ns
    REG_WRITE_ctrl(0xC0190208,0x00000006);//at:138095.258 ns
    REG_WRITE_ctrl(0xC0190208,0x00030006);//at:138169.010 ns
    REG_WRITE_ctrl(0xC0190208,0x00030006);//at:138242.762 ns
    REG_WRITE_ctrl(0xC019020c,0x00000000);//at:138316.514 ns
    REG_WRITE_ctrl(0xC019020c,0x00000200);//at:138390.266 ns
    REG_WRITE_ctrl(0xC019020c,0x00020200);//at:138464.018 ns
    REG_WRITE_ctrl(0xC019020c,0x0f020200);//at:138537.770 ns
    REG_WRITE_ctrl(0xC0190210,0x0000000f);//at:138611.522 ns
    REG_WRITE_ctrl(0xC0190210,0x00000f0f);//at:138685.274 ns
    REG_WRITE_ctrl(0xC0190214,0x00000006);//at:138759.026 ns
    REG_WRITE_ctrl(0xC0190214,0x00000406);//at:138832.778 ns
    REG_WRITE_ctrl(0xC0190214,0x00050406);//at:138906.530 ns
    REG_WRITE_ctrl(0xC0190214,0x08050406);//at:138980.282 ns
    REG_WRITE_ctrl(0xC0190218,0x00000006);//at:139054.034 ns
    REG_WRITE_ctrl(0xC0190218,0x00000406);//at:139127.786 ns
    REG_WRITE_ctrl(0xC0190218,0x000f0406);//at:139201.538 ns
    REG_WRITE_ctrl(0xC0190218,0x0f0f0406);//at:139275.290 ns
    REG_WRITE_ctrl(0xC0190240,0x04000430);//at:139939.058 ns
    REG_WRITE_ctrl(0xC0190240,0x04000430);//at:140012.810 ns
    REG_WRITE_ctrl(0xC0190240,0x04140430);//at:140086.562 ns
    REG_WRITE_ctrl(0xC0190240,0x02140430);//at:140160.314 ns
    REG_WRITE_ctrl(0xC0190244,0x00002211);//at:140234.066 ns
    REG_WRITE_ctrl(0xC0190244,0x00002231);//at:140307.818 ns
    REG_WRITE_ctrl(0xC0190244,0x00002131);//at:140381.570 ns
    REG_WRITE_ctrl(0xC0190244,0x00003131);//at:140455.322 ns
    REG_WRITE_ctrl(0xC0190250,0x00000a05);//at:140529.074 ns
    REG_WRITE_ctrl(0xC0190250,0x00000a05);//at:140602.826 ns
    REG_WRITE_ctrl(0xC0190250,0x00000a01);//at:140676.578 ns
    REG_WRITE_ctrl(0xC0190250,0x00000a01);//at:140750.330 ns
    REG_WRITE_ctrl(0xC0190250,0x00d80a01);//at:140824.082 ns
    REG_WRITE_ctrl(0xC0190250,0x6cd80a01);//at:140897.834 ns
    REG_WRITE_ctrl(0xC0190254,0x00000016);//at:140971.586 ns
    REG_WRITE_ctrl(0xC019025c,0x0f00038a);//at:141635.354 ns
    REG_WRITE_ctrl(0xC019025c,0xf900038a);//at:141709.106 ns
    REG_WRITE_ctrl(0xC0190264,0x0f00f8e2);//at:142362.338 ns
    REG_WRITE_ctrl(0xC0190264,0x4500f8e2);//at:142436.090 ns
    REG_WRITE_ctrl(0xC019026c,0x0f001b37);//at:143099.858 ns
    REG_WRITE_ctrl(0xC019026c,0x5b001b37);//at:143173.610 ns
    REG_WRITE_ctrl(0xC0190300,0x00000001);//at:143247.362 ns
    REG_WRITE_ctrl(0xC0190300,0x00000001);//at:143321.114 ns
    REG_WRITE_ctrl(0xC0190304,0x00000000);//at:143437.010 ns
    REG_WRITE_ctrl(0xC0190304,0x00000000);//at:143552.906 ns
    REG_WRITE_ctrl(0xC019030c,0x00000000);//at:143963.810 ns
    REG_WRITE_ctrl(0xC019030c,0x00000000);//at:144374.714 ns
    REG_WRITE_ctrl(0xC019030c,0x00000000);//at:144785.618 ns
    REG_WRITE_ctrl(0xC019030c,0x00000000);//at:145196.522 ns
    REG_WRITE_ctrl(0xC0190320,0x00000001);//at:145270.274 ns
    REG_WRITE_ctrl(0xC0190400,0x00000010);//at:145955.114 ns
    REG_WRITE_ctrl(0xC0190404,0x000112cc);//at:146661.026 ns
    REG_WRITE_ctrl(0xC0190408,0x000030a0);//at:146734.778 ns
    REG_WRITE_ctrl(0xC0190490,0x00000001);//at:147482.834 ns
    REG_WRITE_ctrl(0xC0190494,0x0012000d);//at:148157.138 ns
    REG_WRITE_ctrl(0xC0190490,0x00000001);//at:148273.034 ns
    REG_WRITE_ctrl(0xC0190304,0x00000000);//at:150485.594 ns
}

int lpddr2_init(void)
{
    //volatile u32 ddr_rd_st, pgcr, pgsr, swstat ,ddr_pir ;
    volatile u32 ddr_rd_st, pgcr, pgsr, swstat ;
    /*Setp 1*/
    REG_WRITE_ctrl(0xC0190304, 0x00000001); /*disable pop queue*/
    REG_WRITE_ctrl(0xC0190030, 0x00000001); /*enable self_refresh*/

    /*Setp 2*/
    ddr_rd_st = REG_READ_ctrl(0xC0190004); /*read and reset value?*/
    printf("2.lpddr2, ddr rd status: 0x%x\n", ddr_rd_st);
    while (ddr_rd_st != 0) {
        ddr_rd_st = REG_READ_ctrl(0xC0190004);
    }
    printf("2. ddr rd status: 0x%x\n", ddr_rd_st);

    /*Setp 3*/
    lpddr2_controller_init();
    printf("3. ddr controller init done\n");

    /*Setp 4*/
    REG_READ_ctrl(0xC0190030);
    REG_WRITE_ctrl(0xC0190030, 0x0000000a);
    REG_READ_ctrl(0xC0190030);
    REG_WRITE_ctrl(0xC0190030, 0x00000008);

    /*Setp 5*/
    REG_WRITE_ctrl(0xC0190320, 0x00000000);
    REG_WRITE_ctrl(0xC01901b0, 0x00000000);

    /*phy init */
    /*Setp 6*/
    pgcr = REG_READ_ctrl(0xC01A0008);
    printf("6. phy pgcr = 0x%x\n", pgcr);

    /*Setp 7*/
    lpddr2_phy_init();
    printf("7. ddr phy init done\n");

    /*Setp 8*/
    pgsr = REG_READ_ctrl(0xC01A000c);
    printf("8. ddr phy pgsr: 0x%x\n", pgsr);
    while ((pgsr&0x1) != 0x1) {
        pgsr = REG_READ_ctrl(0xC01A000c);
        printf("8. ddr phy pgsr: 0x%x\n", pgsr);
    }


    /*Step 9*/
    REG_WRITE_ctrl(0xC01A0004, 0x00000061);

    /*Step 10*/
    pgsr = REG_READ_ctrl(0xC01A000c);
    printf("10.ddr phy pgsr: 0x%x\n", pgsr);
    while ((pgsr&0x1) != 0x1) {
        pgsr = REG_READ_ctrl(0xC01A000c);
        printf("ddr phy pgsr: 0x%x\n", pgsr);
    }

    /*Step 11*/
    /*
       REG_WRITE_ctrl(0xC01A0004,0x00000003);//at:370846.034 ns)

       ddr_pir = REG_READ_ctrl(0xC01a0004);
       printf("11.ddr pir : 0x%x\n", ddr_pir);
       while (ddr_pir != 0) {
       ddr_pir = REG_READ_ctrl(0xC01a0004);
       }

       pgsr = REG_READ_ctrl(0xC01A000c);
       printf("11.ddr phy pgsr: 0x%x\n", pgsr);
       while ((pgsr&0x1) != 0x1) {
       pgsr = REG_READ_ctrl(0xC01A000c);
       printf("ddr phy pgsr: 0x%x\n", pgsr);
       }
       */


    /*Step 12*/
    /*
       REG_WRITE_ctrl(0xC01A0004,0x00000181);//at:370846.034 ns)

       ddr_pir = REG_READ_ctrl(0xC01a0004);
       printf("12.ddr pir : 0x%x\n", ddr_pir);
       while (ddr_pir != 0) {
       ddr_pir = REG_READ_ctrl(0xC01a0004);
       }

       pgsr = REG_READ_ctrl(0xC01A000c);
       printf("12.ddr phy pgsr: 0x%x\n", pgsr);
       while ((pgsr&0x1) != 0x1) {
       pgsr = REG_READ_ctrl(0xC01A000c);
       printf("ddr phy pgsr: 0x%x\n", pgsr);
       }
       */
    /*Step 13*/

    REG_WRITE_ctrl(0xC01901b0,0x00000001);//at:392645.018 ns
    REG_WRITE_ctrl(0xC0190320,0x00000001);//at:392718.770 ns

    swstat = REG_READ_ctrl(0xC0190324);
    while (swstat != 0x1)
        swstat = REG_READ_ctrl(0xC0190324);
    printf("13.ddr swstat: 0x%x\n", swstat);

    /*Step 14*/


    ddr_rd_st = REG_READ_ctrl(0xC0190004); /*read and reset value?*/
    while (ddr_rd_st != 1) {
        ddr_rd_st = REG_READ_ctrl(0xC0190004);
    }


    /*Step 15*/

    REG_WRITE_ctrl(0xC0190030,0x00000008);//at:415107.770 ns
    //REG_WRITE_ctrl(0xC0190030,0x0000000a);//at:415213.130 ns
    REG_WRITE_ctrl(0xC0190030,0x00000000);//dis pd for train, at:415213.130 ns
    REG_WRITE_ctrl(0xC01A01D4,0x3DB0A001);
    REG_WRITE_ctrl(0xC01A0214,0x3DB0A001);


    return 0;
}

#endif
