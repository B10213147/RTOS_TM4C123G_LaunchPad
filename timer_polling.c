/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "gpio.h"
#include "sysctl.h"
#include "timer.h"

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
    // Set the clocking to run directly from the crystal.
    //
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	                       SYSCTL_XTAL_16MHZ);

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

	//
	// Enable the peripherals used by this example.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	//
	// Check if the peripheral access is enabled.
	//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
	{
	}

    //
    // Configure the two 32-bit periodic timers.
    //
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());

    //
    // Enable the timers.
    //
	TimerEnable(TIMER0_BASE, TIMER_A);
}
