/*
 * rtos_task.h
 *
 *  Created on: May 13, 2016
 *      Author: Harvard Tseng
 */

#ifndef RTOS_TASK_H_
#define RTOS_TASK_H_

typedef void (* voidfuncptr)();

extern voidfuncptr priv_task;
extern voidfuncptr sch_tab[];
extern int sch_tab_size;


#endif /* RTOS_TASK_H_ */
