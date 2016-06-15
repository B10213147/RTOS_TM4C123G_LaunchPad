/*
 * rtos.c
 *
 *  Created on: May 13, 2016
 *      Author: Harvard Tseng
 */

#include "rtos.h"
#include "rtos_task.h"
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

// System variables
uint8_t sch_tst, sch_idx;
uint32_t slice_quantum;

// Real time operating system core
void sch_int(void){
	if(sch_tst == task_running) while(1);
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	sch_tst = task_running;

	__disable_irq();
//	(*priv_task)();
	(*(sch_tab[sch_idx]))();
	sch_idx++;
	if(sch_idx == sch_tab_size / sizeof(voidfuncptr)) sch_idx = 0;
	__enable_irq();

	sch_tst = task_completed;
}

// Start real time operating system
// slice ... timeslice in microseconds
void sch_on(uint32_t slice){
	sch_tst = task_completed;
	sch_idx = 0;
	slice_quantum = slice * (SysCtlClockGet() / 1000000);

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
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerIntRegister(TIMER0_BASE, TIMER_A, sch_int);
	TimerLoadSet(TIMER0_BASE, TIMER_A, slice_quantum);

	//
	// Enable the timer & timer interrupt.
	//
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);
}
