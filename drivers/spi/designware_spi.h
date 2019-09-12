#ifndef __DESIGNWARE_SPI_H_
#define __DESIGNWARE_SPI_H_

struct dw_spi_priv {
    void volatile *regs;    // base
    unsigned int freq;		/* Default frequency */
    unsigned int mode;
    u32 clk;
    unsigned long bus_clk_rate;

    u32 cs_gpio;	/* External chip-select gpio */

    int bits_per_word;
    u8 cs;			/* chip select pin */
    u8 tmode;		/* TR/TO/RO/EEPROM */
    u8 type;		/* SPI/SSP/MicroWire */
    int ndf;

    u32 fifo_len;		/* depth of the FIFO buffer */
    void *tx;
    void *tx_end;
    u32 txlen;
    void *rx;
    void *rx_end;
    u32 rxlen;
};

#endif /* __DESIGNWARE_SPI_H_ */
