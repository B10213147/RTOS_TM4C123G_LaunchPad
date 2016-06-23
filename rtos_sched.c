/*
 * rtos_sched.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Harvard Tseng
 */

#include "rtos.h"

void rtos_sched_run(struct rtos_task *task){
	rtos_task_remove(&rtos_ready_tasks, task);
	rtos_running_task = task;

	enable_os();
	rtos_running_task->function(rtos_running_task->agr);
	disable_os();

	rtos_task_insert(&rtos_ready_tasks, task);

}

struct rtos_task_list *rtos_sched_list_update(void){
	struct rtos_task_list *list;

	return list;
}

struct rtos_task *rtos_sched_remove(struct rtos_task_list **list){
	struct rtos_task *task;
	struct rtos_task_list *next;

	task = (*list)->task;
	next = (*list)->next;
	free(*list);
	*list = next;

	return task;
}

void rtos_sched(void){
//	static struct rtos_task_list *list;
//	list = rtos_sched_list_update();
	struct rtos_task *first = rtos_ready_tasks;
	rtos_sched_run(first);
	rtos_running_task = 0;
}


