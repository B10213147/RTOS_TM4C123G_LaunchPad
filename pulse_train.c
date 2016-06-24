/*
 * pulse_train.c
 *
 *  Created on: Jun 13, 2016
 *      Author: Harvard Tseng
 */

#include "pulse_train.h"
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

#define disable	0
#define enable	1
#define delay	2

char frequency[10];
struct rtos_pipe pulse_Fifo = {0, 0, 10, frequency};

uint8_t pin_state, color;
const uint32_t period = 1000;	//unit = ms
char latch;
int count, finish, output = disable;
void pulse_train(void){
	if(output == delay){	//delay 1s
		count++;
		if(count == period){
			output = disable;
		}
	}

	if(output == disable){
		count = 0;
		finish = 0;

		if(rtos_pipe_read(&pulse_Fifo, &latch, 1)){
			if(latch>='0' && latch<='9'){	//number
				output = enable;
			}
			else if(latch=='r' || latch=='R'){
				color = GPIO_PIN_1;
			}
			else if(latch=='g' || latch=='G'){
				color = GPIO_PIN_3;
			}
			else if(latch=='b' || latch=='B'){
				color = GPIO_PIN_2;
			}
			else{
				output = disable;
			}
		}
		else{	//nothing in pipe
			output = disable;
		}
	}

	if(output == enable){
		count++;
		if(count >= period/(latch-48)){
			if(pin_state == 0x00){
				pin_state = color;
			}
			else{
				pin_state = 0x00;
				finish++;
				count = 0;
			}

			GPIOPinWrite(GPIOF_BASE, color, pin_state);

			if(finish == latch-48){
				output = delay;
			}
		}
	}
}

void pulse_train_init(void){
	count = 0;
	finish = 0;
	output = disable;

	//
	// Enable the GPIO port that is used for the on-board LED.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//
	// Check if the peripheral access is enabled.
	//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
	{
	}

	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}
