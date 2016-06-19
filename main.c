/*
 * main.c
 */
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

void startup(void);
extern struct rtos_pipe keys_Fifo;
extern struct rtos_pipe pulse_Fifo;

int main(void) {
	startup();
	rtos_init(1000/4);	//slice = 1000us
	enable_os();

	char temp;
	while(1){
		if(rtos_pipe_read(&keys_Fifo, &temp, 1)){
			rtos_pipe_write(&pulse_Fifo, &temp, 1);
		}
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
	*((volatile uint32_t *)SYSCTL_RCGCGPIO) = 0x20;
	*((volatile uint32_t *)(GPIOF_BASE + GPIO_O_LOCK)) = 0x4C4F434B;
	*((volatile uint32_t *)(GPIOF_BASE + GPIO_O_CR)) = 0x1F;

	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIOF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIOF_BASE, GPIO_PIN_0 | GPIO_PIN_4,
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}
