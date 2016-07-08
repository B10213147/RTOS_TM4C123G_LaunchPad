/*
 * three_axes.h
 *
 *  Created on: July 7, 2016
 *      Author: Harvard Tseng
 */

#ifndef THREE_AXES_H_
#define THREE_AXES_H_

struct axis{
	unsigned int pin;
	char dir;
	unsigned int total;
	unsigned int remain;
	unsigned int current;
	int next;
	char finished;
};

extern struct axis *x_axis;
extern struct axis *y_axis;
extern struct axis *z_axis;

extern void axes_init(void);
extern void pulse_Gen(struct axis *axis);


#endif /* THREE_AXES_H_ */
