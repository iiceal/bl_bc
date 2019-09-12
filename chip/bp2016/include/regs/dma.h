#ifndef _DMA_H_
#define _DMA_H_

#define DMAC_INDEX_VALID(id)          ((id) < 2)
#define HW_DMAC_CH_NUM_MAX            (0x8)
#define HW_DMAC_CONTROLLER_INTERVAL        (0x100)
#define HW_DMAC_CONTROLLER_BASE0           (APB0_BASE+0x30000)
#define HW_DMAC_CONTROLLER_BASE1           (APB0_BASE+0x40000)
#define HW_DMAC_CONTROLLER_CH_BASE    (0x100)

#define HW_DMAC_CH_BLK_MAX            (0x200000)

#define HW_DMAC_ID_REG                     (0x0)
#define HW_DMAC_COMPVER_REG                (0x8)
#define HW_DMAC_CFG_REG                    (0x10)
#define HW_DMAC_CHEN_REG_LOW               (0x18)
#define HW_DMAC_CHEN_REG_HIGH              (0x1c)

#define HW_DMAC_INTSTATUS_REG              (0x30)
#define HW_DMAC_COMMONREG_INTCLEAR_REG     (0x38)
#define HW_DMAC_COMMONREG_INTSTATUS_ENABLE_REG    (0x40)
#define HW_DMAC_COMMONREG_INTSIGNAL_ENABLE_REG    (0x48)
#define HW_DMAC_COMMONREG_INTSTATUS_REG    (0x50)
#define HW_DMAC_RESET_REG                  (0x58)

typedef struct {
	REG32 ID;    // 0x0
    REG32 ID_UPPER;  // reserved

	REG32 DMAC_COMPVER;    // 0x8
    REG32 DMAC_COMPVER_RESV;  // reserved

	REG32 DMAC_CFG;    // 0x10
    REG32 DMAC_CFG_RESV;  // reserved

	REG32 DMAC_CHEN;    // 0x18
    REG32 DMAC_CHEN_UPPER;  // reserved

	REG32 RESV[4];

	REG32 DMAC_INTSTATUS;    // 0x30
    REG32 DMAC_INTSTATUS_RESV;  // reserved

	REG32 DMAC_COMMONREG_INTCLEAR;    // 0x38
    REG32 DMAC_COMMONREG_INTCLEAR_RESV;  // reserved

	REG32 DMAC_COMMONREG_INTSTATUS_ENABLE;    // 0x40
    REG32 DMAC_COMMONREG_INTSTATUS_ENABLE_RESV;  // reserved

	REG32 DMAC_COMMONREG_INTSIGNAL_ENABLE;    // 0x48
    REG32 DMAC_COMMONREG_INTSIGNAL_ENABLE_RESV;  // reserved

	REG32 DMAC_COMMONREG_INTSTATUS;    // 0x50
    REG32 DMAC_COMMONREG_INTSTATUS_RESV;  // reserved

	REG32 DMAC_RST;    // 0x58
    REG32 DMAC_RST_RESV;  // reserved
}DMAC_CONTROLLER_ST;

#define hwp_apDMA0               ((DMAC_CONTROLLER_ST*) (HW_DMAC_CONTROLLER_BASE0))
#define hwp_apDMA1               ((DMAC_CONTROLLER_ST*) (HW_DMAC_CONTROLLER_BASE1))

typedef struct {
	REG32 SAR;
    REG32 SAR_RESV;  // reserved

	REG32 DAR;
    REG32 DAR_RESV;  // reserved

	REG32 BLOCK_TS;
    REG32 BLOCK_TS_RESV;  // reserved

	REG32 CTL;  // 
    REG32 CTL_UPPER;

	REG32 CFG;
    REG32 CFG_UPPER;

	REG32 LLP;
    REG32 LLP_UPPER;

	REG32 STATUS;
    REG32 STATUS_UPPER;

	REG32 SWHSSRC;
    REG32 SWHSSRC_UPPER;

	REG32 SWHSDST;
    REG32 SWHSDST_UPPER;

	REG32 BLK_TFR_RESUMEREQ;
	REG32 BLK_TFR_RESUMEREQ_UPPER;

	REG32 AXI_ID;
	REG32 AXI_ID_UPPER;

	REG32 AXI_QOS;
	REG32 AXI_QOS_UPPER;

	REG32 SSTAT;
	REG32 SSTAT_RESV;

	REG32 DSTAT;
	REG32 DSTAT_RESV;

	REG32 SSTATAR;
	REG32 SSTATAR_RESV;

	REG32 DSTATAR;
	REG32 DSTATAR_RESV;

	REG32 INTSTATUS_ENABLE;
	REG32 INTSTATUS_ENABLE_RESV;

	REG32 INTSTATUS;
	REG32 INTSTATUS_RESV;

	REG32 INTSIGNAL_ENABLE;
	REG32 INTSIGNAL_ENABLE_RESV;

	REG32 INTCLEAR;
	REG32 INTCLEAR_RESV;
}DMAC_CH_CONTROLLER_ST;

// Number of data items to be transferred
typedef enum {
    DMAC_CTL_MSIZE_E_1 = 0,
	DMAC_CTL_MSIZE_E_4,
	DMAC_CTL_MSIZE_E_8,
	DMAC_CTL_MSIZE_E_16,
	DMAC_CTL_MSIZE_E_32,
	DMAC_CTL_MSIZE_E_64,
	DMAC_CTL_MSIZE_E_128,
	DMAC_CTL_MSIZE_E_256,
	DMAC_CTL_MSIZE_E_512,
	DMAC_CTL_MSIZE_E_1024,
	DMAC_CTL_MSIZE_E_ERR
}DMAC_CTL_MSIZE_E;

// Source/Destination Transfer Width
typedef enum {
    DMAC_CTL_TR_WIDTH_E_8 = 0,
	DMAC_CTL_TR_WIDTH_E_16,
	DMAC_CTL_TR_WIDTH_E_32,
	DMAC_CTL_TR_WIDTH_E_64,
	DMAC_CTL_TR_WIDTH_E_128,
	DMAC_CTL_TR_WIDTH_E_256,
	DMAC_CTL_TR_WIDTH_E_512,
	DMAC_CTL_TR_WIDTH_E_ERR
}DMAC_CTL_TR_WIDTH_E;

// Transfer Type and Flow Control
typedef enum {
    DMAC_CFG_TT_FC_E_M_TO_M_DMA = 0,
	DMAC_CFG_TT_FC_E_M_TO_P_DMA,
	DMAC_CFG_TT_FC_E_P_TO_M_DMA,
	DMAC_CFG_TT_FC_E_P_TO_P_DMA,
	DMAC_CFG_TT_FC_E_P_TO_M_P,
	DMAC_CFG_TT_FC_E_P_TO_P_SP,
	DMAC_CFG_TT_FC_E_M_TO_P_P,
	DMAC_CFG_TT_FC_E_P_TO_P_DP
}DMAC_CFG_TT_FC_E;

typedef enum {
    DMA_DIR_E_P2M = 0,
	DMA_DIR_E_M2P
}DMA_DIR_E;

// Address Increment
typedef enum {
    DMAC_CTL_ADDR_INC_E_INC = 0,
	DMAC_CTL_ADDR_INC_E_NO_CHANGE
}DMAC_CTL_ADDR_INC_E;


// Handshaking Interface Polarity
typedef enum {
    DMAC_CFG_HANDSHAKING_POLARITY_E_A_HIGH = 0,
	DMAC_CFG_HANDSHAKING_POLARITY_E_A_LOW
}DMAC_CFG_HANDSHAKING_POLARITY_E;

// Handshaking type
typedef enum {
    DMAC_CFG_HANDSHAKING_TYPE_E_HARD = 0,
	DMAC_CFG_HANDSHAKING_TYPE_E_SOFT
}DMAC_CFG_HANDSHAKING_TYPE_E;

// Channel priority-A priority of 7 is the highest priority, and 0 is the lowest
typedef enum {
    DMAC_CFG_PRIORITY_E_0 = 0,
	DMAC_CFG_PRIORITY_E_1,
	DMAC_CFG_PRIORITY_E_2,
	DMAC_CFG_PRIORITY_E_3,
	DMAC_CFG_PRIORITY_E_4,
	DMAC_CFG_PRIORITY_E_5,
	DMAC_CFG_PRIORITY_E_6,
	DMAC_CFG_PRIORITY_E_7
}DMAC_CFG_PRIORITY_E;

// DMAC Peripheral enum
typedef enum {
    DMAC_PERIPHERAL_TYPE_E_QSPI_TX = 0,
	DMAC_PERIPHERAL_TYPE_E_QSPI_RX,
	DMAC_PERIPHERAL_TYPE_E_SPI0_TX = 2,
	DMAC_PERIPHERAL_TYPE_E_SPI0_RX,
	DMAC_PERIPHERAL_TYPE_E_SPI3_TX = 4,
	DMAC_PERIPHERAL_TYPE_E_SPI3_RX,
	DMAC_PERIPHERAL_TYPE_E_UART6_TX = 6,
	DMAC_PERIPHERAL_TYPE_E_UART6_RX,
	DMAC_PERIPHERAL_TYPE_E_UART7_TX = 8,
	DMAC_PERIPHERAL_TYPE_E_UART7_RX,
	DMAC_PERIPHERAL_TYPE_E_I2C0_TX = 10,
	DMAC_PERIPHERAL_TYPE_E_I2C0_RX,
	DMAC_PERIPHERAL_TYPE_E_I2C1_TX = 12,
	DMAC_PERIPHERAL_TYPE_E_I2C1_RX,
	DMAC_PERIPHERAL_TYPE_E_UART1_TX = 14,
	DMAC_PERIPHERAL_TYPE_E_UART1_RX,
	DMAC_PERIPHERAL_TYPE_E_UNKNOWN,
}DMAC_PERIPHERAL_TYPE_E;

// DMAC Multi Block Transfer Type
typedef enum {
    DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_CONTIGUOUS = 0,
	DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_RELOAD,
	DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_SHADOW,
	DMAC_MULTI_BLOCK_TRANSFER_TYPE_E_LINK_LIST
}DMAC_MULTI_BLOCK_TRANSFER_TYPE_E;

// DMAC CH INT TYPE
typedef enum {
    DMAC_CH_INT_TYPE_E_BLK_TFR_DONE = 0,
	DMAC_CH_INT_TYPE_E_DMA_TFR_DONE,
	DMAC_CH_INT_TYPE_E_2,
	DMAC_CH_INT_TYPE_E_3,
	DMAC_CH_INT_TYPE_E_4,
	DMAC_CH_INT_TYPE_E_SHADOWREG_OR_LLI_INVALID_ERR = 13,
	DMAC_CH_INT_TYPE_E_END
}DMAC_CH_INT_TYPE_E;

// DMAC CFG
#define DMAC_CFG_DMAC_EN_BIT                  (0x0)
#define DMAC_CFG_INT_EN_BIT                   (0x1)
#define DMAC_CFG_DMAC_EN_MASK                 (0x1 << DMAC_CFG_DMAC_EN_BIT)
#define DMAC_CFG_INT_EN_MASK                  (0x1 << DMAC_CFG_INT_EN_BIT)



// CTL LOW 32 bits
#define DMAC_CH_CTL_SMS_BIT               (0)
#define DMAC_CH_CTL_DMS_BIT               (2)
#define DMAC_CH_CTL_SINC_BIT              (4)
#define DMAC_CH_CTL_DINC_BIT              (6)
#define DMAC_CH_CTL_SRC_TR_WIDTH_BIT      (8)
#define DMAC_CH_CTL_DST_TR_WIDTH_BIT      (11)
#define DMAC_CH_CTL_SRC_MSIZE_BIT         (14)
#define DMAC_CH_CTL_DST_MSIZE_BIT         (18)

#define DMAC_CH_CTL_SMS_MASK              (0x1 << DMAC_CH_CTL_SMS_BIT)
#define DMAC_CH_CTL_DMS_MASK              (0x1 << DMAC_CH_CTL_DMS_BIT)
#define DMAC_CH_CTL_SINC_MASK             (0x1 << DMAC_CH_CTL_SINC_BIT)
#define DMAC_CH_CTL_DINC_MASK             (0x1 << DMAC_CH_CTL_DINC_BIT)
#define DMAC_CH_CTL_SRC_TR_WIDTH_MASK     (0x7 << DMAC_CH_CTL_SRC_TR_WIDTH_BIT)
#define DMAC_CH_CTL_DST_TR_WIDTH_MASK     (0x7 << DMAC_CH_CTL_DST_TR_WIDTH_BIT)
#define DMAC_CH_CTL_SRC_MSIZE_MASK        (0xF << DMAC_CH_CTL_SRC_MSIZE_BIT)
#define DMAC_CH_CTL_DST_MSIZE_MASK        (0xF << DMAC_CH_CTL_DST_MSIZE_BIT)

// CTL HIGH 32 bits
#define DMAC_CH_CTL_LLI_VALID_BIT         (31)
#define DMAC_CH_CTL_LLI_LAST_BIT          (30)
#define DMAC_CH_CTL_BLK_TFR_INT_EN_BIT    (26)
#define DMAC_CH_CTL_DST_STAT_EN_BIT       (25)
#define DMAC_CH_CTL_SRC_STAT_EN_BIT       (24)
#define DMAC_CH_CTL_AWLEN_BIT             (16)
#define DMAC_CH_CTL_AWLEN_EN_BIT          (15)
#define DMAC_CH_CTL_ARLEN_BIT             (7)
#define DMAC_CH_CTL_ARLEN_EN_BIT          (6)

#define DMAC_CH_CTL_LLI_VALID_MASK        (0x1 << DMAC_CH_CTL_LLI_VALID_BIT)
#define DMAC_CH_CTL_LLI_LAST_MASK         (0x1 << DMAC_CH_CTL_LLI_LAST_BIT)
#define DMAC_CH_CTL_BLK_TFR_INT_EN_MASK   (0x1 << DMAC_CH_CTL_BLK_TFR_INT_EN_BIT)
#define DMAC_CH_CTL_DST_STAT_EN_MASK      (0x1 << DMAC_CH_CTL_DST_STAT_EN_BIT)
#define DMAC_CH_CTL_SRC_STAT_EN_MASK      (0x1 << DMAC_CH_CTL_SRC_STAT_EN_BIT)
#define DMAC_CH_CTL_AWLEN_MASK            (0xFF << DMAC_CH_CTL_AWLEN_BIT)
#define DMAC_CH_CTL_AWLEN_EN_MASK         (0x1 << DMAC_CH_CTL_AWLEN_EN_BIT)
#define DMAC_CH_CTL_ARLEN_MASK            (0xFF << DMAC_CH_CTL_ARLEN_BIT)
#define DMAC_CH_CTL_ARLEN_EN_MASK         (0x1 << DMAC_CH_CTL_ARLEN_EN_BIT)


// CFG REG
#define DMAC_CH_CFG_DST_OSR_LMT_BIT       (27)
#define DMAC_CH_CFG_SRC_OSR_LMT_BIT       (23)
#define DMAC_CH_CFG_CH_PRIOR_BIT          (17)
#define DMAC_CH_CFG_DEST_PER_BIT          (12)
#define DMAC_CH_CFG_SRC_PER_BIT           (7)
#define DMAC_CH_CFG_DST_HWHS_POL_BIT      (6)
#define DMAC_CH_CFG_SRC_HWHS_POL_BIT      (5)
#define DMAC_CH_CFG_TT_FC_BIT             (0)

#define DMAC_CH_CFG_DST_OSR_LMT_MASK       (0xF<< DMAC_CH_CFG_DST_OSR_LMT_BIT)
#define DMAC_CH_CFG_SRC_OSR_LMT_MASK       (0xF<< DMAC_CH_CFG_SRC_OSR_LMT_BIT)
#define DMAC_CH_CFG_CH_PRIOR_MASK          (0x7 << DMAC_CH_CFG_CH_PRIOR_BIT)
#define DMAC_CH_CFG_DEST_PER_MASK          (0x7 << DMAC_CH_CFG_DEST_PER_BIT)
#define DMAC_CH_CFG_SRC_PER_MASK           (0x7 << DMAC_CH_CFG_SRC_PER_BIT)
#define DMAC_CH_CFG_DST_HWHS_POL_MASK      (0x1 << DMAC_CH_CFG_DST_HWHS_POL_BIT)
#define DMAC_CH_CFG_SRC_HWHS_POL_MASK      (0x1 << DMAC_CH_CFG_SRC_HWHS_POL_BIT)
#define DMAC_CH_CFG_TT_FC_MASK             (0x7 << DMAC_CH_CFG_TT_FC_BIT)

#define DMAC_CH_DST_MULTBLK_TYPE_BIT       (2)
#define DMAC_CH_SRC_MULTBLK_TYPE_BIT       (0)
#define DMAC_CH_DST_MULTBLK_TYPE_MASK      (0x3 << DMAC_CH_DST_MULTBLK_TYPE_BIT)
#define DMAC_CH_SRC_MULTBLK_TYPE_MASK      (0x3 << DMAC_CH_SRC_MULTBLK_TYPE_BIT)

#define DMAC_CH_LLP_LOC_BIT                (6)
#define DMAC_CH_LLP_LOC_MASK               (0x3FFFFFF << DMAC_CH_LLP_LOC_BIT)

#define DMAC_CH_STATUS_DATA_LEFT_IN_FIFO_BIT        (0)
#define DMAC_CH_STATUS_DATA_LEFT_IN_FIFO_MASK       (0x7FFF << DMAC_CH_STATUS_DATA_LEFT_IN_FIFO_BIT)
#define DMAC_CH_STATUS_CMPLTD_BLK_TFR_SIZE_BIT      (0)
#define DMAC_CH_STATUS_CMPLTD_BLK_TFR_SIZE_MASK     (0x3FFFFF << DMAC_CH_STATUS_CMPLTD_BLK_TFR_SIZE_BIT)

// CHx_INTSTATUS_ENABLEREG
#define DMAC_CH_INTSTATUS_ENABLE_DMA_TFR_DONE_BIT     (1)
#define DMAC_CH_INTSTATUS_ENABLE_DMA_TFR_DONE_MASK    (0x1 << DMAC_CH_INTSTATUS_ENABLE_DMA_TFR_DONE_BIT)

#define DMAC_CH_INTSTATUS_ENABLE_BLK_TFR_DONE_BIT     (0)
#define DMAC_CH_INTSTATUS_ENABLE_BLK_TFR_DONE_MASK    (0x1 << DMAC_CH_INTSTATUS_ENABLE_BLK_TFR_DONE_BIT)

#define DMAC_CH_INTSTATUS_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_BIT     (13)
#define DMAC_CH_INTSTATUS_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_MASK    (0x1 << DMAC_CH_INTSTATUS_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_BIT)

// CHx_INTSTATUS_REG
#define DMAC_CH_INTSTATUS_DMA_TFR_DONE_BIT            (1)
#define DMAC_CH_INTSTATUS_DMA_TFR_DONE_MASK           (0x1 << DMAC_CH_INTSTATUS_DMA_TFR_DONE_BIT)

#define DMAC_CH_INTSTATUS_BLK_TFR_DONE_BIT            (0)
#define DMAC_CH_INTSTATUS_BLK_TFR_DONE_MASK           (0x1 << DMAC_CH_INTSTATUS_BLK_TFR_DONE_BIT)

#define DMAC_CH_INTSTATUS_SHADOWREG_OR_LLI_INVALID_ERR_BIT            (13)
#define DMAC_CH_INTSTATUS_SHADOWREG_OR_LLI_INVALID_ERR_MASK           (0x1 << DMAC_CH_INTSTATUS_SHADOWREG_OR_LLI_INVALID_ERR_BIT)

// CHx_INTSIGNAL_ENABLEREG
#define DMAC_CH_INTSIGNAL_ENABLE_DMA_TFR_DONE_BIT     (1)
#define DMAC_CH_INTSIGNAL_ENABLE_DMA_TFR_DONE_MASK    (0x1 << DMAC_CH_INTSIGNAL_ENABLE_DMA_TFR_DONE_BIT)

#define DMAC_CH_INTSIGNAL_ENABLE_BLK_TFR_DONE_BIT     (0)
#define DMAC_CH_INTSIGNAL_ENABLE_BLK_TFR_DONE_MASK    (0x1 << DMAC_CH_INTSIGNAL_ENABLE_BLK_TFR_DONE_BIT)

#define DMAC_CH_INTSIGNAL_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_BIT     (13)
#define DMAC_CH_INTSIGNAL_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_MASK    (0x1 << DMAC_CH_INTSIGNAL_ENABLE_SHADOWREG_OR_LLI_INVALID_ERR_BIT)

// CHx_INTCLEARREG
#define DMAC_CH_INTCLEAR_DMA_TFR_DONE_BIT             (1)
#define DMAC_CH_INTCLEAR_DMA_TFR_DONE_MASK            (0x1 << DMAC_CH_INTCLEAR_DMA_TFR_DONE_BIT)

#define DMAC_CH_INTCLEAR_BLK_TFR_DONE_BIT             (0)
#define DMAC_CH_INTCLEAR_BLK_TFR_DONE_MASK            (0x1 << DMAC_CH_INTCLEAR_BLK_TFR_DONE_BIT)

#define DMAC_CH_INTCLEAR_SHADOWREG_OR_LLI_INVALID_ERR_BIT             (13)
#define DMAC_CH_INTCLEAR_SHADOWREG_OR_LLI_INVALID_ERR_MASK            (0x1 << DMAC_CH_INTCLEAR_SHADOWREG_OR_LLI_INVALID_ERR_BIT)

#endif

