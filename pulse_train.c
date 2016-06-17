/*
 * pulse_train.c
 *
 *  Created on: Jun 13, 2016
 *      Author: Harvard Tseng
 */

#include "pulse_train.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "rtos_pipe.h"

#define disable  0
#define enable  1
#define delay 2

char frequency[10] = {'9', '1', '4', '2'};
struct rtos_pipe pulse_Fifo = {0, 4, 10, frequency};

uint8_t pin_state = 0x00;
int count = 0;
const uint32_t period = 1000;	//unit = ms
char latch;
int output = disable;
int finish = 0;
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
			output = enable;
		}
		else{
			output = disable;
		}
	}

	if(output == enable){
		count++;
		if(count >= period/(latch-48)){
			if(pin_state == 0x00){
				pin_state = GPIO_PIN_3;
			}
			else{
				pin_state = 0x00;
				finish++;
				count = 0;
			}

			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, pin_state);

			if(finish == latch-48){
				output = delay;
			}
		}
	}
}
