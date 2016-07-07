/*
 * pwm_polling.c
 *
 *  Created on: July 6, 2016
 *      Author: Harvard Tseng
 */

#include "pwm_polling.h"
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

void pwm_R(void){
	uint32_t period = 10000;
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, period);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, period/1000);

	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	// Turn on the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
	int num = 100;
	int count = 0;
	while(1){
		if(PWMGenIntStatus(PWM1_BASE, PWM_GEN_2, true) & PWM_INT_CNT_BD){
			PWMGenIntClear(PWM1_BASE, PWM_GEN_2, PWM_INT_CNT_BD);
			count++;
		}
		if(count >= num) break;
	}

	// Disable the PWM generator
	PWMGenDisable(PWM1_BASE, PWM_GEN_2);
	// Turn off the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
}

void pwm_B(void){
	uint32_t period = 10000;
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, period);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, period/1000);

	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	// Turn on the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);

	int num = 100;
	int count = 0;
	while(1){
		if(PWMGenIntStatus(PWM1_BASE, PWM_GEN_3, true) & PWM_INT_CNT_BD){
			PWMGenIntClear(PWM1_BASE, PWM_GEN_3, PWM_INT_CNT_BD);
			count++;
		}
		if(count >= num) break;
	}

	// Disable the PWM generator
	PWMGenDisable(PWM1_BASE, PWM_GEN_3);
	// Turn off the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
}

void pwm_G(void){
	uint32_t period = 10000;
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, period);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, period/1000);

	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	// Turn on the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);

	int num = 100;
	int count = 0;
	while(1){
		if(PWMGenIntStatus(PWM1_BASE, PWM_GEN_3, true) & PWM_INT_CNT_BD){
			PWMGenIntClear(PWM1_BASE, PWM_GEN_3, PWM_INT_CNT_BD);
			count++;
		}
		if(count >= num) break;
	}

	// Disable the PWM generator
	PWMGenDisable(PWM1_BASE, PWM_GEN_3);
	// Turn off the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
}

void pwm1_init(void){
	//Configure PWM Clock to match system
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	// Enable the peripherals used by this program.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins

	//Configure PF1,PF2,PF3 Pins as PWM
	GPIOPinConfigure(GPIO_PF1_M1PWM5);
	GPIOPinConfigure(GPIO_PF2_M1PWM6);
	GPIOPinConfigure(GPIO_PF3_M1PWM7);
	GPIOPinTypePWM(GPIOF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	//Configure PWM Options
	//PWM_GEN_2 Covers M1PWM4 and M1PWM5
	//PWM_GEN_3 Covers M1PWM6 and M1PWM7 See page 207 4/11/13 DriverLib doc
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

	PWMGenIntTrigEnable(PWM1_BASE, PWM_GEN_2, PWM_INT_CNT_BD);
	PWMGenIntTrigEnable(PWM1_BASE, PWM_GEN_3, PWM_INT_CNT_BD);
}



