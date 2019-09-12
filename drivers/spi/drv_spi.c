#include <common.h>
#include "designware_spi.h"

extern void* get_priv(int index);
extern int dw_spi_xfer(struct dw_spi_priv *priv, unsigned int bitlen, const void *dout, void *din, unsigned long flags);
extern int dw_spi_set_speed(struct dw_spi_priv *priv, uint speed);
extern void spi_hw_init(struct dw_spi_priv *priv);

void* spi_init_bus(int index, int cs)
{
    struct dw_spi_priv *priv;
    priv = get_priv(index);
    priv->cs = cs;
    priv->mode = 0;
    priv->bits_per_word = 8;
    priv->type = 0;
    priv->mode = 3;
    priv->tmode = 0;
    priv->bus_clk_rate = 95000000;
    dw_spi_set_speed(priv, 40000000);
    spi_hw_init(priv);
    return priv;
}

int spi_write_data(void* handler, u8* txbuf, u32 txlen)
{
    struct dw_spi_priv *priv = (struct dw_spi_priv *) handler;
    priv->txlen = txlen;
    priv->rxlen = 0;
    dw_spi_xfer((struct dw_spi_priv *)priv, 8, txbuf, NULL, 0);
    return 0;
}

int spi_read_data(void *handler, u8* rxbuf, u32 rxlen)
{
    struct dw_spi_priv *priv = (struct dw_spi_priv *) handler;
    priv->rxlen = rxlen;
    priv->txlen = 0;
    dw_spi_xfer((struct dw_spi_priv *)priv, 8, NULL, rxbuf, 0);
    return 0;
}

int spi_eeprom_data(void *handler, u8* txbuf, u32 txlen, u8* rxbuf, u32 rxlen)
{
    struct dw_spi_priv *priv = (struct dw_spi_priv *) handler;
    priv->rxlen = rxlen;
    priv->txlen = txlen;
    dw_spi_xfer((struct dw_spi_priv *)priv, 8, txbuf, rxbuf, 0);
    return 0;
}
