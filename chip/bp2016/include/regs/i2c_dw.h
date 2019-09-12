#ifndef __I2C_DW_H_
#define __I2C_DW_H_

#define REG_AP_I2C0_BASE             (APB1_BASE+0xc0000)
#define REG_AP_I2C1_BASE             (APB1_BASE+0xd0000)

typedef volatile struct
{
    REG32           dw_ic_con;                      //0x00
    REG32           dw_ic_tar;                      //0x04
    REG32           dw_ic_sar;                      //0x08
    REG32           dw_ic_hs_maddr;                 //0x0c
    REG32           dw_ic_data_cmd;                 //0x10
    REG32           dw_ic_ss_scl_hcnt;              //0x14
    REG32           dw_ic_ss_scl_lcnt;              //0x18
    REG32           dw_ic_fs_scl_hcnt;              //0x1c
    REG32           dw_ic_fs_scl_lcnt;              //0x20
    REG32           dw_ic_hs_scl_hcnt;              //0x24
    REG32           dw_ic_hs_scl_lcnt;              //0x28
    REG32           dw_ic_intr_stat;                //0x2c
    REG32           dw_ic_intr_mask;                //0x30
    REG32           dw_ic_raw_intr_stat;            //0x34
    REG32           dw_ic_rx_tl;                    //0x38
    REG32           dw_ic_tx_tl;                    //0x3c
    REG32           dw_ic_clr_intr;                 //0x40
    REG32           dw_ic_clr_rx_under;             //0x44
    REG32           dw_ic_clr_rx_over;              //0x48
    REG32           dw_ic_clr_tx_over;              //0x4c
    REG32           dw_ic_clr_rd_req;               //0x50
    REG32           dw_ic_clr_tx_abrt;              //0x54
    REG32           dw_ic_clr_rx_done;              //0x58
    REG32           dw_ic_clr_activity;             //0x5c
    REG32           dw_ic_clr_stop_det;             //0x60
    REG32           dw_ic_clr_start_det;            //0x64
    REG32           dw_ic_clr_gen_call;             //0x68
    REG32           dw_ic_enable;                   //0x6c
    REG32           dw_ic_status;                   //0x70
    REG32           dw_ic_txflr;                    //0x74
    REG32           dw_ic_rxflr;                    //0x78
    REG32           dw_ic_sda_hold;                 //0x7c
    REG32           dw_ic_tx_abrt_source;           //0x80
    REG32           dw_ic_slv_data_nack_only;       //0x84
    REG32           dw_ic_dma_cr;                   //0x88
    REG32           dw_ic_dma_tdlr;                 //0x8c
    REG32           dw_ic_dma_rdlr;                 //0x90
    REG32           dw_ic_sda_setup;                //0x94
    REG32           dw_ic_ack_general_call;         //0x98
    REG32           dw_ic_enable_status;            //0x9c
    REG32           dw_ic_fs_spklen;                //0xa0
    REG32           dw_ic_hs_spklen;                //0xa4
    REG32           dw_ic_clr_restart_det;          //0xa8
    REG32           dw_ic_scl_stuck_at_low_timeout; //0xac
    REG32           dw_ic_sda_stuck_at_low_timeout; //0xb0
    REG32           dw_ic_clr_scl_stuck_det;        //0xb4
    REG32           dw_ic_device_id;                //0xb8
    REG32           dw_ic_smbus_clock_low_sext;     //0xbc
    REG32           dw_ic_smbus_clock_low_mext;     //0xc0
    REG32           dw_ic_smbus_thigh_maxidle_count;//0xc4
    REG32           dw_ic_smbus_intr_stat;          //0xc8
    REG32           dw_ic_smbus_intr_mask;          //0xcc
    REG32           dw_ic_smbus_intr_raw_status;    //0xd0
    REG32           dw_ic_clr_smbus_intr;           //0xd4
    REG32           dw_ic_optional_sar;             //0xd8
    REG32           dw_ic_smbus_udid_lsb;           //0xdc

    REG32           dw_ic_padding_0xe0[5];          //0xe0~0xf0

    REG32           dw_ic_comp_param_1;             //0xf4
    REG32           dw_ic_comp_version;             //0xf8
    REG32           dw_ic_comp_type;                //0xfc
} HWP_I2C_AP_T;

#define hwp_apI2c0                ((HWP_I2C_AP_T*) (REG_AP_I2C0_BASE))
#define hwp_apI2c1                ((HWP_I2C_AP_T*) (REG_AP_I2C1_BASE))

#define DW_IC_COMP_PARAM_1          0xf4
#define DW_IC_COMP_VERSION          0xf8
#define DW_IC_COMP_TYPE             0xfc

#define DW_IC_CON_MASTER            0x1
#define DW_IC_CON_SPEED_STD         0x2
#define DW_IC_CON_SPEED_FAST        0x4
#define DW_IC_CON_SPEED_HIGH        0x6
#define DW_IC_CON_10BITADDR_MASTER  0x10
#define DW_IC_CON_RESTART_EN        0x20
#define DW_IC_CON_SLAVE_DISABLE     0x40

#define DW_IC_INTR_RX_UNDER     0x001
#define DW_IC_INTR_RX_OVER      0x002
#define DW_IC_INTR_RX_FULL      0x004
#define DW_IC_INTR_TX_OVER      0x008
#define DW_IC_INTR_TX_EMPTY     0x010
#define DW_IC_INTR_RD_REQ       0x020
#define DW_IC_INTR_TX_ABRT      0x040
#define DW_IC_INTR_RX_DONE      0x080
#define DW_IC_INTR_ACTIVITY     0x100
#define DW_IC_INTR_STOP_DET     0x200
#define DW_IC_INTR_START_DET    0x400
#define DW_IC_INTR_GEN_CALL     0x800
#define DW_IC_INTR_RESTART_DET  0x1000

//dw_ic_status 0x70
#define DW_IC_STATUS_ACTIVITY                   (1UL << 0)
#define DW_IC_STATUS_TFNF                       (1UL << 1)
#define DW_IC_STATUS_TFE                        (1UL << 2)
#define DW_IC_STATUS_RFNE                       (1UL << 3)
#define DW_IC_STATUS_RFF                        (1UL << 4)
#define DW_IC_STATUS_MST_ACTIVITY               (1UL << 5)
#define DW_IC_STATUS_SLV_ACTIVITY               (1UL << 6)
#define DW_IC_STATUS_MST_HOLD_TX_FIFO_EMPTY     (1UL << 7)
#define DW_IC_STATUS_MST_HOLD_RX_FIFO_FULL      (1UL << 8)
#define DW_IC_STATUS_SLV_HOLD_TX_FIFO_EMPTY     (1UL << 9)
#define DW_IC_STATUS_SLV_HOLD_RX_FIFO_FULL      (1UL << 10)
#define DW_IC_STATUS_SDA_STUCK_NOT_RECOVER      (1UL << 11)
#define DW_IC_STATUS_SMBUS_QUICK_CMD_BIT        (1UL << 16)
#define DW_IC_STATUS_SMBUS_SLV_ADDR_VALID       (1UL << 17)
#define DW_IC_STATUS_SMBUS_SLV_ADDR_RESOLVED    (1UL << 18)
#define DW_IC_STATUS_SMBUS_SUSPEND_STAT         (1UL << 19)
#define DW_IC_STATUS_SMBUS_ALERT__STAT          (1UL << 20)

#define DW_IC_INTR_DEFAULT_MASK (DW_IC_INTR_RX_FULL  |  \
                                 DW_IC_INTR_TX_EMPTY |  \
                                 DW_IC_INTR_TX_ABRT  |  \
                                 DW_IC_INTR_STOP_DET)


#define DW_IC_ERR_TX_ABRT       0x1

/*
 * status codes
 */
#define STATUS_IDLE                 0x0
#define STATUS_WRITE_IN_PROGRESS    0x1
#define STATUS_READ_IN_PROGRESS     0x2

/*
 * hardware abort codes from the DW_IC_TX_ABRT_SOURCE register
 *
 * only expected abort codes are listed here
 * refer to the datasheet for the full list
 */
#define ABRT_7B_ADDR_NOACK      0
#define ABRT_10ADDR1_NOACK      1
#define ABRT_10ADDR2_NOACK      2
#define ABRT_TXDATA_NOACK       3
#define ABRT_GCALL_NOACK        4
#define ABRT_GCALL_READ         5
#define ABRT_SBYTE_ACKDET       7
#define ABRT_SBYTE_NORSTRT      9
#define ABRT_10B_RD_NORSTRT     10
#define ABRT_MASTER_DIS         11
#define ARB_LOST                12

#define DW_IC_TX_ABRT_7B_ADDR_NOACK     (1UL << ABRT_7B_ADDR_NOACK)
#define DW_IC_TX_ABRT_10ADDR1_NOACK     (1UL << ABRT_10ADDR1_NOACK)
#define DW_IC_TX_ABRT_10ADDR2_NOACK     (1UL << ABRT_10ADDR2_NOACK)
#define DW_IC_TX_ABRT_TXDATA_NOACK      (1UL << ABRT_TXDATA_NOACK)
#define DW_IC_TX_ABRT_GCALL_NOACK       (1UL << ABRT_GCALL_NOACK)
#define DW_IC_TX_ABRT_GCALL_READ        (1UL << ABRT_GCALL_READ)
#define DW_IC_TX_ABRT_SBYTE_ACKDET      (1UL << ABRT_SBYTE_ACKDET)
#define DW_IC_TX_ABRT_SBYTE_NORSTRT     (1UL << ABRT_SBYTE_NORSTRT)
#define DW_IC_TX_ABRT_10B_RD_NORSTRT    (1UL << ABRT_10B_RD_NORSTRT)
#define DW_IC_TX_ABRT_MASTER_DIS        (1UL << ABRT_MASTER_DIS)
#define DW_IC_TX_ARB_LOST               (1UL << ARB_LOST)

#define DW_IC_TX_ABRT_NOACK (DW_IC_TX_ABRT_7B_ADDR_NOACK | \
                             DW_IC_TX_ABRT_10ADDR1_NOACK | \
                             DW_IC_TX_ABRT_10ADDR2_NOACK | \
                             DW_IC_TX_ABRT_TXDATA_NOACK  | \
                             DW_IC_TX_ABRT_GCALL_NOACK)

#endif /* __I2C_DW_H_ */
