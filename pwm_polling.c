/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "gpio.h"
#include "sysctl.h"
#include "pwm.h"

void startup(void);
void pulse_train(void);

int frequency = 1;
float duty = 0.1;

int key_last_state = 0;
int main(void) {
	startup();

	int key_state, period, width;

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
		period = SysCtlPWMClockGet() / frequency;
		width = period * duty;
		PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, period);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, width);
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
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	//
	// Enable the peripherals used by this example.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	//
	// Check if the peripheral access is enabled.
	//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
	{
	}

	SysCtlPWMClockSet(SYSCTL_PWMDIV_4);	// 16MHz/4 = 4MHz

	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_STOP);

	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
}


