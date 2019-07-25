#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "arc.h"
#include "mlx9640.h"
DEV_UART *dev_uart2 = NULL;

void uart2_init(void)
{
	uint32_t baudrate1 = 115200;	
	dev_uart2 = uart_get_dev(DW_UART_2_ID);
	dev_uart2->uart_open(baudrate1);
}

uint16_t * receive_uart(void)
{  
	static uint16_t rcv_buf[9];
	uint32_t rcv_cnt;
	uint32_t baudrate1 = 115200;
	uint32_t rd_avail = 0;
	dev_uart2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	if(rd_avail > 0)
	{
		rcv_cnt = dev_uart2->uart_read(rcv_buf, rd_avail);		
	}
	return rcv_buf;
}

