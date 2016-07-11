/*
 * three_axes.h
 *
 *  Created on: July 7, 2016
 *      Author: Harvard Tseng
 */

#ifndef THREE_AXES_H_
#define THREE_AXES_H_

#include <stdint.h>
#include <stdbool.h>

struct axis{
	uint8_t pulse_pin;
	uint8_t dir_pin;
	uint8_t pin_state;
	int32_t next_ticks;
	char dir;
	int total;
	int remain;
	int current;
	int next;
	bool working;
};

extern struct axis *x_axis;
extern struct axis *y_axis;
extern struct axis *z_axis;

extern void axes_init(void);
extern void axis_move(struct axis *axis);
extern char pulse_Gen(struct axis *axis);
extern void position_Modify(struct axis *axis);


#endif /* THREE_AXES_H_ */
