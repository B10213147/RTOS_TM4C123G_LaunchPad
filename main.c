/*
 * main.c
 */
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "pulse_train.h"
#include "keys_driver.h"
#include "uart_driver.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

void startup(void);
void print_string(char *string);

struct pulse_info *green_pulse;
int main(void) {
	startup();
	bool fadeUp = true;
	unsigned long increment = 10;
	unsigned long pwmNow = 100;
	while(1)
	{
		for(int i=0; i<16000; i++);
		if (fadeUp) {
			pwmNow += increment;
			if (pwmNow >= 320) { fadeUp = false; }
		}
		else {
			pwmNow -= increment;
			if (pwmNow <= 10) { fadeUp = true; }
		}
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,pwmNow);
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,pwmNow);
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,pwmNow);
	}
	/*
	rtos_task_create(pulse_train, green_pulse, 4);
	rtos_task_create(keys_driver, 0, 2);
	rtos_task_create(uart_driver, 0, 400);


	char temp;
	while(1){
		if(rtos_pipe_read(uart_rx_Fifo, &temp, 1)){
			rtos_pipe_write(uart_tx_Fifo, &temp, 1);
			rtos_pipe_write(pulse_Fifo, &temp, 1);
			print_string("\n\r");	//change line and start from left
		}

		if(rtos_pipe_read(keys_Fifo, &temp, 1)){
			switch(temp){
			case 'R':
				print_string("Right botton pressed\n\r");
				break;
			case 'L':
				print_string("Left botton pressed\n\r");
				break;
			}
		}
	}
	 */
}

void print_string(char *string){
	int byte;
	for(byte = 0; string[byte] != 0; byte++);

	rtos_pipe_write(uart_tx_Fifo, string, byte);
}

void startup(void){
	//
	// Set the clocking to run directly from the crystal.
	//
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_16MHZ);

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
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	//Set the Period (expressed in clock ticks)
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 320);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 320);

	//Set PWM duty-50% (Period /2)
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,100);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,100);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,100);

	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);

	// Turn on the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, false);

	//	rtos_init(1000/4);	//slice = 1000us
	//
	//	keys_driver_init();
	//
	//	green_pulse = pulse_train_init();
	//
	//	uart_driver_init();
	//
	//	enable_os();
}
