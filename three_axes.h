/*
 * three_axes.h
 *
 *  Created on: July 7, 2016
 *      Author: Harvard Tseng
 */

#ifndef THREE_AXES_H_
#define THREE_AXES_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "x_axis.h"
#include "y_axis.h"
#include "z_axis.h"

#define RED		GPIO_PIN_1
#define BLUE	GPIO_PIN_2
#define GREEN	GPIO_PIN_3

struct axis{
	uint8_t pulse_pin;
	uint8_t dir_pin;
	char dir;
	int total;
	int remain;
	int current;
	int next;
	bool working;
};

extern void axes_init(void);
extern void axis_move(struct axis *axis);
extern void position_Modify(struct axis *axis);

extern struct axis *x_axis;
extern struct axis *y_axis;
extern struct axis *z_axis;
extern const float duty;
extern const uint32_t full_Period;

#endif /* THREE_AXES_H_ */
