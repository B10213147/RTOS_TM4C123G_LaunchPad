/*
 * rtos_task.c
 *
 *  Created on: May 13, 2016
 *      Author: Harvard Tseng
 */

#include "rtos_task.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "pulse_train.h"
#include "keys_driver.h"

void empty_task(void){
}

voidfuncptr priv_task = empty_task;
voidfuncptr sch_tab[] = {pulse_train, keys_driver, empty_task, keys_driver};
int sch_tab_size = sizeof(sch_tab);
