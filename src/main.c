/*author:* Ninh DH */

#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "mathh.h"
#include <string.h>
#include "arc.h"
DEV_UART *dev_uart0 = NULL;
DEV_UART *dev_uart2 = NULL;

uint8_t rcv_buf_0[768];

static void delay_us(volatile int time) //1us
{
    volatile uint32_t i;
    for(i=(int)(time*1.4);i>0;i--);
}

static void timer1_isr(void *ptr)
{
    timer_int_clear(TIMER_1);
}

static void timer0_isr(void *ptr)
{
    timer_int_clear(TIMER_0);
}

void uart0_init(void)
{
	uint32_t baudrate0 = 115200;	
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0->uart_open(baudrate0);
}

void uart2_init(void)
{
	uint32_t baudrate2 = 115200;	
	dev_uart2 = uart_get_dev(DW_UART_2_ID);
	dev_uart2->uart_open(baudrate2);
}

void readSensor1(void)
{
	char send_message[1];
	send_message[0] = '1';
	dev_uart0->uart_write(send_message, 1);
	
	for (int i = 0; i < 768; i++)
	{
		rcv_buf_0[i] = 0;
	}
	uint32_t rcv_cnt_0;
	uint32_t baudrate0 = 115200;
	uint32_t rd_avail_0 = 0;
	dev_uart0->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail_0));
	if(rd_avail_0 > 0)
	{
		rcv_cnt_0 = dev_uart0->uart_read(rcv_buf_0, 768);		
	}
}

int main(void)
{
    cpu_lock(); 
    board_init();
    int_disable(INTNO_TIMER1);  
    timer_stop(INTNO_TIMER1);
    int_handler_install(INTNO_TIMER1, timer1_isr);
    int_enable(INTNO_TIMER1);
    timer_start(TIMER_1, TIMER_CTRL_IE, BOARD_CPU_CLOCK);
    cpu_unlock();
    uart0_init();
    uart2_init();
    if (timer_present(TIMER_0))
    {
        timer_stop(TIMER_0);
        int_handler_install(INTNO_TIMER0,timer0_isr);
        int_enable(INTNO_TIMER0);
    }
    timer_start(TIMER_0,1,BOARD_CPU_CLOCK/2000);

    while (1)
    {
		readSensor1();
		for (int i = 0; i < 768; i++)
		{
			EMBARC_PRINTF("%d ", rcv_buf_0[i]);
		}
		EMBARC_PRINTF("\n\n");
    	delay_us(1500000);
    }
    return E_SYS;   
}

/** @} */
