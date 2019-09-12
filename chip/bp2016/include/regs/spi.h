#ifndef _SPI_H_
#define _SPI_H_

#define SPI_0_BASE_ADDR                     (APB1_BASE + 0x80000)
#define SPI_1_BASE_ADDR                     (APB1_BASE + 0x90000)
#define SPI_2_BASE_ADDR                     (APB1_BASE + 0xa0000)
#define SPI_3_BASE_ADDR                     (APB1_BASE + 0xb0000)

typedef volatile struct {
    REG32 ctrl0;            // 0x00
    REG32 ctrl1;            // 0x04
    REG32 ssienr;           // 0x08
    REG32 mwcr;             // 0x0c
    REG32 ser;              // 0x10
    REG32 baudr;            // 0x14
    REG32 txftlr;           // 0x18
    REG32 rxftlr;           // 0x1c
    REG32 txflr;            // 0x20
    REG32 rxflr;            // 0x24
    REG32 sr;               // 0x28
    REG32 imr;              // 0x2c
    REG32 isr;              // 0x30
    REG32 risr;             // 0x34
    REG32 txoicr;           // 0x38
    REG32 rxoicr;           // 0x3c
    REG32 rxuicr;           // 0x40
    REG32 msticr;           // 0x44
    REG32 icr;              // 0x48
    REG32 dmacr;            // 0x4c
    REG32 dmatdlr;          // 0x50
    REG32 dmardlr;          // 0x54
    REG32 idr;              // 0x58
    REG32 ssi_comp_version; // 0x5c
    REG32 dr;               // 0x60
    REG32 padding[35];      // 0x64~0xec
    REG32 rx_sample_dly;    // 0xf0
    REG32 spi_ctrlr0;       // 0xf4
    REG32 rsvd_1;           // 0xf8
    REG32 rsvd_2;           // 0xfc
} HWP_SPI_T;

#define hwp_ap_Spi0                         ((HWP_SPI_T *) (SPI_0_BASE_ADDR))
#define hwp_ap_Spi1                         ((HWP_SPI_T *) (SPI_1_BASE_ADDR))
#define hwp_ap_Spi2                         ((HWP_SPI_T *) (SPI_2_BASE_ADDR))
#define hwp_ap_Spi3                         ((HWP_SPI_T *) (SPI_3_BASE_ADDR))

// 0x00 ctrl0
#define SPI_CTRL0_DFS_OFFSET                0
#define SPI_CTRL0_DFS_MASK                  (0xF << SPI_CTRL0_DFS_OFFSET)
#define SPI_CTRL0_DFS_8BIT                  ((0x7 << SPI_CTRL0_DFS_OFFSET) & SPI_CTRL0_DFS_MASK)

#define SPI_CTRL0_FRF_OFFSET                4
#define SPI_CTRL0_FRF_MASK                  (0X3 << SPI_CTRL0_FRF_OFFSET)
#define SPI_CTRL0_FRF_MOTO_SPI              ((0X0 << SPI_CTRL0_FRF_OFFSET) & SPI_CTRL0_FRF_MASK)
#define SPI_CTRL0_FRF_TI_SSP                ((0X1 << SPI_CTRL0_FRF_OFFSET) & SPI_CTRL0_FRF_MASK)
#define SPI_CTRL0_FRF_NS_MW                 ((0X2 << SPI_CTRL0_FRF_OFFSET) & SPI_CTRL0_FRF_MASK)

#define SPI_CTRL0_SCPH_OFFSET               6
#define SPI_CTRL0_SCPH_MIDDLE_TOGGLE        (0x0 << SPI_CTRL0_SCPH_OFFSET)
#define SPI_CTRL0_SCPH_START_TOGGLE         (0x1 << SPI_CTRL0_SCPH_OFFSET)

#define SPI_CTRL0_SCPOL_OFFSET              7
#define SPI_CTRL0_SCPOL_LOW                 (0x0 << SPI_CTRL0_SCPOL_OFFSET)
#define SPI_CTRL0_SCPOL_HIGH                (0x1 << SPI_CTRL0_SCPOL_OFFSET)

#define SPI_CTRL0_TMOD_OFFSET               8
#define SPI_CTRL0_TMOD_MASK                 (0x3 << SPI_CTRL0_TMOD_OFFSET)
#define SPI_CTRL0_TMOD_TR                   ((0x0 << SPI_CTRL0_TMOD_OFFSET) & SPI_CTRL0_TMOD_MASK)      /* xmit & recv */
#define SPI_CTRL0_TMOD_TO                   ((0x1 << SPI_CTRL0_TMOD_OFFSET) & SPI_CTRL0_TMOD_MASK)      /* xmit only */
#define SPI_CTRL0_TMOD_RO                   ((0x2 << SPI_CTRL0_TMOD_OFFSET) & SPI_CTRL0_TMOD_MASK)      /* recv only */
#define SPI_CTRL0_TMOD_EPROMREAD            ((0x3 << SPI_CTRL0_TMOD_OFFSET) & SPI_CTRL0_TMOD_MASK)      /* eeprom read mode */

#define SPI_CTRL0_SLVOE_OFFSET              10
#define SPI_CTRL0_SLVOE_TX_ENABLE           (0x0 << SPI_CTRL0_SLVOE_OFFSET)
#define SPI_CTRL0_SLVOE_TX_DISABLE          (0x1 << SPI_CTRL0_SLVOE_OFFSET)

#define SPI_CTRL0_SRL_OFFSET                11
#define SPI_CTRL0_SRL_NORMAL_MODE           (0x0 << SPI_CTRL0_SRL_OFFSET)
#define SPI_CTRL0_SRL_TEST_MODE             (0x1 << SPI_CTRL0_SRL_OFFSET)

#define SPI_CTRL0_CFS_OFFSET                12

#define SPI_CTRL0_DFS32_OFFSET              16
#define SPI_CTRL0_DFS32_MASK                (0x1f << SPI_CTRL0_DFS32_OFFSET)
#define SPI_CTRL0_DFS32_8BIT                ((0x7 << SPI_CTRL0_DFS32_OFFSET) & SPI_CTRL0_DFS32_MASK)
#define SPI_CTRL0_DFS32_16BIT                ((0xf << SPI_CTRL0_DFS32_OFFSET) & SPI_CTRL0_DFS32_MASK)
#define SPI_CTRL0_DFS32_21BIT                ((0x14 << SPI_CTRL0_DFS32_OFFSET) & SPI_CTRL0_DFS32_MASK)

#define SPI_CTRL0_SPI_FRF_OFFSET            21
#define SPI_CTRL0_SPI_FRF_MASK              (0x3 << SPI_CTRL0_SPI_FRF_OFFSET)
#define SPI_CTRL0_SPI_FRF_STD_SPI           ((0x0 << SPI_CTRL0_SPI_FRF_OFFSET) & SPI_CTRL0_SPI_FRF_MASK)
#define SPI_CTRL0_SPI_FRF_DUAL_SPI          ((0x1 << SPI_CTRL0_SPI_FRF_OFFSET) & SPI_CTRL0_SPI_FRF_MASK)
#define SPI_CTRL0_SPI_FRF_QUAD_SPI          ((0x2 << SPI_CTRL0_SPI_FRF_OFFSET) & SPI_CTRL0_SPI_FRF_MASK)

// 0x08 ssienr
#define SPI_SSIENR_DISABLE_SPI              0x0
#define SPI_SSIENR_ENABLE_SPI               0x1

// 0x18/0x1c txftlr/rxftlr
#define SPI_INT_RX_THRESHOLD                0
#define SPI_INT_TX_THRESHOLD                8

/* Bit fields in SR, 7 bits */
// 0x28 sr
#define SR_MASK                             0x7f        /* cover 7 bits */
#define SR_BUSY                             (1 << 0)
#define SR_TF_NOT_FULL                      (1 << 1)
#define SR_TF_EMPT                          (1 << 2)
#define SR_RF_NOT_EMPT                      (1 << 3)
#define SR_RF_FULL                          (1 << 4)
#define SR_TX_ERR                           (1 << 5)
#define SR_DCOL                             (1 << 6)

/* Bit fields in ISR, IMR, RISR, 7 bits */
// 0x2c/0x30/0x34 imr/isr/risr
#define SPI_INT_TXEI                        (1 << 0)
#define SPI_INT_TXOI                        (1 << 1)
#define SPI_INT_RXUI                        (1 << 2)
#define SPI_INT_RXOI                        (1 << 3)
#define SPI_INT_RXFI                        (1 << 4)
#define SPI_INT_MSTI                        (1 << 5)
#define SPI_INT_DISABLE_ALL                 0x0
#define SPI_INT_DEFAULT_MASK                (SPI_INT_RXFI | \
                                            SPI_INT_TXEI)
// 0x4c dmacr
#define SPI_DMACR_RDMAE                     (1 << 0)
#define SPI_DMACR_TDMAE                     (1 << 1)
#define SPI_DMACR_DISABLE_ALL               (0)

#endif /*_SPI_H_*/
