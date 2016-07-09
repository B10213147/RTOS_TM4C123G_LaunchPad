/*
 * three_axes_pwm.h
 *
 *  Created on: July 9, 2016
 *      Author: Harvard Tseng
 */

#ifndef THREE_AXES_PWM_H_
#define THREE_AXES_PWM_H_

struct axis_pwm{
	unsigned int pulse_pin;
	unsigned int dir_pin;
	char dir;
	int total;
	int remain;
	int current;
	int next;
	char finished;
};

extern struct axis_pwm *x_axis_pwm;
extern struct axis_pwm *y_axis_pwm;
extern struct axis_pwm *z_axis_pwm;

extern void axes_pwm_init(void);
extern void start_pwm_X(void);
extern void pwm_X_GEN(void);



#endif /* THREE_AXES_PWM_H_ */
