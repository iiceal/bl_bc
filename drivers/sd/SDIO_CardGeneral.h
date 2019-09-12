//  *******************************************************************
// 
//   Copyright (c) 2011  Evatronix SA 
//
//  *******************************************************************
//
//   Please review the terms of the license agreement before using     
//   this file. If you are not an authorized user, please destroy this 
//   source code file and notify Evatronix SA immediately that you     
//   inadvertently received an unauthorized copy.                      
//
//  *******************************************************************
/// @file           SDIO_CardGeneral.h
/// @brief          SDIO card general definitions. 
///                 It contians card command definitions, card register
///                 bit definitions and response types definitions
/// @version        $Revision: 1.1 $
/// @author         Piotr Sroka
/// @date           $Date: 2011/10/18 06:11:13 $
//  *******************************************************************

#ifndef SDIO_CARD_GENERAL_H
#define SDIO_CARD_GENERAL_H

//---------------------------------------------------------------------------
/// @name Commands definitions
//---------------------------------------------------------------------------
//@{
/// Reset SD card, go to idle state
#define SDIO_CMD0    0
/// Asks the card, in idle state, to send its Operating Conditions Register contents in the response on the SDIO_CMD line.
#define SDIO_CMD1    1
/// Asks the card to send its CID number on the SDIO_CMD line
#define SDIO_CMD2    2
/// Assigns relative address to the card
#define SDIO_CMD3    3
/// Programs the DSR of the card
#define SDIO_CMD4    4
/// It is used to inquire about the voltage range needed by the I/O card
#define SDIO_CMD5    5
/// Switch function command it is used to switch or expand memory card functions
#define SDIO_CMD6    6
/// Command toggles a card between the stand-by and transfer states or between the programming and disconnect states.
#define SDIO_CMD7    7
/// Sends SD Memory Card interface condition, which includes host supply voltage information and asks the card whether card supports voltage.
#define SDIO_CMD8    8
/// Addressed card sends its card-specific data (CSD) on the SDIO_CMD line.
#define SDIO_CMD9    9
/// Addressed card sends its card identification (CID) on SDIO_CMD the line.
#define SDIO_CMD10   10
/// Reads data stream from the card, starting at the given address, until a STOP_TRANSMISSION follows. (MMC card only)
#define SDIO_CMD11   11
/// Forces the card to stop transmission
#define SDIO_CMD12   12
/// Addressed card sends its status register. 
#define SDIO_CMD13   13
/// A host reads the reversed bus testing data pattern from a card. (MMC card only)
#define SDIO_CMD14   14
/// Sets the card to inactive state
#define SDIO_CMD15   15
/// Setss the block length of the SD/SDIO card
#define SDIO_CMD16   16
/// Reads a block of the size selected by the SET_BLOCKLEN command.
#define SDIO_CMD17   17
/// Continuously transfers data blocks from card to host until interrupted by a stop command, 
#define SDIO_CMD18   18
/// A host sends the bus test data pattern to a card. (MMC card only)
#define SDIO_CMD19   19
///  Writes a data stream from the host, starting at the given address, until a STOP_TRANSMISSION follows. (MMC card only)
#define SDIO_CMD20   20
/// Defines the number of blocks which are going to be transferred in the immediately succeeding multiple block read or write command.(MMC card only)
#define SDIO_CMD23   23
/// Writes a block of the size selected by the SET_BLOCKLEN command.
#define SDIO_CMD24   24
/// Continuously writes blocks of data until a STOP_TRANSMISSION follows or the requested number of block received.
#define SDIO_CMD25   25
/// Programming of the programmable bits of the CSD.
#define SDIO_CMD27   27
/// If the card has write protection features, this command sets the write protection bit of the addressed group. 
#define SDIO_CMD28   28
/// If the card provides write protection features, this command clears the write protection bit of the addressed group.
#define SDIO_CMD29   29
/// Ask the card to send the status of the write protection. (If card support write protection)
#define SDIO_CMD30   30
/// Sets the address of the first write block to be erased.
#define SDIO_CMD32   32
/// Sets the address of the last write block of the continuous range to be erased.
#define SDIO_CMD33   33
/// Sets the address of the first write block to be erased for MMC device
#define SDIO_CMD35   35
/// Sets the address of the last write block of the continuous range to be erased for MMC device
#define SDIO_CMD36   36
/// Erases all previously selected write blocks.
#define SDIO_CMD38   38
/// Used to set/reset the password or lock/unlock the card. The size of the data block is set by the SET_BLOCK_LEN command.
#define SDIO_CMD42   42
/// Access a single register within the total 128K of register space
#define SDIO_CMD52   52
/// Extended access a single register within the total 128K of register space
#define SDIO_CMD53   53
/// Commmand informs that the next command is an application specific command rather than a standard command
#define SDIO_CMD55   55
/// Used either to transfer a data block to the card or to get a data block from the card for general purpose/application specific commands.
#define SDIO_CMD56   56
/// Read OCR register in SPI mode
#define SDIO_CMD58   58
/// Turns the CRC option on or off in SPI mode
#define SDIO_CMD59   59
/// Set the data bus width. 
#define SDIO_ACMD6   6
/// Read SD card status
#define SDIO_ACMD13  13
/// Command is used to get the number of the written (without errors) write blocks. Responds with 32bit+CRC data block.
#define SDIO_ACMD22  22
/// Set the number of write blocks to be preerased before writing
#define SDIO_ACMD23  23
/// Sends host capacity support information (HCS) and asks the accessed card 
/// to send its operating condition register (OCR) content in the response on the CMD line.
#define SDIO_ACMD41  41
/// Connect[1]/Disconnect[0] the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card.
#define SDIO_ACMD42  42
/// Reads the SD Configuration Register (SCR).
#define SDIO_ACMD51  51
//}@
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/// @name OCR register bits defnitions of SD memory cards
//---------------------------------------------------------------------------
//{@
#define SDCARD_REG_OCR_1_8_0_0  (1ul << 7)
#define SDCARD_REG_OCR_2_7_2_8  (1ul << 15)
#define SDCARD_REG_OCR_2_8_2_9  (1ul << 16)
#define SDCARD_REG_OCR_2_9_3_0  (1ul << 17)
#define SDCARD_REG_OCR_3_0_3_1  (1ul << 18)
#define SDCARD_REG_OCR_3_1_3_2  (1ul << 19)
#define SDCARD_REG_OCR_3_2_3_3  (1ul << 20)
#define SDCARD_REG_OCR_3_3_3_4  (1ul << 21)
#define SDCARD_REG_OCR_3_4_3_5  (1ul << 22)
#define SDCARD_REG_OCR_3_5_3_6  (1ul << 23)
/// card capacity status (this bit is don't aviable in the SDIO cards)
#define SDCARD_REG_OCR_CCS      (1ul << 30 )
/// card power up busy status (this bit is don't aviable in the SDIO cards)
#define SDCARD_REG_OCR_READY    (1ul << 31 )
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name SCR register bits defnitions and slot masks
//---------------------------------------------------------------------------
//@{
/// SD supported bus width 1 bit
#define SDCARD_REG_SCR_SBW_1BIT         (1ul << 16)
/// SD supported bus width 1 bit
#define SDCARD_REG_SCR_SBW_4BIT         (1ul << 18)
/// SD bus width mask
#define SDCARD_REG_SCR_SBW_MASK         0x000F0000ul
/// SD security mask
#define SDCARD_REG_SCR_SEC_MASK         0x00700000ul
/// SD security - no security       
#define SDCARD_REG_SCR_SEC_NO           0x00000000ul
/// SD security version 1.01
#define SDCARD_REG_SCR_SEC_VER_101      0x00200000ul
/// SD security version 2.00
#define SDCARD_REG_SCR_SEC_VER_200      0x00300000ul
/// Data state after erase is 1
#define SDCARD_REG_SCR_DSAE_1           0x00800000ul
/// Physical Layer Specification Version supported by the card mask.
#define SDCARD_REG_SCR_SPEC_MAS         0x0F000000ul
/// Physical Layer Specification Version 1.00 - 1.01
#define SDCARD_REG_SCR_SPEC_VER_100     0x00000000ul
/// Physical Layer Specification Version 1.10
#define SDCARD_REG_SCR_SPEC_VER_110     0x01000000ul
/// Physical Layer Specification Version 2.00
#define SDCARD_REG_SCR_SPEC_VER_200     0x02000000ul
/// Physical Layer Specification mask
#define SDCARD_REG_SCR_SPEC_VER_MASK    0x0F000000ul
/// SCR structure mask
#define SDCARD_REG_SCR_STRUCTURE_MASK   0xF0000000ul
/// SCR version 1.0
#define SDCARD_REG_SCR_VER_10           0x00000000ul
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Response R4 bit definitions
//---------------------------------------------------------------------------
//@{
/// card ready bit
#define SDCARD_R4_CARD_READY        (1ul << 31)
/// memory present bit
#define SDCARD_R4_MEMORY_PRESENT    (1ul << 27)
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Command masks
//---------------------------------------------------------------------------
//@{
/// host hight capacity support - 
#define SDCARD_ACMD41_HCS   (1ul << 30)
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Response SPI R1 for SD memory cards bits defnitions
//---------------------------------------------------------------------------
//@{
/// The card is in idle state and running the initializing process. 
#define SDCARD_RESP_R1_IDLE                 0x01
/// An erase sequence was cleared before executing because an out of erase sequence command was received. 
#define SDCARD_RESP_R1_ERASE_RESET          0x02
/// An illegal command code was detected. 
#define SDCARD_RESP_R1_ILLEGAL_CMD_ERR      0x04
/// The CRC check of the last command failed. 
#define SDCARD_RESP_R1_COM_CRC_ERR          0x08
/// An error in the sequence of erase commands occurred.  
#define SDCARD_RESP_R1_ERASE_SEQUENCE_ERR   0x10
/// A misaligned address that did not match the block length was used in the command. 
#define SDCARD_RESP_R1_ADDRESS_ERR          0x20
/// The command�s argument (e.g. address, block length) was outside the allowed 
#define SDCARD_RESP_R1_PARAM_ERR            0x40
/// All errors mask
#define SDCARD_RESP_R1_ALL_ERRORS           (SDCARD_RESP_R1_ILLEGAL_CMD_ERR\
                                            | SDCARD_RESP_R1_COM_CRC_ERR\
                                            | SDCARD_RESP_R1_ERASE_SEQUENCE_ERR\
                                            | SDCARD_RESP_R1_ADDRESS_ERR\
                                            | SDCARD_RESP_R1_PARAM_ERR)
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Response SPI modified R1 for SDIO cards bit defnitions, 
// These bits are a part of the R5 response in SPI mode as also
//---------------------------------------------------------------------------
//@{
/// The card is in idle state and running the initializing process. 
#define SDCARD_RESP_MOD_R1_IDLE                 0x01
/// An illegal command code was detected. 
#define SDCARD_RESP_MOD_R1_ILLEGAL_CMD_ERR      0x04
/// The CRC check of the last command failed. 
#define SDCARD_RESP_MOD_R1_COM_CRC_ERR          0x08
///  Function number error 
#define SDCARD_RESP_MOD_R1_FUN_NUMB_ERR         0x10
/// A misaligned address that did not match the block length was used in the command. 
#define SDCARD_RESP_MOD_R1_PARAM_ERR            0x40
/// All errors mask
#define SDCARD_RESP_MOD_R1_ALL_ERRORS       (SDCARD_RESP_MOD_R1_ILLEGAL_CMD_ERR\
                                            | SDCARD_RESP_MOD_R1_COM_CRC_ERR\
                                            | SDCARD_RESP_MOD_R1_FUN_NUMB_ERR\
                                            | SDCARD_RESP_MOD_R1_PARAM_ERR)
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name R5 response flags bit definitions 
//---------------------------------------------------------------------------
//@{
/// The command's argument was out of the allowed range for this card.
#define SDCARD_R5_FLAGS_OUT_OF_RANGE_ERR    0x0100
/// An invalid function number was requested
#define SDCARD_R5_FLAGS_FUNC_NUMB_ERR       0x0200
/// A general or an unknown error occurred during the operation
#define SDCARD_R5_FLAGS_GENERAL_ERR         0x0800
/// IO current state - disabled
#define SDCARD_R5_FLAGS_STATE_DIS           0x0000
/// IO current state - DAT line is free
#define SDCARD_R5_FLAGS_STATE_CMD           0x1000
/// IO current state -  Command executing with data transfer using DAT[0] or DAT[3:0] lines
#define SDCARD_R5_FLAGS_STATE_TRN           0x2000
/// Command not legal for the card state
#define SDCARD_R5_FLAGS_ILLEGAL_CMD_ERR     0x4000
/// The CRC check for the previous command failed
#define SDCARD_R5_FLAGS_COM_CRC_ERR         0x8000
/// All flags mask
#define SDCARD_R5_FLAGS_ALL                 0xFF00
// All error flags mask
#define SDCARD_R5_FLAGS_ALL_ERROR           (SDCARD_R5_FLAGS_OUT_OF_RANGE_ERR\
                                                | SDCARD_R5_FLAGS_FUNC_NUMB_ERR\
                                                | SDCARD_R5_FLAGS_GENERAL_ERR\
                                                | SDCARD_R5_FLAGS_ILLEGAL_CMD_ERR\
                                                | SDCARD_R5_FLAGS_ILLEGAL_CMD_ERR)
//@}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/// @anchor CCCR
/// @name CCCR card control registers definitions
//---------------------------------------------------------------------------
//@{
/// CCCR version number and SDIO specification version number register
#define SDCARD_CCCR_CCCR_SDIO_REV   0x00
/// SD version number register
#define SDCARD_CCCR_SD_SPEC_REV     0x01
/// IO enable function register
#define SDCARD_CCCR_IO_ENABLE       0x02
/// IO function ready register
#define SDCARD_CCCR_IO_READY        0x03
/// Interrupt enable register
#define SDCARD_CCCR_INT_ENABLE      0x04
/// Interrupt pending register
#define SDCARD_CCCR_INT_PENDING     0x05
/// IO Abort register. It used to stop a function transfer.
#define SDCARD_CCCR_IO_ABORT        0x06
/// Bus interface control register
#define SDCARD_CCCR_BUS_CONTROL     0x07
/// Card capability register
#define SDCARD_CCCR_CARD_CAPABILITY 0x08
/// Pointer to card� s common Card Information Structure (CIS) 
#define SDCARD_CCCR_CIS_POINTER     0x09
/// Bus suspend register
#define SDCARD_CCCR_BUS_SUSPENDED   0x0C
/// Function select register
#define SDCARD_CCCR_FUNCTION_SELECT 0x0D
/// Exec flags register. The bits of this register are used by the host to determine the current execution status of all functions (1-7) and memory (0).
#define SDCARD_CCCR_EXEC_FLAGS      0x0E
/// Ready flags register. The bits of this register tell the host the read or write busy status for functions (1-7) and memory (0).
#define SDCARD_CCCR_READY_FLAGS     0x0F
/// I/O block size for Function 0 
#define SDCARD_CCCR_FN0_BLOCK_SIZE  0x10
/// Power control register
#define SDCARD_CCCR_POWER_CONTROL   0x12
/// High speed register
#define SDCARD_CCCR_HIGH_SPEED      0x14
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Bus interface control register bit definitions
//---------------------------------------------------------------------------
//@{
/// Data 4 bit bus width 
#define SDCARD_BIS_BUS_WIDTH_4BIT   0x02
/// Data 1 bit bus width 
#define SDCARD_BIS_BUS_WIDTH_1BIT   0x00
/// Connect[0]/Disconnect[1] the 10K-90K ohm pull-up resistor on CD/DAT[3] (pin 1) of card.
#define SDCARD_BIS_CD_DISABLE       0x80
/// Support contiunous SPI interrupt (irrespective of the state the CS line)  
#define SDCARD_BIS_SCSI             0x40
/// Enable contiunous SPI interrupt (irrespective of the state the CS line)
#define SDCARD_BIS_ECSI             0x20
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Card capability register bit definitions
//---------------------------------------------------------------------------
//@{
/// Card supports direct commands during data trnsfer. (only in SD mode)
#define SDCARD_CCR_SDC  0x01
/// Card supports multiblock
#define SDCARD_CCR_SMB  0x02
/// Card supports read wait
#define SDCARD_CCR_SRW  0x04
/// Card supports Suspend/Resume
#define SDCARD_CCR_SBS  0x08
/// Card supports interrupt between blocks of data in 4-bit SD mode.
#define SDCARD_CCR_S4MI 0x10
/// Enable interrupt between blocks of data in 4-bit SD mode.
#define SDCARD_CCR_E4MI 0x20
/// Card is a low-speed card
#define SDCARD_CCR_LSB  0x40
/// 4 bit support for Low-Speed cards
#define SDCARD_CCR_4BLS 0x80
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Bus suspend register bit definitions
//---------------------------------------------------------------------------
//@{
/// Bus status. Currently addressed function is transferring dara on the DAT line.
#define SDCARD_BS_BS    0x01
/// Bus release request/status. This bit is used to request that the addressed function to suspend operation.
#define SDCARD_BS_BR    0x02
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Power control register bit definitions
//---------------------------------------------------------------------------
//@{
/// Support master power control.
#define SDCARD_PC_SMPC  0x01
/// Enable master power control.
#define SDCARD_PC_EMPC  0x02
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name High speed register bit definitions
//---------------------------------------------------------------------------
//@{
/// Support high speed.
#define SDCARD_HS_SHS   0x01
/// Enable high speed.
#define SDCARD_PC_EHS   0x02
//@}
//---------------------------------------------------------------------------

#define GETBYTE(dword, byte_nr)(((dword) >> ((byte_nr) * 8)) & 0xFF)
#define GET_BYTE_FROM_BUFFER(buffer, byte_nr) GETBYTE((*(DWORD*)(buffer + (byte_nr & ~3))), (byte_nr & 3))
#define GET_BYTE_FROM_BUFFER2(buffer, buffer_size, byte_nr)\
		(GET_BYTE_FROM_BUFFER(buffer, (buffer_size - 1 - byte_nr)))


//---------------------------------------------------------------------------
/// @name SD card function register bits, masks and macros definitions
//---------------------------------------------------------------------------
//@{
/// Check function mode - is used to query if the card supports a specific function or functions.
#define SDCARD_SWITCH_FUNC_MODE_SWITCH      (0x1ul << 31)
/// Set function mode  - is used to switch the functionality of the card.
#define SDCARD_SWITCH_FUNC_MODE_CHECK       0x0
					
/// Card access mode 25MHz
#define SDCARD_SWITCH_FUNC_NR_HIGH_SPEED       0x1
/// Card command system - eCommerce command set
#define SDCARD_SWITCH_FUNC_NR_E_COMMERCE       0x1
/// Card command system - vendor specific command set
#define SDCARD_SWITCH_FUNC_NR_VENDOR           0xE

#define SDCARD_SWITCH_GROUP_NR_1    1
#define SDCARD_SWITCH_GROUP_NR_2    2
#define SDCARD_SWITCH_GROUP_NR_3    3
#define SDCARD_SWITCH_GROUP_NR_4    4
#define SDCARD_SWITCH_GROUP_NR_5    5
#define SDCARD_SWITCH_GROUP_NR_6    6


/// Macro gets function busy status from the switch funnction status data structure
/// Macro returns 1 if function is busy 0 otherwise
#define SDCARD_SWICH_FUNC_GET_BUSY_STAT(VAL, GROUP_NUM, FUNC_NUM)\
    (\
        (BYTE)((((DWORD)GET_BYTE_FROM_BUFFER2(VAL, 64, (34 - (GROUP_NUM - 1) * 2))\
        | (DWORD)(GET_BYTE_FROM_BUFFER2(VAL, 64, (35 - (GROUP_NUM - 1) * 2))) << 8)  \
        & (1ul << FUNC_NUM)) != 0)\
     )

/// Macro gets function status code from the switch funnction status data structure
#define SDCARD_SWICH_FUNC_GET_STAT_CODE(VAL, GROUP_NUM) \
    ((BYTE)GET_BYTE_FROM_BUFFER2(VAL, 64, (47 + (GROUP_NUM - 1) / 2)) \
     >> (((( GROUP_NUM - 1 ) % 2 ) * 4 ) & 0xF))

/// Macro returns 1 if given function is supported by the card
#define SDCARD_SWITCH_FUNC_IS_FUNC_SUPPORTED(VAL, GROUP_NUM, FUNC_NUM)\
    (\
        (BYTE)((((DWORD)GET_BYTE_FROM_BUFFER2(VAL, 64, (50 - (GROUP_NUM - 1) * 2))\
        | (DWORD)(GET_BYTE_FROM_BUFFER2(VAL, 64, (51 - (GROUP_NUM - 1) * 2))) << 8)  \
        & (1ul << FUNC_NUM)) != 0)\
     )
        
/// Macro gets version number of data structure
#define SDCARD_SWITCH_FUNC_GET_DATA_STRUCT_VER(VAL) \
    ((BYTE)GET_BYTE_FROM_BUFFER2(VAL, 64, 46)) 

//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Card status bits and masks definitions for cards (SD SDIO MMC)
//---------------------------------------------------------------------------
//@{
/// Error authentication process 
#define CARD_SATUS_AKE_SEQ_ERROR          (0x1ul << 3)
/// The card will expect ACMD, or an indication that the command has been interpreted as ACMD 
#define CARD_SATUS_APP_CMD                (0x1ul << 5)
/// Card didn't switch to the expected mode as requested by the SWITCH command
#define CARD_STATUS_SWITCH_ERROR          (0x1ul << 7)
/// Corresponds to buffer empty signaling on the bus - buffer is ready 
#define CARD_SATUS_READY_FOR_DATA         (0x1ul <<8)
/// The state of the card when receiving the command. Below are definded all 9 satuses.
#define CARD_SATUS_CURRENT_STATUS_MASK    (0xFul << 9)
/// Current status card is in Idle State
#define CARD_STATUS_CS_IDLE               (0x0ul << 9)
/// Current status card is in Ready State  
#define CARD_STATUS_CS_READY              (0x1ul <<9)
/// Current status card is Identification State
#define CARD_STATUS_CS_IDENT              (0x2ul << 9)
/// Current status card is in Stand-by State 
#define CARD_STATUS_CS_STBY               (0x3ul << 9)
/// Current status card is in Transfer State 
#define CARD_STATUS_CS_TRAN               (0x4ul << 9)
/// Current status card is in Sending-data State 
#define CARD_STATUS_CS_DATA               (0x5ul << 9)
/// Current status card is in Receive-data State 
#define CARD_STATUS_CS_RCV                (0x6ul << 9)
/// Current status card is in Programming State
#define CARD_STATUS_CS_PRG                (0x7ul << 9)
/// Current status card is in Disconnect State
#define CARD_STATUS_CS_DIS                (0x8ul << 9)
/// An erase sequence was cleared before executing because an out of erase sequence command was received 
#define CARD_STATUS_ERASE_RESET           (0x1ul << 13)
/// The command has been executed without using the internal ECC.  
#define CARD_STATUS_CARD_ECC_DISABLED     (0x1ul << 14)
/// Problem with erase part of memory because it is protected
#define CARD_STATUS_WP_ERASE_SKIP         (0x1ul << 15)
/// Can be either one of the following errors: 
/// - The read only section of the CSD does not match the card content. 
/// - An attempt to reverse the copy (set as original) or permanent WP (unprotected) bits was made. 
#define CARD_STATUS_CSD_OVERWRITE         (0x1ul << 16)
/// The card could not sustain data programming in stream write mode
#define CARD_STATUS_OVERRUN               (0x1ul << 17)
/// The card could not sustain data transfer in stream read mode
#define CARD_STATUS_UNDERRUN              (0x1ul << 18)
/// A general or an unknown error occurred during the operation. 
#define CARD_STATUS_ERROR                 (0x1ul << 19)
/// Internal card controller error
#define CARD_STATUS_CC_ERROR              (0x1ul << 20)
/// Card internal ECC was applied but failure failed to correct the data. 
#define CARD_STATUS_CARD_ECC_FAILED       (0x1ul << 21)
/// Command not legal for the card state
#define CARD_STATUS_ILLEGAL_COMMAND       (0x1ul << 22)
/// The CRC check of the previous error command failed. 
#define CARD_STATUS_COM_CRC_ERROR         (0x1ul << 23)
/// Set when a sequence or password error has been detected in lock/unlock card command. 
#define CARD_STATUS_LOCK_UNLOCK_FAILED    (0x1ul << 24)
/// When set, signals that the card is card locked by the host
#define CARD_STATUS_CARD_IS_LOCKED        (0x1ul << 25)
/// Set when the host attempts to write to a protected block or to the temporary or permanent write protected card. 
#define CARD_STATUS_WP_VIOLATION          (0x1ul << 26)
/// An invalid selection of write-blocks for erase occurred.
#define CARD_STATUS_ERASE_PARAM           (0x1ul << 27)
/// An error in the sequence of erase error commands occurred.
#define CARD_STATUS_ERASE_SEQ_ERROR       (0x1ul << 28)
/// The transferred block length is not allowed for this card, or the number 
/// of transferred bytes does not match the block length. 
#define CARD_STATUS_BLOCK_LEN_ERROR       (0x1ul << 29)
/// A misaligned address which did not match the block length was used in the command. 
#define CARD_STATUS_ADDRESS_ERROR         (0x1ul << 30)
/// The command� s argument was out of the allowed range for this card. 
#define CARD_STATUS_OUT_OF_RANGE          (0x1ul << 31)

/// All errors mask definition
#define CARD_STATUS_ALL_ERRORS_MASK (   CARD_STATUS_OUT_OF_RANGE        \
                                      | CARD_STATUS_ADDRESS_ERROR       \
                                      | CARD_STATUS_BLOCK_LEN_ERROR     \
                                      | CARD_STATUS_ERASE_SEQ_ERROR     \
                                      | CARD_STATUS_ERASE_PARAM         \
                                      | CARD_STATUS_WP_VIOLATION        \
                                      | CARD_STATUS_LOCK_UNLOCK_FAILED  \
                                      | CARD_STATUS_COM_CRC_ERROR       \
                                      | CARD_STATUS_ILLEGAL_COMMAND     \
                                      | CARD_STATUS_CARD_ECC_FAILED     \
                                      | CARD_STATUS_CC_ERROR            \
                                      | CARD_STATUS_ERROR               \
                                      | CARD_STATUS_UNDERRUN            \
                                      | CARD_STATUS_OVERRUN             \
                                      | CARD_STATUS_WP_ERASE_SKIP       \
                                      | CARD_STATUS_SWITCH_ERROR        \
                                      | CARD_SATUS_AKE_SEQ_ERROR )
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Response R6 argument field bit and masks definitions for SD memory card
//---------------------------------------------------------------------------
//@{
//---------------------------
// SD memory only definitions
//---------------------------
/// error authentication process 
#define SDCARD_R6_SDMEM_STATUS_AKE_SEQ_ERROR        (0x1ul <<3)
///The card will expect ACMD, or an indication that the command has been interpreted as ACMD 
#define SDCARD_R6_SDMEM_STATUS_APP_CMD              (0x1ul <<5)
/// Corresponds to buffer empty signaling on the bus - buffer is ready 
#define SDCARD_R6_SDMEM_STATUS_READY_FOR_DATA       (0x1ul <<8)
/// The state of the card when receiving the command. Below are definded all 9 satuses.
#define SDCARD_R6_SDMEM_STATUS_CURRENT_STATUS_MASK  (0xF << 9)
/// Current status card is in Idle State
#define SDCARD_R6_SDMEM_STATUS_CS_IDLE              (0x0 << 9)
/// Current status card is in Ready State  
#define SDCARD_R6_SDMEM_STATUS_CS_READY             (0x1ul <<9)
/// Current status card is Identification State
#define SDCARD_R6_SDMEM_STATUS_CS_IDENT             (0x2 << 9)
/// Current status card is in Stand-by State 
#define SDCARD_R6_SDMEM_STATUS_CS_STBY              (0x3 << 9)
/// Current status card is in Transfer State 
#define SDCARD_R6_SDMEM_STATUS_CS_TRAN              (0x4 << 9)
/// Current status card is in Sending-data State 
#define SDCARD_R6_SDMEM_STATUS_CS_DATA              (0x5 << 9)
/// Current status card is in Receive-data State 
#define SDCARD_R6_SDMEM_STATUS_CS_RCV               (0x6 << 9)
/// Current status card is in Programming State
#define SDCARD_R6_SDMEM_STATUS_CS_PRG               (0x7 << 9)
/// Current status card is in Disconnect State
#define SDCARD_R6_SDMEM_STATUS_CS_DIS               (0x8 << 9)
/// Status field from SD memory card response R6 
#define SDCARD_SDMEM_STATUS_MASK                    0xFFFF
//---------------------------

//---------------------------
// Common SDIO and SD memory definitions
//---------------------------
/// A general or an unknown error C occurred during the operation. 
#define SDCARD_R6_STATUS_ERROR                 (0x1ul <<13)
/// Command not legal for the card state
#define SDCARD_R6_STATUS_ILLEGAL_COMMAND       (0x1ul <<14)
/// The CRC check of the previous error command failed. 
#define SDCARD_R6_STATUS_COM_CRC_ERROR         (0x1ul <<15)
/// Both SDIO and SD memory status mask part
#define SDCARD_R6_STATUS_MASK                   0xE000
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor TupleDefinitions
/// @name Tuple names definitions of SDIO card
//---------------------------------------------------------------------------
//@{
/// Null tuple
#define SDIO_TUPLE_CISTPL_NULL      0x00
/// Checksum controll
#define SDIO_TUPLE_CISTPL_CHECKSUM  0x10
/// Level 1 version/product information
#define SDIO_TUPLE_CISTPL_VERS_1    0x15
/// Alternate language string tuple
#define SDIO_TUPLE_CISTPL_ALTSTR    0x16
/// Manufacturer identification string tuple
#define SDIO_TUPLE_CISTPL_MANFID    0x20
/// Function identification tuple
#define SDIO_TUPLE_CISTPL_FUNCID    0x21
/// Function Extensions
#define SDIO_TUPLE_CISTPL_FUNCE     0x22
/// Additional information for functions built to support application specifications for standard SDIO functions.
#define SDIO_TUPLE_CISTPL_SDIO_STD  0x91
/// Reserved for future use with SDIO devices.
#define SDIO_TUPLE_CISTPL_SDIO_EXT  0x92
/// The End-of-chain Tuple
#define SDIO_TUPLE_CISTPL_END       0xFF
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Access modes of MMC card switch function
//---------------------------------------------------------------------------
//@{
///
/// The command set is changed according to the Cmd Set field of the argument 
#define MMC_CMD6_ARG_MODE_COMMAND_SET (0x0uL << 24)
/// The bits in the pointed byte are set, according to the �1� bits in the Value field
#define MMC_CMD6_ARG_MODE_SET_BITS    (0x1uL << 24)
/// bits in the pointed byte are cleared, according to the �1� bits in the Value field
#define MMC_CMD6_ARG_MODE_CLEAR_BITS  (0x2uL << 24)
/// The Value field is written into the pointed byte
#define MMC_CMD6_ARG_MODE_WRITE_BYTE  (0x3uL << 24)
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Bit index definitions in the exetendend CSD register of MMC card.  
//---------------------------------------------------------------------------
/// MMC card power class
#define MMC_SWITCH_INDEX_POWER_CLASS    187
/// High speed interface timing
#define MMC_SWITCH_INDEX_HS_TIMING      185
/// Bus width mode
#define MMC_SWITCH_INDEX_BUS_WIDTH      183
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name Auxilary macros which can be used to prepare argument to CMD6 command for MMC cards
//---------------------------------------------------------------------------
//@{
/// Set access field in CMD6 command argument
#define MMC_CMD6_ARG_VALUE(VAL) ((DWORD)VAL << 8)
/// Set index field in CMD6 command argument
#define MMC_CMD6_ARG_INDEX(VAL) ((DWORD)VAL << 16)
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name MMC card OCR register - mode definitions 
//---------------------------------------------------------------------------
//@{
/// MMC card access mode - sector mode
#define MMC_REG_OCR_SECTOR_MODE (0x2ul << 29 )
/// MMC card access mode - byte mode
#define MMC_REG_OCR_BYTE_MODE   (0x0ul << 29 )
//@}
//---------------------------------------------------------------------------

#endif
