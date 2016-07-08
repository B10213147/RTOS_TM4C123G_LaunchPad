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

float duty = 0.001;
uint32_t full_Period = 160000; //unit = ticks/10ms

void pulse_Gen(struct axis *axis){
	uint32_t period = full_Period / axis->next;	//unit = ticks/cycle
	uint32_t width_L = period * (1 - duty);
	uint32_t width_H = period - width_L;
	uint8_t pin_state = 0;
	uint32_t next_ticks = TimerValueGet(TIMER1_BASE, TIMER_A) - width_L;

	while(1){
		if(TimerValueGet(TIMER1_BASE, TIMER_A) < next_ticks){
			if(pin_state == 0){
				next_ticks -= width_H;
				pin_state = axis->pin;
			}
			else{
				next_ticks -= width_L;
				pin_state = 0;
				axis->next--;
			}

			if(axis->next <= 0) {
				GPIOPinWrite(GPIOF_BASE, axis->pin, 0);
				break;
			}

			GPIOPinWrite(GPIOF_BASE, axis->pin, pin_state);
		}
	}
}

void axes_init(void){
	x_axis = (struct axis*)malloc(sizeof(struct axis));
	x_axis->pin = RED;
	x_axis->dir = 'r';
	x_axis->total = 0;
	x_axis->remain = 0;
	x_axis->current = 0;
	x_axis->next = 0;
	x_axis->finished = 'y';

	y_axis = (struct axis*)malloc(sizeof(struct axis));
	z_axis = (struct axis*)malloc(sizeof(struct axis));

	*y_axis = *x_axis;
	y_axis->pin = GREEN;
	*z_axis = *x_axis;
	z_axis->pin = BLUE;

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
