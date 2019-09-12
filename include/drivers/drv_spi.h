#ifndef __DRV_SPI_H_
#define __DRV_SPI_H_

void* spi_init_bus(int index, int cs);
int spi_write_data(void* handler, u8* txbuf, u32 txlen);
int spi_read_data(void *handler, u8* rxbuf, u32 rxlen);
int spi_eeprom_data(void *handler, u8* txbuf, u32 txlen, u8* rxbuf, u32 rxlen);

#endif /*__DRV_SPI_H_*/
