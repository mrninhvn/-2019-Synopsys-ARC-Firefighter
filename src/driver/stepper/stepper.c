#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "arc.h"
#include "stepper.h"
DEV_GPIO_PTR port_pump;

static void delay_us(volatile int time) //1us
{
	volatile uint32_t i;
	for(i=(int)(time*1.4);i>0;i--);
}

void stepMotor(uint8_t number, uint8_t direction, uint32_t step)
{
	static DEV_GPIO_PTR PMOD3_L;
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
					delay_us(200);
					PMOD3_L->gpio_write(0x20c20000, 0xf0cf0000);
					delay_us(200);
				}
			}
			else if (direction == 0)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0x10c10000, 0xf0cf0000);
					delay_us(200);
					PMOD3_L->gpio_write(0x00c00000, 0xf0cf0000);
					delay_us(200);
				}
			}
			break;
		case 2:
			if (direction == 1)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0xc0cc0000, 0xf0cf0000);
					delay_us(200);
					PMOD3_L->gpio_write(0x80c80000, 0xf0cf0000);
					delay_us(200);
				}
			}
			else if (direction == 0)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0x40c40000, 0xf0cf0000);
					delay_us(200);
					PMOD3_L->gpio_write(0x00c00000, 0xf0cf0000);
					delay_us(200);
				}
			}
			break;
	}
}