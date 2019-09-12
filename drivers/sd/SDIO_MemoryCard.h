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
/// @file           SDIO_MemoryCard.h
/// @brief          Header file of memory card driver 
/// @version        $Revision: 1.1 $
/// @author         Piotr Sroka
/// @date           $Date: 2011/10/18 06:11:13 $
//  *******************************************************************

#ifndef SD_MMC_CARD_H
#define SD_MMC_CARD_H

#include "SDIO_Types.h"

//-------------------------------------------------------------------------------------------
/// @anchor CardConfigureCommands
/// @name Card configuration commands   
//-------------------------------------------------------------------------------------------
//@{
/// Lock card command
#define SD_MMC_CARD_CONF_CARD_LOCK          0x4
/// Unlock card command
#define SD_MMC_CARD_CONF_CARD_UNLOCK        0x0
/// Set password to the card
#define SD_MMC_CARD_CONF_SET_PASSWORD       0x1
/// Reset password to the card
#define SD_MMC_CARD_CONF_RESET_PASSWORD     0x2
//@}
//-------------------------------------------------------------------------------------------


typedef struct MEMORY_CARD_INFO{
    /// Block size
    WORD BlockSize;
    /// Card command classes
    WORD CommandClasses;
    /// Device size
    DWORD DeviceSizeMB;
    /// Defines whether partial block sizes can be used in block read commands.
    BYTE PartialReadAllowed;
    /// Defines whether partial block sizes can be used in block write commands.
    BYTE PartialWriteAllowed;
    /// Defines if the data block to be written by one command can be 
    /// spread over more than one physical block of the memory device.    
    BYTE WriteBlkMisalign;
    /// Defines if the data block to be read by one command can be 
    /// spread over more than one physical block of the memory device.    
    BYTE ReadBlkMisalign;
    /// The size of an erasable sector. 
    /// The content of this register is a 7-bit binary coded value, defining the 
    /// number of write blocks (see WRITE_BL_LEN
    WORD SectorSize;
    /// The EraseBlkEn defines the granularity of the unit size of the data to be erased.
    BYTE EraseBlkEn;
    /// The maximum read data block length.
    BYTE ReadBlLen;
    /// The maximum write data block length.
    BYTE WriteBlLen;
} MEMORY_CARD_INFO, *PMEMORY_CARD_INFO;


/***************************************************************/
/*!  							   
 * @fn       void MemoryCard_LoadDriver(void)
 * @brief 	Function loads memory card driver to host drivers list
*/
/***************************************************************/
void MemoryCard_LoadDriver(void);


/***************************************************************/
/*!  							   
 * @fn      BYTE MemoryCard_DataTransfer( PSDIO_DEVICE pDevice, 
 *                                        DWORD Address, 
 *                                        BYTE *Buffer, 
 *                                        DWORD BufferSize, 
 *                                        BYTE TransferDirection );
 * @brief   Function transfers data to/from memory card. 
 *              Function operates on 512 data blocks
 * @param   pDevice Device card to which data shall be send
 * @param   Address Addres in 512 bytes blocks on memory card where data 
 *              shall be transfer to/from. 
 * @param   Buffer Buffer with data write/read
 * @param   BufferSize Size of Buffer in bytes. 
 *              Buffer size should be divisible by 512     
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions 
 *              to find something out
 * @return	Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
#if 0
BYTE MemoryCard_DataTransfer( PSDIO_DEVICE pDevice, 
                              DWORD Address, 
                              BYTE *Buffer, 
                              DWORD BufferSize, 
                              BYTE TransferDirection );
#endif
/***************************************************************/
/*!  							   
 * @fn      BYTE MemoryCard_Configure ( PSDIO_DEVICE pDevice, BYTE Cmd, 
 *                                      void *Data, BYTE SizeOfData );
 * @brief 	Function configures card
 * @param   pDevice Device card which shall be configure
 * @param   Cmd This command defines what operation will be execute.
 *              All commands are defined here @ref CardConfigureCommands  
 * @param   Data If some data are need to execute Cmd command then they should 
 *              be placed in Data buffer
 * @param   SizeOfData Size of Data in bytes     
 * @return	Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
BYTE MemoryCard_Configure ( PSDIO_DEVICE pDevice, BYTE Cmd, 
                            void *Data, BYTE SizeOfData );



/***************************************************************/
/*!  							   
 * @fn      static BYTE MemoryCard_GetCSD( PSDIO_DEVICE pDevice );
 * @brief 	Function extracts some information from CSD register
                and writes them to pDevice variable.
 * @param   pDevice Device on which operation shall be executed
 * @return	Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
//static BYTE MemoryCard_GetCSD( PSDIO_DEVICE pDevice )


/***************************************************************/
/*!  							   
 * @fn      BYTE MemoryCard_DataErase(PSDIO_DEVICE pDevice, 
 *                        DWORD StartBlockAddress, 
 *                        DWORD BlockCount)
 * @brief   Function erases specyfied blocks
 * @param   pDevice Device card which shall be configure
 * @param   StartBlockAddress  
 * @param   BlockCount   
 * @return  Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
BYTE MemoryCard_DataErase(PSDIO_DEVICE pDevice, 
                          DWORD StartBlockAddress, 
                          DWORD BlockCount);


/***************************************************************/
/*!  							   
 * @fn      BYTE MemoryCard_PartialDataTransfer(PSDIO_DEVICE pDevice, 
                                    DWORD Address, 
                                    BYTE *Buffer,
                                    DWORD BufferSize, 
                                    BYTE TransferDirection);
 * @brief   Function transfers data to/from memory card. 
 *              Function operates on bytes 
 * @param   pDevice Device card to which data shall be send
 * @param   Address Addres in bytes on memory card where data 
 *              shall be transfer to/from.
 * @param   Buffer Buffer with data write/read
 * @param   BufferSize Size of Buffer in bytes. 
 *              Buffer size should be divisible by 512     
 * @param   TransferDirection This parameter defines transfer direction
 *              please see the @ref TransferDirections definitions 
 *              to find something out
 * @return	Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
BYTE MemoryCard_PartialDataTransfer(PSDIO_DEVICE pDevice, 
                                    DWORD Address, 
                                    BYTE *Buffer,
                                    DWORD BufferSize, 
                                    BYTE TransferDirection);

#endif

