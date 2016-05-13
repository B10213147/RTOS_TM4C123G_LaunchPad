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
void pulse_train(void);

int frequency = 1;
float duty = 0.1;
int fvpb;

int key_last_state = 0;
int main(void) {
	startup();

	int key_state;
	fvpb = SysCtlClockGet();	//system clock 16MHz

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

	while(1){
		key_state = ~(GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0) | GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1) |
				GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) | GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3));

		//frequency increase 1Hz
		if((key_state & GPIO_PIN_0 != 0) && (key_last_state & GPIO_PIN_0 == 0)){
			frequency += 1;
			if(frequency>10)
			{frequency = 10;}
		}
		//frequency decrease 1Hz
		if((key_state & GPIO_PIN_1 != 0) && (key_last_state & GPIO_PIN_1 == 0)){
			frequency -= 1;
			if(frequency<1)
			{frequency = 1;}
		}
		//duty cycle increase 0.1
		if((key_state & GPIO_PIN_2 != 0) && (key_last_state & GPIO_PIN_2 == 0)){
			duty += 0.07;
			if(duty>1)
			{duty = 0.99;}
		}
		//duty cycle decrease 0.1
		if((key_state & GPIO_PIN_3 != 0) && (key_last_state & GPIO_PIN_3 == 0)){
			duty -= 0.07;
			if(duty<0)
			{duty = 0.01;}
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

	//
	// Enable the GPIO port that is used for the on-board LED.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//
	// Check if the peripheral access is enabled.
	//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
	{
	}

	//
	// Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
	// enable the GPIO pin for digital function.
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);


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
	TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
	TimerIntRegister(TIMER0_BASE, TIMER_BOTH, pulse_train);
	TimerMatchSet(TIMER0_BASE, TIMER_BOTH, 10);

	//
	// Enable the timer & timer interrupt.
	//
	TimerIntEnable(TIMER0_BASE, TIMER_CAPA_MATCH);
	TimerEnable(TIMER0_BASE, TIMER_BOTH);
}

int pin_state = 0;
void pulse_train(void){
	int nH, nL;

	//refresh nH & nL
	nH = duty * fvpb / frequency;
	nL = (1-duty) * fvpb / frequency;

	if(pin_state == 0){
		TimerMatchSet(TIMER0_BASE, TIMER_A, nH);
		pin_state = 1;
	}
	else{
		TimerMatchSet(TIMER0_BASE, TIMER_A, nL);
		pin_state = 0;
	}
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, pin_state);

	TimerIntClear(TIMER0_BASE, TIMER_CAPA_MATCH);
}

