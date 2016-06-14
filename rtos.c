/*
 * rtos.c
 *
 *  Created on: May 13, 2016
 *      Author: Harvard Tseng
 */

#include "rtos.h"
#include "rtos_task.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

// System variables
uint8_t sch_tst, sch_idx;
uint32_t slice_quantum;

// Enable IRQ interrupts
void enable_irq()
{
	__asm(" stmfd sp!, {r0}");
	__asm(" mrs   r0, primask");
	__asm(" bic   r0, r0, #0x1");
	__asm(" msr   primask, r0");
	__asm(" ldmfd sp!, {r0}");
}

// Disable IRQ interrupts
void disable_irq()
{
	__asm(" stmfd sp!, {r0}");
	__asm(" mrs   r0, primask");
	__asm(" orr   r0, r0, #0x1");
	__asm(" msr   primask, r0");
	__asm(" ldmfd sp!, {r0}");
}

// Real time operating system core
void sch_int(void){
	if(sch_tst == task_running) while(1);
	sch_tst = task_running;
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	disable_irq();
//	(*priv_task)();
	(*(sch_tab[sch_idx]))();
	sch_idx++;
	if(sch_idx == sch_tab_size / sizeof(voidfuncptr)) sch_idx = 0;
	enable_irq();
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
