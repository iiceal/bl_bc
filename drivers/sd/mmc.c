/*
 * The driver for Evatronix SDIO Host Controller.
 *
 * Author: Pan Ruochen <ruochen.pan@nufront.com>
 *
 * 2011 (c) Nufront, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#include <common.h>
#include "SDIO_Types.h"
#include "mmc.h"
#include "mmc_host_def.h"
#include "SDIO_Host.h"
#include "SDIO_CardGeneral.h"
#include "sdio.h"
#include "drivers/drv_sd.h"

#define writel(v, c) ((*(volatile u32 *) (c)) = (v))
#define readl(c) (*(volatile u32 *) (c))

#define DDR_SD_WRITE_TEST_ADDR      (U32)0X20000000
#define DDR_SD_READ_TEST_ADDR       (U32)0X28000000

#define SRAM_SD_WRITE_TEST_ADDR      (U32)0X200000
#define SRAM_SD_READ_TEST_ADDR       (U32)0X280000

//#define DDR_TEST 1
#ifdef  DDR_TEST
char *dbuf = (char*) (DDR_SD_WRITE_TEST_ADDR);
char *rdbuf = (char*) (DDR_SD_READ_TEST_ADDR);
#else
char *dbuf = (char*) (SRAM_SD_WRITE_TEST_ADDR);
char *rdbuf = (char*) (SRAM_SD_READ_TEST_ADDR);
#endif

#define    sd_mdelay  mdelay

#define SD_PRINT_LOG 0
#if (SD_PRINT_LOG > 0)
#define sd_printf   printf
#else
#define sd_printf(fmt, ...)
#endif

#define SDMMC_SECTOR_SIZE 512
static struct sdio_slot slots[2];

extern void v7_dma_inv_range(u32 s, u32 e);
extern void v7_dma_clean_range(u32 s, u32 e);
int sd_irq_init(void);
int wait_transfer_complete(struct sdio_slot *slot, unsigned int count);

inline static int sd_get_isr_status(struct sdio_slot *slot)
{
    return (slot->io->SRS12);
}

inline void sd_clr_isr_status(struct sdio_slot *slot)
{
    slot->io->SRS12 = 0xffffffff;
}

static int WaitForValue(volatile unsigned int *addr, unsigned int mask, int is_set,
                        unsigned int timeout)
{
    if (is_set == 0) {
        // wait until bit/bits will clear
        while ((*addr & mask) != 0) {
        }
    } else {
        // wait until a bit/bits will set
        while ((*addr & mask) == 0) {
        }
    }
    return SDIO_ERR_NO_ERROR;
}

static inline int check_cmd_error(struct sdio_slot *slot)
{
	static const unsigned char error_code[] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		SDIO_ERR_COMMAND_TIMEOUT_ERROR,
		SDIO_ERR_COMMAND_CRC_ERROR,
		SDIO_ERR_COMMAND_END_BIT_ERROR,
		SDIO_ERR_COMMAND_INDEX_ERROR,
		SDIO_ERR_DATA_TIMEOUT_ERROR,
		SDIO_ERR_DATA_CRC_ERROR,
		SDIO_ERR_DATA_END_BIT_ERROR,
		SDIO_ERR_CURRENT_LIMIT_ERROR,
		SIO_ERR_AUTOCMD12_NOT_EXECUTED,
		SDIO_ERR_ADMA,
		SDIO_ERR_TUNING,
	};
	unsigned long val;
	int i;

	val = slot->io->SRS12;
	if( val == 0 )
		return 0;
	asm("clz %0,%1\n\t" : "=r"(i) : "r"(val) );
	return error_code[31 - i];
/*
	switch(31 - i) {
	case 16:  return SDIO_ERR_COMMAND_TIMEOUT_ERROR;
	case 17:  return SDIO_ERR_COMMAND_CRC_ERROR;
	case 18:  return SDIO_ERR_COMMAND_END_BIT_ERROR;
	case 19:  return SDIO_ERR_COMMAND_INDEX_ERROR;
	case 20:  return SDIO_ERR_DATA_TIMEOUT_ERROR;
	case 21:  return SDIO_ERR_DATA_CRC_ERROR;
	case 22:  return SDIO_ERR_DATA_END_BIT_ERROR;
	case 23:  return SDIO_ERR_CURRENT_LIMIT_ERROR;
	case 24:  return SIO_ERR_AUTOCMD12_NOT_EXECUTED;
	case 25:  return SDIO_ERR_ADMA;
	case 26:  return SDIO_ERR_TUNING;
	}
	return 0;
*/
}

/*
 * function: send one command to CID, which need wait CD bit of CMD register
 * parameter:
 *      cmd -- command, write it to CMD register
 *      arg -- command argument, write it to CMDARG register
 *      response -- responses of the current command will write to it
*/
static int SDIOHost_ExecCardCommandTimeout(struct sdio_slot *slot, unsigned int cmd,
                                           unsigned int arg, unsigned int timeout)
{
    // volatile unsigned int mmc_stat;
    int status;

    /* Clear all interrupt status */
    slot->io->SRS12 = 0xffffffff;

    sd_printf("CMD%u: 0x%08x, ARG: 0x%08x\n", (cmd >> 24) % 64, cmd, arg);

    slot->cmd = cmd;
    if (cmd & __CMD_DATA_PRESENT) {
        slot->io->SRS1 = (slot->blkcnt << 16) | slot->blksize | SFR1_DMA_BUFF_SIZE_512KB;
        if (slot->blkcnt > 1)
            cmd |= 0x22; /* Enable Multi Block, Block Count */
        sd_printf("BlockCount=%u, BlockSize=%u\n", slot->blkcnt, slot->blksize);
    }

    while (slot->io->SRS9 & 1) {
    }
    slot->io->SRS2 = arg;
    slot->io->SRS3 = cmd;
    while (1) {
        status = WaitForValue(&slot->io->SRS9, 1, 0, timeout);
        if (status != 0)
            goto EXE_CMD_ERR;
        status = WaitForValue(&slot->io->SRS12, 1, 1, timeout);
        if (status != 0)
            goto EXE_CMD_ERR;

        status = check_cmd_error(slot);
        if (status != 0) {
            goto EXE_CMD_ERR;
        }

        if (RESPONSE_48BIT(cmd)) {
            slot->response[0] = slot->io->SRS4;
            sd_printf("RESP48:0x%08x\n", slot->response[0]);
        }
        // long responses
        if (RESPONSE_136BIT(cmd)) {
            slot->response[3] = slot->io->SRS4 << 8;
            slot->response[2] = (slot->io->SRS5 << 8) | ((slot->io->SRS4 >> 24) & 0xff);
            slot->response[1] = (slot->io->SRS6 << 8) | ((slot->io->SRS5 >> 24) & 0xff);
            slot->response[0] = (slot->io->SRS7 << 8) | ((slot->io->SRS6 >> 24) & 0xff);

            sd_printf("RESP136 [0]:0x%08x\n", slot->response[0]);
            sd_printf("RESP136 [1]:0x%08x\n", slot->response[1]);
            sd_printf("RESP136 [2]:0x%08x\n", slot->response[2]);
            sd_printf("RESP136 [3]:0x%08x\n", slot->response[3]);
        }
        break;
    }
    return SDIO_ERR_NO_ERROR;
EXE_CMD_ERR:
    sd_printf("CMD-- return 0x%x\n", status);
    return status;
}

#define SDIOHost_ExecCardCommand(slot, cmd, arg)                                                   \
    SDIOHost_ExecCardCommandTimeout(slot, cmd, arg, 0xFFFFFFFF)

static int SDIOHost_ExecCMD55Command(struct sdio_slot *slot)
{
    return SDIOHost_ExecCardCommand(slot, SDMMC_CMD55, slot->RCA << 16);
}

static int SDIOHost_ReadDataBlock(struct sdio_slot *slot, void *buf)
{
    unsigned int i, blkcnt;
    int status;

    blkcnt = slot->blkcnt;
    while (blkcnt > 0) {
        status = WaitForValue(&slot->io->SRS12, (SFR12_BUFFER_READ_READY | SFR12_ERROR_INTERRUPT),
                              1, COMMANDS_TIMEOUT);
        if (status || (slot->io->SRS12 & SFR12_ERROR_INTERRUPT))
            return SDIO_ERR_TIMEOUT;
        slot->io->SRS12 = SFR12_BUFFER_READ_READY;
        for (i = 0; i < slot->blksize; i += 4) {
            unsigned int tmp = slot->io->SRS8;
            writel(tmp, buf);
            buf += 4;
        }
        blkcnt--;
    }
    return 0;
}

static int SDIOHost_WriteDataBlock(struct sdio_slot *slot, const char *buf)
{
    unsigned int i, blkcnt;
    int status;
    int *dbuf = (int *) buf;
    blkcnt = slot->blkcnt;
    while (blkcnt > 0) {
        status = WaitForValue(&slot->io->SRS12, (SFR12_BUFFER_WRITE_READY | SFR12_ERROR_INTERRUPT),
                              1, COMMANDS_TIMEOUT);
        if (status || (slot->io->SRS12 & SFR12_ERROR_INTERRUPT))
            return SDIO_ERR_TIMEOUT;
        slot->io->SRS12 = SFR12_BUFFER_WRITE_READY;
        for (i = 0; i < slot->blksize / 4; i += 1) {
            slot->io->SRS8 = dbuf[i];
        }
        blkcnt--;
        dbuf += (slot->blksize / 4);
    }
    return 0;
}

static int SDIOHost_SetPower(struct sdio_slot *slot)
{
    unsigned int tmp;

    tmp = slot->io->SRS10;
    tmp &= ~(SFR10_SD_BUS_POWER);

    // disable SD bus power
    slot->io->SRS10 = tmp;

    tmp |= SFR10_SD_BUS_POWER;
    slot->io->SRS10 = tmp;
    return SDIO_ERR_NO_ERROR;
}

int SDIOHost_Set4Bus(struct sdio_slot *slot)
{
    slot->io->SRS10 |= SFR10_DATA_WIDTH_4BIT;
    return SDIO_ERR_NO_ERROR;
}

int SDIOHost_SetHighSpeed(struct sdio_slot *slot)
{
    slot->io->SRS10 |= SFR10_HIGH_SPEED_ENABLE;
    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_Send4bus(struct sdio_slot *slot)
{
    int status;
    unsigned int argument = SFR10_DATA_WIDTH_4BIT;

    status = SDIOHost_ExecCMD55Command(slot);
    if (status)
        return status;

    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD6, argument);
    if (status != SDIO_ERR_NO_ERROR)
        return status;

    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_SetSDCLK(struct sdio_slot *slot, unsigned int *frequency /* in KHZ */)
{
    unsigned int baseclk = (CONFIG_APB_CLOCK / 2 / 1000); /* in KHZ */
    unsigned int i;
    unsigned int tmp;
    unsigned int status;

    // set SD clock off
    tmp = slot->io->SRS11;
    slot->io->SRS11 = tmp & (~SFR11_SD_CLOCK_ENABLE);

// read base clock frequency for SD clock in kilo herz
// baseclk = SFR16_GET_BASE_CLK_FREQ_MHZ( slot->io->SRS16 ) * 1000;

// if ( !baseclk ){
//	baseclk = SYTEM_CLK_KHZ;
//}
//	baseclk = CFG_AHB_SDIO_CLOCK;
#if 1
    for (i = 1; i < 256; i = 2 * i) {
        if ((baseclk / i) <= *frequency) {
            break;
        }
    }
#endif
    i = (baseclk / 2) / (*frequency);

    sd_printf("SetClk %d=%d/%d\n", *frequency, baseclk, i);

    // read current value of SFR11 register
    tmp = slot->io->SRS11;

    // clear old frequency base settings
    tmp &= ~SFR11_SEL_FREQ_BASE_MASK;

    // Set SDCLK Frequency Select and Internal Clock Enable
    tmp |= ((i) << 8) | SFR11_INT_CLOCK_ENABLE;
    slot->io->SRS11 = tmp;

    // wait for clock stable is 1
    status = WaitForValue(&slot->io->SRS11, SFR11_INT_CLOCK_STABLE, 1, COMMANDS_TIMEOUT);
    if (status)
        return status;

    // set SD clock on
    tmp = slot->io->SRS11;
    slot->io->SRS11 = (tmp | SFR11_SD_CLOCK_ENABLE);

    // write to frequency the real value of set frequecy
    //*frequency = baseclk / i;

    return SDIO_ERR_NO_ERROR;
}

static BYTE SDIOHost_CheckDeviceType(struct sdio_slot *slot)
{
    BYTE DeviceType = 0;
    BYTE status;

    // do not check status because MMC card doesn't respond
    // on this command if it is not initialized
    status = SDIOHost_ExecCMD55Command(slot);

    if (status) {

    } else {
        // execute ACMD41 command
        status = SDIOHost_ExecCardCommand(slot, SDMMC_ACMD41, 0);
    }

    do {
        // if no response on ACMD41 command then check
        // CMD1 command response
        if (status == SDIO_ERR_COMMAND_TIMEOUT_ERROR) {

            // execute CMD0 command to go to idle state
            status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD0, 0);
            if (status)
                return status;

            // execute CMD1 command
            status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD1, 0);
            switch (status) {
            case SDIO_ERR_COMMAND_TIMEOUT_ERROR:
                // it is not MMC card
                break;
            case SDIO_ERR_NO_ERROR:
                DeviceType = SDIOHOST_CARD_TYPE_MMC;
                break;
            default:
                return status;
            }

            if (DeviceType == SDIOHOST_CARD_TYPE_MMC)
                break;
        } else if (status == SDIO_ERR_NO_ERROR)
            DeviceType = SDIOHOST_CARD_TYPE_SDMEM;
        else
            return status;

        // execute CMD0 command to go to idle state
        status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD0, 0);
        if (status)
            return status;

        status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD5, 0);
        switch (status) {
        case SDIO_ERR_COMMAND_TIMEOUT_ERROR:
            break;
        case SDIO_ERR_NO_ERROR:
            // it can be a SDIO device or combo device
            DeviceType |= SDIOHOST_CARD_TYPE_SDIO;
            break;
        default:
            return status;
        }
    } while (0);

    if (DeviceType == 0)
        return SDIO_ERR_UNUSABLE_CARD;

    slot->DeviceType = DeviceType;
    slot->IsSelected = 0;
    slot->RCA = 0;

    // execute CMD0 command to go to idle state
    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD0, 0);
    if (status)
        return status;
    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_ResetCard(struct sdio_slot *slot)
{
    return SDIOHost_ExecCardCommand(slot, SDMMC_CMD0, 0);
}

static int SDIOHost_SetTimeout(struct sdio_slot *slot, unsigned int timeout)
{
    unsigned int tmp;

    if (!(timeout & SFR11_TIMEOUT_MASK))
        return SDIO_ERR_WRONG_VALUE;

    tmp = LEToCPU32(slot->io->SRS11);
    tmp &= SFR11_TIMEOUT_MASK;
    tmp |= timeout;
    slot->io->SRS11 = tmp;

    return SDIO_ERR_NO_ERROR;
}

static int GetMemCardOCR(struct sdio_slot *slot, unsigned int *OCR)
{
    unsigned int command;
    int status;

    *OCR = 0;
    if ((slot->DeviceType & SDIOHOST_CARD_TYPE_SDMEM) ||
        (slot->DeviceType == SDIOHOST_CARD_TYPE_MMC)) {
        if (slot->DeviceType == SDIOHOST_CARD_TYPE_MMC) {
            command = SDMMC_CMD1;
        } else {
            status = SDIOHost_ExecCMD55Command(slot);
            if (status)
                return status;
            command = SDMMC_ACMD41;
        }

        // execute command to check a card OCR register
        status = SDIOHost_ExecCardCommand(slot, command, 0);
        if (status == SDIO_ERR_NO_ERROR)
            *OCR = slot->response[0];
    }
    return SDIO_ERR_NO_ERROR;
}

/*
static int SDIOHOST_MatchVoltage(struct sdio_slot *slot, DWORD OCR, DWORD *CardVoltage, 
								  DWORD *ControllerVoltage)
{
	/// host voltage capabilities
	DWORD HostCapabilities;

	*CardVoltage = 0;
	*ControllerVoltage = 0;

	HostCapabilities = slot->io->SRS16;
	// check the voltage capabilities of the SDIO host controller and a card
	// to set appriopriate voltage
	do{
		if ( HostCapabilities & SFR16_VOLTAGE_3_3V_SUPPORT ){
			if ( OCR & SDCARD_REG_OCR_3_3_3_4 ){
				*CardVoltage = SDCARD_REG_OCR_3_3_3_4;
				*ControllerVoltage = SFR10_SET_3_3V_BUS_VOLTAGE;
				break;
			}
			if ( OCR & SDCARD_REG_OCR_3_2_3_3 ){
				*CardVoltage = SDCARD_REG_OCR_3_2_3_3;
				*ControllerVoltage = SFR10_SET_3_3V_BUS_VOLTAGE;
				break;
			}
		}
		if ( HostCapabilities & SFR16_VOLTAGE_3_0V_SUPPORT ){
			if ( OCR & SDCARD_REG_OCR_3_0_3_1 ){
				*CardVoltage = SDCARD_REG_OCR_3_0_3_1;
				*ControllerVoltage = SFR10_SET_3_0V_BUS_VOLTAGE;
				break;
			}
			if ( OCR & SDCARD_REG_OCR_2_9_3_0 ){
				*CardVoltage = SDCARD_REG_OCR_2_9_3_0;
				*ControllerVoltage = SFR10_SET_3_0V_BUS_VOLTAGE;
				break;
			}
		}
		if ( HostCapabilities & SFR16_VOLTAGE_1_8V_SUPPORT ){
			if ( OCR & SDCARD_REG_OCR_1_8_0_0 ){
				*CardVoltage = SDCARD_REG_OCR_1_8_0_0;
				*ControllerVoltage = SFR10_SET_1_8V_BUS_VOLTAGE;
				break;
			}
		}

	}while(0);
	return SDIO_ERR_NO_ERROR;
}
*/

static BYTE SetMemCardVoltage(struct sdio_slot *slot, DWORD Voltage, BYTE F8, unsigned int *CCS)
{
    BYTE status, Condition;
    volatile DWORD Delay;
    unsigned int command, argument;

    argument = 0;
    if (slot->DeviceType & SDIOHOST_CARD_TYPE_SDMEM) {
        command = SDMMC_ACMD41;
    } else
        command = SDMMC_CMD1; // MMC

    if (slot->DeviceType & SDIOHOST_CARD_TYPE_SDMEM) {
        // if card responsed on CMD8 command
        // set host hight capacity flag to 1
        if (F8)
            argument |= SDCARD_ACMD41_HCS;
    } else // if(slot->id == 1)
    {      // MMC card, sector mode
        argument = 0x40000000;
    }

    Delay = 10000;
    argument |= Voltage;

    sd_printf("argument = 0x%x\n", argument);

    do {
        if (command == SDMMC_ACMD41) {
            status = SDIOHost_ExecCMD55Command(slot);
            if (status)
                return SDIO_ERR_UNUSABLE_CARD;
        }

        // execute ACMD41 or CMD1 command with setting the supply voltage as argument
        status = SDIOHost_ExecCardCommand(slot, command, argument);
        if (status != SDIO_ERR_NO_ERROR)
            return SDIO_ERR_UNUSABLE_CARD;

        Condition = ((slot->response[0] & SDCARD_REG_OCR_READY) != 0);
        sd_mdelay(10);
        // if no response from card it is unusable return error code
    } while (!Condition && --Delay); // wait until card will be ready

    if (Delay == 0) // card is busy to much time
        return SDIO_ERR_UNUSABLE_CARD;

    if (slot->DeviceType & SDIOHOST_CARD_TYPE_SDMEM) {
        // check CCS card flag
        if (slot->response[0] & SDCARD_REG_OCR_CCS)
            *CCS = 1;
        else
            *CCS = 0;
    } else if (slot->DeviceType == SDIOHOST_CARD_TYPE_MMC) {
        if (slot->response[0] & MMC_REG_OCR_SECTOR_MODE)
            *CCS = 1;
        else
            *CCS = 0;
    }
    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_ReadRCA(struct sdio_slot *slot)
{
    int status;
    unsigned int argument;
    // RCA address for RCA cards for MMC cards
    DWORD RCA = 0x1000;

    RCA--;
    if (slot->DeviceType == SDIOHOST_CARD_TYPE_MMC) {
        slot->RCA = RCA;
        argument = (DWORD) slot->RCA << 16;
    } else {
        argument = 0;
    }

    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD3, argument);
    if (status != SDIO_ERR_NO_ERROR)
        return status;

    if (slot->DeviceType != SDIOHOST_CARD_TYPE_MMC) {
        // get RCA CMD 3
        slot->RCA = (slot->response[0] >> 16) & 0xFFFF;
    }
    sd_printf("\nslot->RCA=%x\n", slot->RCA);
    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_SelectCard(struct sdio_slot *slot, WORD RCA)
{
    unsigned int command;
    int status;

    if ((RCA == slot->RCA) && slot->IsSelected) {
        sd_printf("\nrca no 0\n");
        return SDIO_ERR_NO_ERROR;
    }

    if ((RCA == 0) && !slot->IsSelected) {
        sd_printf("\nrca = 0\n");
        return SDIO_ERR_NO_ERROR;
    }

    if (RCA != 0) /* select */
        command = SDMMC_CMD7_SEL;
    else
        command = SDMMC_CMD7_DES;

    status = SDIOHost_ExecCardCommand(slot, command, (DWORD) RCA << 16);
    if (status == 0) {
        if (RCA)
            slot->IsSelected = 1;
        else
            slot->IsSelected = 0;
    }
    return status;
}

static int SDIOHost_ReadCID(struct sdio_slot *slot)
{
    int status;
    unsigned int argument;

    status = SDIOHost_SelectCard(slot, 0);
    argument = (DWORD) slot->RCA << 16;
    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD10, argument);
    if (status != SDIO_ERR_NO_ERROR)
        return status;
    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_ReadCSD(struct sdio_slot *slot)
{
    int status;
    unsigned int argument;

    status = SDIOHost_SelectCard(slot, 0);
    argument = (DWORD) slot->RCA << 16;
    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD9, argument);
    if (status != SDIO_ERR_NO_ERROR)
        return status;
    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_ReadSCR(struct sdio_slot *slot)
{
    BYTE Buffer[8] = {0};
    DWORD SCR = 0;
    int status;
    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;

    status = SDIOHost_ExecCMD55Command(slot);
    if (status)
        return status;

    // the size of SCR register is 64 bits (8 bytes)
    /*  Block Count: 1
     *  Block Size:  8
     */
    slot->blkcnt = 1;
    slot->blksize = 8;
    status = SDIOHost_ExecCardCommand(slot, SDMMC_ACMD51, 0);
    if (status)
        return status;
    SDIOHost_ReadDataBlock(slot, Buffer);
    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD12, 0);

    // get more significant 32 bits from SCR register
    // SCR = (*((DWORD*)Buffer));
    // SCR = (*((DWORD*)&Buffer[0]));
    memcpy(&SCR, (void *) Buffer, 4);
    SCR = ((SCR & 0xFF) << 24) | ((SCR & 0xFF000000) >> 24) | ((SCR & 0xFF0000) >> 8) |
          ((SCR & 0xFF00) << 8);

    slot->SupportedBusWidths = (SCR & SDCARD_REG_SCR_SBW_MASK) >> 16;
    slot->SpecVersNumb = (SCR & SDCARD_REG_SCR_SPEC_VER_MASK) >> 24;
    sd_printf("\nSCR = 0x%08x\n", SCR);
    return SDIO_ERR_NO_ERROR;
}

static int MemoryCard_SetBlockLength(struct sdio_slot *slot, DWORD BlockLength)
{
    int status;

    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;
    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD16, BlockLength);
    return status;
}

static int SDIOHost_Configure(struct sdio_slot *slot, int Cmd, void *data, unsigned int *dsize)
{
    int status;

    switch (Cmd) {
    case SDIOHOST_CONFIG_SET_CLK:
        if (*dsize == sizeof(WORD)) {
            status = SDIOHost_SetSDCLK(slot, data);
            if (status) {
                sd_printf("SDIOHost_Configure", "Set host CLK to %d failed\n", *((WORD *) data));
                return status;
            }
        } else {
            sd_printf("SDIOHost_Configure", "dsize should be 2 but is %d\n", *dsize);
            return SDIO_ERR_WRONG_VALUE;
        }
        break;
    default:
        return SDIO_ERR_WRONG_VALUE;
    }

    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_DeviceAttach(struct sdio_slot *slot)
{
    int status;
    unsigned int frequency = SD_IDENTIFY_CLK, size;
    unsigned int tmp, CardVoltage, CCS = 0;

    int FlagF8 = 0;

    CardVoltage = SDCARD_REG_OCR_3_3_3_4;

    unsigned int OcrMem = 0xFFFFFF;

    // set 1 bit bus mode
    tmp = slot->io->SRS10;
    tmp |= 0x7 << 9; // set bvs
    slot->io->SRS10 = tmp & (~SFR10_DATA_WIDTH_4BIT);
    tmp = slot->host_io->HSFR0;
    // dissable mmc8 mode
    slot->host_io->HSFR0 = tmp & ~(1 << (24 + slot->id));

    // enable bus power
    // status = SDIOHost_SetPower( slot, CurrentControllerVoltage );
    status = SDIOHost_SetPower(slot);
    if (status)
        return status;

    // SD clock supply before Issue a SD command
    // clock frequency is set to 400kHz
    status = SDIOHost_SetSDCLK(slot, &frequency);
    if (status)
        return status;

    sd_mdelay(10);

    status = SDIOHost_ResetCard(slot);
    if (status) {
        return status;
    }

    // function checks the type of card device
    // it can be MMC, SD memory or SDIO
    status = SDIOHost_CheckDeviceType(slot);
    if (status)
        return status;
    sd_printf("\ndevice type is %x\n", slot->DeviceType);
    // check if card supports High Capacity Memory.
    if (slot->DeviceType & SDIOHOST_CARD_TYPE_SDMEM) {
        // VHS voltage 2.7-3.6V
        status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD8, 0x1a5);

        switch (status) {
        case SDIO_ERR_COMMAND_TIMEOUT_ERROR:
            FlagF8 = 0;
            break;
        case SDIO_ERR_NO_ERROR:
            FlagF8 = 1;
            break;
        default:
            return status;
        }
    }

    // get OCR register from memory card
    if ((slot->DeviceType & SDIOHOST_CARD_TYPE_SDMEM) ||
        (slot->DeviceType == SDIOHOST_CARD_TYPE_MMC)) {
        status = GetMemCardOCR(slot, &OcrMem);
        if (status)
            return status;
    }
    // set voltage for memory card
    if ((slot->DeviceType & SDIOHOST_CARD_TYPE_SDMEM) ||
        (slot->DeviceType == SDIOHOST_CARD_TYPE_MMC)) {

        switch (slot->DeviceType) {
        case SDIOHOST_CARD_TYPE_MMC:
            sd_printf("card type: MMC, LowVoltage = 0x%x\n", slot->LowVoltage);
            break;
        case SDIOHOST_CARD_TYPE_SDMEM:
            sd_printf("card type: SD, LowVoltage = 0x%x\n", slot->LowVoltage);
            break;
        }
        if ((slot->LowVoltage == 1) && (slot->DeviceType == SDIOHOST_CARD_TYPE_MMC)) {
            CardVoltage = SDCARD_REG_OCR_1_8_0_0;
            sd_printf("1.8v voltage = 0x%x\n", CardVoltage);
        }
        status = SetMemCardVoltage(slot, CardVoltage, FlagF8, &CCS);
        if (status) {
            sd_printf("Card not ready\n");
            return status;
        }
        // set device capacity info relay on CCS flag from card OCR register
        if (CCS)
            slot->DeviceCapacity = SDIOHOST_CAPACITY_HIGH;
        else
            slot->DeviceCapacity = SDIOHOST_CAPACITY_NORMAL;
    }

    if (slot->DeviceType != SDIOHOST_CARD_TYPE_SDIO) {
        status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD2, 0);
        if (status)
            return status;
    }

    status = SDIOHost_ReadRCA(slot);
    if (status)
        return status;

    slot->inserted = 1;
    // for SD memory only card get information about card from SCR register
#if 0
    if (slot->DeviceType == SDIOHOST_CARD_TYPE_SDMEM) {
        status = SDIOHost_ReadSCR(slot);
        if (status)
            return status;
    }
#endif
    status = MemoryCard_SetBlockLength(slot, SDMMC_SECTOR_SIZE);
    if (status)
        return status;

    frequency = SD_TRANSFER_CLK;
    //	frequency = CFG_SDIO_CLOCK;
    size = 2;
    status = SDIOHost_Configure(slot, SDIOHOST_CONFIG_SET_CLK, &frequency, &size);
    if (status) {
        return status;
    }
    sd_printf("Clock is set to %dKHz\n", frequency);

    return 0;
}

static int SDIOHost_HostInitialize(int i)
{
    volatile unsigned int tmp;

    // HostBusMode = HOST_BUS_MODE;

    // reset controller for sure
    tmp = slots[i].host_io->HSFR0;
    tmp |= HSFR0_SOFTWARE_RESET;
    slots[i].host_io->HSFR0 = tmp;
    WaitForValue(&slots[i].host_io->HSFR0, HSFR0_SOFTWARE_RESET, 0, COMMANDS_TIMEOUT);

#if DEBOUNCING_TIME > 0xFFFFFF
#error WRONG VALUE OF DEBOUNCING TIME IN CONFIG.H FILE
#endif
    slots[i].host_io->HSFR1 = CPUToLE32(DEBOUNCING_TIME);

    // set srs16 of slot 1 to only 1.8V support
    // tmp = *(volatile unsigned int*)(SDIO_REGISTERS_OFFSET + 0x200 + SDIO_REG_SLOT_SFR16_OFFSET);
    // tmp &= 0xFCFFFFFF;
    // slots[i].host_io->HSFR8 = tmp;

    return SDIO_ERR_NO_ERROR;
}

static int MemoryCard_DataTransfer(struct sdio_slot *slot, unsigned int address, void *buff,
                                   unsigned int count)
{
    unsigned int cmd, argument = 0;
    int status;

    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;

    slot->io->SRS12 = SFR12_TRANSFER_COMPLETE;

    SDIOHost_Send4bus(&slots[0]);
    SDIOHost_SetHighSpeed(&slots[0]);
    SDIOHost_Set4Bus(&slots[0]);

    if (slot->DeviceCapacity == SDIOHOST_CAPACITY_NORMAL) {
        // Data address is in byte units in a Standard Capacity SD memory card and MMC memory card
        argument = address * SDMMC_SECTOR_SIZE;
    } else if (slot->DeviceCapacity == SDIOHOST_CAPACITY_HIGH) {
        argument = address;
    }

    sd_printf("addr argu = %x\n", argument);
    slot->blkcnt = count;
    slot->blksize = SDMMC_SECTOR_SIZE;

    if (count == 1)
        cmd = SDMMC_CMD17;
    else
        cmd = SDMMC_CMD18;
    status = SDIOHost_ExecCardCommand(slot, cmd, argument);
    if (status)
        return status;
    SDIOHost_ReadDataBlock(slot, buff);
    status = wait_transfer_complete(slot, count);
    //status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD12, 0);
    return SDIO_ERR_NO_ERROR;
}

static int MemoryCard_Data_program(struct sdio_slot *slot, unsigned int address, void *buff,
                                   unsigned int count)
{
    unsigned int cmd, argument = 0;
    int status;

    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;

    slot->io->SRS12 = SFR12_TRANSFER_COMPLETE;
    
    SDIOHost_Send4bus(&slots[0]);
    SDIOHost_SetHighSpeed(&slots[0]);
    SDIOHost_Set4Bus(&slots[0]);

    if (slot->DeviceCapacity == SDIOHOST_CAPACITY_NORMAL) {
        // Data address is in byte units in a Standard Capacity SD memory card and MMC memory card
        argument = address * SDMMC_SECTOR_SIZE;
    } else if (slot->DeviceCapacity == SDIOHOST_CAPACITY_HIGH) {
        argument = address;
    }
    slot->blkcnt = count;
    slot->blksize = SDMMC_SECTOR_SIZE;

    if (count == 1)
        cmd = SDMMC_CMD24;
    else
        cmd = SDMMC_CMD25;
    status = SDIOHost_ExecCardCommand(slot, cmd, argument);
    if (status)
        return status;
    SDIOHost_WriteDataBlock(slot, buff);
    status = wait_transfer_complete(slot, count);
    //status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD12, 0);
    return SDIO_ERR_NO_ERROR;
}

int wait_blk_cnt_complete(struct sdio_slot *slot, unsigned int count)
{
    int timeout = count * 2;

    while (timeout) {
        if ((slot->io->SRS1 >> 16) == 0) {
            return timeout == 0 ? SDIO_ERR_TIMEOUT : SDIO_ERR_NO_ERROR;
        }
        sd_mdelay(1);
        timeout--;
    }

    return timeout == 0 ? SDIO_ERR_TIMEOUT : SDIO_ERR_NO_ERROR;
}

int wait_transfer_complete(struct sdio_slot *slot, unsigned int count)
{
#if 1
    int timeout = count * 2;

    while (timeout) {
        if ((slot->io->SRS12 & 0x02) == 0x02) {
            return timeout == 0 ? SDIO_ERR_TIMEOUT : SDIO_ERR_NO_ERROR;
        }
        sd_mdelay(1);
        timeout--;
    }
    return timeout == 0 ? SDIO_ERR_TIMEOUT : SDIO_ERR_NO_ERROR;

#else
    while ((slot->io->SRS12 & 0x02) != 0x02)
        ;
    return 0;
#endif
}

static int MemoryCard_Data_dma_transfer(struct sdio_slot *slot, unsigned int address, void *buff,
                                        unsigned int count)
{
    unsigned int cmd, argument = 0;
    int status;

    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;

    slot->io->SRS0 = (u32) buff;
    slot->io->SRS1 = 0x200 | (count << 16);
    slot->io->SRS10 &= ~0x18;

    if (slot->DeviceCapacity == SDIOHOST_CAPACITY_NORMAL) {
        // Data address is in byte units in a Standard Capacity SD memory card and MMC memory card
        argument = address * SDMMC_SECTOR_SIZE;
    } else if (slot->DeviceCapacity == SDIOHOST_CAPACITY_HIGH) {
        argument = address;
    }
    slot->blkcnt = count;
    slot->blksize = SDMMC_SECTOR_SIZE;

    if (count == 1)
        cmd = SDMMC_CMD17_DMA;
    else
        cmd = SDMMC_CMD18_DMA;
    status = SDIOHost_ExecCardCommand(slot, cmd, argument);
    if (status)
        return status;
    //    wait_blk_cnt_complete(slot, count);
    wait_transfer_complete(slot, count);
    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD12, 0);
    return SDIO_ERR_NO_ERROR;
}

static int MemoryCard_Data_adma_transfer(struct sdio_slot *slot, unsigned int address, void *buff,
                                         unsigned int count)
{
    unsigned int cmd, argument = 0;
    int status;

    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;
    SDIOHost_Send4bus(&slots[0]);
    SDIOHost_SetHighSpeed(&slots[0]);
    SDIOHost_Set4Bus(&slots[0]);

    slot->io->SRS22 = (int) buff; // buff as dma system addr
    slot->io->SRS0 = 0;
    slot->io->SRS1 = (0x200 | (count << 16));
    slot->io->SRS10 |= 0x10;

    if (slot->DeviceCapacity == SDIOHOST_CAPACITY_NORMAL) {
        // Data address is in byte units in a Standard Capacity SD memory card and MMC memory card
        argument = address * SDMMC_SECTOR_SIZE;
    } else if (slot->DeviceCapacity == SDIOHOST_CAPACITY_HIGH) {
        argument = address;
    }
    slot->blkcnt = count;
    slot->blksize = SDMMC_SECTOR_SIZE;

    if (count == 1)
        cmd = SDMMC_CMD17_DMA;
    else
        cmd = SDMMC_CMD18_DMA;
    status = SDIOHost_ExecCardCommand(slot, cmd, argument);
    if (status)
        return status;
    status = wait_blk_cnt_complete(slot, count);
    if (status)
        sd_printf("\nwarning wait blkcnt status = 0x%x", status);
    
    status = wait_transfer_complete(slot, count);
    if (status)
        sd_printf("\nwarning  wait transfer status = 0x%x", status);
    sd_printf("\nsrs1 = %08x, srs21 - 0x%08x\n", slot->io->SRS1, slot->io->SRS21);
    
    //status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD12, 0);
    return SDIO_ERR_NO_ERROR;
}

static int MemoryCard_Data_dma_program(struct sdio_slot *slot, unsigned int address, void *buff,
                                       unsigned int count)
{
    unsigned int cmd, argument = 0;
    int status;

    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;

    slot->io->SRS1 = 0x200 | (count << 16);
    slot->io->SRS10 &= ~0x18;
    slot->io->SRS0 = (int) buff;
    if (slot->DeviceCapacity == SDIOHOST_CAPACITY_NORMAL) {
        // Data address is in byte units in a Standard Capacity SD memory card and MMC memory card
        argument = address * SDMMC_SECTOR_SIZE;
    } else if (slot->DeviceCapacity == SDIOHOST_CAPACITY_HIGH) {
        argument = address;
    }
    slot->blkcnt = count;
    slot->blksize = SDMMC_SECTOR_SIZE;

    if (count == 1)
        cmd = SDMMC_CMD24_DMA;
    else
        cmd = SDMMC_CMD25_DMA;
    slot->io->SRS9 |= (1 << 10);
    status = SDIOHost_ExecCardCommand(slot, cmd, argument);
    if (status)
        return status;
    wait_blk_cnt_complete(slot, count);
    wait_transfer_complete(slot, count);
    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD12, 0);
    return SDIO_ERR_NO_ERROR;
}

static int MemoryCard_Data_adma_program(struct sdio_slot *slot, unsigned int address, void *buff,
                                        unsigned int count)
{
    unsigned int cmd, argument = 0;
    int status = 0;

    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;
    SDIOHost_Send4bus(&slots[0]);
    SDIOHost_SetHighSpeed(&slots[0]);
    SDIOHost_Set4Bus(&slots[0]);

    slot->io->SRS22 = (int) buff; // buff as dma system addr
    slot->io->SRS0 = 0;
    slot->io->SRS1 = 0x20200;
    slot->io->SRS10 |= 0x10;
    sd_printf("\nbuffaddr = %x\n", (int) buff);
    if (slot->DeviceCapacity == SDIOHOST_CAPACITY_NORMAL) {
        // Data address is in byte units in a Standard Capacity SD memory card and MMC memory card
        argument = address * SDMMC_SECTOR_SIZE;
    } else if (slot->DeviceCapacity == SDIOHOST_CAPACITY_HIGH) {
        argument = address;
    }
    slot->blkcnt = count;
    slot->blksize = SDMMC_SECTOR_SIZE;

    if (count == 1)
        cmd = SDMMC_CMD24_DMA;
    else
        cmd = SDMMC_CMD25_DMA;
    // slot->io->SRS9 |= (1 << 10);
    status = SDIOHost_ExecCardCommand(slot, cmd, argument);
    if (status)
        return status;

    status = wait_blk_cnt_complete(slot, count);
    if (status) {
        sd_printf("\nwarning wait blkcnt status = 0x%x", status);
    }

    status = wait_transfer_complete(slot, count);
    if (status) {
        sd_printf("\nwarning  wait transfer status = 0x%x", status);
    }
    sd_printf("\nsrs1 = %08x, srs21 - 0x%08x\n", slot->io->SRS1, slot->io->SRS21);
    //status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD12, 0);
    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_Readcardstatus(struct sdio_slot *slot)
{
    int status = 0;

    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD13, (DWORD) slot->RCA << 16);
    if (status)
        return status;
    return SDIO_ERR_NO_ERROR;
}

static int SDIOHost_ReadSDstatus(struct sdio_slot *slot)
{
    int status = 0;

    status = SDIOHost_SelectCard(slot, slot->RCA);
    if (status)
        return status;

    status = SDIOHost_ExecCMD55Command(slot);
    if (status)
        return status;

    status = SDIOHost_ExecCardCommand(slot, SDMMC_ACMD13, (slot->RCA << 16));
    if (status)
        return status;

    slot->blkcnt = 1;
    slot->blksize = 64;
    SDIOHost_ReadDataBlock(slot, dbuf);
    status = SDIOHost_ExecCardCommand(slot, SDMMC_CMD12, 0);
    return SDIO_ERR_NO_ERROR;
}

/*=============================================================*/
/*  External Interfaces                                        */
/*=============================================================*/
int sd_init(void)
{
    int status = 0;
    int i = 0;
    memset(slots, 0, sizeof(slots));

    slots[i].host_io = (void *) (SDIO_REGISTERS_OFFSET + i * 0x1000);
    slots[i].io = (void *) ((unsigned int) (slots[i].host_io) + (i + 1) * 0x100);

    if ((slots[i].host_io->HSFR0 & (0x10000)) == 0)
        return 0;
    status = SDIOHost_HostInitialize(i);
    if (status != 0) {
        sd_printf("\ninit failed\n");
        return status;
    }

    status = WaitForValue(&slots[i].io->SRS9, SFR9_CARD_STATE_STABLE, 1, COMMANDS_TIMEOUT);
    if (status) {
        sd_printf("\nno card\n");
        return status;
    }

    slots[i].io->SRS13 = 0xffffffff;
    slots[i].io->SRS14 = (1 << 3);
    slots[i].id = 0;
    if (i != 0)
    slots[i].LowVoltage = 1;
    else slots[i].LowVoltage = 0;

    SDIOHost_SetTimeout(&slots[i], SFR11_TIMEOUT_TMCLK_X_2_POWER_24);

    status = SDIOHost_DeviceAttach(&slots[i]);
    if (status) {
        sd_printf("\nattach failed\n");
    }

    sd_irq_init();
    return status;
}

int sd_read(const int addr, const int count)
{
    //int j = 0;
    int status = 0;
    unsigned long long s, e;
    unsigned int us;

    s = timer_get_tick();
    status = MemoryCard_DataTransfer(&slots[0], addr, rdbuf, count);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    printf("read need %d us (%d ms)\n", us, us / 1000);
#if 0
    sd_printf("\n");
    for (j = 0; j < (512 * count); j++) {
        sd_printf("%02x ", rdbuf[j]);
    }
    sd_printf("\n");
    sd_printf("Test need %d us (%d ms)\n", us, us / 1000);
#endif
    return status;
}

int sd_write(const int addr, const int count)
{
    int j = 0;
    int status = 0;
    unsigned long long s, e;
    unsigned int us;
#if 1
    for (j = 0; j < count * 512; j++) {
        dbuf[j] = j % 256 + 0x55;
    }
#endif
    s = timer_get_tick();
    status = MemoryCard_Data_program(&slots[0], addr, dbuf, count);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    printf("write need %d us (%d ms)\n", us, us / 1000);
    return status;
}

int sd_erase(const int addr, const int count)
{
    int j = 0;
    int status = 0;
    unsigned long long s, e;
    unsigned int us;
    for (j = 0; j < count * 512; j++) {
        dbuf[j] = 0xff;
    }
    s = timer_get_tick();
    status = MemoryCard_Data_program(&slots[0], addr, dbuf, count);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    printf("Test need %d us (%d ms)\n", us, us / 1000);
    return status;
}

int read_sd_csd(void)
{
    SDIOHost_ReadCSD(&slots[0]);
    return 0;
}

int read_sd_cid(void)
{
    SDIOHost_ReadCID(&slots[0]);
    return 0;
}

int read_sd_scr(void)
{
    SDIOHost_ReadSCR(&slots[0]);
    return 0;
}

int read_card_status(void)
{
    SDIOHost_Readcardstatus(&slots[0]);
    return 0;
}

int read_SD_status(void)
{
    int i = 0;
    read_card_status();
    SDIOHost_ReadSDstatus(&slots[0]);
    for (i = 0; i < 64; i++) {
        sd_printf("%02x ", dbuf[i]);
    }
    sd_printf("\n");
    return 0;
}

int send_sd_command(int cmd, int argument)
{
    int status = 0;

    read_card_status();

    if (argument == 0) {
        argument = 0;
    }
    // rca
    if (argument == 1) {
        argument = slots[0].RCA << 16;
    }

    switch (cmd) {
    case 2: {
        cmd = SDMMC_CMD2;
        break;
    }
    case 10: {
        cmd = SDMMC_CMD10;
        sd_printf("\ncmd = %x\n", cmd);
        break;
    }

    default:
        break;
    }

    status = SDIOHost_ExecCardCommand(&slots[0], cmd, argument);
    if (status)
        return status;

    return 0;
}

static int sd_irq_handler(int irq, void *dev_id)
{
    int status = sd_get_isr_status(&slots[0]);
    // sd_clr_isr_status(&slots[0]);
    printf("\nst=%x\n", status);
    return 0;
}

int sd_irq_init(void)
{
    request_irq(SYS_IRQ_ID_SD, sd_irq_handler, "sd", (void *) SDIO_REGISTERS_OFFSET);
    return 0;
}

int sd_dma_program(int address, char *buf, int count, int dir)
{
    unsigned long long s, e;
    unsigned int us;
#if 1
    int i = 0;
    for (i = 0; i < (512 * count); i++) {
        dbuf[i] = 0xaa + i;
    }
#endif
    // clean
    v7_dma_clean_range((u32) dbuf, (u32)(dbuf + 512 * count));
    sd_printf("\ndbuf addr = 0x%x\n", dbuf);

    s = timer_get_tick();
    MemoryCard_Data_dma_program(&slots[0], address, dbuf, count);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    printf("Test need %d us (%d ms)\n", us, us / 1000);
    return 0;
}

int sd_dma_transfer(int address, char *buf, int count, int dir)
{
    int i = 0;
    unsigned long long s, e;
    unsigned int us;

    v7_dma_inv_range((u32) dbuf, (u32)(dbuf + count * 512));
    s = timer_get_tick();
    MemoryCard_Data_dma_transfer(&slots[0], address, dbuf, count);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    printf("Test need %d us (%d ms)\n", us, us / 1000);
    sd_printf("\ndbuf addr = 0x%x\n", dbuf);

    for (i = 0; i < count * 512; i++) {
        sd_printf("%02x ", dbuf[i]);
    }
    sd_printf("\n");
    return 0;
}

#define SD_ADMA_DESP_SIZE    0X8000
ALIGNED(8) int dmabuf[SD_ADMA_DESP_SIZE] = {0};
static int sd_adma_control_linklist(char *buf, u32 length)
{
    int count = length / 512;
    int i = 0;
    
    sd_printf("\nadma length= 0x%x,count = 0x%x\n", length, count);
    sd_printf("\ndmabuf addr = 0x%x, buf addr = 0x%x\n", dmabuf, buf);
    for(i = 0; i < 2 * (count - 1); i += 2) {
        dmabuf[i] = 0x02000021;
        dmabuf[i + 1] = (u32) (buf + i * 256);
    }
    dmabuf[i] = 0x02000023;
    dmabuf[i + 1] = (u32) (buf + i * 256);
    sd_printf("\nadma i = 0x%x\n", i);
    
    v7_dma_clean_range((u32) dmabuf, (u32)(dmabuf + SD_ADMA_DESP_SIZE));
    return 0;
}

int sd_adma_write(const int addr, const int count)
{
    unsigned long long s, e;
    unsigned int us;
#if 1
    int i = 0;
    for (i = 0; i < (512 * count); i++) {
        dbuf[i] = 0x33 + i;
    }
    dbuf[i - 1] = 0xee;
#endif
    v7_dma_clean_range((u32) dbuf, (u32)(dbuf + 512 * count));
    
    sd_adma_control_linklist(dbuf, count * 512);

    s = timer_get_tick();
    MemoryCard_Data_adma_program(&slots[0], addr, dmabuf, count);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    printf("dmaw need %d us (%d ms)\n", us, us / 1000);
    sd_printf("\nsrs1 = %08x, srs21 - 0x%08x\n", slots[0].io->SRS1, slots[0].io->SRS21);
    return 0;
}

int sd_adma_read(const int addr, const int count)
{
    unsigned long long s, e;
    unsigned int us;
    v7_dma_inv_range((u32) rdbuf, (u32)(rdbuf + 512 * count));
    
    sd_adma_control_linklist(rdbuf, count * 512);

    s = timer_get_tick();
    MemoryCard_Data_adma_transfer(&slots[0], addr, dmabuf, count);
    e = timer_get_tick();
    us = tick_to_us((unsigned long) (e - s));
    printf("dmar need %d us (%d ms)\n", us, us / 1000);
    return 0;
}

