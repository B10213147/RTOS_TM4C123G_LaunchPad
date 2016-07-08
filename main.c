/*
 * main.c
 */
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "pulse_train.h"
#include "keys_driver.h"
#include "uart_driver.h"
#include "pwm_polling.h"
#include "three_axes.h"


void startup(void);
void print_string(char *string);

struct pulse_info *green_pulse;
int main(void) {
	startup();
	axes_init();

	x_axis->total = 51;
	y_axis->total = -24;
	while(1){
		if(x_axis->finished == 'y'){
			if(x_axis->total <= 0){
				x_axis->dir = 'l';
				x_axis->remain = -x_axis->total;
			}
			else{
				x_axis->dir = 'r';
				x_axis->remain = x_axis->total;
			}
		}

		if(y_axis->finished == 'y'){
			if(y_axis->total <= 0){
				y_axis->dir = 'l';
				y_axis->remain = -y_axis->total;
			}
			else{
				y_axis->dir = 'r';
				y_axis->remain = y_axis->total;
			}
		}

		if(x_axis->next == 0) x_axis->next = 8;
		axis_move(x_axis);
		if(y_axis->next == 0) y_axis->next = 5;
		axis_move(y_axis);
	}
	/*
//	rtos_task_create(pulse_train, green_pulse, 4);
	rtos_task_create(keys_driver, 0, 2);
	rtos_task_create(uart_driver, 0, 10);
	rtos_task_create(pwm_R, 0, 100);
	rtos_task_create(pwm_G, 0, 50);
	rtos_task_create(pwm_B, 0, 100);

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

	pwm1_init();

	rtos_init(1000);	//slice = 1000us

	keys_driver_init();

	//	green_pulse = pulse_train_init();

	uart_driver_init();

	enable_os();
}
