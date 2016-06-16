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

extern int frequency;
extern float duty;
extern uint32_t fvpb;

uint8_t pin_state = 0x00;
void pulse_train(void){
	//refresh nH & nL
	uint32_t nH = duty * fvpb / frequency;
	uint32_t nL = (1-duty) * fvpb / frequency;

	if(pin_state == 0){
		TimerLoadSet(TIMER0_BASE, TIMER_A, nH);
		pin_state = GPIO_PIN_3;
	}
	else{
		TimerLoadSet(TIMER0_BASE, TIMER_A, nL);
		pin_state = 0x00;
	}
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, pin_state);

	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}


