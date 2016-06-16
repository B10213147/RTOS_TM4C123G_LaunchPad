/*
 * rtos.h
 *
 *  Created on: May 13, 2016
 *      Author: Harvard Tseng
 */

#ifndef RTOS_H_
#define RTOS_H_

#include <stdint.h>
#include <stdbool.h>

// System status
#define task_completed	0
#define task_running	1

// Function declarations
extern void sch_on(uint32_t slice);
extern void sch_int();

#endif /* RTOS_H_ */
