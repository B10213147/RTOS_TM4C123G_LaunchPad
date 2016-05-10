/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "gpio.h"
#include "sysctl.h"

void startup(void);

int main(void) {
	int i;

	startup();

	while(1){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 1);
		for(i=0; i<5000; i++);

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
		for(i=0; i<5000; i++);
	}
}

void startup(void){
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
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
}
