/*
 * uart_driver.c
 *
 *  Created on: Jun 23, 2016
 *      Author: Harvard Tseng
 */

#include "uart_driver.h"
#include "rtos.h"
#include "TM4C123GH6PM.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

void uart_init(void){
	UARTDisable(UART0_BASE);
	GPIOPinTypeUART(GPIOA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(),
			9600, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
	UARTFIFOEnable(UART0_BASE);
	UARTEnable(UART0_BASE);
}






