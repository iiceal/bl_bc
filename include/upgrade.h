#ifndef _UPGRADE_H_
#define _UPGRADE_H_

#include <common.h>


#define	PK_HEADER		(0x7e)
#define	PK_TAIL			(0x7e)

#define KEY_SPACE       (0x20)
#define KEY_ESC         (0x1b)

#define UPGRADE_STATE_VALID(x)   ((x) <= PK_STATUS_EXEC_DATA)
#define ZERO_LEN_RESET_CNT        (5)

typedef struct status_m{
    U32  stat;
    U32  (*deal_fun)(u8* buff, U32 len, bool *switch_state);
    void (*success_fun)(int);
    void (*fail_fun)(int);
}STAT_MANAGER_ST;

typedef enum {
    PK_CMD_ENTER,  // 0
	PK_CMD_CONNECT,  // 1
	PK_CMD_START_DATA,  // 2
	PK_CMD_MIDST_DATA,  // 3
	PK_CMD_END_DATA,  // 4
	PK_CMD_EXEC_DATA,  // 5

	PK_CMD_REP_VER,  // 6
	PK_CMD_REP_ACK,  // 7
	PK_CMD_REP_NACK,  // 8
	PK_CMD_REP_ERR  // 9 总的数据包判断出现问题时，返回该状态，PC重新开始，进入初始状态
} PK_CMD_TYPE;

typedef enum {
    NACK_OK = 0,  // 正确
    NACK_ZERO_LEN = 1,  // 未收到数据包
    NACK_LESS_LEN,  // 数据包长度小于格式头
    NACK_HEADER_ERR,  // 数据包头或包尾错误
    NACK_LEN_ERR,  // 数据包长度错误
    NACK_CMD_ERR,  // 命令字错误
    NACK_CRC_ERR,  // 数据包校验错误
    NACK_STAT_ERR,  // 状态错误（在某种状态下收到不符合预期的命令字）
    NACK_START_DATA_ERR,  // 下载地址或长度错误
    NACK_EXEC_ADDR_ERR,  // 执行地址错误
    NACK_RESET,  // 需要重启状态机，恢复IDLE状态
    NACK_NO_FLASH  // 需要重启状态机，恢复IDLE状态
} NACK_TYPE;

#define NACK_DATA(x)    (((x)<<0x8) | PK_CMD_REP_NACK)

typedef enum {
	PK_STATUS_IDLE = 0,  // 0
	PK_STATUS_SEND_VER,  // 1
	PK_STATUS_CONNECTED,  // 2
	PK_STATUS_DATA,  // 3
	PK_STATUS_EXEC_DATA,  // 4
} PK_STATE;

#define SYS_UPGRADE_UART_PORT    0x1
//#define SYS_SRAM_ADDR_BASE       0x00100000
//#define SYS_SRAM_LENGTH          0x00200000
#define SYS_SRAM_ADDR_BASE       CONFIG_SRAM_START
#define SYS_SRAM_LENGTH          CONFIG_SRAM_SIZE
#define UPGRADE_RECV_TIMEOUT     0x5
#define UPGRADE_ACK              0x7C
#define UPGRADE_HDR_FORMAT_LEN   (10)

#define SYS_RAM_START_ADDR       SYS_SRAM_ADDR_BASE
#define SYS_RAM_LENGTH           SYS_SRAM_LENGTH
#define SYS_RAM_RESV_LENGTH      (64 * 1024)

#define UPGRADE_RECV_BUFF_LEN        (32768 + 10)  //32 * 1024
#define UPGRADE_SEND_BUFF_LEN        (128)
#define UPGRADE_CONNECT_CMD_LEN      (10)
#define UPGRADE_START_DATA_CMD_LEN   (18)
#define UPGRADE_EXEC_DATA_CMD_LEN    (14)
#define PROTOCOL_PAYLOAD_OFFSET      (0X5)
#define PROTOCOL_CMD_TYPE_LEN        (0x4)

#define TIMEOUT_BEFORE_CMD_LOOP      (5000)
#define UPGRADE_RECV_DATA_TIMEOUT    (2000)    // 毫秒单位
#define UPGRADE_SEND_NACK_COUNT_MAX  (200)      // 整个传输中可以接受的nack最大个数

#define PK_HEADER_INVALID(x)        (x != PK_HEADER)
#define PK_TAIL_INVALID(x)          (x != PK_TAIL)
#define PK_DATA_LEN_INVALID(a,b)    ((a)+10 != (b))
#define UPGRADE_GET_U16(a,b)        ((a<<8)|(b))
#define UPGRADE_GET_U32(a,b,c,d)    ((a<<24)|(b<<16)|(c<<8)|(d))

#define UPGRADE_START_DATA_VALID_SRAM(addr, len)  \
        ((CONFIG_SRAM_START <= addr) && ((addr + len) < (CONFIG_SRAM_START + SYS_RAM_LENGTH)))

#define UPGRADE_SPIFLASH_ADDR_VALID(addr, len) \
        ((SPIFLASH_D_BASE <= addr) && ((addr + len) < SPIFLASH_D_BASE + 0x2000000))

#ifdef USE_DDR
// to be modified when use ddr
#define UPGRADE_START_DATA_VALID_DDR(addr, len)  \
        (((CONFIG_SRAM_START + SYS_RAM_RESV_LENGTH) <= addr) \
		&& ((addr + len) < (CONFIG_SRAM_START + CONFIG_SRAM_SIZE - SYS_RAM_RESV_LENGTH)))
#endif

U8 * upgrade_get_send_buff(void);
int upgrade_package_data(U16 cmd, u8* buff, U16 length);
void upgrade_start(void);

#endif /* _UPGRADE_H_ */

