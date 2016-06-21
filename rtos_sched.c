/*
 * rtos_sched.c
 *
 *  Created on: 2016¦~6¤ë19¤é
 *      Author: USER
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


