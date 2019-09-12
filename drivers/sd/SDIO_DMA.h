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
/// @file           SDIO_DMA.h
/// @brief          SDIO DMA header file
/// @version        $Revision: 1.1 $
/// @author         Piotr Sroka
/// @date           $Date: 2011/10/18 06:11:13 $
//  *******************************************************************


#ifndef SDIO_DMA_H
#define SDIO_DMA_H

#include "SDIO_Types.h"

#define ADMA1_SIZE_OF_DESCRIPTOR 32u
#define ADMA2_SIZE_OF_DESCRIPTOR 64u

#define SDMA_MODE       0x00
#define ADMA1_MODE      0x01
#define ADMA2_MODE      0x02
#define NONEDMA_MODE    0xFF

/** ADMA1 Descriptor Fields */

/// Set address/lenght field
#define ADMA1_DESCRIPTOR_ADDRESS_LENGHT(Val) (Val << 12)
/// No operation – go to next descriptor on the list.
#define ADMA1_DESCRIPTOR_TYPE_NOP   (0x0 << 4)
/// Set data page length and go to next descriptor on the list.
#define ADMA1_DESCRIPTOR_TYPE_SET   (0x1 << 4)
/// Transfer data from the pointed page and go to next descriptor on the list.
#define ADMA1_DESCRIPTOR_TYPE_TRAN  (0x2 << 4)
/// Go to the next descriptor list
#define ADMA1_DESCRIPTOR_TYPE_LINK  (0x3 << 4)
/// the ADMA interrupt is generated 
/// when the ADMA1 engine finishes processing the descriptor.
#define ADMA1_DESCRIPTOR_INT        (0x1 << 2)
/// it signals termination of the transfer
/// and generates Transfer Complete Interrupt 
/// when this transfer is completed
#define ADMA1_DESCRIPTOR_END        (0x1 << 1)
/// it indicates the valid descriptor on a list
#define ADMA1_DESCRIPTOR_VAL        (0x1 << 0)

/// Set address/lenght field
#define ADMA2_DESCRIPTOR_LENGHT(Val) ((DWORD)Val << 16)
/// No operation – go to next descriptor on the list.
#define ADMA2_DESCRIPTOR_TYPE_NOP   (0x0 << 4)
/// Transfer data from the pointed page and go to next descriptor on the list.
#define ADMA2_DESCRIPTOR_TYPE_TRAN  (0x2 << 4)
/// Go to the next descriptor list
#define ADMA2_DESCRIPTOR_TYPE_LINK  (0x3 << 4)
/// the ADMA interrupt is generated 
/// when the ADMA1 engine finishes processing the descriptor.
#define ADMA2_DESCRIPTOR_INT        (0x1 << 2)
/// it signals termination of the transfer
/// and generates Transfer Complete Interrupt 
/// when this transfer is completed
#define ADMA2_DESCRIPTOR_END        (0x1 << 1)
/// it indicates the valid descriptor on a list
#define ADMA2_DESCRIPTOR_VAL        (0x1 << 0)


/***************************************************************/
/*!  							   
 * @fn      BYTE DMA_PrepareTransfer(PSDIO_SLOT pSlot, 
 *									 PREQUEST pRequest)
 * @brief 	Function prepare DMA module to data transfer
 * @param	pSlot Slot on which the DMA transfer shall be execute
 * @param   pRequest it describes request to execute
 * @return	Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
BYTE DMA_PrepareTransfer(PSDIO_SLOT pSlot, PREQUEST pRequest);

/***************************************************************/
/*!  							   
 * @fn      BYTE DMA_PrepareTransfer(PSDIO_SLOT pSlot, 
 *									 PREQUEST pRequest)
 * @brief 	Function handles DMA interrupt
 * @param	pSlot Slot on which DMA interrupt occured
 * @param   pRequest it describes request which 
 *				is currently executed
 * @return	Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
BYTE DMA_HandleInterrupt(PSDIO_SLOT pSlot, PREQUEST pRequest, DWORD Status); 

/***************************************************************/
/*!  							   
 * @fn      BYTE DMA_FinishTransfer(PSDIO_SLOT pSlot, 
 *						     		PREQUEST pRequest)
 * @brief 	Function finishes DMA transfer
 * @param	pSlot Slot on which the DMA transfer is executing
 * @param   pRequest it describes executing request
 * @return	Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
BYTE DMA_FinishTransfer(PSDIO_SLOT pSlot, PREQUEST pRequest);

/***************************************************************/
/*!  							   
 * @fn      BYTE DMA_FinishTransfer(PSDIO_SLOT pSlot, 
 *						     		PREQUEST pRequest)
 * @brief 	Function selects optimal DMA transmission mode
 * @param	pSlot Slot on which the DMA transfer shall be execute
 * @param   pRequest it describes request to execute
 * @return	Function returns 0 if everything is ok 
 *              otherwise returns error number   
*/
/***************************************************************/
BYTE DMA_SpecifyTransmissionMode(PSDIO_SLOT pSlot, PREQUEST pRequest);

#endif
