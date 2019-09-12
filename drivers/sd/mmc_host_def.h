/*
 * FILE : mmc_host_def.h
 * Desc : define of mmc controler
 * Author : jinying.wu
 * Data :
 * Modify :
 * Version :
 * Group : NuSmart
 */

#ifndef MMC_HOST_DEF_H
#define MMC_HOST_DEF_H

/* Evatronix SDIO-HOST Register Map */
#define SD_OFFSET_ADDR              0x20000
#define SD_OFFSET_SLOT0             0x100
#define SD_OFFSET_SLOT1             0x200
#define SD_OFFSET_SLOT2             0x300
#define SD_OFFSET_SLOT3             0x400

#define SD_BASE_ADDR                (APB0_BASE + SD_OFFSET_ADDR)
#define SDIO_REGISTERS_OFFSET       SD_BASE_ADDR

#define SRS_OFFSET 0x100
/* Slot Register Set */

#define RESPONSE_136BIT(cmd) 	((((cmd) >> 16) & 3) == 1)
#define RESPONSE_48BIT(cmd) 	(((cmd) >> 16) & 2)

struct host_register_set {
    unsigned int HSFR0;
    unsigned int HSFR1;
    unsigned int HSFR2;
    unsigned int HSFR3;
    unsigned int HSFR4;
    unsigned int HSFR5;
    unsigned int HSFR6;
    unsigned int HSFR7;
    unsigned int HSFR8;
    unsigned int HSFR9;
    unsigned int HSFR10;
    unsigned int HSFR11;
    unsigned int HSFR12;
    unsigned int HSFR13;
    unsigned int HSFR14;
    unsigned int HSFR15;
    unsigned int HSFR16;
    unsigned int HSFR17;
    unsigned int HSFR18;
    unsigned int HSFR19;
    unsigned int HSFR20;
    unsigned int HSFR21;
    unsigned int HSFR22;
    unsigned int HSFR23;
    unsigned int HSFR24;
    unsigned int HSFR25;
    unsigned int HSFR26;
    unsigned int HSFR27;
    unsigned int HSFR28;
    unsigned int HSFR29;
    unsigned int HSFR30;
    unsigned int HSFR31;
    unsigned int HSFR32;
    unsigned int HSFR33;
    unsigned int HSFR34;
    unsigned int HSFR35;
    unsigned int HSFR36;
    unsigned int HSFR37;
    unsigned int HSFR38;
    unsigned int HSFR39;
    unsigned int HSFR40;
    unsigned int HSFR41;
    unsigned int HSFR42;
    unsigned int HSFR43;
};

struct slot_register_set {
    unsigned int SRS0;
    unsigned int SRS1;
    unsigned int SRS2;
    unsigned int SRS3;
    unsigned int SRS4;
    unsigned int SRS5;
    unsigned int SRS6;
    unsigned int SRS7;
    unsigned int SRS8;
    unsigned int SRS9;
    unsigned int SRS10;
    unsigned int SRS11;
    unsigned int SRS12;
    unsigned int SRS13;
    unsigned int SRS14;
    unsigned int SRS15;
    unsigned int SRS16;
    unsigned int SRS17;
    unsigned int SRS18;
    unsigned int SRS19;
    unsigned int SRS20;
    unsigned int SRS21;
    unsigned int SRS22;
    unsigned int SRS23;
    unsigned int SRS24;
    unsigned int SRS25;
    unsigned int SRS26;
    unsigned int SRS27;
    unsigned int SRS28;
    unsigned int SRS29;
    unsigned int SRS30;
    unsigned int SRS31;
    unsigned int SRS32;
    unsigned int SRS33;
    unsigned int SRS34;
    unsigned int SRS35;
    unsigned int SRS36;
    unsigned int SRS37;
    unsigned int SRS38;
    unsigned int SRS39;
    unsigned int SRS40;
    unsigned int SRS41;
    unsigned int SRS42;
    unsigned int SRS43;
    unsigned int SRS44;
    unsigned int SRS45;
    unsigned int SRS46;
    unsigned int SRS47;
    unsigned int SRS48;
    unsigned int SRS49;
    unsigned int SRS50;
    unsigned int SRS51;
    unsigned int SRS52;
    unsigned int SRS53;
    unsigned int SRS54;
    unsigned int SRS55;
    unsigned int SRS56;
};

#if 0
#define hwp_ap_sd_host                      (HWP_SD_HOST_T *) (SD_BASE_ADDR)
#define hwp_ap_sd_slot0                     (HWP_SD_SLOT_T *) (SD_BASE_ADDR + SD_OFFSET_SLOT0)
#endif

#endif /* MMC_HOST_DEF_H */
