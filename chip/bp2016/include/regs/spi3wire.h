#ifndef _SPI_3WIRE_H__
#define _SPI_3WIRE_H__

#define SPI_4_BASE_ADDR                     (APB2_BASE + 0x40000)

typedef volatile struct {
    REG32 ctrl;             // 0x00
    REG32 length;           // 0x04
    REG32 wdata;            // 0x08
    REG32 addrin;           // 0x0c
    REG32 clkdiv;           // 0x10
    REG32 rsv_0x14;         // 0x14
    REG32 rsv_0x18;         // 0x18
    REG32 rsv_0x1c;         // 0x1c
    REG32 busy;             // 0x20
    REG32 rdata;            // 0x24
} HWP_SPI_3WIRE_T;

#define hwp_ap_Spi4                         ((HWP_SPI_3WIRE_T*) (SPI_4_BASE_ADDR))

// 0x00 ctrl
#define SPI_3WIRE_CTRL_MASK                 0xf
#define SPI_3WIRE_CTRL_START                ((0x1 << 0) & SPI_3WIRE_CTRL_MASK)
#define SPI_3WIRE_CTRL_READ                 ((0x1 << 1) & SPI_3WIRE_CTRL_MASK)
#define SPI_3WIRE_CTRL_WRITE                (0 << 1)
#define SPI_3WIRE_CTRL_CS0                  (0 << 2)
#define SPI_3WIRE_CTRL_CS1                  ((0x1 << 2) & SPI_3WIRE_CTRL_MASK)
#define SPI_3WIRE_CTRL_CS2                  ((0x2 << 2) & SPI_3WIRE_CTRL_MASK)
#define SPI_3WIRE_CTRL_CS3                  ((0x3 << 2) & SPI_3WIRE_CTRL_MASK)

// 0x04 length
#define SPI_3WIRE_LENGTH_DATA_OFFSET        0x0
#define SPI_3WIRE_LENGTH_DATA_MASK          (0xff << SPI_3WIRE_LENGTH_DATA_OFFSET)
#define SPI_3WIRE_LENGTH_DATA_20BIT         ((0x14 << SPI_3WIRE_LENGTH_DATA_OFFSET) & SPI_3WIRE_LENGTH_DATA_MASK)
#define SPI_3WIRE_LENGTH_ADDR_OFFSET        0x10
#define SPI_3WIRE_LENGTH_ADDR_MASK          (0xff & SPI_3WIRE_LENGTH_ADDR_OFFSET)
#define SPI_3WIRE_LENGTH_ADDR_5BIT          ((0x5 << SPI_3WIRE_LENGTH_ADDR_OFFSET) & SPI_3WIRE_LENGTH_ADDR_MASK)
#define SPI_3WIRE_LENGTH_ADDR_6BIT          ((0x5 << SPI_3WIRE_LENGTH_ADDR_OFFSET) & SPI_3WIRE_LENGTH_ADDR_MASK)

// 0x10 clkdiv
#define SPI_3WIRE_CLKDIV_DEFAULT            0x251

// 0x20 busy
#define SPI_3WIRE_BUSY                      0x1

#endif /* _SPI_3WIRE_H__ */
