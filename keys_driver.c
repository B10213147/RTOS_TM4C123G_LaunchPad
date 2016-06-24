/*
 * keys_driver.c
 *
 *  Created on: Jun 18, 2016
 *      Author: Harvard Tseng
 */

#include "keys_driver.h"
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

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

void keys_driver_init(void){
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

	//
	// Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
	// enable the GPIO pin for digital function.
	//
	SYSCTL->RCGCGPIO = 0x20;
	*((volatile uint32_t *)(GPIOF_BASE + GPIO_O_LOCK)) = 0x4C4F434B;
	*((volatile uint32_t *)(GPIOF_BASE + GPIO_O_CR)) = 0x1F;

	GPIOPinTypeGPIOInput(GPIOF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIOF_BASE, GPIO_PIN_0 | GPIO_PIN_4,
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


