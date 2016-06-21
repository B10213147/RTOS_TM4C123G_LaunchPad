/*
 * rtos_sched.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Harvard Tseng
 */

#include "rtos.h"

uint8_t sch_tst, sch_idx;

void rtos_sched_run(struct rtos_task *task){
	if(!rtos_running_task) while(1);

//	sch_tst = task_running;
	rtos_task_remove(&rtos_ready_tasks, task);
	rtos_running_task = task;

	enable_os();
//	(*priv_task)();
//	(*(sch_tab[sch_idx]))();
//	sch_idx++;
//	if(sch_idx == sch_tab_size / sizeof(voidfuncptr)) sch_idx = 0;
	rtos_running_task->function(rtos_running_task->agr);
	disable_os();

//	sch_tst = task_completed;

	rtos_task_insert(&rtos_ready_tasks, task);

}

struct rtos_task_list *rtos_sched_list_update(void){
	struct rtos_task_list *list;

	return list;
}

struct rtos_task *rtos_sched_remove(struct rtos_task_list **list){
	struct rtos_task *task;

	return task;
}

void rtos_sched(void){
	static struct rtos_task_list *list;
	list = rtos_sched_list_update();
	rtos_sched_run(rtos_sched_remove(&list));
	rtos_running_task = 0;
}


