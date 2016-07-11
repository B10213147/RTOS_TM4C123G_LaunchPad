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
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"

#define RED		GPIO_PIN_1
#define BLUE	GPIO_PIN_2
#define GREEN	GPIO_PIN_3

struct axis *x_axis;
struct axis *y_axis;
struct axis *z_axis;

void axis_move(struct axis *axis){
//	axis->working = 'n';

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
		axis->working = 'n';
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

float duty = 0.01;
uint32_t full_Period = 160000; //unit = ticks/10ms
char pulse_Gen(struct axis *axis){
	uint32_t period = full_Period / axis->current;	//unit = ticks/cycle
	uint32_t width_L = period * (1 - duty);
	uint32_t width_H = period - width_L;

	if(axis->working == false){
		axis->next_ticks = TimerValueGet(TIMER2_BASE, TIMER_A) - width_L;
		axis->working = true;
	}

	if(TimerValueGet(TIMER2_BASE, TIMER_A) < axis->next_ticks){
		if(axis->pin_state == 0){
			axis->next_ticks -= width_H;
			axis->pin_state = axis->pulse_pin;
		}
		else{
			axis->next_ticks -= width_L;
			axis->pin_state = 0;
		}
		GPIOPinWrite(GPIOF_BASE, axis->pulse_pin, axis->pin_state);
		if(axis->pin_state == 0) return 'f';	//falling edge
		else return 'r';	//rising edge
	}

	return 0;
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
	x_axis->working = false;

	y_axis = (struct axis*)malloc(sizeof(struct axis));
	z_axis = (struct axis*)malloc(sizeof(struct axis));

	*y_axis = *x_axis;
	y_axis->pulse_pin = GREEN;
	*z_axis = *x_axis;
	z_axis->pulse_pin = BLUE;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);

	TimerLoadSet(TIMER2_BASE, TIMER_A, 0x0fffffff);

	TimerEnable(TIMER2_BASE, TIMER_A);
}
