#include "common.h"
#include "serial_16550_low.h"
#include <drivers/drv_serial.h>
#include "regs/uart.h"
#include "cp15.h"
#include "regs/scm.h"

#ifdef CONFIG_DRV_DMA
#include "drivers/drv_dma.h"
#endif

static UART_CONFIG_ST uart_config[HWP_SERIAL_PORT_NUM];

#ifdef UART_IRQ_TEST
void uart_irq_test_init(U32 port_id);
int uart_irq_test_handler(int irq, void *dev_id);
#endif

UART_RX_M_ST uart_rx_m[HWP_SERIAL_PORT_NUM];
int uart_irq_handler(int irq, void *dev_id);

#ifdef UART_IRQ_CALLBACK
typedef void (*PTRFUN)(U32, U8 *, U32);
PTRFUN uart_call_back[HWP_SERIAL_PORT_NUM];
#endif

// Uart RX FIFO Depth Test
void uart_rx_fifo_depth_test(U32 port_id)
{
    U32 rx_count = 0;
    U8 data_temp;
    HWP_UART_T *hw_uart = NULL;

    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
    } else {
        return;
    }

    uart_init(port_id, 115200, 0);

    while (1) {
        mdelay(1000);
        if (hw_uart->lsr & 0x02) {
            while (hw_uart->lsr & 0x01) {
                data_temp += hw_uart->rbr_thr_dll;
                rx_count++;
            }
            printf("test end:rx fifo depth is 0x%x~\n", rx_count);
            return;
        }
        printf("testing:rx fifo doesn't overrun~\n");
    }
}

void serial_reset_rx_fifo(U32 port_id)
{
    HWP_UART_T *hw_uart = NULL;

    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
    } else {
        return;
    }

    serial_FIFO_reset(hw_uart, FIFO_RESET_RX);
}

U32 get_fifo_addr(U32 port)
{
    return (U32) serial_hwp_get(port);
}

#ifdef UART_IRQ_CALLBACK
void register_irq_call_back(U32 port, PTRFUN pfn)
{
    uart_call_back[port] = pfn;
}
#endif

int uart_irq_handler(int irq, void *dev_id)
{
    U32 flag_read = 0;
    U32 byte_count = 0;

    U32 port_id = (U32) dev_id;
    HWP_UART_T *hw_uart = serial_hwp_get(port_id);
    u32 line_status = hw_uart->lsr;
    U32 irq_id = (hw_uart->iir_fcr & 0x0F);

    // printf("irq, lsr:0x%x, id:0x%x\n", line_status, irq_id);

    if ((0x4 == irq_id) || (0x6 == irq_id) || (0xC == irq_id)) {
        if ((line_status & 0x01) != 0) {
            flag_read = 1;
            byte_count = 0;
            while (flag_read) {
                if (byte_count < UART_RECV_BUFFER_SIZE) {
                    // 读取数据
                    uart_rx_m[port_id].buff[byte_count] = hw_uart->rbr_thr_dll;
                    byte_count++;

                    //考察串口标志位，所有数据读取完毕后清除读取标志，退出读取
                    if (0x0 == (hw_uart->lsr & 0x01)) {
                        flag_read = 0;
                    }

                    // 增加对串口读取数据总长度判断，如果大于buffersize，无法继续保存，直接退出
                    if (byte_count >= UART_RECV_BUFFER_SIZE) {
                        flag_read = 0;
                    }
                }
            }
            uart_rx_m[port_id].rx_len = byte_count;
#ifdef UART_IRQ_CALLBACK
            if (NULL != uart_call_back[port_id]) {
                (*uart_call_back[port_id])(port_id, &uart_rx_m[port_id].buff[0], byte_count);
            }
#endif
        }
    } else if (0x2 == irq_id) // tx empty int
    {
    } else {
        // 记录未知中断次数
    }

    return 0;
}

void uart_irq_init(U32 port)
{
    U32 irq_id = serial_irq_num_get(port);
    if (0xFFFFFFFF != irq_id) {
        request_irq(irq_id, uart_irq_handler, NULL, (void *) port);
    }
    memset(&uart_rx_m[port].buff[0], 0, UART_RECV_BUFFER_SIZE);
}

// 串口初始化，入参可以根据项目具体情况继续修改添加
U32 uart_init(U32 port_id, U32 baut_rate, U8 int_enable)
{
    HWP_UART_T *hw_uart = NULL;
    U32 count = 0, rst_reg_val = 0;
    U8 lcr;

    uart_clock_enable(port_id);

    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
    } else {
        return 0xFFFFFFFF;
    }
    // reset uart controller, temp, shoule be replaced by scm reset interface
    rst_reg_val = hwp_apSCM->swrst_ctrl;
    rst_reg_val &= (~(0x1 << (11 + port_id)));
    hwp_apSCM->swrst_ctrl = rst_reg_val;
    mdelay(1);
    rst_reg_val |= (0x1 << (11 + port_id));
    hwp_apSCM->swrst_ctrl = rst_reg_val;


    while (((hw_uart->usr & 0x1) != 0) && (count < 1000)) {
        mdelay(1);
        count++;
    }
    if (1000 == count) {
        printf("\n uart busy, return!\n");
        return 0xFFFFFFFF;
    }

    lcr = uart_config_set(hw_uart, \
	                      UART_DATA_LEN_SELECT_E_8, \
						  UART_FCL_PARITY_DISABLE, \
	                      UART_EVEN_PARITY_SELECT_E_EVEN, \
						  UART_STOP_BIT_NUM_E_1);
    uart_para_config_init(hw_uart, lcr);
    if ((port_id >= 6) || (int_enable > 1)) /* for test only wwzz */
        hw_uart->iir_fcr = (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR /*| FCR_DMA_MODE*/ |
                            (0x3 << 4) /* tx 1/2 full */ | (0x2 << 6) /* rx 1/2 full */);
    else
        hw_uart->iir_fcr = (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR);
    // if(0x1 == int_enable)
    if (0x1 <= int_enable) {
#ifdef UART_IRQ_TEST
        uart_irq_test_init(port_id);
#endif
        uart_irq_init(port_id);
        // uart6 and uart7 use dma
        if ((port_id >= 6) || (int_enable > 1)) /* for test only wwzz */
            serial_int_enable(hw_uart, (/*IER_ELSI | IER_ERBFI | IER_ETBEI |*/ IER_PTIME));
        else
            serial_int_enable(hw_uart, (IER_ELSI | IER_ERBFI /*| IER_PTIME*/));
    } else {
        serial_int_disable(hw_uart, IER_ALL);
    }

    uart_bautrate_set(port_id, baut_rate);
    return 0;
}

// serial transfer
int uart_send_buffer(U32 port_id, void *buff, U32 len)
{
    HWP_UART_T *hw_uart = NULL;
    U32 length = len;
    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
    } else {
        return -1;
    }

    if (NULL == buff) {
        return 0;
    }

    while (length--) {
        serial_putc_hw(hw_uart, *(char *) buff++);
    }

    return len;
}

#if 0
U32 uart_get_serial_buffer(U32 port_id, U8 *buff, U32 len, U32 timeout)
{
    unsigned long long s, e;
    U32 length = 0;
    U32 total_timeout = timeout;
    U32 char_timeout = timeout; // 毫秒为单位
    HWP_UART_T *hw_uart = NULL;
    TIMER timer;

    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
    } else {
        return -1;
    }

    timeout_setup_ms(&timer, total_timeout);

    // memset(buff, 0xee, UPGRADE_RECV_BUFF_LEN);

    s = timer_get_tick();
    while (1) {
        if (__serial_tstc(hw_uart)) {
            buff[length] = __serial_getc_direct(hw_uart);
            length++;
            if (length == len) {
                break;
            }
            s = timer_get_tick(); // 更新刚收到新字符的最末时间
        }

        // 从收到第一个字符开始进行两个字符的最长超时判断，连续30毫秒没有新的字符，认为发送完毕，直接返回
        if (0 < length) {
            e = timer_get_tick();
            if (tick_to_ms((unsigned long) (e - s)) >= char_timeout) {
                // upgrade_printf("char timeout 0x%x ms, recv end. \n", char_timeout);
                break;
            }
        }

        if ((0 == length) && timeout_check(&timer)) {
            // 总超时时间到，跳出
            // upgrade_printf("total timeout 0x%x ms \n", total_timeout);
            break;
        }
    }
    // upgrade_printf("recv 0x%x bytes, 0x%x \n", length, buff[0]);
    return length;
}
#else
U32 uart_get_serial_buffer(U32 port_id, U8 *buff, U32 len, U32 timeout)
{
    unsigned long long s, e;
    U32 length = 0;
    U32 total_timeout = timeout; // 毫秒为单位
    HWP_UART_T *hw_uart = NULL;

    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
    } else {
        return -1;
    }

    // s = timer_get_tick();
    s = arch_counter_get_cntpct();
    while (1) {
        if (__serial_tstc(hw_uart)) {
            buff[length] = __serial_getc_direct(hw_uart);
            length++;
            if (length == len) {
                break;
            }
            // s = timer_get_tick(); // 更新刚收到新字符的最末时间
            s = arch_counter_get_cntpct(); // 更新刚收到新字符的最末时间
        }
        e = arch_counter_get_cntpct();
        if (((unsigned long) (e - s)/(CONFIG_COUNTER_FREQ/1000)) >= total_timeout) {
            // upgrade_printf("timeout 0x%x ms, recv end. \n", total_timeout);
            break;
        }
    }
    // upgrade_printf("recv 0x%x bytes, 0x%x \n", length, buff[0]);
    return length;
}
#endif

U32 uart_set_config(U32 port_id, UART_CONFIG_ST *config)
{
    HWP_UART_T *hw_uart = NULL;
    U8 lcr, ier;
    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
    } else {
        return -1;
    }

    if ((config->data_len_sel > UART_DATA_LEN_SELECT_E_8) ||
        (config->parity_sel > UART_EVEN_PARITY_SELECT_E_ODD) ||
        (config->stop_sel > UART_STOP_BIT_NUM_E_1p5_2)) {
        return -1;
    }

    if (config->baud_rate > 230400) {
        return -1;
    }
    lcr = uart_config_set(hw_uart, config->data_len_sel, config->parity_en, config->parity_sel,
                          config->stop_sel);
    ier = hw_uart->dlh_ier;
    hw_uart->dlh_ier = 0;
    uart_para_config_init(hw_uart, lcr);
    uart_bautrate_set(port_id, config->baud_rate);
    hw_uart->dlh_ier = ier;
    memcpy(&uart_config[port_id], config, sizeof(UART_CONFIG_ST));
    return 0;
}

U32 uart_get_config(U32 port_id, UART_CONFIG_ST *config)
{
    // HWP_UART_T *hw_uart = NULL;
    if (serial_port_valid(port_id)) {
        // hw_uart = serial_hwp_get(port_id);
        memcpy(config, &uart_config[port_id], sizeof(UART_CONFIG_ST));
        return 0;
    } else {
        return -1;
    }
}

#ifdef UART_IRQ_TEST
// interface for BB.
static UART_MANAGER_ST uart_manager[HWP_SERIAL_PORT_NUM];

void uart_irq_test_init(U32 port_id)
{
    memset((U8 *) (&uart_manager[port_id].uart_recv.buff[0]), 0x0, UART_RECV_BUFFER_SIZE);
    uart_manager[port_id].uart_recv.pread_point = (U8 *) (&uart_manager[port_id].uart_recv.buff[0]);
    uart_manager[port_id].uart_recv.pwrite_point = (U8 *) (&uart_manager[port_id].uart_recv.buff[0]);
}

BOOL uart_recv_buffer_full(U32 port_id)
{
    if (uart_manager[port_id].uart_recv.pread_point !=
        (U8 *) (&uart_manager[port_id].uart_recv.buff[0])) {
        if (uart_manager[port_id].uart_recv.pwrite_point + 1 ==
            uart_manager[port_id].uart_recv.pread_point) {
            return true;
        }
        return false;
    } else {
        if (uart_manager[port_id].uart_recv.pwrite_point + 1 ==
            (U8 *) (&uart_manager[port_id].uart_recv.buff[UART_RECV_BUFFER_SIZE])) {
            return true;
        }
        return false;
    }
}

BOOL uart_recv_buffer_empty(U32 port_id)
{
    if (uart_manager[port_id].uart_recv.pread_point ==
        uart_manager[port_id].uart_recv.pwrite_point) {
        return true;
    }
    return false;
}

int uart_irq_test_handler(int irq, void *dev_id)
{
    U32 flag_read = 0;
    U32 byte_count = 0;
    U32 i;
    U8 data_temp;

    U32 port_id = (U32) dev_id;
    HWP_UART_T *hw_uart = serial_hwp_get(port_id);
    u32 line_status = hw_uart->lsr;
    U32 irq_id = (hw_uart->iir_fcr & 0x0F);

    printf("irq enter, lsr:0x%x, id:0x%x\n", line_status, irq_id);

    if ((0x4 == irq_id) || (0x6 == irq_id) || (0xC == irq_id)) {
        if ((line_status & 0x01) != 0) {
            flag_read = 1;
            byte_count = 0;
            while (flag_read) {
                if (false == uart_recv_buffer_full(port_id)) {
                    // 读取数据
                    *(uart_manager[port_id].uart_recv.pwrite_point) = hw_uart->rbr_thr_dll;
                    uart_manager[port_id].uart_recv.total++;
                    // printf("getc:%c\n", data_temp);
                    byte_count++;
                    // 修改写buffer指针并判断是否到达buffer尾部，进行rewind
                    uart_manager[port_id].uart_recv.pwrite_point++;
                    if (uart_manager[port_id].uart_recv.pwrite_point ==
                        (U8 *) (&uart_manager[port_id].uart_recv.buff[UART_RECV_BUFFER_SIZE])) {
                        uart_manager[port_id].uart_recv.pwrite_point =
                            (U8 *) (&uart_manager[port_id].uart_recv.buff[0]);
                    }

                    //考察串口标志位，所有数据读取完毕后清除读取标志，退出读取
                    if (0x0 == (hw_uart->lsr & 0x01)) {
                        flag_read = 0;
                    }

                    // 增加对串口读取数据总长度判断，如果大于buffersize，认为硬件故障，退出
                    if ((byte_count >= UART_RECV_BUFFER_SIZE) ||
                        (uart_manager[port_id].uart_recv.total >= UART_RECV_BUFFER_SIZE)) {
                        flag_read = 0;
                    }
                } else {
                    // recv buff full, get char and desert.
                    for (i = 0; i < UART_RECV_BUFFER_SIZE; i++) {
                        if (hw_uart->lsr & 0x01) {
                            data_temp = hw_uart->rbr_thr_dll;
                            printf("full-getc:%c\n", data_temp);
                        } else {
                            break;
                        }
                    }

                    // 读取完毕多余的数据，退出
                    break;
                }
            }
        }

        // 记录此次接收到的数据总个数
        uart_manager[port_id].uart_status_manager.recv_total += byte_count;

        // 处理线错误其他中断
        if (0x6 == irq_id) {
            if ((line_status & 0x10) != 0) {
                uart_manager[port_id].uart_status_manager.break_int_count++;
            }

            if ((line_status & 0x08) != 0) {
                uart_manager[port_id].uart_status_manager.frame_error_count++;
            }

            if ((line_status & 0x04) != 0) {
                uart_manager[port_id].uart_status_manager.parity_error_count++;
            }

            if ((line_status & 0x02) != 0) {
                uart_manager[port_id].uart_status_manager.overrun_error_count++;
            }
        }
    } else if (0x2 == irq_id) // tx empty int
    {
        printf("send left:%d\n", uart_manager[port_id].uart_send.send_left);
        while (uart_manager[port_id].uart_send.send_left > 0) {
            if (serial_test_te(hw_uart)) {
                serial_putc_hw_direct(hw_uart, *uart_manager[port_id].uart_send.send_buff++);
                uart_manager[port_id].uart_send.send_left--;
                printf("int while send -left:%d\n", uart_manager[port_id].uart_send.send_left);
            } else {
                // tx empty int clear-ed.
                break;
            }
        }

        if (0 == uart_manager[port_id].uart_send.send_left) {
            printf("send over, disable txe int\n");
            // 发送完成，关闭中断
            serial_int_disable(hw_uart, IER_ETBEI /*| IER_PTIME*/);
        }
    } else {
        // 记录未知中断次数
        uart_manager[port_id].uart_status_manager.unknown_int_count++;
    }

    return 1;
}

int uart_get_serial_buffer_irq(U32 port_id, U8 *buff, U32 len, U32 timeout)
{
    HWP_UART_T *hw_uart = NULL;
    U32 irq_id = 0xFFFF;
    TIMER timer;
    U32 copy_total = 0;

    if (0 == len) {
        return 0;
    }

    if (NULL == buff) {
        return -1;
    }

    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
        irq_id = serial_irq_num_get(port_id);
    } else {
        return -1;
    }

    // set timer
    timeout_setup_ms(&timer, timeout);
    request_irq(irq_id, uart_irq_test_handler, NULL, (void *) port_id);
    serial_int_enable(hw_uart, IER_ERBFI);

    while (copy_total != len) {
        gic_mask_irq(32 + irq_id);
        if (uart_manager[port_id].uart_recv.total) {
            *(buff + copy_total) = *(uart_manager[port_id].uart_recv.pread_point);
            uart_manager[port_id].uart_recv.pread_point++;
            if (uart_manager[port_id].uart_recv.pread_point ==
                &uart_manager[port_id].uart_recv.buff[UART_RECV_BUFFER_SIZE]) {
                uart_manager[port_id].uart_recv.pread_point =
                    &uart_manager[port_id].uart_recv.buff[0];
            }
            copy_total++;
            uart_manager[port_id].uart_recv.total--;
        }

        gic_unmask_irq(32 + irq_id);

        // check timeout
        if (timeout_check(&timer)) {
            // 总超时时间到，跳出
            break;
        }
    }

    gic_mask_irq(32 + irq_id);
    serial_int_disable(hw_uart, IER_ALL);
    unrequest_irq(irq_id);
    gic_unmask_irq(32 + irq_id);

    return copy_total;
}

int uart_send_buffer_irq(U32 port_id, void *buff, U32 len)
{
    U32 irq_id = 0xFFFF;
    HWP_UART_T *hw_uart = NULL;

    if (serial_port_valid(port_id)) {
        hw_uart = serial_hwp_get(port_id);
        irq_id = serial_irq_num_get(port_id);
    } else {
        return -1;
    }

    if (NULL == buff) {
        return 0;
    }

    uart_manager[port_id].uart_send.send_len = len;
    uart_manager[port_id].uart_send.send_left = len;
    uart_manager[port_id].uart_send.send_buff = buff;
    gic_mask_irq(32 + irq_id);
    request_irq(irq_id, uart_irq_test_handler, NULL, (void *) port_id);
    serial_int_enable(hw_uart, IER_ETBEI /*| IER_PTIME*/);
    gic_unmask_irq(32 + irq_id);

    // wait for send complete
    while ((0 != uart_manager[port_id].uart_send.send_left)) {
        // continue send...
        ;
    }

    gic_mask_irq(32 + irq_id);
    serial_int_disable(hw_uart, IER_ALL);
    unrequest_irq(irq_id);
    gic_unmask_irq(32 + irq_id);

    return len;
}
#endif // UART_IRQ_TEST

#ifdef CONFIG_DRV_DMA
int uart_send_buffer_dma(U32 ch, U32 port, void *buff, U32 len)
{
    U32 dst, per_type;
    // para check
    if ((port != 6) && (port != 7) && (port != 1 /* test only */)) {
        return -1;
    }

    per_type = get_uart_tx_per(port);

    dst = get_fifo_addr(port);
    dma_m2p_single(0, ch, (U32) buff, dst, len, 8, 1, per_type, false);
    return 0;
}

int uart_get_buffer_dma(U32 ch, U32 port, void *buff, U32 len)
{
    U32 src, per_type;
    // para check
    if ((port != 6) && (port != 7) && (port != 1 /* test only */)) {
        return -1;
    }

    per_type = get_uart_rx_per(port);

    src = get_fifo_addr(port);
    v7_dma_inv_range((U32) buff, (U32) buff + len);
    dma_p2m_single(0, ch, src, (U32) buff, len, 0, 0, per_type, false);
    return 0;
}
#endif
