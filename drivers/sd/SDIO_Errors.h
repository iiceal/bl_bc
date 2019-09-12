// *******************************************************************
//
//  Copyright (c) 2011  Evatronix SA 
//
// *******************************************************************
//
//  Please review the terms of the license agreement before using     
//  this file. If you are not an authorized user, please destroy this 
//  source code file and notify Evatronix SA immediately that you     
//  inadvertently received an unauthorized copy.                      
//
//  *******************************************************************
/// @file           SDIO_Errors.h
/// @brief          All error definitions.
/// @version        $Revision: 1.1 $
/// @author         Piotr Sroka
/// @date           $Date: 2011/10/18 06:11:13 $
//  *******************************************************************

#ifndef SDIO_ERRORS_H
#define SDIO_ERRORS_H

/// There is no error during execute procedure
#define SDIO_ERR_NO_ERROR                   0x00    
/// Error Host base clock is zero
#define SDIO_ERR_BASE_CLK_IS_ZERO           0x01
/// Error can't power supply
#define SDIO_ERR_NO_POWER_SUPPLY            0x02
/// Error timeout
#define SDIO_ERR_TIMEOUT                    0x03
/// Error during configuration - current limit error
#define SDIO_ERR_CURRENT_LIMIT_ERROR        0x04
/// Error during transfer data - end bit error
#define SDIO_ERR_DATA_END_BIT_ERROR         0x05
/// Error during transfer data - crc bit error
#define SDIO_ERR_DATA_CRC_ERROR             0x06
/// Error during transfer data - timeout error
#define SDIO_ERR_DATA_TIMEOUT_ERROR         0x07
/// Error during execute command - command index error
#define SDIO_ERR_COMMAND_INDEX_ERROR        0x08
/// Error during execute command - command line conflict
#define SDIO_ERR_CMD_LINE_CONFLICT          0x09
/// Error during execute command - timeout error
#define SDIO_ERR_COMMAND_TIMEOUT_ERROR      0x0A
/// Error during execute command - end bit error
#define SDIO_ERR_COMMAND_END_BIT_ERROR      0x0B
/// Error during execute command - CRC error
#define SDIO_ERR_COMMAND_CRC_ERROR          0x0C
/// Card is in inactive state, it is unusable
#define SDIO_ERR_UNUSABLE_CARD              0x0D
/// Wrong argument for SDIO card function
#define SDIO_ERR_PARAM_ERR                  0x0E
/// Wrong SDIO function number error
#define SDIO_ERR_FUN_NUM_ERR                0x0F
/// The CRC check for the previous command failed
#define SDIO_ERR_COM_CRC_ERR                0x10
/// Illegal command error
#define SDIO_ERR_ILLEGAL_CMD                0x11
/// An error in the sequence of erase commands occurred. 
#define SDIO_ERR_ERASE_SEQUENCE_ERR         0x12
/// A general or an unknown error occurred during the operation
#define SDIO_ERR_GENERAL                    0x13
/// A misaligned address that did not match the block length was used in the command
#define SDIO_ERR_ADDRESS_ERR                0x14
/// Can't switch device to high speed mode because it is not supported either by controller or by card
#define SDIO_ERR_HIGH_SPEED_UNSUPP          0x15
/// Command not inssued by auto CMD 12 error 
/// CMD_wo_DAT is not executed due to an Auto CMD12 error (D04-D01)
/// in the Auto CMD12 error status register
#define SIO_ERR_AUTOCMD12_NOT_ISSUED        0x16
/// Command index error occurs in response to a command
#define SIO_ERR_AUTOCMD12_INDEX_ERROR       0x17
/// End bit of command response is 0
#define SIO_ERR_AUTOCMD12_END_BIT_ERROR     0x18
/// Error CRC in the Auto CMD 12 command response
#define SIO_ERR_AUTOCMD12_CRC_ERROR         0x19
/// No response is returned within 64 SDCLK cycles from the end bit of command
/// In this error occur then other error status bits are meaningless
#define SIO_ERR_AUTOCMD12_TIMEOUT           0x1A
/// Host controller cannot issue Auto CMD12 to stop memory multiple 
/// block data transfer due to some error.
#define SIO_ERR_AUTOCMD12_NOT_EXECUTED      0x1B
/// Auto CMD12 error - CMD line conflict
#define SDIO_ERR_AUTOCMD12_LINE_CONFLICT    0x1C
/// Touple not found in CIS register error
#define SDIO_ERR_TUPLE_NOT_FOUND            0x1D
/// Software driver to service inserted card was not implemented
#define SDIO_ERR_DRIVER_NOT_IMPLEMENTED     0x1E
/// Function malloc error
#define SDIO_ERR_MEM_ALLOC                  0x1F               
/// Error DAT line is busy, can't execute command
#define SDIO_ERR_DAT_LINE_BUSY              0x20
/// Error CMD line is busy, can't execute command
#define SDIO_ERR_CMD_LINE_BUSY              0x21
/// Internal card controller error
#define SDIO_ERR_CC_ERROR                   0x22
/// Card internal ECC was applied but failure
#define SDIO_ERR_CARD_ECC                   0x23
/// Sequence or password error has been detected in lock/unlock card command
#define SDIO_ERR_LOCK_UNLOCK_FAILED         0x24
/// Host attempts to write to a protected block or to the temporary or permanent write protected card.
#define SDIO_ERR_WP_VIOLATION               0x25
/// An invalid selection of write-blocks for erase occurred
#define SDIO_ERR_ERASE_PARAM                0x26
/// The transferred block length is not allowed for this card, or the number 
/// of transferred bytes does not match the block length
#define SDIO_ERR_BLOCK_LEN_ERROR            0x27
/// The command’ s argument was out of the allowed range for this card
#define SDIO_ERR_OUT_OF_RANGE               0x28
/// Unrecoverable error ocured
#define SDIO_ERR_NON_RECOVERABLE_ERROR      0x29
/// An error occurred in CMD_wo_DAT, but not in the SD memory transfer.
#define SDIO_ERR_ACMD12_RECOVERABLE_A       0x2A
/// An error occurred in CMD_wo_DAT, and also occurred in the SD memory transfer
#define SDIO_ERR_ACMD12_RECOVERABLE_B       0x2B
/// An error did not occur in CMD_wo_DAT, but an error occurred in the SD memory transfer
#define SDIO_ERR_ACMD12_RECOVERABLE_C       0x2C
/// CMD_wo_DAT was not issued, and an error occurred in the SD memory transfer
#define SDIO_ERR_ACMD12_RECOVERABLE_D       0x2D
/// Card is card locked by the host
#define SDIO_ERR_CARD_IS_LOCKED             0x2E
/// The card could not sustain data transfer in stream read mode
#define SDIO_ERR_UNDERRUN                   0x2F
/// The card could not sustain data transfer in stream write mode
#define SDIO_ERR_OVERRUN                    0x30
/// Problem with erase part of memory because it is protected
#define SDIO_ERR_WP_ERASE_SKIP              0x31
/// Card didn't switch to the expected mode as requested by the SWITCH command
#define SDIO_ERR_SWITCH_ERROR               0x32
/// Error authentication process
#define SDIO_ERR_AKE_SEQ_ERROR              0x33
/// SLOT is busy can't execute a command
#define SDIO_ERR_SLOT_IS_BUSY               0x34
// Error command is not supported by the card device
#define SDIO_ERR_UNSUPPORTED_COMMAND        0x35
/// Device is null pointer
#define SDIO_ERR_DEV_NULL_POINTER           0x36
/// ADMA error
#define SDIO_ERR_ADMA                       0x37
/// unexepted DMA interrupt (probably wrong descriptor settings)
#define SDIO_ERR_DMA_UNEXCEPTED_INTERRUPT   0x38

/// invalid function parameter
#define SDIO_ERR_INVALID_PARAMETER          0x39
/// error trying write or erase locked card.
#define SDIO_ERR_CARD_WRITE_PROTECTED       0x3A
/// error function is not supported (CMD6 command)
#define SDIO_ERR_FUNCTION_UNSUPP            0x3B

#define SDIO_ERR_TUNING                     0x3C

/// Request is currently executing
#define SDIO_STATUS_PENDING                 0xFF
/// Request was aborted
#define SDIO_STATUS_ABORTED                 0xFE
/// Undefined error
#define SDIO_ERR_UNDEFINED                  0xEF
/// Hardware problem error
#define SDIO_ERR_HARDWARE_PROBLEM           0xEE
/// Driver procedure was called with wrong value of parameter
#define SDIO_ERR_WRONG_VALUE                0xED
// Cant execute function
#define SDIO_ERR_CANT_EXECUTE               0xEC




extern unsigned char **Errors;



#endif

