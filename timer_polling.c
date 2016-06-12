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

void startup(void);

int frequency = 1;
float duty = 0.01;
uint32_t fvpb;
int32_t key_last_state = 0;

int main(void) {
	startup();

	uint32_t nH, nL, next;
	int32_t key_state;
	uint8_t pin_state = 0x00;
	fvpb = SysCtlClockGet();

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, pin_state);
	nL = (1-duty) * fvpb / frequency;
	next = TimerValueGet(TIMER0_BASE, TIMER_A) + nL;

	while(1){
		key_state = ~(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4));
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
		//refresh nH & nL
		nH = duty * fvpb / frequency;
		nL = (1-duty) * fvpb / frequency;

		key_last_state = key_state;

		if(TimerValueGet(TIMER0_BASE, TIMER_A) > next){
			if(pin_state == 0x00){
				next += nH;
				pin_state = GPIO_PIN_3;
			}
			else{
				next += nL;
				pin_state = 0x00;
			}
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, pin_state);
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
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
	TimerMatchSet(TIMER0_BASE, TIMER_A, 0xFFFFFFFF);

	//
	// Enable the timers.
	//
	TimerEnable(TIMER0_BASE, TIMER_A);
}
