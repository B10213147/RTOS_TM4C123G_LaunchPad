/*
 * pulse_train.h
 *
 *  Created on: Jun 13, 2016
 *      Author: Harvard Tseng
 */

#ifndef PULSE_TRAIN_H_
#define PULSE_TRAIN_H_

extern struct rtos_pipe *pulse_Fifo;

extern void pulse_train_init(void);
extern void pulse_train(void);

#endif /* PULSE_TRAIN_H_ */
