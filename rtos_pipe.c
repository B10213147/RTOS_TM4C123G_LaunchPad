/*
 * rtos_pipe.c
 *
 *  Created on: May 19, 2016
 *      Author: USER
 */

#include "rtos_pipe.h"

unsigned int rtos_pipe_read(struct rtos_pipe *pipe, char *data, unsigned int num_bytes){
	unsigned int i;

	for(i=0; i<num_bytes; i++){
		if(pipe->begin == pipe->end) break;
		data[i] = pipe->data[pipe->begin];
		pipe->begin = (pipe->begin + 1) % pipe->size;
	}

	return i;
}

unsigned int rtos_pipe_write(struct rtos_pipe *pipe, char *data, unsigned int num_bytes){
	unsigned int i, idx;

	for(i=0; i<num_bytes; i++){
		idx = (pipe->end + 1) % pipe->size;
		if(idx == pipe->begin) break;
		pipe->data[pipe->end] = data[i];
		pipe->end = idx;
	}

	return i;
}




