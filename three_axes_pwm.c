/*
 * three_axes_pwm.c
 *
 *  Created on: July 9, 2016
 *      Author: Harvard Tseng
 */

#include "three_axes_pwm.h"
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

#define RED		GPIO_PIN_1
#define BLUE	GPIO_PIN_2
#define GREEN	GPIO_PIN_3

char pwm_X_working(void);

struct axis_pwm *x_axis_pwm;
struct axis_pwm *y_axis_pwm;
struct axis_pwm *z_axis_pwm;

float duty_pwm = 0.001;
uint32_t full_Period_pwm = 65000 *3; //unit = ticks/10ms
void start_pwm_X(void){
	uint32_t period = full_Period_pwm / x_axis_pwm->current;	//unit = ticks/cycle
	uint32_t width_H = period * duty_pwm;

	TimerLoadSet(TIMER2_BASE, TIMER_A, x_axis_pwm->current);

	TimerEnable(TIMER2_BASE, TIMER_A);

	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, period);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, width_H);
	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	// Turn on the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
}

void pwm_X_GEN(void){
	if(x_axis_pwm->working == false){
		x_axis_pwm->current = 1 + (x_axis_pwm->current % 10);
		start_pwm_X();
		x_axis_pwm->working = true;
	}
}

void Timer2_Handler(void){
	if(TimerIntStatus(TIMER2_BASE, true) & TIMER_CAPA_MATCH){
		// Disable the PWM generator
		PWMGenDisable(PWM1_BASE, PWM_GEN_3);
		// Turn off the Output pins
		PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
		TimerIntClear(TIMER2_BASE, TIMER_CAPA_MATCH);
		x_axis_pwm->working = false;
	}

	if(TimerIntStatus(TIMER2_BASE, true) & TIMER_CAPB_MATCH){
		// Disable the PWM generator
		PWMGenDisable(PWM1_BASE, PWM_GEN_3);
		// Turn off the Output pins
		PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
		TimerIntClear(TIMER2_BASE, TIMER_CAPB_MATCH);
		y_axis_pwm->working = false;
	}
}


void axes_pwm_init(void){
	x_axis_pwm = (struct axis_pwm*)malloc(sizeof(struct axis_pwm));
	x_axis_pwm->pulse_pin = RED;
	x_axis_pwm->dir_pin = BLUE;
	x_axis_pwm->dir = 'r';
	x_axis_pwm->total = 0;
	x_axis_pwm->remain = 0;
	x_axis_pwm->current = 0;
	x_axis_pwm->next = 0;
	x_axis_pwm->working = false;

	y_axis_pwm = (struct axis_pwm*)malloc(sizeof(struct axis_pwm));
	z_axis_pwm = (struct axis_pwm*)malloc(sizeof(struct axis_pwm));

	*y_axis_pwm = *x_axis_pwm;
	y_axis_pwm->pulse_pin = GREEN;
	*z_axis_pwm = *x_axis_pwm;
	z_axis_pwm->pulse_pin = BLUE;

	//Configure PWM Clock to match system
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	// Enable the peripherals used by this program.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1);

	//Configure PF2,PF3 Pins as PWM
	GPIOPinConfigure(GPIO_PF2_M1PWM6);
	GPIOPinConfigure(GPIO_PF3_M1PWM7);
	GPIOPinTypePWM(GPIOF_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	//Configure PB0,PB1 Pins as Timer Capture
	GPIOPinConfigure(GPIO_PB0_T2CCP0);
	GPIOPinConfigure(GPIO_PB1_T2CCP1);
	GPIOPinTypeTimer(GPIOB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//Configure PWM Options
	//PWM_GEN_3 Covers M1PWM6 and M1PWM7 See page 207 4/11/13 DriverLib doc
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

	//
	// Configure the timer captures.
	//
	TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT | TIMER_CFG_B_CAP_COUNT);
	TimerControlEvent(TIMER2_BASE, TIMER_BOTH, TIMER_EVENT_NEG_EDGE);
	TimerIntRegister(TIMER2_BASE, TIMER_BOTH, Timer2_Handler);
	IntPrioritySet(INT_TIMER2A, 0x01);	//set Timer2A to 1 priority
	IntPrioritySet(INT_TIMER2B, 0x01);	//set Timer2B to 1 priority
	TimerMatchSet(TIMER2_BASE, TIMER_BOTH, 0);
	TimerIntEnable(TIMER2_BASE, TIMER_CAPA_MATCH | TIMER_CAPB_MATCH);
}

