/*
 * keys_driver.h
 *
 *  Created on: Jun 18, 2016
 *      Author: Harvard Tseng
 */

#ifndef KEYS_DRIVER_H_
#define KEYS_DRIVER_H_

extern void keys_driver_init(void);
extern void keys_driver(void);

extern struct rtos_pipe keys_Fifo;

#endif /* KEYS_DRIVER_H_ */
