/*
 * rtos_sched.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Harvard Tseng
 */

#include "rtos.h"

uint8_t sch_tst, sch_idx;

void rtos_sched(void){
	if(sch_tst == task_running) while(1);

	sch_tst = task_running;

	enable_os();
//	(*priv_task)();
	(*(sch_tab[sch_idx]))();
	sch_idx++;
	if(sch_idx == sch_tab_size / sizeof(voidfuncptr)) sch_idx = 0;
	disable_os();

	sch_tst = task_completed;
}


