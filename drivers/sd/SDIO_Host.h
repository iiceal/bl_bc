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
/// @file           SDIO_Host.h
/// @brief          SDIO Host registers definitions. 
///                  And functions declarations
/// @version        $Revision: 1.1 $
/// @author         Piotr Sroka
/// @date           $Date: 2011/10/18 06:11:13 $
//  *******************************************************************

#ifndef SDIO_HOST_H
#define SDIO_HOST_H

#include "SDIO_Types.h"
#include "SDIO_Config.h"
#include "SDIO_Errors.h"

/// Memory address of start SDIO registers
//#define SDIO_REGISTERS_OFFSET 0xC5020000
//#define SDIO_REGISTERS_OFFSET 0xC5070000


//-------------------------------------------------------------------------------------------
/// @name Definitions of SDIO controller registers
//-------------------------------------------------------------------------------------------
//@{
/// general information register
/// initialization setting register
/// DMA setting register




/// System addres register it contains physical memory address for DMA operations
#define SDIO_REG_SLOT_SFR0_OFFSET	0x00
/// Block count and size register it contains informations about transmission size
#define SDIO_REG_SLOT_SFR1_OFFSET	0x04
/// Argument register it contains command argument of SD device
#define SDIO_REG_SLOT_SFR2_OFFSET	0x08
/// Transfer mode and command information register
#define SDIO_REG_SLOT_SFR3_OFFSET	0x0C
/// Response #0 register
#define SDIO_REG_SLOT_SFR4_OFFSET	0x10
/// Response #1 register
#define SDIO_REG_SLOT_SFR5_OFFSET	0x14
/// Response #2 register
#define SDIO_REG_SLOT_SFR6_OFFSET	0x18
/// Response #3 register
#define SDIO_REG_SLOT_SFR7_OFFSET	0x1C
/// Buffer data port register
#define SDIO_REG_SLOT_SFR8_OFFSET	0x20
/// Preset state register it contains the status of the SLOT
#define SDIO_REG_SLOT_SFR9_OFFSET	0x24
/// Host control settings #0 register
#define SDIO_REG_SLOT_SFR10_OFFSET	0x28
/// Host control settings #1 register 
#define SDIO_REG_SLOT_SFR11_OFFSET	0x2C
/// Interrupt status register
#define SDIO_REG_SLOT_SFR12_OFFSET	0x30
/// Interrupt status enable register
#define SDIO_REG_SLOT_SFR13_OFFSET	0x34
/// Interrupt signal enable register
#define SDIO_REG_SLOT_SFR14_OFFSET	0x38
/// Auto CMD12 error status register
#define SDIO_REG_SLOT_SFR15_OFFSET	0x3C
/// Capabilities register
#define SDIO_REG_SLOT_SFR16_OFFSET	0x40
/// Maximum current capabilities
#define SDIO_REG_SLOT_SFR18_OFFSET	0x48
/// Event Trigger Register
#define SDIO_REG_SLOT_SFR20_OFFSET	0x50
/// ADMA Error Status Register
#define SDIO_REG_SLOT_SFR21_OFFSET	0x54
/// ADMA System Address Register
#define SDIO_REG_SLOT_SFR22_OFFSET	0x58

/// register from common register area it contains host controller version and slot interrupt status information
#define SDIO_REG_COMM_SFR63     (*(volatile DWORD *) (SDIO_REGISTERS_OFFSET + 0x00FC))
//@}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
/// @name General information register (HSFR0) - masks
//-------------------------------------------------------------------------------------------
//@{
/// 4 slots are accesible in the SDIO host
#define HSFR0_4_ACCESABLE_SLOTS     (0x1uL << 19)
/// 3 slots are accesible in the SDIO host
#define HSFR0_3_ACCESABLE_SLOTS     (0x1uL << 18)
/// 2 slots are accesible in the SDIO host
#define HSFR0_2_ACCESABLE_SLOTS     (0x1uL << 17)
/// 1 slots are accesible in the SDIO host
#define HSFR0_1_ACCESABLE_SLOT      (0x1uL << 16)
/// Reset all internal registers including SFR RAM. The card detection unit is reset also.
#define HSFR0_SOFTWARE_RESET        (1ul << 0)
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Initialization setting register (HSFR1) - masks and macros
//-------------------------------------------------------------------------------------------
//@{
/// SDIO host uses SD mode to initialize card
#define HSFR1_INITIALIZATION_MODE_SD    0x10000000ul
/// SDIO host uses SPI mode to initialize card
#define HSFR1_INITIALIZATION_MODE_SPI   0x00000000ul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name DMA settings register (HSFR2) - values   
//-------------------------------------------------------------------------------------------
//@{
/// set 2048 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_2048    0xC
/// set 1024 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_1024    0xB
/// set 512 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_512     0xA
/// set 256 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_256     0x9
/// set 128 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_128     0x8
/// set 64 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_64      0x7
/// set 32 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_32      0x6
/// set 16 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_16      0x5
/// set 8 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_8       0x4
/// set 4 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_4       0x3
/// set 2 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_2       0x2
/// set 1 words as a maximum that can be transfered within one DMA transaction
#define HSFR2_PROGRAMMABLE_BURST_LENGTH_1       0x1
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Block count and size register (SFR1) - masks
//-------------------------------------------------------------------------------------------
//@{
/// Block count for current transfer mask
#define SFR1_BLOCK_COUNT            0xFFFF0000ul
/// DMA buffer size 4kB
#define SFR1_DMA_BUFF_SIZE_4KB      0x00000000ul
/// DMA buffer size 8kB
#define SFR1_DMA_BUFF_SIZE_8KB      0x00001000ul
/// DMA buffer size 16kB
#define SFR1_DMA_BUFF_SIZE_16KB     0x00002000ul
/// DMA buffer size 32kB
#define SFR1_DMA_BUFF_SIZE_32KB     0x00003000ul
/// DMA buffer size 64kB
#define SFR1_DMA_BUFF_SIZE_64KB     0x00004000ul
/// DMA buffer size 128kB
#define SFR1_DMA_BUFF_SIZE_128KB    0x00005000ul
/// DMA buffer size 265kB
#define SFR1_DMA_BUFF_SIZE_256KB    0x00006000ul
/// DMA buffer size 512kB
#define SFR1_DMA_BUFF_SIZE_512KB    0x00007000ul
/// DMA buffer size mask            
#define SFR1_DMA_BUFF_SIZE_MASK     0x00007000ul
/// Transfer block size mask
#define SFR1_BLOCK_SIZE             0x00000FFFul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Transfer mode and command information register (SFR3) - masks
//-------------------------------------------------------------------------------------------
//@{
/// Abort CMD12, CMD52 for writing “I/O Abort?in CCCR 
#define SFR3_ABORT_CMD          0xC00000ul
/// Resume CMD52 for writing “Function Select?in CCCR
#define SRF3_RESUME_CMD         0x800000ul
///Suspend CMD52 for writing “Bus Suspend?in CCCR  
#define SFR3_SUSPEND_CMD        0x400000ul
/// data is present and will be transferred using the DAT line
#define SFR3_DATA_PRESENT       0x200000ul
/// index check enable
#define SFR3_INDEX_CHECK_EN     0x100000ul
/// response CRC check enable
#define SFR3_CRC_CHECK_EN       0x080000ul
/// response type - no response
#define SFR3_NO_RESPONSE        0x000000ul
/// response type - response length 136 
#define SFR3_RESP_LENGTH_136    0x010000ul
/// response type - response length 48
#define SFR3_RESP_LENGTH_48     0x020000ul
/// response type - response length 48 and check Busy after response 
#define SFR3_RESP_LENGTH_48B    0x030000ul
/// multi block DAT line data transfers
#define SFR3_MULTI_BLOCK_SEL    0x000020ul
/// data transfer direction - write
#define SFR3_TRANS_DIRECT_WRITE 0x000000ul
/// data transfer direction - read
#define SFR3_TRANS_DIRECT_READ  0x000010ul
/// Auto CMD12 enable
#define SFR3_AUTOCMD12_ENABLE   0x000004ul
/// Block count enable
#define SFR3_BLOCK_COUNT_ENABLE 0x000002ul
/// DMA enable
#define SFR3_DMA_ENABLE         0x000001ul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Present state register masks (SFR9) - masks
//-------------------------------------------------------------------------------------------
//@{
/// CMD line signal level
#define SFR9_CMD_SIGNAL_LEVEL   0x1000000ul
/// DAT3 signal level
#define SFR9_DAT3_SIGNAL_LEVEL  0x0800000ul
/// DAT2 signal level
#define SFR9_DAT2_SIGNAL_LEVEL  0x0400000ul
/// DAT1 signal level
#define SFR9_DAT1_SIGNAL_LEVEL  0x0200000ul  
/// DAT0 signal level
#define SFR9_DAT0_SIGNAL_LEVEL  0x0100000ul 
/// Write protect switch pin level
#define SFR9_WP_SWITCH_LEVEL    0x0080000ul 
/// Card detect pin level
#define SFR9_CARD_DETECT_LEVEL  0x0040000ul 
/// Card state stable
#define SFR9_CARD_STATE_STABLE  0x0020000ul
/// Card inserted
#define SFR9_CARD_INSERTED      0x0010000ul 
/// Buffer read enable
#define SFR9_BUFF_READ_EN       0x0000800ul
/// Buffer write enable
#define SFR9_BUFF_WRITE_EN      0x0000400ul
/// Read transfer active
#define SFR9_READ_TRANS_ACTIVE  0x0000200ul
/// Write transfer active
#define SFR9_WRITE_TRANS_ACTIVE 0x0000100ul
/// DAT line active
#define SFR9_DAT_LINE_ACTIVE    0x0000004ul
/// Command Inhibit (DAT)
#define SFR9_CMD_INHIBIT_DAT    0x0000002ul
/// Command Inhibit (CMD)
#define SFR9_CMD_INHIBIT_CMD    0x0000001ul

//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @anchor Voltages
/// @name Host control settings #0 (SFR10) - masks
//-------------------------------------------------------------------------------------------
//@{
/// Wakeup event enable on SD card removal
#define SFR10_WAKEUP_EVENT_CARD_REM_ENABLE  0x4000000ul
/// Wakeup event enable on SD card inserted
#define SFR10_WAKEUP_EVENT_CARD_INS_ENABLE  0x2000000ul
/// Wakeup event enable on SD card interrupt
#define SFR10_WAKEUP_EVENT_CARD_INT_ENABLE  0x1000000ul
/// Interrupt at block gap for a muliple transfer
#define SFR10_INTERRUPT_BLOCK_GAP           0x0080000ul
/// Read wait control
#define SFR10_READ_WAIT_CONTROL             0x0040000ul
/// Continue request
#define SFR10_CONTINUE_REQUEST              0x0020000ul
/// Stop at block gap request
#define SFR10_STOP_AT_BLOCK_GAP             0x0010000ul
/// SD bus voltage - 3.3V
#define SFR10_SET_3_3V_BUS_VOLTAGE          0x0000E00ul
/// SD bus voltage - 3.0V
#define SFR10_SET_3_0V_BUS_VOLTAGE          0x0000C00ul
/// SD bus voltage - 1.8V
#define SFR10_SET_1_8V_BUS_VOLTAGE          0x0000A00ul
/// SD bus voltage mask
#define SFR10_BUS_VOLTAGE_MASK              0x0000E00ul
/// SD bus power. The SD device is powered.
#define SFR10_SD_BUS_POWER                  0x0000100ul


#define SFR10_DMA_SELECT_SDMA   (0x0 << 3)
#define SFR10_DMA_SELECT_ADMA1  (0x1 << 3)
#define SFR10_DMA_SELECT_ADMA2  (0x2 << 3)
#define SFR10_DMA_SELECT_MASK   (0x3 << 3)

/// High speed enable.
#define SFR10_HIGH_SPEED_ENABLE             0x0000004ul
/// Set 4 bit data transfer width
#define SFR10_DATA_WIDTH_4BIT               0x0000002ul
/// Turning on the LED.
#define SFR10_TURN_ON_LED                   0x0000001ul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Host control settings #1 (SFR11) - masks
//-------------------------------------------------------------------------------------------
//{@
/// Software reset for DAT line
#define SFR11_SOFT_RESET_DAT_LINE           0x4000000ul
/// Software reset for CMD line
#define SFR11_SOFT_RESET_CMD_LINE           0x2000000ul
/// Software reset for all. Restart entrie controller except the card detection circuit.
#define SFR11_SOFT_RESET_ALL                0x1000000ul

/// Data timeout TMCLK x 2 raised to the 27-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_27    0x00E0000ul
/// Data timeout TMCLK x 2 raised to the 26-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_26    0x00D0000ul
/// Data timeout TMCLK x 2 raised to the 25-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_25    0x00C0000ul
/// Data timeout TMCLK x 2 raised to the 24-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_24    0x00B0000ul
/// Data timeout TMCLK x 2 raised to the 23-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_23    0x00A0000ul
/// Data timeout TMCLK x 2 raised to the 22-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_22    0x0090000ul
/// Data timeout TMCLK x 2 raised to the 21-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_21    0x0080000ul
/// Data timeout TMCLK x 2 raised to the 20-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_20    0x0070000ul
/// Data timeout TMCLK x 2 raised to the 19-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_19    0x0060000ul
/// Data timeout TMCLK x 2 raised to the 18-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_18    0x0050000ul
/// Data timeout TMCLK x 2 raised to the 17-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_17    0x0040000ul
/// Data timeout TMCLK x 2 raised to the 16-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_16    0x0030000ul
/// Data timeout TMCLK x 2 raised to the 15-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_15    0x0020000ul
/// Data timeout TMCLK x 2 raised to the 14-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_14    0x0010000ul
/// Data timeout TMCLK x 2 raised to the 13-th power
#define SFR11_TIMEOUT_TMCLK_X_2_POWER_13    0x0000000ul
/// Data timeout mask
#define SFR11_TIMEOUT_MASK                  0x00F0000ul

/// SDCLK Frequency select. Divide base clock by 256.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_256     0x0008000ul
/// SDCLK Frequency select. Divide base clock by 128.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_128     0x0004000ul
/// SDCLK Frequency select. Divide base clock by 64.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_64      0x0002000ul
/// SDCLK Frequency select. Divide base clock by 32.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_32      0x0001000ul
/// SDCLK Frequency select. Divide base clock by 16.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_16      0x0000800ul
/// SDCLK Frequency select. Divide base clock by 8.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_8       0x0000400ul
/// SDCLK Frequency select. Divide base clock by 4.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_4       0x0000200ul
/// SDCLK Frequency select. Divide base clock by 2.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_2       0x0000100ul
/// SDCLK Frequency select. Divide base clock by 1.
#define SFR11_SEL_FREQ_BASE_CLK_DIV_1       0x0000000ul
/// SDCLK Frequency mask                    
#define SFR11_SEL_FREQ_BASE_MASK            0x000FF00ul

/// SD clock enable
#define SFR11_SD_CLOCK_ENABLE              0x0000004ul
/// Internal clock stable
#define SFR11_INT_CLOCK_STABLE             0x0000002ul
/// internal clock enable
#define SFR11_INT_CLOCK_ENABLE             0x0000001ul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Interrupt status register (SFR12) - masks
//-------------------------------------------------------------------------------------------
//@{
/// ADMA error
#define SFR12_ADMA_ERROR	        0x2000000ul
/// Auto CMD12 error
#define SFR12_AUTO_CMD12_ERROR      0x1000000ul
/// Current limit error host controller is not supplying power to SD card due some failure.
#define SFR12_CURRENT_LIMIT_ERROR   0x0800000ul
/// Data end bit error
#define SFR12_DATA_END_BIT_ERROR    0x0400000ul
/// Data CRC error
#define SFR12_DATA_CRC_ERROR        0x0200000ul
/// Data timeout error
#define SFR12_DATA_TIMEOUT_ERROR    0x0100000ul
/// Command index error. Index error occurs in the command response.
#define SFR12_COMMAND_INDEX_ERROR   0x0080000ul
/// Command end bit error
#define SFR12_COMMAND_END_BIT_ERROR 0x0040000ul
/// Command CRC error
#define SFR12_COMMAND_CRC_ERROR     0x0020000ul
/// Command timeout error
#define SFR12_COMMAND_TIMEOUT_ERROR 0x0010000ul
/// Error interrupt
#define SFR12_ERROR_INTERRUPT       0x0008000ul
/// Card interrupt
#define SFR12_CARD_INTERRUPT        0x0000100ul
/// Card removal
#define SFR12_CARD_REMOVAL          0x0000080ul
/// Card insertion
#define SFR12_CARD_INSERTION        0x0000040ul
/// Buffer read ready. Host is ready to read the buffer.
#define SFR12_BUFFER_READ_READY     0x0000020ul
/// Buffer write ready. Host is ready for writing data to the buffer.
#define SFR12_BUFFER_WRITE_READY    0x0000010ul
/// DMA interrupt
#define SFR12_DMA_INTERRUPT         0x0000008ul
/// Block gap event
#define SFR12_BLOCK_GAP_EVENT       0x0000004ul
/// Transfer complete
#define SFR12_TRANSFER_COMPLETE     0x0000002ul
/// Command complete
#define SFR12_COMMAND_COMPLETE      0x0000001ul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Interrupt status enable register (SFR13) - masks
//-------------------------------------------------------------------------------------------
//@{
/// Auto CMD12 error status enable
#define SFR13_AUTO_CMD12_ERR_STAT_EN        0x1000000ul
/// Current limit error status enable
#define SFR13_CURRENT_LIMIT_ERR_STAT_EN     0x0800000ul
/// Data end bit error status enable
#define SFR13_DATA_END_BIT_ERR_STAT_EN      0x0400000ul
/// Data CRC error status enable
#define SFR13_DATA_CRC_ERR_STAT_EN          0x0200000ul
/// Data timeout error status enable
#define SFR13_DATA_TIMEOUT_ERR_STAT_EN      0x0100000ul
/// Command index error status enable
#define SFR13_COMMAND_INDEX_ERR_STAT_EN     0x0080000ul
/// Command end bit error status enable
#define SFR13_COMMAND_END_BIT_ERR_STAT_EN   0x0040000ul
/// Command CRC error status enable
#define SFR13_COMMAND_CRC_ERR_STAT_EN       0x0020000ul
/// Command timeout error status enable
#define SFR13_COMMAND_TIMEOUT_ERR_STAT_EN   0x0010000ul
/// Card interrupt status enable
#define SFR13_CARD_INTERRUPT_STAT_EN        0x0000100ul
/// Card removal status enable
#define SFR13_CARD_REMOVAL_STAT_EN          0x0000080ul
/// Card insertion status enable
#define SFR13_CARD_INERTION_STAT_EN         0x0000040ul
/// Buffer read ready status enable
#define SFR13_BUFFER_READ_READY_STAT_EN     0x0000020ul
/// Buffer write ready status enable
#define SFR13_BUFFER_WRITE_READY_STAT_EN    0x0000010ul
/// DMA interrupt status enable
#define SFR13_DMA_INTERRUPT_STAT_EN         0x0000008ul
/// Block gap event status enable
#define SFR13_BLOCK_GAP_EVENT_STAT_EN       0x0000004ul
/// Transfer complete status enable
#define SFR13_TRANSFER_COMPLETE_STAT_EN     0x0000002ul
/// Command complete status enable
#define SFR13_COMMAND_COMPLETE_STAT_EN      0x0000001ul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Interrupt signal enable register (SFR14) - masks
//-------------------------------------------------------------------------------------------
//{@
/// Auto CMD12 error signal enable
#define SFR14_AUTO_CMD12_ERR_SIG_EN        0x1000000ul
/// Current limit error signal enable
#define SFR14_CURRENT_LIMIT_ERR_SIG_EN     0x0800000ul
/// Data end bit error signal enable
#define SFR14_DATA_END_BIT_ERR_SIG_EN      0x0400000ul
/// Data CRC error signal enable
#define SFR14_DATA_CRC_ERR_SIG_EN          0x0200000ul
/// Data timeout error signal enable
#define SFR14_DATA_TIMEOUT_ERR_SIG_EN      0x0100000ul
/// Command index error signal enable
#define SFR14_COMMAND_INDEX_ERR_SIG_EN     0x0080000ul
/// Command end bit error signal enable
#define SFR14_COMMAND_END_BIT_ERR_SIG_EN   0x0040000ul
/// Command CRC error signal enable
#define SFR14_COMMAND_CRC_ERR_SIG_EN       0x0020000ul
/// Command timeout error signal enable
#define SFR14_COMMAND_TIMEOUT_ERR_SIG_EN   0x0010000ul
/// Card interrupt signal enable
#define SFR14_CARD_INTERRUPT_SIG_EN        0x0000100ul
/// Card removal signal enable
#define SFR14_CARD_REMOVAL_SIG_EN          0x0000080ul
/// Card insertion signal enable
#define SFR14_CARD_INERTION_SIG_EN         0x0000040ul
/// Buffer read ready signal enable
#define SFR14_BUFFER_READ_READY_SIG_EN     0x0000020ul
/// Buffer write ready signal enable
#define SFR14_BUFFER_WRITE_READY_SIG_EN    0x0000010ul
/// DMA interrupt signal enable
#define SFR14_DMA_INTERRUPT_SIG_EN         0x0000008ul
/// Block gap event signal enable
#define SFR14_BLOCK_GAP_EVENT_SIG_EN       0x0000004ul
/// Transfer complete signal enable
#define SFR14_TRANSFER_COMPLETE_SIG_EN     0x0000002ul
/// Command complete signal enable
#define SFR14_COMMAND_COMPLETE_SIG_EN      0x0000001ul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Auto CMD12 error status register (SFR15) - masks
//-------------------------------------------------------------------------------------------
//@{
/// Command not issued bu auto CMD12 error
#define SFR15_CMD_NOT_ISSUED_ERR        0x80ul
/// Auto CMD12 index error
#define SFR15_AUTO_CMD12_INDEX_ERR      0x10ul
/// Auto CMD12 end bit error
#define SFR15_AUTO_CMD12_END_BIT_ERR    0x08ul
/// Auto CMD12 CRC error
#define SFR15_AUTO_CMD12_CRC_ERR        0x04ul
/// Auto CMD12 timeout error
#define SFR15_AUTO_CMD12_TIMEOUT_ERR    0x02ul
/// Autp CMD12 not executed
#define SFR15_AUTO_CMD12_NOT_EXECUTED   0x01ul
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Capabilities register (SFR16) - masks
//-------------------------------------------------------------------------------------------
//@{
/// 64-bit System Bus Support
#define SFR16_VOLTAGE_64BIT_SUPPORT     0x08000000ul
/// Voltage 1.8V is supported
#define SFR16_VOLTAGE_1_8V_SUPPORT      0x04000000ul
/// Voltage 3.0V is supported
#define SFR16_VOLTAGE_3_0V_SUPPORT      0x02000000ul
/// Voltage 3.3V is supported
#define SFR16_VOLTAGE_3_3V_SUPPORT      0x01000000ul
/// Suspend and resume functionality are supported
#define SFR16_RESUME_SUPPORT            0x00800000ul
/// Host controller is capable of using SDMA
#define SFR16_DMA_SUPPORT               0x00400000ul
/// Host controller and the host system support High Speed mode.
#define SFR16_HIGH_SPEED_SUPPORT        0x00200000ul
/// Host controller is capable of using ADMA1
#define SFR16_ADMA1_SUPPORT        		0x00100000ul
/// Host controller is capable of using ADMA2
#define SFR16_ADMA2_SUPPORT        		0x00080000ul
/// 512 is the maximum block size that can be written to the buffer in the Host Controller.
#define SFR16_MAX_BLOCK_LENGTH_512      0x00000000ul
/// 1024 is the maximum block size that can be written to the buffer in the Host Controller.
#define SFR16_MAX_BLOCK_LENGTH_1024     0x00010000ul
/// 2048 is the maximum block size that can be written to the buffer in the Host Controller.
#define SFR16_MAX_BLOCK_LENGTH_2048     0x00020000ul
/// timeout unit clock is MHz
#define SFR16_TIMEOUT_CLOCK_UNIT_MHZ    0x00000100ul
/// this macro can be used to get base clock frequency for SD clock, from a value which was read from the SFR16 register
#define SFR16_GET_BASE_CLK_FREQ_MHZ(VALUE) ( ( VALUE >> 8 ) & 0x3F )
/// this macro can be used to get timeout clock frequency from a value which was read from the SFR16 register
#define SFR16_GET_TIMEOUT_CLK_FREQ(VALUE) ( VALUE & 0x1F )
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Maximum current capabilities register (SFR18) - masks
//-------------------------------------------------------------------------------------------
//@{
//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name ADMA Error Status Register
//-------------------------------------------------------------------------------------------
//@{
/// ADMA Length Mismatch Error
#define SFR21_ADMA_ERROR_LENGTH_MISMATCH	(0x1uL << 2)
/// ADMA machine Stopped
#define SFR21_ADMA_ERROR_STATE_STOP			(0x0uL << 0)
/// ADMA Fetching descriptor
#define SFR21_ADMA_ERROR_STATE_FDS			(0x1uL << 0)
/// ADMA machine Transfer data
#define SFR21_ADMA_ERROR_STATE_TRF			(0x3uL << 0)

//@}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/// @name Host controller version/ slot interrupt status register (SFR63) - masks
//-------------------------------------------------------------------------------------------
//@{
/// Interrupt signal for slot 0
#define SFR63_SLOT0_INTERRUPT       0x0
/// Interrupt signal for slot 1
#define SFR63_SLOT1_INTERRUPT       0x1
/// Interrupt signal for slot 2
#define SFR63_SLOT2_INTERRUPT       0x2
/// Interrupt signal for slot 3
#define SFR63_SLOT3_INTERRUPT       0x3
/// Interrupt signal for slot 4
#define SFR63_SLOT4_INTERRUPT       0x4
/// Interrupt signal for slot 5
#define SFR63_SLOT5_INTERRUPT       0x5
/// Interrupt signal for slot 6
#define SFR63_SLOT6_INTERRUPT       0x6
/// Interrupt signal for slot 7
#define SFR63_SLOT7_INTERRUPT       0x7
//-------------------------------------------------------------------------------------------

/// Maximum size of response in bytes
#define MAX_CARD_RESPONSE_BYTES 120 

//---------------------------------------------------------------------------
/// @name Endian conversions.
//---------------------------------------------------------------------------
//@{
/// Convert 2 bytes data to little endian format
#define CPUToLE16(a)                (a)
/// Convert 4 bytes data to little endian format
#define CPUToLE32(a)                (a)
/// Convert 2 bytes data from little endian format
#define LEToCPU16(a)                (a)
/// Convert 4 bytes data from little endian format
#define LEToCPU32(a)                (a)
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor WakeupCondition
/// @name Wakeup condition definition
//---------------------------------------------------------------------------
//@{
/// Wake up host when card is instert
#define SDIOHOST_WAKEUP_COND_CARD_INS       0x1
/// Wake up host when card is remove
#define SDIOHOST_WAKEUP_COND_CARD_REM       0x2
/// Wake up host when card interrupt occur
#define SDIOHOST_WAKEUP_COND_CARD_INT       0x4
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @name CCCR transfer direction definitions
//---------------------------------------------------------------------------
//@{
/// Read data from CCCR register
#define SDIOHOST_CCCR_READ  0
/// Write data to CCCR register
#define SDIOHOST_CCCR_WRITE 1
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor HostConfigureCmd
/// @name Host configure commands
//---------------------------------------------------------------------------
//@{
/// Set host clock frequency
#define SDIOHOST_CONFIG_SET_CLK             0x1
/// Set host bus width
#define SDIOHOST_CONFIG_SET_BUS_WIDTH       0x2
/// Set timout on DAT line
#define SDIOHOST_CONFIG_SET_DAT_TIMEOUT     0x3
/// Disable signal interrupts
#define SDIOHOST_DISABLE_SIGNAL_INTERRUPT   0x4
/// Restore signal interrupts
#define SDIOHOST_RESTORE_SIGNAL_INTERRUPT   0x5
//@}
//---------------------------------------------------------------------------


#define IS_CARD_WRITE_PROTECT(pSlot)(!(*pSlot->Registers.SFR9 & SFR9_WP_SWITCH_LEVEL))


#endif


