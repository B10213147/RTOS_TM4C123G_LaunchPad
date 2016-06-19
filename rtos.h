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

typedef void (* voidfuncptr)();

struct rtos_pipe{
  unsigned int begin;
  unsigned int end;
  unsigned int size;
  char *data;
};

struct rtos_task{
	struct rtos_task *next;
	voidfuncptr function;
	void *agr;
};

struct rtos_task_list{
	struct rtos_task *task;
	struct rtos_task_list *next;	// next task in the list
};

// Function & variable declarations
// rtos
extern void enable_os(void);
extern void disable_os(void);
extern void rtos_Timer0_irq(void);
extern void rtos_init(uint32_t slice);

// rtos_sched
extern void rtos_sched(void);
extern void rtos_Timer0_irq();
extern uint8_t sch_tst, sch_idx;

// rtos_task
extern voidfuncptr priv_task;
extern voidfuncptr sch_tab[];
extern int sch_tab_size;

// rtos_pipe
extern unsigned int rtos_pipe_read(struct rtos_pipe *pipe, char *data, unsigned int num_bytes);
extern unsigned int rtos_pipe_write(struct rtos_pipe *pipe, char *data, unsigned int num_bytes);

#endif /* RTOS_H_ */
