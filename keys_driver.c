/*
 * keys_driver.c
 *
 *  Created on: 2016¦~6¤ë18¤é
 *      Author: USER
 */

#include "keys_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"
#include "driverlib/gpio.h"
#include "rtos_pipe.h"

char keys[10];
struct rtos_pipe keys_Fifo = {0, 0, 10, keys};

int32_t keys_last_state = 0;
void keys_driver(void){
	int32_t keys_state = ~GPIOPinRead(GPIOF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	char data;

	if((keys_state & GPIO_PIN_0) != 0 && (keys_last_state & GPIO_PIN_0) == 0){
		data = '5';
		rtos_pipe_write(&keys_Fifo, &data, 1);
	}
	if((keys_state & GPIO_PIN_4) != 0 && (keys_last_state & GPIO_PIN_4) == 0){
		data = '9';
		rtos_pipe_write(&keys_Fifo, &data, 1);
	}

	keys_last_state = keys_state;
}


