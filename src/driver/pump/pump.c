#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "arc.h"
#include "pump.h"
DEV_GPIO_PTR port_pump;
static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}

void pumpOn(void)
{
	port_pump = gpio_get_dev(DW_GPIO_PORT_C);
	port_pump->gpio_open(0x10000000);
	port_pump->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000000);

	port_pump->gpio_write(0x10000000, 0x10000000);
}
	
void pumpOff(void)
{
	port_pump = gpio_get_dev(DW_GPIO_PORT_C);
	port_pump->gpio_open(0x10000000);
	port_pump->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000000);

	port_pump->gpio_write(0x00000000, 0x10000000);
}