//#ifdef CONFIG_UPV_SERIAl
#ifdef CONFIG_AUTO_UPGRADE
#include <common.h>
#include "regs/uart.h"
#include "upgrade.h"
#include "drivers/drv_serial.h"

/*Data format defined: */
/***************************************************************************************
*        |             |               |                      |               |       |
* header |     type    |      length   |         data         |     crc       |  tail |  	  
*  1Bytes|     2Bytes  |      2Bytes   |         N Bytes      |   4 Bytes     | 1Bytes| 
*        |             |               |                      |               |       |
****************************************************************************************/


#define UPV_DEBUG
#ifdef UPV_DEBUG
#define upgrade_printf(msg...)  \
        do{ \
          if(0 != select_id) printf(msg); \
}while(0)
#else
#define upgrade_printf(msg...)
#endif


/**********************************************************/
//#define bink_debug
#ifdef bink_debug
#define d_printf(msg...) printf(msg)
#else
#define d_printf(msg...)
#endif

unsigned long long t_begin = 0;
unsigned long long t_end = 0;

/*********************************************************/

char version_curr[64] = "20170523";
u8  upgrade_recv_serial_buffer[UPGRADE_RECV_BUFF_LEN];  // 接收buffer
//u8 * upgrade_recv_serial_buffer = malloc_buf;
u8  upgrade_send_buffer[UPGRADE_SEND_BUFF_LEN];  // 发送buffer
volatile U16 upgrade_recv_data_type;  // 解析后得到的数据帧类型
U16 upgrade_recv_data_len;   // 
volatile U32 ram_save_addr, ram_save_len;  // start_data 命令中下发的保存数据目标地址，下发的数据总长度
U32 recv_data_len_total = 0;  // 当前接收到的数据 payload 总长度
volatile U32 exec_addr = 0;  // 命令下发的跳转执行地址
volatile U32 upgrade_send_nack_total = 0;  // 全部传输过程中，发送的nack总数，用来判定病重启传输
int select_id = -1;   // upgrade uart port_id

void upgrade_global_para_reset(void);

extern inline int __serial_getc_direct(HWP_UART_T *hw_uart);
extern int __serial_tstc(HWP_UART_T *hw_uart);
extern void flush_dcache_all(void);

// 超时读取串口数据的函数，用来从主机获取串口数据，使用超时机制判断主机发送结束
static HWP_UART_T *hw_uart_upgrade;
static U32 more_count = 0;
U32 upgrade_get_serial_buffer(U8 *buff)
{
	unsigned long long s_c, s_t, e;
	U32 length = 0;
	U32 total_timeout = UPGRADE_RECV_DATA_TIMEOUT;
	U32 char_timeout = 10;  // 毫秒为单位

	memset(buff, 0xee, UPGRADE_RECV_BUFF_LEN);

    //s = timer_get_tick();
    //s_t = arch_counter_get_cntpct();
    s_t = arch_counter_get_current();
    s_c = s_t;
	while(1)
	{
		if(__serial_tstc(hw_uart_upgrade))
		{
			buff[length] = __serial_getc_direct(hw_uart_upgrade);
			length++;
			if(length == UPGRADE_RECV_BUFF_LEN)
			{
				if(__serial_tstc(hw_uart_upgrade))
				{
					more_count++;
					upgrade_printf("%d --data more in rx fifo.\n", more_count);
				}
				upgrade_printf("get %d bytes, break.\n", length);
				break;
			}
			//s = timer_get_tick();   // 更新刚收到新字符的最末时间
            //s_c = arch_counter_get_cntpct();
            s_c = arch_counter_get_current();
		}

        //e = arch_counter_get_cntpct();
        e = arch_counter_get_current();
		// 从收到第一个字符开始进行两个字符的最长超时判断，连续30毫秒没有新的字符，认为发送完毕，直接返回
		if(0 < length)
		{
			//e = timer_get_tick();
			if((((unsigned long)(e-s_c))/(CONFIG_COUNTER_FREQ/1000)) >= char_timeout)
			{
				upgrade_printf("char timeout 0x%x ms, recv end. \n", char_timeout);
				break;
			}
		}
        else
		{
			// 总超时时间到，跳出
			if((((unsigned long)(e-s_t))/(CONFIG_COUNTER_FREQ/1000)) >= total_timeout)
            {
                upgrade_printf("total timeout 0x%x ms \n", total_timeout);
                break;
            }
        }
	}
	return length;
}

U8 * upgrade_get_send_buff(void)
{
	return upgrade_send_buffer;
}

volatile PK_STATE upgrade_state = PK_STATUS_IDLE;


int upgrade_package_data(U16 cmd, u8* buff, U16 length)
{
	U32 crc;
	U16 len = length;
	
	if(NULL == buff)
	{
		len = 0;
	}
	if(len > (UPGRADE_SEND_BUFF_LEN - UPGRADE_HDR_FORMAT_LEN))
	{
		upgrade_printf("payload length too big(0x%x) bytes\n", length);
		return -1;
	}

	memset(upgrade_send_buffer, 0, UPGRADE_SEND_BUFF_LEN);

	upgrade_send_buffer[0] = PK_HEADER;
	upgrade_send_buffer[1] = (U8)((cmd & 0xFF00) >> 8);
	upgrade_send_buffer[2] = (U8)(cmd & 0x00FF);
	upgrade_send_buffer[3] = (U8)((len & 0xFF00) >> 8);
	upgrade_send_buffer[4] = (U8)(len & 0x00FF);
	if(NULL != buff)
	{
        memcpy(&upgrade_send_buffer[5], buff, len);
	}

	crc = crc32(0, upgrade_send_buffer, len+5);

	upgrade_send_buffer[len + 5] = (U8)((crc & 0xFF000000) >> 24);
	upgrade_send_buffer[len + 6] = (U8)((crc & 0x00FF0000) >> 16);
	upgrade_send_buffer[len + 7] = (U8)((crc & 0x0000FF00) >> 8);
	upgrade_send_buffer[len + 8] = (U8)((crc & 0x000000FF) >> 0);
	upgrade_send_buffer[5 + len + 4] = PK_TAIL;

	return (UPGRADE_HDR_FORMAT_LEN + len);
}

U16 upgrade_get_cmd(u8 *buff, U32 len)
{
    if ((1 == len) && (PK_HEADER == buff[0]) && (upgrade_state == PK_STATUS_IDLE)) {
        upgrade_recv_data_type = PK_CMD_ENTER;
        upgrade_printf("recv start char 0x7E, cmd %d\n", upgrade_recv_data_type);
    } else {
        upgrade_recv_data_type = UPGRADE_GET_U16(buff[1], buff[2]);
    }
    return upgrade_recv_data_type;
}

void upgrade_get_start_data(u8 *buff, U32 len)
{
    ram_save_addr =
        UPGRADE_GET_U32(buff[PROTOCOL_PAYLOAD_OFFSET + 0], buff[PROTOCOL_PAYLOAD_OFFSET + 1],
                        buff[PROTOCOL_PAYLOAD_OFFSET + 2], buff[PROTOCOL_PAYLOAD_OFFSET + 3]);

    ram_save_len =
        UPGRADE_GET_U32(buff[PROTOCOL_PAYLOAD_OFFSET + 4], buff[PROTOCOL_PAYLOAD_OFFSET + 5],
                        buff[PROTOCOL_PAYLOAD_OFFSET + 6], buff[PROTOCOL_PAYLOAD_OFFSET + 7]);
    upgrade_printf("parse ram_save_addr=0x%x, ram_save_len=0x%x \n", ram_save_addr, ram_save_len);
    d_printf("parse ram_save_addr=0x%x, ram_save_len=0x%x \n", ram_save_addr, ram_save_len);
}

void upgrade_get_exec_data(u8* buff, U32 len)
{
    exec_addr =
        UPGRADE_GET_U32(buff[PROTOCOL_PAYLOAD_OFFSET + 0], buff[PROTOCOL_PAYLOAD_OFFSET + 1],
                        buff[PROTOCOL_PAYLOAD_OFFSET + 2], buff[PROTOCOL_PAYLOAD_OFFSET + 3]);
    upgrade_printf("parse exec_addr=0x%x \n", exec_addr);
}

inline void upgrade_send_ack(int info)
{
	int data_len = upgrade_package_data(PK_CMD_REP_ACK, NULL, 0);
	if(-1 != data_len)
	{
		uart_send_buffer(select_id, upgrade_send_buffer, data_len);
		upgrade_printf("*****send ack\n");
	}
}

inline void upgrade_send_nack(int err_no)
{
    int data_len;
    U16 cmd;

    upgrade_send_nack_total++;
    if (upgrade_send_nack_total >= UPGRADE_SEND_NACK_COUNT_MAX) {
        upgrade_printf("nack total(%d), must reset to IDLE.\n", upgrade_send_nack_total);
        err_no = NACK_RESET;
    } 

    if(NACK_RESET == err_no){
        upgrade_global_para_reset();
        upgrade_printf("send nack reset and reset to idle state.\n");
    }
    cmd = (U16)(((err_no & 0xFF) << 0x8) | (PK_CMD_REP_NACK & 0xFF));
    data_len = upgrade_package_data(cmd, NULL, 0);
    if (-1 != data_len) {
        uart_send_buffer(select_id, upgrade_send_buffer, data_len);
        upgrade_printf("###### send nack:%d\n", upgrade_send_nack_total);
    }
}

inline void upgrade_send_version(int info)
{
	int data_len;
	//memset(version_curr, 0x0, 64);
	//memcpy(version_curr, (U8 *)HG_BOOTROM_VERSION_STR, strlen(HG_BOOTROM_VERSION_STR));
	
    d_printf("BDebug: begin****************\n.");
    
    data_len = upgrade_package_data(PK_CMD_REP_VER, (U8 *)version_curr, 54);
	if(-1 != data_len)
	{
		uart_send_buffer(select_id, upgrade_send_buffer, data_len);
		upgrade_printf("recv 0x7E and send version.\n");
	}
}

inline U32 upgrade_start_data_valid(void)
{
	//upgrade_printf("start addr:0x%x, limit:0x%x \n", ram_save_addr, CONFIG_SRAM_RUNTIME_BASE);
	if(UPGRADE_START_DATA_VALID_SRAM(ram_save_addr, ram_save_len))
	{
		return 1;
	}

#ifdef USE_DDR
	if(UPGRADE_START_DATA_VALID_DDR(ram_save_addr, ram_save_len))
	{
		return 1;
	}
#endif
    upgrade_printf("start addr invalid:0x%x, limit:(0x%x~0x%x) \n", ram_save_addr, CONFIG_SRAM_START, CONFIG_SRAM_BL_START);
	return 0;
}

void upgrade_global_para_reset(void)
{
    upgrade_state = PK_STATUS_IDLE;
    recv_data_len_total = 0;
	ram_save_len = 0xFFFFFFFF;
	ram_save_addr = 0xFFFFFFFF;
	upgrade_recv_data_type = 0xFFFF;
	upgrade_send_nack_total = 0;
}

inline U32 upgrade_exec_data_valid(U32 _exec_addr)
{
	upgrade_printf("write to flash addr:0x%x\n", _exec_addr);
    if(UPGRADE_SPIFLASH_ADDR_VALID(_exec_addr, ram_save_len))
	{
		return 1;
	}
#ifdef USE_DDR

#endif
    upgrade_printf("upgrade addr 0x%x invalid! \n", _exec_addr);
	return 0;
}

extern U8 fl_upgrade_if(unsigned int flash_addr, unsigned char * buff, unsigned int len);


extern int loadx_total_len;
int upv_upgrade(u32 fl_addr, u32 mem_addr, u32 len)
{
    if(len < loadx_total_len) {
        printf("write len(0x%x) less than file size(0x%x), use file size..\n", 
                len, loadx_total_len);
        len = loadx_total_len;
    }

#ifdef CONFIG_UPV_QSPI
    return fl_upgrade_if(fl_addr, (u8 *)mem_addr, len);
#else
    return -1;
#endif
}



void upgrade_flash(int info)
{
#ifdef CONFIG_UPV_QSPI
    unsigned long long t0,t1;
    unsigned long long cost; 
    t0 = arch_counter_get_current();
    d_printf("BDebug: write flash size = %d. begin. \n",ram_save_len);
    if(0 == fl_upgrade_if((exec_addr - SPIFLASH_D_BASE), 
                (u8 *)ram_save_addr, ram_save_len))
    {
        upgrade_send_ack(NACK_OK);
        // return to start status, prepare to receive other file
        // Run here, upgrade_state will be added by 1 automatically
        // and the func upgrade_global_para_reset() will be invoked 
        // upgrade_global_para_reset();
    }
    else
    {
        upgrade_send_nack(NACK_NO_FLASH);
    }
    t1 = arch_counter_get_current(); 
    t_end = arch_counter_get_current();
    cost = (unsigned long)(t1-t0)/CONFIG_COUNTER_FREQ/1000;
    cost = cost;
    d_printf("BDebug: cost = %d.\n",cost);
    d_printf("BDebug: end*************************,t0 = %d,t1 = %d. \n\n\n",t0,t1);
#else
    upgrade_send_nack(NACK_NO_FLASH);
#endif
}

int upgrade_data_check(U8 *buff, U32 len)
{
    unsigned short cmd_type, data_len;
    unsigned int crc;
    upgrade_recv_data_type = 0xFFFF;
    upgrade_recv_data_len = 0;
    U32 i = 0, print_len;

    // 0. check start character 0x7E
    if ((1 == len) && (PK_HEADER == buff[0]) && (upgrade_state == PK_STATUS_IDLE)) {
		return 0;
	}

	if(len < UPGRADE_HDR_FORMAT_LEN)
	{
		upgrade_printf("data len fail(%d)\n", len);
		upgrade_printf("char-(0x%x), upgrade_state = %d\n", buff[0], upgrade_state);
		return NACK_LESS_LEN;
	}

	// 1. check header and tail
    if(PK_HEADER_INVALID(buff[0]) || PK_HEADER_INVALID(buff[len-1]))
    {
		upgrade_printf("recv data len:%d\n", len);
		print_len = (len >= 16)?16:len;
		upgrade_printf("header or tail data err\n");
		upgrade_printf("header data:\n");
		for(i = 0; i < print_len; i++)
		{
			upgrade_printf("0x%x ", buff[i]);			
		}
		upgrade_printf("\n\n tail data:\n");
		for(i = 0; i < print_len; i++)
		{
			upgrade_printf("0x%x ", buff[(len-print_len+i)]);			
		}
		upgrade_printf("\n tail data done:\n");
		return NACK_HEADER_ERR;
	}

	cmd_type = UPGRADE_GET_U16(buff[1], buff[2]);
	data_len = UPGRADE_GET_U16(buff[3], buff[4]);
    upgrade_printf("parse data_len=0x%x, len=0x%x, cmd_type=%d\n", data_len, len, cmd_type);
	// 2. check data length and payload length
    if(PK_DATA_LEN_INVALID(data_len, len))
	{
		upgrade_printf("package (data_len-len) not match(%d-%d)\n", data_len, len);
		return NACK_LEN_ERR;
	}

	// 3. check command type
	if(cmd_type > PK_CMD_REP_ERR)
	{
		upgrade_printf("cmd_type(%d) err, max(%d)\n", cmd_type, PK_CMD_REP_ERR);
		return NACK_CMD_ERR;
	}

	// 4.check crc
	crc = UPGRADE_GET_U32(buff[len-5],buff[len-4],buff[len-3],buff[len-2]);
    if(crc != crc32(0, buff, len - 5))
	{
		upgrade_printf("crc error\n");
		return NACK_CRC_ERR;
	}

	upgrade_printf("crc ok,cmd:0x%x \n", cmd_type);
	upgrade_recv_data_type = cmd_type;
	return 0;
}

static STAT_MANAGER_ST stat_manager[8] = {{0}};

U32 process_idle(u8* buff, U32 len, bool *switch_state)
{
    U16 cmd = upgrade_get_cmd(buff, len);
    if (PK_CMD_ENTER == cmd) {
        upgrade_global_para_reset();
        upgrade_printf("send version back\n");
        *switch_state = true;
        return NACK_OK;
    } else {
        upgrade_printf("current state enter, want 0x7E, but rec %d\n", cmd);
        *switch_state = false;
        return NACK_STAT_ERR;
    }

}

U32 process_start(u8* buff, U32 len, bool *switch_state)
{
    U16 cmd = upgrade_get_cmd(buff, len);
    if(PK_CMD_CONNECT == cmd)
    {
        *switch_state = true;
        return NACK_OK;
    }
    else
    {
        upgrade_printf("current state idle, want cmd_connect, but rec %d\n", cmd);
        *switch_state = false;
        return NACK_STAT_ERR;
    }
}

U32 process_connected(u8* buff, U32 len, bool *switch_state)
{
    U16 cmd = upgrade_get_cmd(buff, len);
    if (PK_CMD_START_DATA == cmd) {
        upgrade_get_start_data(buff, len);
        if (upgrade_start_data_valid()) {
            *switch_state = true;
            return NACK_OK;
        } else {
            upgrade_printf("start data err, rec start addr:0x%x, len:0x%x\n",
                    ram_save_addr, ram_save_len);
            *switch_state = false;
            return NACK_START_DATA_ERR;
        }
    }
    else
    {
        upgrade_printf("current state connect, want cmd start_data, but rec %d\n", upgrade_recv_data_type);
        *switch_state = false;
        return NACK_STAT_ERR;
    }
}

U32 process_data(u8* buff, U32 len, bool *switch_state)
{
    U32 payload_len;
    U16 cmd = upgrade_get_cmd(buff, len);
    if ((PK_CMD_MIDST_DATA != cmd) && (PK_CMD_END_DATA != cmd)) {
        upgrade_printf("current state data_recv, want data, but rec %d\n", cmd );
        *switch_state = false;
        return NACK_STAT_ERR;
    }

    payload_len = len - UPGRADE_HDR_FORMAT_LEN;
    // data length check
    if (recv_data_len_total + payload_len > ram_save_len) {
        upgrade_printf("get data too long(want:0x%x, recv total:0x%x). \n", ram_save_len,
                recv_data_len_total + payload_len);
        return NACK_RESET;
    }

    // 拷贝数据到目标地址适当位置
    memcpy((U8 *) (ram_save_addr + recv_data_len_total),
            &buff[PROTOCOL_PAYLOAD_OFFSET], payload_len);
    recv_data_len_total += payload_len;

    // 数据发送完毕，进入 end_data 状态
    if ((ram_save_len == recv_data_len_total) && (PK_CMD_END_DATA == cmd)) {
        upgrade_printf("goto exec, len:0x%x, recv total:0x%x \n", ram_save_len,
                recv_data_len_total);
        *switch_state = true;
    } else if (ram_save_len > recv_data_len_total) {
        upgrade_printf("go on recv, len:0x%x, recv total:0x%x \n", ram_save_len,
                recv_data_len_total);
        *switch_state = false;
    }
    return NACK_OK;
}

U32 process_exec_data(u8* buff, U32 len, bool *switch_state)
{
    U16 cmd = upgrade_get_cmd(buff, len);
    if (PK_CMD_EXEC_DATA == cmd) {
        upgrade_get_exec_data(buff, len);
        if(upgrade_exec_data_valid(exec_addr))
        {
            *switch_state = true;
            return NACK_OK;
        }
        else
        {
            upgrade_printf("exec addr(0x%x) error\n", exec_addr);
            *switch_state = false;
            return NACK_EXEC_ADDR_ERR;
        }
    } else {
        upgrade_printf("current state exec_data, want execute data, but rec %d\n",
                upgrade_recv_data_type);
        *switch_state = false;
        return NACK_STAT_ERR;
    }
}

void upgrade_stat_m_init(void)
{
    stat_manager[0].stat = PK_STATUS_IDLE;  // wait 0x7E
    stat_manager[0].deal_fun = process_idle;
    stat_manager[0].success_fun = upgrade_send_version;
    stat_manager[0].fail_fun = NULL;

    stat_manager[1].stat = PK_STATUS_SEND_VER;  // wait connect cmd
    stat_manager[1].deal_fun = process_start;
    stat_manager[1].success_fun = upgrade_send_ack;
    stat_manager[1].fail_fun = upgrade_send_nack;

    stat_manager[2].stat = PK_STATUS_CONNECTED;  // wait start data
    stat_manager[2].deal_fun = process_connected;
    stat_manager[2].success_fun = upgrade_send_ack;
    stat_manager[2].fail_fun = upgrade_send_nack;

    stat_manager[3].stat = PK_STATUS_DATA;  // wait data
    stat_manager[3].deal_fun = process_data;
    stat_manager[3].success_fun = upgrade_send_ack;
    stat_manager[3].fail_fun = upgrade_send_nack;

    stat_manager[4].stat = PK_STATUS_EXEC_DATA;  // wait exec
    stat_manager[4].deal_fun = process_exec_data;
    stat_manager[4].success_fun = upgrade_flash;
    stat_manager[4].fail_fun = upgrade_send_nack;
}

void upgrade_processing(void)
{
	U32 len;
    int ret;
    int zero_len_cnt = 0;;
    bool sw;
    upgrade_stat_m_init();
	// 1. reset all global variables for upgrade process
	upgrade_global_para_reset();
	// 2. reset uart rx fifo
    serial_reset_rx_fifo(select_id);
	// 3. wait a while for uart rx fifo reset.
	mdelay(1);

	while(1)
    {
        upgrade_printf("upgrade state (%d)\n", upgrade_state);
        // 1. check state 
        if(!UPGRADE_STATE_VALID(upgrade_state)){
            upgrade_printf("upgrade_state(%d)error, send error and reset transfer\n", upgrade_state);
            upgrade_global_para_reset();
            upgrade_send_nack(NACK_RESET);
            continue;
        }

		// 2. get data from uart port.
		len = upgrade_get_serial_buffer(upgrade_recv_serial_buffer);
		if(0 == len)
		{
            zero_len_cnt++;
            if(ZERO_LEN_RESET_CNT == zero_len_cnt){
                upgrade_global_para_reset();
                zero_len_cnt = 0;
                upgrade_printf("no data %d loop, reset to IDLE\n", zero_len_cnt);
            } else {
                //upgrade_send_nack(NACK_ZERO_LEN);
                upgrade_printf("%d no data received, continue\n", zero_len_cnt);
            }
            continue;
        }
        zero_len_cnt = 0;

		// 3. data check
		ret = upgrade_data_check(upgrade_recv_serial_buffer, len);
		if(0 != ret)
		{
			upgrade_send_nack(ret);
			upgrade_printf("upgrade_data_check err(%d), send nack\n", ret);
			continue;
		}

        // 4. reset whenever received 0x7E
        if(PK_CMD_ENTER == upgrade_get_cmd(upgrade_recv_serial_buffer, len)){
			upgrade_printf("recv reset cmd, reset state and para!\n");
            upgrade_global_para_reset();
        }

        //upgrade_printf("upgrade_state(%d), process\n", upgrade_state);
        // 5. process frame
        ret = stat_manager[upgrade_state].deal_fun(upgrade_recv_serial_buffer, len, &sw);
        if(NACK_OK == ret){
            if(NULL != stat_manager[upgrade_state].success_fun){
                stat_manager[upgrade_state].success_fun(0);
            }
            if(true == sw){
                upgrade_state++;
                 upgrade_printf("deal upgrade state (%d)\n", upgrade_state);
            }
        }else {
            if(NULL != stat_manager[upgrade_state].fail_fun){
                stat_manager[upgrade_state].fail_fun(ret);
            }
        }
	}
	
	return;	
}

void upgrade_start(void)
{
	U32 count = 0;
	
	TIMER timer;
	timeout_setup_ms(&timer, TIMEOUT_BEFORE_CMD_LOOP);

	while(1)
	{
		count++;
        hw_uart_upgrade = hwp_dbgUart; 
		if(__serial_tstc(hw_uart_upgrade))
		{
			if(PK_HEADER == __serial_getc_direct(hw_uart_upgrade))
			{
				select_id = 0;
				break;
			}
		}
		
		hw_uart_upgrade = hwp_upvUart; 
		if(__serial_tstc(hw_uart_upgrade))
		{
			if(PK_HEADER == __serial_getc_direct(hw_uart_upgrade))
			{
				//select_id = 1;
				select_id = CONFIG_UPV_SERIAL_NUM;
				break;
			}
		}

		if(timeout_check(&timer))
		{
			// 总超时时间到，跳出
			upgrade_printf("\n %d count no upgrade tools, enter cmd_loop! \n", count);
			return;
		}
	}
    upgrade_printf("\n select uart %d to upgrade! \n", select_id);

	upgrade_processing();
}

#endif

