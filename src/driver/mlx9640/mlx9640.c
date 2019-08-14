#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "arc.h"
#include "mlx9640.h"
DEV_UART *dev_uart0 = NULL;
DEV_UART *dev_uart1 = NULL;
DEV_UART *dev_uart2 = NULL;

void uart0_init(void)
{
	uint32_t baudrate0 = 115200;	
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0->uart_open(baudrate0);
}

void uart1_init(void)
{
	uint32_t baudrate1 = 115200;	
	dev_uart1 = uart_get_dev(DW_UART_1_ID);
	dev_uart1->uart_open(baudrate1);
}

void uart2_init(void)
{
	uint32_t baudrate2 = 115200;	
	dev_uart2 = uart_get_dev(DW_UART_2_ID);
	dev_uart2->uart_open(baudrate2);
}

uint16_t * receive_uart2(void)
{  
	static uint16_t rcv_buf_2[9];
	uint32_t rcv_cnt_2;
	uint32_t baudrate2 = 115200;
	uint32_t rd_avail_2 = 0;
	dev_uart2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail_2));
	if(rd_avail_2 > 0)
	{
		rcv_cnt_2 = dev_uart2->uart_read(rcv_buf_2, rd_avail_2);		
	}
	return rcv_buf_2;
}

uint16_t * receive_uart1(void)
{  
	static uint16_t rcv_buf_1[9];
	uint32_t rcv_cnt_1;
	uint32_t baudrate1 = 115200;
	uint32_t rd_avail_1 = 0;
	dev_uart1->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail_1));
	if(rd_avail_1 > 0)
	{
		rcv_cnt_1 = dev_uart1->uart_read(rcv_buf_1, rd_avail_1);		
	}
	return rcv_buf_1;
}

uint16_t * receive_uart0(void)
{  
	static uint8_t rcv_buf_0[20];
	uint32_t rcv_cnt_0;
	uint32_t baudrate0 = 115200;
	uint32_t rd_avail_0 = 0;
	dev_uart0->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail_0));
	if(rd_avail_0 > 0)
	{
		rcv_cnt_0 = dev_uart0->uart_read(rcv_buf_0, rd_avail_0);		
	}
	return rcv_buf_0;
}

void send_uart0(void)
{
	uart0_init();
	char send_message[1];
	send_message[0] = '1';
	dev_uart0->uart_write(send_message, 1);
	// EMBARC_PRINTF("%c ", send_message[0]);
}