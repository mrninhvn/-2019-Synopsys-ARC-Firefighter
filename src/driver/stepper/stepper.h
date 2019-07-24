#ifndef _STEPPER_H_
#define _STEPPER_H_


static void delay_us(volatile int time);
extern void stepMotor(uint8_t number, uint8_t direction, uint32_t step);

#endif /* _STEPPER_H_ */