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
/// @file           SDIO_Types.h
/// @brief          Types definitions. 
/// @version        $Revision: 1.1 $
/// @author         Piotr Sroka
/// @date           $Date: 2011/10/18 06:11:13 $
//  *******************************************************************


#ifndef TYPES_H
#define TYPES_H
//#include <limits.h>

//---------------------------------------------------------------------------
/// @name Definitions of the mutex type and functions
//---------------------------------------------------------------------------
#define MUTEX char
#define MUTEX_LOCK(MUTEX_VAR)
#define MUTEX_UNLOCK(MUTEX_VAR)
#define MUTEX_INIT(MUTEX_VAR)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor Commands
/// @name Definitions of the command types
//---------------------------------------------------------------------------
//@{
/// Ordinary type of command ( none of below three )
#define SDIOHOST_CMD_TYPE_OTHER     0x00
/// Abort command
#define SDIOHOST_CMD_TYPE_ABORT     0x03
/// Suspend command
#define SDIOHOST_CMD_TYPE_SUSPEND   0x01
/// Resume command
#define SDIOHOST_CMD_TYPE_RESUME    0x02
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor Responses
/// @name Definitions of the command responses
//---------------------------------------------------------------------------
//@{
/// No response
#define SDIOHOST_RESPONSE_NO_RESP   0x0
/// Response R1
#define SDIOHOST_RESPONSE_R1        0x1
/// Response R1 with busy check
#define SDIOHOST_RESPONSE_R1B       0x2
/// Response R2
#define SDIOHOST_RESPONSE_R2        0x3
/// Response R3
#define SDIOHOST_RESPONSE_R3        0x4
/// Response R4
#define SDIOHOST_RESPONSE_R4        0x5
/// Response R5
#define SDIOHOST_RESPONSE_R5        0x6
/// Response R5 with busy check
#define SDIOHOST_RESPONSE_R5B       0x7
/// Response R6
#define SDIOHOST_RESPONSE_R6        0x8
/// Response R7
#define SDIOHOST_RESPONSE_R7        0x9
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor Devices
/// @name Device types definitions
//---------------------------------------------------------------------------
//@{
/// SDIO card 
#define SDIOHOST_CARD_TYPE_SDIO     0x1
/// SD memory only card 
#define SDIOHOST_CARD_TYPE_SDMEM    0x2
/// SD combo card ( SDIOHOST_CARD_TYPE_SDIO | SDIOHOST_CARD_TYPE_SDMEM )
#define SDIOHOST_CARD_TYPE_COMBO    0x3
/// MMC memory card
#define SDIOHOST_CARD_TYPE_MMC      0x4
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor Capacities
/// @name Device capacity definitions
//---------------------------------------------------------------------------
//@{
/// Standard capacity card
#define SDIOHOST_CAPACITY_NORMAL    0x1
/// Height capacity card
#define SDIOHOST_CAPACITY_HIGH      0x2
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor BusWidths
/// @name Bus width definitions
//---------------------------------------------------------------------------
//@{
/// SD bus with 1 bit bus width
#define SDIOHOST_BUS_WIDTH_1        0x1
/// SD bus with 4 bit bus width
#define SDIOHOST_BUS_WIDTH_4        0x4
/// SD bus with 8 bit bus width
#define SDIOHOST_BUS_WIDTH_8        0x8
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor BusModes
/// @name Host bus mode definitions
//---------------------------------------------------------------------------
//@{
/// SPI bus mode
#define SDIOHOST_BUS_MODE_SPI       0x1
/// SD bus mode
#define SDIOHOST_BUS_MODE_SD        0x2
//@}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @anchor TransferDirections
/// @name Request flags definitions
//---------------------------------------------------------------------------
//@{
/// Read operation flag
#define SDIOHOST_FLAGS_READ     0x1
/// Write operation flag
#define SDIOHOST_FLAGS_WRITE    0x0
//@}
//---------------------------------------------------------------------------



/// type which size is one byte
typedef unsigned char BYTE;

/// type which size is two bytes
typedef unsigned short WORD;

/// type which size is four bytes
typedef unsigned long DWORD;

struct SDIO_DEVICE;
struct SDIO_SLOT;

/**
 * @struct REQUEST_FLAGS
 * @brief Structure describes a parameters of request
 */
typedef struct REQUEST_FLAGS {
    /// Type of command. Types of command types are define here @ref Commands
    unsigned CommandType : 2;
    /// This field specifies if request will transfer the data. 1 - data is present, 0 - no data
    unsigned DataPresent : 1;
    /// Response type. Type of response types are defined here @ref Responses
    unsigned ResponseType : 4;
    /// This variable specifies the data direction transfer. 1 - read, 0 - write
    unsigned DataTransferDirection : 1; 
    /// 1 - enable Auto CMD12, 0 - dissable Auto CMD12
    unsigned AutoCMD12Enable : 1; 
} REQUEST_FLAGS;


/**
 * @struct REQUEST
 * @brief Structure describes a request which should be executed by host.
 *
 * @typedef PREQUEST
 * @brief Pointer to REQUEST structure
 */
typedef struct REQUEST {
    /// Argument of command to execute
    DWORD Argument;           
    /// Request flags. They are defined above
    REQUEST_FLAGS RequestFlags;      
    /// Command to execute
    BYTE Command;            
    /// Buffer for command response
    DWORD Response[4];                      
    /// Number of blocks to send or receive
    WORD BlockCount;                        
    /// Length of block to send or receive
    WORD BlockLen;                        
    /// Starting address of buffer
    void *pDataBuffer;            
    /// Current buffer possition
    void *pBufferPos;
    /// Number of bytes remainding in the transfer
    DWORD DataRemaining;                    
    /// Status of request
    BYTE Status;
    ///
    DWORD *ADMADescriptorTable;
} REQUEST, *PREQUEST;

/**
 * @struct STANDARD_REGISTERS
 * @brief Structure contains a list of pointers to all standadard SDIO registers.
 */
typedef struct STANDARD_REGISTERS {
    /// Pointer to system addres register
    volatile DWORD *SFR0;
    /// Pointer to block count and size register
    volatile DWORD *SFR1;
    /// Pointer to argument register
    volatile DWORD *SFR2;
    /// Pointer to transfer mode and command information register
    volatile DWORD *SFR3;
    /// Pointer to response #0 register
    volatile DWORD *SFR4;
    /// Pointer to response #1 register
    volatile DWORD *SFR5;
    /// Pointer to response #2 register
    volatile DWORD *SFR6;
    /// Pointer to response #3 register
    volatile DWORD *SFR7;
    /// Pointer to buffer data port register
    volatile DWORD *SFR8;
    /// Pointer to preset state register
    volatile DWORD *SFR9;
    /// Pointer to host control settings #0 register
    volatile DWORD *SFR10;
    /// Pointer to host control settings #1 register
    volatile DWORD *SFR11;
    /// Pointer to interrupt status register
    volatile DWORD *SFR12;
    /// Pointer to interrupt status enable register
    volatile DWORD *SFR13;
    /// Pointer to interrupt signal enable register
    volatile DWORD *SFR14;
    /// Pointer to Auto CMD12 error status register
    volatile DWORD *SFR15;
    /// Pointer to capabilities register 
    volatile DWORD *SFR16;
    /// Pointer to maximum current capabilities
    volatile DWORD *SFR18;

    volatile DWORD *SFR20;

    volatile DWORD *SFR21;

    volatile DWORD *SFR22;


} STANDARD_REGISTERS;


/** 
 * @struct SDIO_DEVICE
 * @brief Structure contains information about inserted card and functions to handle them.
 *
 * @typedef PSDIO_DEVICE
 * @brief Pointer to SDIO_DEVICE structure
 */
typedef struct SDIO_DEVICE {
    /// Type of device. Types of devices are defined here @ref Devices 
    BYTE DeviceType;
    /// Card capacity information. Types of device capacities are define here @ref Capacities
    BYTE DeviceCapacity;
    /// Relative card address 
    WORD RCA;
    /// Specification Version Number
    BYTE SpecVersNumb;
    /// Card device supported bus width. They are defined here @ref BusWidths. This parameter is logic sum of all supported bus widths 
    BYTE SupportedBusWidths;
    /// Addres of card’s common Card Information Structure (CIS)
    DWORD CommonCISAddress;
    /// This variable defines if the card is selected or unselected.
    BYTE IsSelected;
    /// Function pointer which should point to interrupt function which hanle card interrupt.
    BYTE (*pCardInterruptHandler)( struct SDIO_DEVICE *pDevice );
    /// Function pointer which should point to  function which deinitialize the card device
    BYTE (*pCardDeinitialize)( struct SDIO_DEVICE *pDevice);
    /// Private driver data
    void *CardDriverData;
    /// Slot in which card is inserted
    struct SDIO_SLOT *pSlot;    
} SDIO_DEVICE, *PSDIO_DEVICE;



/** 
 * @struct SDIO_SLOT
 * @brief Structure contains information a SDIO Host slot.
 *
 * @typedef PSDIO_SLOT
 * @brief Pointer to SDIO_SLOT structure
 */
typedef struct SDIO_SLOT {
    /// All standadard SDIO registers.
    STANDARD_REGISTERS Registers;
    /// Size of buffer in the system memory which is used by DMA module.
    DWORD DMABufferBoundary;
    /// Pointer to structure which decribe inserted card. 
    PSDIO_DEVICE pDevice;
    /// Current set bus width. The bus widths are defined here \ref BusWidths 
    BYTE BusWidth;
    /// Pointer to current executing request. 
    PREQUEST pCurrentRequest;
    /// 1 - card is inserted, 0 - there is no card in slot
    BYTE CardInserted;
    /// Abort current transaction.
    BYTE AbortRequest;
    /// Settings of signaling the interrupts
    DWORD IntSettings;
	DWORD MMC8SwitchMask;  
    BYTE ErrorRecorvering;  
} SDIO_SLOT, *PSDIO_SLOT;

/** 
 * @struct CIDREGISTER
 * @brief Structure defines CID register fields
 *
 * @typedef PCIDREGISTER
 * @brief Pointer to CIDREGISTER structure
 */
typedef struct CIDREGISTER {
    /// An 8-bit binary number that identifies the card manufacturer.
    BYTE ManufacturerID;
    /// A 2-character ASCII string that identifies the card OEM and/or the card contents
    WORD OEMApplicationID;
    /// Product name is a string, 5-character ASCII string
    BYTE ProductName[5];
    /// The product revision is composed of two Binary Coded Decimal (BCD) digits
    BYTE ProductRevision;
    /// Product serial number
    DWORD ProductSN;
    ///The manufacturing date is composed of two hexadecimal digits, 
    /// one is 8 bits representing the year(y) and the other is 4 bits representing the month (m).
    WORD ManufaturingDate;
}CIDREGISTER, *PCIDREGISTER;



/** 
 * @struct SDIO_DEVICE_INFO
 * @brief Structure contains information that will be used
 *          to recognize type of supported devices
 *
 * @typedef PSDIO_DEVICE_INFO
 * @brief Pointer to SDIO_DEVICE_INFO structure
 */
typedef struct SDIO_DEVICE_INFO {
    /// Manufacturer code read from SDIO card CIS register
    WORD ManufacturerCode;
    /// Manufacturer information(Part Number and/or Revision) read from SDIO card CIS register
    WORD ManufacturerInformation;
    /// Type of device (SDIO card, SD/MMC memory card or combo card)
    BYTE DeviceType;
    /// Function pointer which should point to interrupt function which hanle SDIO card interrupt
    BYTE (*pCardInterruptHandler)( PSDIO_DEVICE pDevice );
    /// Function pointer which should point to  function which initialize the card device
    BYTE (*pCardInitialize)( PSDIO_DEVICE pDevice );
    /// Function pointer which should point to  function which deinitialize the card device
    BYTE (*pCardDeinitialize)( PSDIO_DEVICE pDevice );
} SDIO_DEVICE_INFO, *PSDIO_DEVICE_INFO;


/** 
 * @struct LIST_NODE
 * @brief Structure defines a list node
 */
typedef struct LIST_NODE {
    void *Item;
    struct LIST_NODE *Next;
} LIST_NODE;

/** 
 * @struct LIST_NODE
 * @brief Structure defines a object of interrupt. 
 * It contains all need informations to handle occurred interrupt.
 */
typedef struct SDIO_DSR {
    /// pointer to slot on which interrupt occurs
    PSDIO_SLOT pSlot;
    /// value of interrupt status register after interrupt occurence
    DWORD SlotStatus;
} SDIO_DSR, *PSDIO_DSR;

#endif // SDIO_TYPES_H
