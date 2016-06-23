/*
 * main.c
 */
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "pulse_train.h"
#include "keys_driver.h"
#include "uart_driver.h"

void startup(void);

int main(void) {
	startup();
	rtos_task_create(pulse_train, 0);
	rtos_task_create(keys_driver, 0);
	rtos_task_create(empty_task, 0);
//	rtos_task_create(keys_driver, 0);
	rtos_task_create(uart_driver, 0);


	char temp;
	while(1){
		if(rtos_pipe_read(&uart_rx_Fifo, &temp, 1)){
			rtos_pipe_write(&uart_tx_Fifo, &temp, 1);
			rtos_pipe_write(&pulse_Fifo, &temp, 1);
		}
	}
}

void startup(void){
	//
	// Set the clocking to run directly from the crystal.
	//
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_16MHZ);

	keys_driver_init();

	pulse_train_init();

	uart_driver_init();

	rtos_init(1000/4);	//slice = 1000us
	enable_os();
}
