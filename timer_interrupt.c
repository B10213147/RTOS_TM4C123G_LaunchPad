/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "pulse_train.h"

void startup(void);

int frequency = 1;
float duty = 0.01;
uint32_t fvpb;

int32_t key_last_state = 0;
int main(void) {
	startup();

	while(1){
		int32_t key_state = ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

		//frequency increase 1Hz
		if(((key_state & GPIO_PIN_0) != 0) && ((key_last_state & GPIO_PIN_0) == 0)){
			frequency += 1;
			if(frequency>10)
			{frequency = 10;}
		}
		//frequency decrease 1Hz
		if(((key_state & GPIO_PIN_4) != 0) && ((key_last_state & GPIO_PIN_4) == 0)){
			frequency -= 1;
			if(frequency<1)
			{frequency = 1;}
		}

		key_last_state = key_state;
	}
}

void startup(void){
	//
	// Set the clocking to run directly from the crystal.
	//
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_16MHZ);

	fvpb = SysCtlClockGet();	//system clock 16MHz

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
	*((volatile uint32_t *)(GPIO_PORTF_BASE + GPIO_O_LOCK)) = 0x4C4F434B;
	*((volatile uint32_t *)(GPIO_PORTF_BASE + GPIO_O_CR)) = 0x1F;

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4,
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

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
	// Configure the 32-bit periodic timer.
	//
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerIntRegister(TIMER0_BASE, TIMER_A, pulse_train);
	TimerLoadSet(TIMER0_BASE, TIMER_A, 10);

	//
	// Enable the timer & timer interrupt.
	//
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);
}



