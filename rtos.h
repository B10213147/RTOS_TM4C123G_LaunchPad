/*
 * rtos.h
 *
 *  Created on: May 13, 2016
 *      Author: Harvard Tseng
 */

#ifndef RTOS_H_
#define RTOS_H_

// System status
#define task_completed	0
#define task_running	1

// Function declarations
extern void sch_on(unsigned int slice);
extern void sch_int();

#endif /* RTOS_H_ */
