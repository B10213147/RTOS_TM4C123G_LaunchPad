/*
 * three_axes.c
 *
 *  Created on: July 7, 2016
 *      Author: Harvard Tseng
 */

#include "three_axes.h"
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"

#define RED		GPIO_PIN_1
#define BLUE	GPIO_PIN_2
#define GREEN	GPIO_PIN_3

struct axis *x_axis;
struct axis *y_axis;
struct axis *z_axis;

void axis_move(struct axis *axis){
	axis->finished = 'n';

	if(axis->remain > 0){
		if(axis->dir == 'r' || axis->dir == 'u'){
			GPIOPinWrite(GPIOF_BASE, axis->dir_pin, axis->dir_pin);
		}
		else{
			GPIOPinWrite(GPIOF_BASE, axis->dir_pin, 0);
		}

		axis->current = axis->next;
		pulse_Gen(axis);
		axis->remain -= axis->current;
	}

	if(axis->remain < 0){
		position_Modify(axis);
	}

	if(axis->remain == 0){
		axis->finished = 'y';
		axis->next = 0;
		GPIOPinWrite(GPIOF_BASE, axis->dir_pin, 0);
	}
}

void position_Modify(struct axis *axis){
	//inverse direction
	if(axis->dir == 'r' || axis->dir == 'u'){
		GPIOPinWrite(GPIOF_BASE, axis->dir_pin, 0);
	}
	else{
		GPIOPinWrite(GPIOF_BASE, axis->dir_pin, axis->dir_pin);
	}

	axis->remain = -axis->remain;	//prefer positive value
	axis->next = axis->remain % 5;
	do{
		axis->current = axis->next;
		pulse_Gen(axis);
		axis->remain -= axis->current;
		if(axis->next == 0) axis->next = 5;
	}while(axis->remain != 0);
}

float duty = 0.02;
uint32_t full_Period = 16000000; //unit = ticks/10ms
void pulse_Gen(struct axis *axis){
	uint32_t period = full_Period / axis->next;	//unit = ticks/cycle
	uint32_t width_L = period * (1 - duty);
	uint32_t width_H = period - width_L;
	uint8_t pin_state = 0;
	uint32_t next_ticks = TimerValueGet(TIMER1_BASE, TIMER_A) - width_L;

	while(axis->next > 0){
		if(TimerValueGet(TIMER1_BASE, TIMER_A) < next_ticks){
			if(pin_state == 0){
				next_ticks -= width_H;
				pin_state = axis->pulse_pin;
			}
			else{
				next_ticks -= width_L;
				pin_state = 0;
				axis->next--;
			}
			GPIOPinWrite(GPIOF_BASE, axis->pulse_pin, pin_state);
		}
	}
}

void axes_init(void){
	x_axis = (struct axis*)malloc(sizeof(struct axis));
	x_axis->pulse_pin = RED;
	x_axis->dir_pin = BLUE;
	x_axis->dir = 'r';
	x_axis->total = 0;
	x_axis->remain = 0;
	x_axis->current = 0;
	x_axis->next = 0;
	x_axis->finished = 'y';

	y_axis = (struct axis*)malloc(sizeof(struct axis));
	z_axis = (struct axis*)malloc(sizeof(struct axis));

	*y_axis = *x_axis;
	y_axis->pulse_pin = GREEN;
	*z_axis = *x_axis;
	z_axis->pulse_pin = BLUE;

	//
	// Enable the GPIO port that is used for the on-board LED.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	//
	// Check if the peripheral access is enabled.
	//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1))
	{
	}

	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	//
	// Configure the 32-bit periodic timer.
	//
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

	TimerLoadSet(TIMER1_BASE, TIMER_A, 0xffffffff);

	TimerEnable(TIMER1_BASE, TIMER_A);
}
