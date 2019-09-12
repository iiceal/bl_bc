#ifndef __DRV_I2C_0720_H__
#define __DRV_I2C_0720_H__
#include <common.h>
#include "regs/i2c_dw.h"

#define DATA_SIZE (32) // EEPROM PAGE SIZE 32 BYTE
#define SLV_ADDR 0X50
//#define SLV_ADDR 0X60

typedef enum {
    I2C_TEST_READ,
    I2C_TEST_WRITE,
    I2C_TEST_UNKNOWN = -1,
} I2C_TEST_TYPE_T;

/*
*   external functions
*/
int dw_i2c_send_bytes(HWP_I2C_AP_T *hwp_i2c, unsigned char *out_buf, unsigned int len);
int dw_i2c_read(HWP_I2C_AP_T *hwp_i2c, unsigned char *rbuf, unsigned int rlen);
int dw_i2c_smbus_read(HWP_I2C_AP_T *, unsigned char *, unsigned int, unsigned char *,
                       unsigned int);
void dw_i2c_master_init(HWP_I2C_AP_T *hwp_i2c, unsigned char slv_addr, const unsigned int mode);

//used for irq mode transfer
int i2c_dw_xfer_msg(HWP_I2C_AP_T *hwp_i2c, unsigned char *out_buf, unsigned int len);
int i2c_dw_read(HWP_I2C_AP_T *hwp_i2c, unsigned char *out_buf, unsigned int len);
HWP_I2C_AP_T *get_i2c_hwp(u32 index);

#endif //__DRV_I2C_0720_H__
