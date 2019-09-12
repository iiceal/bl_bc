#ifndef _UART_16550_H_
#define _UART_16550_H_

//#ifdef UART_RX_IRQ
#define UART_RECV_BUFFER_SIZE   256 //(HWP_SERIAL_RX_BUF_MAX) 
//#endif

typedef enum {
    UART_EVEN_PARITY_SELECT_E_ODD = 0,
    UART_EVEN_PARITY_SELECT_E_EVEN
}UART_EVEN_PARITY_SELECT_E;

typedef enum {
    UART_STOP_BIT_NUM_E_1 = 0,
    UART_STOP_BIT_NUM_E_1p5_2
}UART_STOP_BIT_NUM_E;

typedef enum {
    UART_DATA_LEN_SELECT_E_5 = 0,
    UART_DATA_LEN_SELECT_E_6,
    UART_DATA_LEN_SELECT_E_7,
    UART_DATA_LEN_SELECT_E_8
}UART_DATA_LEN_SELECT_E;

typedef struct uart_config{
    UART_DATA_LEN_SELECT_E  data_len_sel;
    BOOL parity_en;
    UART_EVEN_PARITY_SELECT_E  parity_sel;
    UART_STOP_BIT_NUM_E  stop_sel;
    U32  baud_rate;
}UART_CONFIG_ST;

typedef volatile struct
{
    U32  recv_total;                /* received data total(bytes) */
    U32  send_total;                /* send data total(bytes) */
    U32  break_int_count;    
    U32  frame_error_count; 
    U32  parity_error_count; 
    U32  overrun_error_count; 
    U32  unknown_int_count;
    U32  pad; 
}UART_STATUS_RECORD_ST;

typedef volatile struct
{
    U8 *  pwrite_point;         /* received data total(bytes) */
    U8 *  pread_point;          /* send data total(bytes) */
    UART_STATUS_RECORD_ST  uart_status_manager;    
}UART_BUFFER_MANAGER_ST;

typedef volatile struct uart_send_manager{
    U32  send_len;
    U32  send_left;
    U8   *send_buff;    
}UART_MANAGER_SEND_ST;

typedef struct uart_recv_manager{
    U8   *pwrite_point;         /* received data total(bytes) */
    U8   *pread_point;          /* send data total(bytes) */ 
    U32  total;
    U8   buff[UART_RECV_BUFFER_SIZE];
}UART_MANAGER_RECV_ST;

typedef struct uart_manager{
    UART_MANAGER_SEND_ST   uart_send;
    UART_MANAGER_RECV_ST   uart_recv;
    UART_STATUS_RECORD_ST  uart_status_manager; 
}UART_MANAGER_ST;

typedef struct uart_rx_m{
    U32 rx_len;
    U8  buff[UART_RECV_BUFFER_SIZE]; 
}UART_RX_M_ST;

/* func declaration */
extern U32 uart_init(U32 port_id, U32 baut_rate, U8 int_enable);
int uart_send_buffer(U32 port_id, void *buff, U32 len);
U32 uart_get_serial_buffer(U32 port_id, U8 *buff, U32 len, U32 timeout);
void serial_reset_rx_fifo(U32 port_id);
U32 uart_set_config(U32 port_id, UART_CONFIG_ST *config);
U32 uart_get_config(U32 port_id, UART_CONFIG_ST *config);
void uart_rx_fifo_depth_test(U32 port_id);
U32 get_fifo_addr(U32 port);

#ifdef CONFIG_DRV_DMA
int uart_send_buffer_dma(U32 ch, U32 port, void *buff, U32 len);
#endif

#endif  /*_UART_16550_H_*/
