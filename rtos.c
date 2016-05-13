/*
 * rtos.c
 *
 *  Created on: May 13, 2016
 *      Author: Harvard Tseng
 */

#include "rtos.h"
#include "rtos_task.h"
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "sysctl.h"
#include "timer.h"

// System variables
int sch_tst, sch_idx, slice_quantum;

// Enable IRQ interrupts
void enable_irq()
{
	asm(" stmfd sp!, {r0}");
	asm(" mrs   r0, cpsr");
	asm(" bic   r0, r0, #0x80");
	asm(" msr   cpsr_c, r0");
	asm(" ldmfd sp!, {r0}");
}

// Disable IRQ interrupts
void disable_irq()
{
	asm(" stmfd sp!, {r0}");
	asm(" mrs   r0, cpsr");
	asm(" orr   r0, r0, #0x80");
	asm(" msr   cpsr_c, r0");
	asm(" ldmfd sp!, {r0}");
}

// Start real time operating system
// slice ... timeslice in microseconds
void sch_on(unsigned int slice){
	sch_tst = task_completed;
	sch_idx = 0;
	slice_quantum = slice;

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
	// Configure the two 32-bit periodic timers.
	//
	TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerIntRegister(TIMER0_BASE, TIMER_BOTH, sch_int);
	TimerMatchSet(TIMER0_BASE, TIMER_BOTH, slice_quantum);

	//
	// Enable the timers & timer interrupt.
	//
	TimerIntEnable(TIMER0_BASE, TIMER_CAPA_MATCH | TIMER_CAPB_MATCH);
	TimerEnable(TIMER0_BASE, TIMER_BOTH);
}

// Real time operating system core
void sch_int(){
	if(sch_tst == task_running) while(1);
	sch_tst = task_running;
	TimerIntClear(TIMER0_BASE, TIMER_CAPA_MATCH | TIMER_CAPB_MATCH);
	TimerMatchSet(TIMER0_BASE, TIMER_BOTH, TimerMatchGet(TIMER0_BASE, TIMER_A) + slice_quantum);
	if(TimerValueGet(TIMER0_BASE, TIMER_A) - TimerMatchGet(TIMER0_BASE, TIMER_A) > 0){
		TimerLoadSet(TIMER0_BASE, TIMER_A, TimerMatchGet(TIMER0_BASE, TIMER_A) - slice_quantum);
	}
	enable_irq();
	(*priv_task)();
	(*(sch_tab[sch_idx]))();
	sch_idx++;
	if(sch_idx == sch_tab_size / sizeof(voidfuncptr)) sch_idx = 0;
	disable_irq();
	sch_tst = task_completed;
}




