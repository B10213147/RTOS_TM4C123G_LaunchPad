/*
 * rtos_task.c
 *
 *  Created on: May 13, 2016
 *      Author: Harvard Tseng
 */

#include "rtos.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "pulse_train.h"
#include "keys_driver.h"

void empty_task(void){
}

voidfuncptr priv_task = empty_task;
voidfuncptr sch_tab[] = {pulse_train, keys_driver, empty_task, keys_driver};
int sch_tab_size = sizeof(sch_tab);

struct rtos_task *rtos_running_task;		// Currently running task.
struct rtos_task *rtos_ready_tasks;			// List of ready to run tasks.

void rtos_task_create(voidfuncptr func, void *arg){
	struct rtos_task *task;

	disable_os();
	task = (struct rtos_task *)malloc(sizeof(struct rtos_task));
	rtos_task_insert(&rtos_ready_tasks, task);
	task->function = func;
	task->agr = arg;
	enable_os();
}

void rtos_task_delete(struct rtos_task *task){
	disable_os();
	free(task);
	enable_os();
}

void rtos_task_insert(struct rtos_task **list, struct rtos_task *task){
	task->next = *list;
	*list = task;
}

void rtos_task_remove(struct rtos_task **list, struct rtos_task *task){
	struct rtos_task *prev, *current;

	if(*list == task) *list = (*list)->next;	//first in the list
	else{
		//serch task in list
		for(prev = *list, current = (*list)->next; current != task;
				prev = current, current = current->next);
		prev->next = current->next;
	}
	task->next = 0;
}
