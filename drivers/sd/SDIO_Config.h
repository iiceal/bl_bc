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
/// @file           SDIO_Config.h
/// @brief          SDIO software configuration file.
/// @version        $Revision: 1.1 $
/// @author         Piotr Sroka
/// @date           $Date: 2011/10/18 06:11:13 $
//  *******************************************************************

#ifndef SDIO_CONFIG_H
#define SDIO_CONFIG_H

/// Enable interrupt signal support form SDIO Host driver 
#define SDIO_INTERRUPT_SUPPORT  0

/// SDMA support for SDIO host driver
#define SDIO_SDMA_SUPPORTED     1
/// ADMA1 support for SDIO host driver
#define SDIO_ADMA1_SUPPORTED    0
/// ADMA2 support for SDIO host driver
#define SDIO_ADMA2_SUPPORTED    1

/// size of DSR circual buffer, it shouldn't be greater than 127
#define DSR_COUNT               30
/// Debug level possible values (0, 1, 2, 3)
#define DEBUG_LEVEL             2
/// Choose SD bus mode SD or SPI ( currently only SD bus mode is supported )
#define HOST_BUS_MODE           SDIOHOST_BUS_MODE_SD
/// Set debouncing period
#define DEBOUNCING_TIME         0x300000ul
/// Commands timeout is a iteration count after which timeout error will be report
/// if a command will not execute (mainly using in WaitForValue function)
#define COMMANDS_TIMEOUT        100000ul
/// Enable or disable detecting on the card interrupt
/// You should enable this option only if you have implemented 
/// procedure to hanlde card interrupt
#define ENABLE_CARD_INTERRUPT   0
/// define this definition if you use Tasking compiler. Define it as 0 otherwise.
#define TASKING                 0
// system clock in Hz 
#define SYTEM_CLK_KHZ (8000uL)



/****************************************************************************/
/// DMA support for SDIO host driver
/*#define SDIO_DMA_SUPPORTED      (SDIO_SDMA_SUPPORTED || SDIO_ADMA1_SUPPORTED \
                                    || SDIO_ADMA2_SUPPORTED)*/
/****************************************************************************/



#endif
