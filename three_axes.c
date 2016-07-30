/*
 * three_axes.c
 *
 *  Created on: July 7, 2016
 *      Author: Harvard Tseng
 */

#include "three_axes.h"
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "inc/hw_ints.h"

#define RED		GPIO_PIN_1
#define BLUE	GPIO_PIN_2
#define GREEN	GPIO_PIN_3

void Timer2_Handler(void);
void start_pwm_X(void);
void start_pwm_Y(void);

struct axis *x_axis;
struct axis *y_axis;
struct axis *z_axis;

void axis_move(struct axis *axis){
	axis->working = false;

	if(axis->remain > 0){
		if(axis->dir == 'r' || axis->dir == 'u'){
			GPIOPinWrite(GPIOF_BASE, axis->dir_pin, axis->dir_pin);
		}
		else{
			GPIOPinWrite(GPIOF_BASE, axis->dir_pin, 0);
		}

		axis->current = axis->next;
		//pulse_Gen(axis);
		axis->remain -= axis->current;
	}

	if(axis->remain < 0){
		position_Modify(axis);
	}

	if(axis->remain == 0){
		axis->working = true;
		axis->next = 0;
		GPIOPinWrite(GPIOF_BASE, axis->dir_pin, 0);
	}
}

void position_Modify(struct axis *axis){
	//inverse direction
	if(axis->dir == 'r' || axis->dir == 'u'){
		GPIOPinWrite(GPIOF_BASE, axis->dir_pin, 0);
	}
	else{
		GPIOPinWrite(GPIOF_BASE, axis->dir_pin, axis->dir_pin);
	}

	axis->remain = -axis->remain;	//prefer positive value
	axis->next = axis->remain % 5;
	do{
		axis->current = axis->next;
		//pulse_Gen(axis);
		axis->remain -= axis->current;
		if(axis->next == 0) axis->next = 5;
	}while(axis->remain != 0);
}

void pwm_X_GEN(void){
	if(x_axis->working == false){
		x_axis->current = 1 + (x_axis->current % 10);
		start_pwm_X();
		x_axis->working = true;
	}
}

void pwm_Y_GEN(void){
	if(y_axis->working == false){
		y_axis->current = 3 + (y_axis->current % 10);
		start_pwm_Y();
		y_axis->working = true;
	}
}

void Timer2_Handler(void){
	if(TimerIntStatus(TIMER2_BASE, true) & TIMER_CAPA_MATCH){
		// Disable the PWM generator
		PWMGenDisable(PWM1_BASE, PWM_GEN_2);
		// Turn off the Output pins
		PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
		TimerIntClear(TIMER2_BASE, TIMER_CAPA_MATCH);
		x_axis->working = false;
	}

	if(TimerIntStatus(TIMER2_BASE, true) & TIMER_CAPB_MATCH){
		// Disable the PWM generator
		PWMGenDisable(PWM1_BASE, PWM_GEN_3);
		// Turn off the Output pins
		PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
		TimerIntClear(TIMER2_BASE, TIMER_CAPB_MATCH);
		y_axis->working = false;
	}
}

float duty = 0.01;
uint32_t full_Period = 65000; //unit = ticks/10ms
void start_pwm_X(void){
	uint32_t period = full_Period / x_axis->current;	//unit = ticks/cycle
	uint32_t width_H = period * duty;

	TimerLoadSet(TIMER2_BASE, TIMER_A, x_axis->current);

	TimerEnable(TIMER2_BASE, TIMER_A);

	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, period);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, width_H);
	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	// Turn on the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
}

void start_pwm_Y(void){
	uint32_t period = full_Period / y_axis->current;	//unit = ticks/cycle
	uint32_t width_H = period * duty;

	TimerLoadSet(TIMER2_BASE, TIMER_B, y_axis->current);

	TimerEnable(TIMER2_BASE, TIMER_B);

	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, period);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, width_H);
	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	// Turn on the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
}

void axes_init(void){
	x_axis = (struct axis*)malloc(sizeof(struct axis));
	x_axis->pulse_pin = RED;
	x_axis->dir_pin = BLUE;
	x_axis->dir = 'r';
	x_axis->total = 0;
	x_axis->remain = 0;
	x_axis->current = 0;
	x_axis->next = 0;
	x_axis->working = false;

	y_axis = (struct axis*)malloc(sizeof(struct axis));
	z_axis = (struct axis*)malloc(sizeof(struct axis));

	*y_axis = *x_axis;
	y_axis->pulse_pin = GREEN;
	*z_axis = *x_axis;
	z_axis->pulse_pin = BLUE;

	//Configure PWM Clock to match system
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	// Enable the peripherals used by this program.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

	//Configure PF2,PF3 Pins as PWM
	GPIOPinConfigure(GPIO_PF1_M1PWM5);
	GPIOPinConfigure(GPIO_PF2_M1PWM6);
	GPIOPinConfigure(GPIO_PF3_M1PWM7);
	GPIOPinTypePWM(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	//Configure PB0,PB1 Pins as Timer Capture
	GPIOPinConfigure(GPIO_PB0_T2CCP0);
	GPIOPinConfigure(GPIO_PB1_T2CCP1);
	GPIOPinTypeTimer(GPIOB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//Configure PWM Options
	//PWM_GEN_3 Covers M1PWM6 and M1PWM7 See page 207 4/11/13 DriverLib doc
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

	//
	// Configure the timer captures.
	//
	TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT | TIMER_CFG_B_CAP_COUNT);
	TimerControlEvent(TIMER2_BASE, TIMER_BOTH, TIMER_EVENT_NEG_EDGE);
	TimerIntRegister(TIMER2_BASE, TIMER_A, Timer2_Handler);
	TimerIntRegister(TIMER2_BASE, TIMER_B, Timer2_Handler);
	IntPrioritySet(INT_TIMER2A, 0x01);	//set Timer2A to 1 priority
	IntPrioritySet(INT_TIMER2B, 0x01);	//set Timer2B to 1 priority
	TimerMatchSet(TIMER2_BASE, TIMER_BOTH, 0);
	TimerIntEnable(TIMER2_BASE, TIMER_CAPA_MATCH | TIMER_CAPB_MATCH);
}
