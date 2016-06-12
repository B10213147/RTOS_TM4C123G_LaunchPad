/*
 * rtos_task.c
 *
 *  Created on: May 13, 2016
 *      Author: USER
 */

#include "rtos_task.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

void empty_task(void);
voidfuncptr priv_task;
voidfuncptr sch_tab[] = {empty_task};
int sch_tab_size;


void empty_task(void){

}


