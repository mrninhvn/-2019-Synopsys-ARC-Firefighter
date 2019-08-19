/*author:* Ninh DH */

#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "mathh.h"
#include <string.h>
#include "arc.h"
DEV_UART *dev_uart0 = NULL;
DEV_UART *dev_uart2 = NULL;


static void delay_us(volatile int time);
static void delay_ms(volatile int z);
static void timer1_isr(void *ptr);
static void timer0_isr(void *ptr);
void uart0_init(void);
void uart2_init(void);
void readSensor1(void);
void readSensor2(void);
void flameLocation(void);
void pumpOn(void);	
void pumpOff(void);
void stepMotor(uint8_t number, uint8_t direction, uint32_t step, int stepTime);

uint8_t maxTemper1, xMax1, yMax1;
int maxTemper2, xMax2, yMax2;
double alpha1, alpha2;
double beta2;
double fx, fy;
double A = 30;

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

    }
    return E_SYS;   
}

static void delay_us(volatile int time) //1us
{
    volatile uint32_t i;
    for(i=(int)(time*1.4);i>0;i--);
}

static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
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
	char send_message_1[1];
	send_message_1[0] = '1';
	dev_uart0->uart_write(send_message_1, 1);
	uint8_t rcv_buf_0[768];
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

	maxTemper1 = rcv_buf_0[0];
	xMax1 = 0; yMax1 = 0;
	uint8_t count1 = 1;

	for (int i = 0; i < 768; i++)
	{
		if (rcv_buf_0[i] == maxTemper1)
		{
			count1++;
			xMax1 = xMax1 + i%32;
			yMax1 = yMax1 + (int)(i/24);
		}
		else if (rcv_buf_0[i] > maxTemper1)
		{
			maxTemper1 = rcv_buf_0[i];
			count1 = 1;
			xMax1 = i%32;
			yMax1 = (int)(i/24);
		}
	}

	xMax1 = (int)(xMax1/count1);
	yMax1 = (int)(yMax1/count1);

	// EMBARC_PRINTF("%d ", maxTemper1);
	// EMBARC_PRINTF("(%d, %d)\n", xMax1, yMax1);

	// for (int i = 0; i < 768; i++)
	// {
	// 	if (i%32 == 0)
	// 	{
	// 		EMBARC_PRINTF("\n");
	// 	}
	// 	EMBARC_PRINTF("%d ", rcv_buf_0[i]);
	// }
	// EMBARC_PRINTF("\n\n");
}

void readSensor2(void)
{
	char send_message_2[1];
	send_message_2[0] = '1';
	dev_uart2->uart_write(send_message_2, 1);
	uint8_t rcv_buf_2[768];
	for (int i = 0; i < 768; i++)
	{
		rcv_buf_2[i] = 0;
	}
	uint32_t rcv_cnt_2;
	uint32_t baudrate2 = 115200;
	uint32_t rd_avail_2 = 0;
	dev_uart2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail_2));
	if(rd_avail_2 > 0)
	{
		rcv_cnt_2 = dev_uart2->uart_read(rcv_buf_2, 768);		
	}

	maxTemper2 = rcv_buf_2[0];
	xMax2 = 0; yMax1 = 0;
	uint8_t count2 = 1;

	for (int i = 0; i < 768; i++)
	{
		if (rcv_buf_2[i] == maxTemper2)
		{
			count2++;
			xMax2 = xMax2 + i%32;
			yMax2 = yMax2 + (int)(i/24);
		}
		else if (rcv_buf_2[i] > maxTemper2)
		{
			maxTemper2 = rcv_buf_2[i];
			count2 = 1;
			xMax2 = i%32;
			yMax2 = (int)(i/24);
		}
	}
	xMax2 = (int)(xMax2/count2);
	yMax2 = (int)(yMax2/count2);
	// EMBARC_PRINTF("%d ", maxTemper2);
	// EMBARC_PRINTF("(%d, %d)\n", xMax2, yMax2);

	// for (int i = 0; i < 768; i++)
	// {
	// 	if (i%32 == 0)
	// 	{
	// 		EMBARC_PRINTF("\n");
	// 	}
	// 	EMBARC_PRINTF("%d ", rcv_buf_2[i]);
	// }
	// EMBARC_PRINTF("\n\n");
}

void flameLocation(void)
{
	alpha1 = PI*(11*(xMax1 - 16)/207360);
	alpha2 = PI*(11*(xMax2 - 16)/207360);
	beta2 = PI*(7*(yMax2 - 16)/864);

	double y = (A*tan1(alpha1))/(2*tan1(alpha2));
	fx = y*tan1(alpha2);
	double yv = y/(cos1(alpha2));
	fy = yv*tan1(beta2);
}

void pumpOn(void)
{
	DEV_GPIO_PTR port_pump;
	port_pump = gpio_get_dev(DW_GPIO_PORT_C);
	port_pump->gpio_open(0x10000000);
	port_pump->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000000);

	port_pump->gpio_write(0x10000000, 0x10000000);
}
	
void pumpOff(void)
{
	DEV_GPIO_PTR port_pump;
	port_pump = gpio_get_dev(DW_GPIO_PORT_C);
	port_pump->gpio_open(0x10000000);
	port_pump->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000000);

	port_pump->gpio_write(0x00000000, 0x10000000);
}

void stepMotor(uint8_t number, uint8_t direction, uint32_t step, int stepTime)
{
	DEV_GPIO_PTR PMOD3_L;
	PMOD3_L = gpio_get_dev(DW_GPIO_PORT_A);
	PMOD3_L->gpio_open(0xf9ff0000);
	PMOD3_L->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0xf9ff0000);
	switch (number)
	{
		case 1:
			if (direction == 1)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0x30c30000, 0xf0cf0000);
					delay_us(stepTime);
					PMOD3_L->gpio_write(0x20c20000, 0xf0cf0000);
					delay_us(stepTime);
				}
			}
			else if (direction == 0)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0x10c10000, 0xf0cf0000);
					delay_us(stepTime);
					PMOD3_L->gpio_write(0x00c00000, 0xf0cf0000);
					delay_us(stepTime);
				}
			}
			break;
		case 2:
			if (direction == 1)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0xc0cc0000, 0xf0cf0000);
					delay_us(stepTime);
					PMOD3_L->gpio_write(0x80c80000, 0xf0cf0000);
					delay_us(stepTime);
				}
			}
			else if (direction == 0)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0x40c40000, 0xf0cf0000);
					delay_us(stepTime);
					PMOD3_L->gpio_write(0x00c00000, 0xf0cf0000);
					delay_us(stepTime);
				}
			}
			break;
	}
}


/** @} */
