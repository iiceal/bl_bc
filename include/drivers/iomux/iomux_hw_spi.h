#ifndef __IOMUX_HW_SPI_2017122_H__
#define __IOMUX_HW_SPI_2017122_H__

void scm_qspi_en(void);
void scm_spi0_en(void);

/*
 * Func: Enable spi1 pin mux
 * Argument: cs index, 0: disble cs, 1: enable cs
 * Notice: At least enable one cs
 */
int scm_spi1_en(int cs0, int cs1);
int scm_spi2_en(int cs0, int cs1, int cs2, int cs3, int cs4, int cs5, int cs6, int cs7);
void scm_spi3_en(void);
int scm_spi4_en(int cs0, int cs1);

#endif //__IOMUX_HW_SPI_2017122_H__
