/*
 * three_axes.h
 *
 *  Created on: July 7, 2016
 *      Author: Harvard Tseng
 */

#ifndef THREE_AXES_H_
#define THREE_AXES_H_

struct axis{
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
extern void x_move(void);


#endif /* THREE_AXES_H_ */
