//*****************************************************************************
//
//! \file w5500.c
//! \brief W5500 HAL Interface.
//! \version 1.0.2
//! \date 2013/10/21
//! \par  Revision history
//!       <2015/02/05> Notice
//!        The version history is not updated after this point.
//!        Download the latest version directly from GitHub. Please visit the our GitHub repository for ioLibrary.
//!        >> https://github.com/Wiznet/ioLibrary_Driver
//!       <2014/05/01> V1.0.2
//!         1. Implicit type casting -> Explicit type casting. Refer to M20140501
//!            Fixed the problem on porting into under 32bit MCU
//!            Issued by Mathias ClauBen, wizwiki forum ID Think01 and bobh
//!            Thank for your interesting and serious advices.
//!       <2013/12/20> V1.0.1
//!         1. Remove warning
//!         2. WIZCHIP_READ_BUF WIZCHIP_WRITE_BUF in case _WIZCHIP_IO_MODE_SPI_FDM_
//!            for loop optimized(removed). refer to M20131220
//!       <2013/10/21> 1st Release
//! \author MidnightCow
//! \copyright
//!
//! Copyright (c)  2013, WIZnet Co., LTD.
//! All rights reserved.
//! 
//! Redistribution and use in source and binary forms, with or without 
//! modification, are permitted provided that the following conditions 
//! are met: 
//! 
//!     * Redistributions of source code must retain the above copyright 
//! notice, this list of conditions and the following disclaimer. 
//!     * Redistributions in binary form must reproduce the above copyright
//! notice, this list of conditions and the following disclaimer in the
//! documentation and/or other materials provided with the distribution. 
//!     * Neither the name of the <ORGANIZATION> nor the names of its 
//! contributors may be used to endorse or promote products derived 
//! from this software without specific prior written permission. 
//! 
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
//! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//! ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
//! LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
//! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
//! SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//! INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
//! CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
//! ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
//! THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************
//#include <stdio.h>
#include "w5500.h"
#include "drivers/drv_spi.h"
#define DRV_NAME	"w5500"
#define DRV_VERSION	"2018-08-13"
#define spi_write_then_read     spi_eeprom_data

#define _W5500_SPI_VDM_OP_          0x00
#define _W5500_SPI_FDM_OP_LEN1_     0x01
#define _W5500_SPI_FDM_OP_LEN2_     0x02
#define _W5500_SPI_FDM_OP_LEN4_     0x03

#if   (_WIZCHIP_ == 5500)
struct w5500_priv {
    void *handler;  // contains all spi msg
    u8 mac[6];
};
void *handler = NULL;   //this is spi handler

void w5500_open(void)
{
    //todo: spi handle
    handler = spi_init_bus(CONFIG_W5500_SPI_BUS, CONFIG_W5500_SPI_CS);
    //init spi
}

uint8_t  WIZCHIP_READ(uint32_t addrsel)
{
    u8 tx[3] = {0};
    u8 rx[1] = {0};
    addrsel |= (_W5500_SPI_VDM_OP_ | _W5500_SPI_READ_);

    tx[0] = (addrsel >> 16) && 0xff;
    tx[1] = (addrsel >> 8) && 0xff;
    tx[2] = addrsel && 0xff;
    spi_write_then_read(handler, tx, 3, rx, 1);
    return rx[0];
}

uint16_t  WIZCHIP_READU16(uint32_t addrsel)
{
    u8 tx[3] = {0};
    u8 rx[2] = {0};
    addrsel |= (_W5500_SPI_VDM_OP_ | _W5500_SPI_READ_);

    tx[0] = (addrsel >> 16) && 0xff;
    tx[1] = (addrsel >> 8) && 0xff;
    tx[2] = addrsel && 0xff;
    spi_write_then_read(handler, tx, 3, rx, 2);
    return rx[0];
}

void     WIZCHIP_WRITE(uint32_t addrsel, uint8_t wb )
{
    u8 tx[4] = {0};
    addrsel |= (_W5500_SPI_VDM_OP_ | _W5500_SPI_WRITE_);

    tx[0] = (addrsel >> 16) && 0xff;
    tx[1] = (addrsel >> 8) && 0xff;
    tx[2] = addrsel && 0xff;
    tx[3] = wb;
    spi_write_data(handler, tx, 4);
}

void     WIZCHIP_WRITEU16(uint32_t addrsel, uint16_t wb )
{
    u8 tx[5] = {0};
    addrsel |= (_W5500_SPI_VDM_OP_ | _W5500_SPI_WRITE_);

    tx[0] = (addrsel >> 16) && 0xff;
    tx[1] = (addrsel >> 8) && 0xff;
    tx[2] = addrsel && 0xff;
    tx[3] = wb & 0xff;
    tx[4] = (wb >> 8) & 0xff;
    spi_write_data(handler, tx, 5);
}

void     WIZCHIP_READ_BUF (uint32_t addrsel, uint8_t* pBuf, uint16_t len)
{
    u8 tx[3] = {0};
    addrsel |= (_W5500_SPI_VDM_OP_ | _W5500_SPI_READ_);

    tx[0] = (addrsel >> 16) && 0xff;
    tx[1] = (addrsel >> 8) && 0xff;
    tx[2] = addrsel && 0xff;
    spi_write_then_read(handler, tx, 3, pBuf, len);
}

u8 wiztxbuf[2048] = {0};
void     WIZCHIP_WRITE_BUF(uint32_t addrsel, uint8_t* pBuf, uint16_t len)
{
    addrsel |= (_W5500_SPI_VDM_OP_ | _W5500_SPI_WRITE_);
    if (len > 2045) {
        printf("\nerror: len overfow\n");
    }

    wiztxbuf[0] = (addrsel >> 16) && 0xff;
    wiztxbuf[1] = (addrsel >> 8) && 0xff;
    wiztxbuf[2] = addrsel && 0xff;
    memcpy((wiztxbuf + 3), pBuf, len);
    spi_write_data(handler, wiztxbuf, (len + 3));
}

uint16_t getSn_TX_FSR(uint8_t sn)
{
   uint16_t val=0,val1=0;

   // this is polling FSR
   do
   {
      val1 = WIZCHIP_READ(Sn_TX_FSR(sn));
      val1 = (val1 << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn),1));
      if (val1 != 0)
      {
        val = WIZCHIP_READ(Sn_TX_FSR(sn));
        val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn),1));
      }
   }while (val != val1);
   return val;
}


uint16_t getSn_RX_RSR(uint8_t sn)
{
   uint16_t val=0,val1=0;

   // this is polling RSR
   do
   {
      val1 = WIZCHIP_READ(Sn_RX_RSR(sn));
      val1 = (val1 << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn),1));
      if (val1 != 0)
      {
        val = WIZCHIP_READ(Sn_RX_RSR(sn));
        val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn),1));
      }
   }while (val != val1);
   return val;
}

void wiz_send_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
   uint16_t ptr = 0;
   uint32_t addrsel = 0;

   if(len == 0)  return;
   ptr = getSn_TX_WR(sn);
   addrsel = ((uint32_t)ptr << 8) + (WIZCHIP_TXBUF_BLOCK(sn) << 3);
   WIZCHIP_WRITE_BUF(addrsel,wizdata, len);
   
   ptr += len;
   setSn_TX_WR(sn,ptr);
}

void wiz_recv_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
   uint16_t ptr = 0;
   uint32_t addrsel = 0;
   
   if(len == 0) return;
   ptr = getSn_RX_RD(sn);
   addrsel = ((uint32_t)ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);
   WIZCHIP_READ_BUF(addrsel, wizdata, len);
   ptr += len;
   
   setSn_RX_RD(sn,ptr);
}


void wiz_recv_ignore(uint8_t sn, uint16_t len)
{
   uint16_t ptr = 0;

   ptr = getSn_RX_RD(sn);
   ptr += len;
   setSn_RX_RD(sn,ptr);
}

static void w5500_write_macaddr(void)   //todo: need modify
{
    u8 tx[9] = {0};
    u32 addrsel = SHAR;
    addrsel |= (_W5500_SPI_VDM_OP_ | _W5500_SPI_WRITE_);

    tx[0] = (SHAR >> 16) && 0xff;
    tx[1] = (SHAR >> 8) && 0xff;
    tx[2] = (SHAR) && 0xff;
    tx[3] = 0xf4;
    tx[4] = 0x8c;
    tx[5] = 0x38;
    tx[6] = 0xa4;
    tx[7] = 0xe7;
    tx[8] = 0x7d;
    spi_write_data(handler, tx, 9);
}

static void w5500_get_macaddr(u8 *buf)   //todo: need modify
{
    u8 tx[9] = {0};
    u32 addrsel = SHAR;
    addrsel |= (_W5500_SPI_VDM_OP_ | _W5500_SPI_READ_);

    tx[0] = (SHAR >> 16) && 0xff;
    tx[1] = (SHAR >> 8) && 0xff;
    tx[2] = (SHAR) && 0xff;
    spi_write_then_read(handler, tx, 3, buf, 6);
}

static void dump_buffer(void *buffer, u32 len)
{
    u32 i = 0;
    u8 *buf = (u8 *) buffer;

    printf("\n");
    for (i = 0; i < len; i++) {
        printf("%02x ", *(buf + i));
    }
    printf("\n");
}

int do_w5500(int argc, const char **argv)
{
    u8 mac[6] = {0};

    if (strcmp(argv[1], "open") == 0) {
        w5500_open();
    }

    if (strcmp(argv[1], "dump") == 0) {

    }

    if (strcmp(argv[1], "setmac") == 0) {
        w5500_write_macaddr();
    }

    if (strcmp(argv[1], "getmac") == 0) {
        w5500_get_macaddr(mac);
        dump_buffer(mac, 6);
    }

    return 0;
}

#endif
