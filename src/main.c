/*author:* Ninh DH */

#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "mathh.h"
#include <string.h>
#include "arc.h"
DEV_UART *dev_uart0 = NULL;

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

int main(void)
{
    uint32_t baudrate = 115200; 
    cpu_lock(); 
    board_init();
    int_disable(INTNO_TIMER1);  
    timer_stop(INTNO_TIMER1);
    int_handler_install(INTNO_TIMER1, timer1_isr);
    int_enable(INTNO_TIMER1);
    timer_start(TIMER_1, TIMER_CTRL_IE, BOARD_CPU_CLOCK);
    cpu_unlock();
    uart2_init();
    dev_uart0 = uart_get_dev(DW_UART_0_ID);
    dev_uart0->uart_open(baudrate);
    if (timer_present(TIMER_0))
    {
        timer_stop(TIMER_0);
        int_handler_install(INTNO_TIMER0,timer0_isr);
        int_enable(INTNO_TIMER0);
    }
    timer_start(TIMER_0,1,BOARD_CPU_CLOCK/2000);

    while (1)
    {
    	uint16_t *uartBuf;
    	uartBuf = receive_uart();
    	for (int i = 0; i < 9; i++)
		{
			EMBARC_PRINTF("%d ", *(uartBuf + i));
		}
		EMBARC_PRINTF("\n");
    	delay_us(1000000);
    }
    return E_SYS;   
}

/** @} */
