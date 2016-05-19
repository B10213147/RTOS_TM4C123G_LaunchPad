/*
 * rtos_pipe.h
 *
 *  Created on: May 19, 2016
 *      Author: USER
 */

#ifndef RTOS_PIPE_H_
#define RTOS_PIPE_H_

struct rtos_pipe{
  unsigned int begin;
  unsigned int end;
  unsigned int size;
  char *data;
};

extern unsigned int rtos_pipe_read(struct rtos_pipe *pipe, char *data, unsigned int num_bytes);
extern unsigned int rtos_pipe_write(struct rtos_pipe *pipe, char *data, unsigned int num_bytes);

#endif /* RTOS_PIPE_H_ */
