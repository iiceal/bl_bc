#include <common.h>
#include <regs/scm.h>
#include "iomux_hw_regs.h"
#include <drivers/iomux/iomux_hw.h>

void scm_if0_0_if4_12(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN0_BIT, SCM_IOMUX_8C_PRM1_DIN0_MASK,
                  SCM_IOMUX_8C_PRM1_DIN0_IF0_0_IF4_12);
}

void scm_if0_1_if4_13(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN1_BIT, SCM_IOMUX_8C_PRM1_DIN1_MASK,
                  SCM_IOMUX_8C_PRM1_DIN1_IF0_1_IF4_13);
}

void scm_if0_2_if4_14(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN2_BIT, SCM_IOMUX_8C_PRM1_DIN2_MASK,
                  SCM_IOMUX_8C_PRM1_DIN2_IF0_2_IF4_14);
}

void scm_if0_3_if4_15(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN3_BIT, SCM_IOMUX_8C_PRM1_DIN3_MASK,
                  SCM_IOMUX_8C_PRM1_DIN3_IF0_3_IF4_15);
}

void scm_if0_4_if6_12_if4_6(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN4_BIT, SCM_IOMUX_8C_PRM1_DIN4_MASK,
                  SCM_IOMUX_8C_PRM1_DIN4_IF0_4_IF6_12_IF4_6);
}

void scm_if0_5_if6_13_if4_7(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN5_BIT, SCM_IOMUX_8C_PRM1_DIN5_MASK,
                  SCM_IOMUX_8C_PRM1_DIN5_IF0_5_IF6_13_IF4_7);
}

void scm_if0_6_if6_14(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN6_BIT, SCM_IOMUX_8C_PRM1_DIN6_MASK,
                  SCM_IOMUX_8C_PRM1_DIN6_IF0_6_IF6_14);
}

void scm_if0_7_if6_15(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN7_BIT, SCM_IOMUX_8C_PRM1_DIN7_MASK,
                  SCM_IOMUX_8C_PRM1_DIN7_IF0_7_IF6_15);
}

void scm_if0_8(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN8_BIT, SCM_IOMUX_8C_PRM1_DIN8_MASK,
                  SCM_IOMUX_8C_PRM1_DIN8_IF0_8);
}

void scm_if0_9(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN9_BIT, SCM_IOMUX_8C_PRM1_DIN9_MASK,
                  SCM_IOMUX_8C_PRM1_DIN9_IF0_9);
}

void scm_if0_10(void)
{
    scm_io_mux_8c(SCM_IOMUX_8C_PRM1_DIN10_BIT, SCM_IOMUX_8C_PRM1_DIN10_MASK,
                  SCM_IOMUX_8C_PRM1_DIN10_IF0_10);
}

void scm_if0_11(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN11_BIT, SCM_IOMUX_90_PRM1_DIN11_MASK,
                  SCM_IOMUX_90_PRM1_DIN11_IF0_11);
}

void scm_if0_12(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN12_BIT, SCM_IOMUX_90_PRM1_DIN12_MASK,
                  SCM_IOMUX_90_PRM1_DIN12_IF0_12);
}

void scm_if0_13(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN13_BIT, SCM_IOMUX_90_PRM1_DIN13_MASK,
                  SCM_IOMUX_90_PRM1_DIN13_IF0_13);
}

void scm_if0_14(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN14_BIT, SCM_IOMUX_90_PRM1_DIN14_MASK,
                  SCM_IOMUX_90_PRM1_DIN14_IF0_14);
}

void scm_if0_15(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN15_BIT, SCM_IOMUX_90_PRM1_DIN15_MASK,
                  SCM_IOMUX_90_PRM1_DIN15_IF0_15);
}

void scm_if1_0_if4_8(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN16_BIT, SCM_IOMUX_90_PRM1_DIN16_MASK,
                  SCM_IOMUX_90_PRM1_DIN16_IF1_0_IF4_8);
}

void scm_if1_1_if4_9(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN17_BIT, SCM_IOMUX_90_PRM1_DIN17_MASK,
                  SCM_IOMUX_90_PRM1_DIN17_IF1_1_IF4_9);
}

void scm_if1_2_if4_10(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN17_BIT, SCM_IOMUX_90_PRM1_DIN17_MASK,
                  SCM_IOMUX_90_PRM1_DIN18_IF1_2_IF4_10);
}

void scm_if1_3_if4_11(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN19_BIT, SCM_IOMUX_90_PRM1_DIN19_MASK,
                  SCM_IOMUX_90_PRM1_DIN19_IF1_3_IF4_11);
}

void scm_if1_4_if6_8(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN20_BIT, SCM_IOMUX_90_PRM1_DIN20_MASK,
                  SCM_IOMUX_90_PRM1_DIN20_IF1_4_IF6_8);
}

void scm_if1_5_if6_9(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN21_BIT, SCM_IOMUX_90_PRM1_DIN21_MASK,
                  SCM_IOMUX_90_PRM1_DIN21_IF1_5_IF6_9);
}

void scm_if1_6_if6_10(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN22_BIT, SCM_IOMUX_90_PRM1_DIN22_MASK,
                  SCM_IOMUX_90_PRM1_DIN22_IF1_6_IF6_10);
}

void scm_if1_7_if6_11(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN23_BIT, SCM_IOMUX_90_PRM1_DIN23_MASK,
                  SCM_IOMUX_90_PRM1_DIN23_IF1_7_IF6_11);
}

void scm_if1_8(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN24_BIT, SCM_IOMUX_90_PRM1_DIN24_MASK,
                  SCM_IOMUX_90_PRM1_DIN24_IF1_8);
}

void scm_if1_9(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN25_BIT, SCM_IOMUX_90_PRM1_DIN25_MASK,
                  SCM_IOMUX_90_PRM1_DIN25_IF1_9);
}

void scm_if1_10(void)
{
    scm_io_mux_90(SCM_IOMUX_90_PRM1_DIN26_BIT, SCM_IOMUX_90_PRM1_DIN26_MASK,
                  SCM_IOMUX_90_PRM1_DIN26_IF1_10);
}

void scm_if1_11(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN27_BIT, SCM_IOMUX_94_PRM1_DIN27_MASK,
                  SCM_IOMUX_94_PRM1_DIN27_IF1_11);
}

void scm_if1_12(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN28_BIT, SCM_IOMUX_94_PRM1_DIN28_MASK,
                  SCM_IOMUX_94_PRM1_DIN28_IF1_12);
}

void scm_if1_13(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN29_BIT, SCM_IOMUX_94_PRM1_DIN29_MASK,
                  SCM_IOMUX_94_PRM1_DIN29_IF1_13);
}

void scm_if1_14(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN30_BIT, SCM_IOMUX_94_PRM1_DIN30_MASK,
                  SCM_IOMUX_94_PRM1_DIN30_IF1_14);
}

void scm_if1_15(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN31_BIT, SCM_IOMUX_94_PRM1_DIN31_MASK,
                  SCM_IOMUX_94_PRM1_DIN31_IF1_15);
}

void scm_if2_0_if4_4_if5_12(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN32_BIT, SCM_IOMUX_94_PRM1_DIN32_MASK,
                  SCM_IOMUX_94_PRM1_DIN32_IF2_0_IF4_4_IF5_12);
}

void scm_if2_1_if4_5_if5_13(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN33_BIT, SCM_IOMUX_94_PRM1_DIN33_MASK,
                  SCM_IOMUX_94_PRM1_DIN33_IF2_1_IF4_5_IF5_13);
}

void scm_if2_2_if4_6_if5_14(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN34_BIT, SCM_IOMUX_94_PRM1_DIN34_MASK,
                  SCM_IOMUX_94_PRM1_DIN34_IF2_2_IF4_6_IF5_14);
}

void scm_if2_3_if4_7_if5_15(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN35_BIT, SCM_IOMUX_94_PRM1_DIN35_MASK,
                  SCM_IOMUX_94_PRM1_DIN35_IF2_3_IF4_7_IF5_15);
}

void scm_if2_4_if7_12(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN36_BIT, SCM_IOMUX_94_PRM1_DIN36_MASK,
                  SCM_IOMUX_94_PRM1_DIN36_IF2_4_IF7_12);
}

void scm_if2_5_if7_13(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN37_BIT, SCM_IOMUX_94_PRM1_DIN37_MASK,
                  SCM_IOMUX_94_PRM1_DIN37_IF2_5_IF7_13);
}

void scm_if2_6_if7_14(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN38_BIT, SCM_IOMUX_94_PRM1_DIN38_MASK,
                  SCM_IOMUX_94_PRM1_DIN38_IF2_6_IF7_14);
}

void scm_if2_7_if7_15(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN39_BIT, SCM_IOMUX_94_PRM1_DIN39_MASK,
                  SCM_IOMUX_94_PRM1_DIN39_IF2_7_IF7_15);
}

void scm_if2_8_if11_12(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN40_BIT, SCM_IOMUX_94_PRM1_DIN40_MASK,
                  SCM_IOMUX_94_PRM1_DIN40_IF2_8_IF11_12);
}

void scm_if2_9_if11_13(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN41_BIT, SCM_IOMUX_94_PRM1_DIN41_MASK,
                  SCM_IOMUX_94_PRM1_DIN41_IF2_9_IF11_13);
}

void scm_if2_10_if11_14(void)
{
    scm_io_mux_94(SCM_IOMUX_94_PRM1_DIN42_BIT, SCM_IOMUX_94_PRM1_DIN42_MASK,
                  SCM_IOMUX_94_PRM1_DIN42_IF2_10_IF11_14);
}

void scm_if2_11_if11_15(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN43_BIT, SCM_IOMUX_98_PRM1_DIN43_MASK,
                  SCM_IOMUX_98_PRM1_DIN43_IF2_11_IF11_15);
}

void scm_if2_12(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN44_BIT, SCM_IOMUX_98_PRM1_DIN44_MASK,
                  SCM_IOMUX_98_PRM1_DIN44_IF2_12);
}

void scm_if2_13(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN45_BIT, SCM_IOMUX_98_PRM1_DIN45_MASK,
                  SCM_IOMUX_98_PRM1_DIN45_IF2_13);
}

void scm_if2_14(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN46_BIT, SCM_IOMUX_98_PRM1_DIN46_MASK,
                  SCM_IOMUX_98_PRM1_DIN46_IF2_14);
}

void scm_if2_15(void)
{
    scm_io_mux_98(SCM_IOMUX_98_PRM1_DIN47_BIT, SCM_IOMUX_98_PRM1_DIN47_MASK,
                  SCM_IOMUX_98_PRM1_DIN47_IF2_15);
}

void scm_if3_0_if5_8_if9_12(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D0_BIT, SCM_IOMUX_84_PRM1_D0_MASK,
                  SCM_IOMUX_84_PRM1_D0_IF3_0_IF5_8_IF9_12);
}

void scm_if3_1_if5_9_if9_13(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D1_BIT, SCM_IOMUX_84_PRM1_D1_MASK,
                  SCM_IOMUX_84_PRM1_D1_IF3_1_IF5_9_IF9_13);
}

void scm_if3_2_if5_10_if9_14(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D2_BIT, SCM_IOMUX_84_PRM1_D2_MASK,
                  SCM_IOMUX_84_PRM1_D2_IF3_2_IF5_10_IF9_14);
}

void scm_if3_3_if5_11_if9_15(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D3_BIT, SCM_IOMUX_84_PRM1_D3_MASK,
                  SCM_IOMUX_84_PRM1_D3_IF3_3_IF5_11_IF9_15);
}

void scm_if3_4_if5_6_if7_8_if8_12(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D4_BIT, SCM_IOMUX_84_PRM1_D4_MASK,
                  SCM_IOMUX_84_PRM1_D4_IF3_4_IF5_6_IF7_8_IF8_12);
}

void scm_if3_5_if5_7_if7_9_if8_13(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D5_BIT, SCM_IOMUX_84_PRM1_D5_MASK,
                  SCM_IOMUX_84_PRM1_D5_IF3_5_IF5_7_IF7_9_IF8_13);
}

void scm_if3_6_if7_10_if8_14(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D6_BIT, SCM_IOMUX_84_PRM1_D6_MASK,
                  SCM_IOMUX_84_PRM1_D6_IF3_6_IF7_10_IF8_14);
}

void scm_if3_7_if7_11_if8_15(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D7_BIT, SCM_IOMUX_84_PRM1_D7_MASK,
                  SCM_IOMUX_84_PRM1_D7_IF3_7_IF7_11_IF8_15);
}

void scm_if3_8_if10_12(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D8_BIT, SCM_IOMUX_84_PRM1_D8_MASK,
                  SCM_IOMUX_84_PRM1_D8_IF3_8_IF10_12);
}

void scm_if3_9_if10_13(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D9_BIT, SCM_IOMUX_84_PRM1_D9_MASK,
                  SCM_IOMUX_84_PRM1_D9_IF3_9_IF10_13);
}

void scm_if3_10_if10_14(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D10_BIT, SCM_IOMUX_84_PRM1_D10_MASK,
                  SCM_IOMUX_84_PRM1_D10_IF3_10_IF10_14);
}

void scm_if3_11_if10_15(void)
{
    scm_io_mux_84(SCM_IOMUX_84_PRM1_D11_BIT, SCM_IOMUX_84_PRM1_D11_MASK,
                  SCM_IOMUX_84_PRM1_D11_IF3_11_IF10_15);
}

void scm_if3_12(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_D12_BIT, SCM_IOMUX_88_PRM1_D12_MASK,
                  SCM_IOMUX_88_PRM1_D12_IF3_12);
}

void scm_if3_13(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_D13_BIT, SCM_IOMUX_88_PRM1_D13_MASK,
                  SCM_IOMUX_88_PRM1_D13_IF3_13);
}

void scm_if3_14(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_D14_BIT, SCM_IOMUX_88_PRM1_D14_MASK,
                  SCM_IOMUX_88_PRM1_D14_IF3_14);
}

void scm_if3_15(void)
{
    scm_io_mux_88(SCM_IOMUX_88_PRM1_D15_BIT, SCM_IOMUX_88_PRM1_D15_MASK,
                  SCM_IOMUX_88_PRM1_D15_IF3_15);
}

int scm_if_en(int index)
{
    switch (index) {
    case 0:
        scm_if0_0_if4_12();
        scm_if0_1_if4_13();
        scm_if0_2_if4_14();
        scm_if0_3_if4_15();
        scm_if0_4_if6_12_if4_6();
        scm_if0_5_if6_13_if4_7();
        scm_if0_6_if6_14();
        scm_if0_7_if6_15();
        scm_if0_8();
        scm_if0_9();
        scm_if0_10();
        scm_if0_11();
        scm_if0_12();
        scm_if0_13();
        scm_if0_14();
        scm_if0_15();
        break;
    case 1:
        scm_if1_0_if4_8();
        scm_if1_1_if4_9();
        scm_if1_2_if4_10();
        scm_if1_3_if4_11();
        scm_if1_4_if6_8();
        scm_if1_5_if6_9();
        scm_if1_6_if6_10();
        scm_if1_7_if6_11();
        scm_if1_8();
        scm_if1_9();
        scm_if1_10();
        scm_if1_11();
        scm_if1_12();
        scm_if1_13();
        scm_if1_14();
        scm_if1_15();
        break;
    case 2:
        scm_if2_0_if4_4_if5_12();
        scm_if2_1_if4_5_if5_13();
        scm_if2_2_if4_6_if5_14();
        scm_if2_3_if4_7_if5_15();
        scm_if2_4_if7_12();
        scm_if2_5_if7_13();
        scm_if2_6_if7_14();
        scm_if2_7_if7_15();
        scm_if2_8_if11_12();
        scm_if2_9_if11_13();
        scm_if2_10_if11_14();
        scm_if2_11_if11_15();
        scm_if2_12();
        scm_if2_13();
        scm_if2_14();
        scm_if2_15();
        break;
    case 3:
        scm_if3_0_if5_8_if9_12();
        scm_if3_1_if5_9_if9_13();
        scm_if3_2_if5_10_if9_14();
        scm_if3_3_if5_11_if9_15();
        scm_if3_4_if5_6_if7_8_if8_12();
        scm_if3_5_if5_7_if7_9_if8_13();
        scm_if3_6_if7_10_if8_14();
        scm_if3_7_if7_11_if8_15();
        scm_if3_8_if10_12();
        scm_if3_9_if10_13();
        scm_if3_10_if10_14();
        scm_if3_11_if10_15();
        scm_if3_12();
        scm_if3_13();
        scm_if3_14();
        scm_if3_15();
        break;
    default:
        iomux_printf("IF: index %d error.", index);
        return DRV_OP_FAILED;
    }

    return DRV_OP_SUCCESS;
}
