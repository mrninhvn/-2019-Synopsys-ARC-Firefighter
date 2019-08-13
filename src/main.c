/*author:* Ninh DH */

#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "mathh.h"
#include <string.h>
#include "arc.h"
// DEV_UART *dev_uart0 = NULL;

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
    	send_uart0();
    	uint16_t *uartBuf;
    	uartBuf = receive_uart0();
    	for (int i = 0; i < 768; i++)
		{
			EMBARC_PRINTF("%d ", *(uartBuf + i));
		}
		EMBARC_PRINTF("\n");
    	delay_us(1500000);

  //   	uint16_t *uartBuf1;
  //   	uartBuf1 = receive_uart2();
  //   	for (int i = 0; i < 8; i++)
		// {
		// 	EMBARC_PRINTF("%d ", *(uartBuf1 + i));
		// }
		// EMBARC_PRINTF("\n");
  //   	delay_us(1000000);
    }
    return E_SYS;   
}

/** @} */
