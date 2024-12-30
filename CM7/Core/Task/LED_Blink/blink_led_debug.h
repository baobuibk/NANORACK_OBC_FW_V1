/*
 * blink_led_debug.h
 *
 *  Created on: Nov 20, 2024
 *      Author: CAO HIEU
 */

#include "../../BSP/Scheduler_M7/scheduler.h"
#include "main.h"

#ifndef TASK_BLINK_LED_DEBUG_BLINK_LED_DEBUG_H_
#define TASK_BLINK_LED_DEBUG_BLINK_LED_DEBUG_H_

typedef	enum Led_DisplayStateTypedef
{POWERUP=0, NORMAL, POWERING_SUB, OVERCURRENT}Led_DisplayStateTypedef;

void  status_led_set_status(Led_DisplayStateTypedef status);
void  status_led_create_task(void);


#endif /* TASK_BLINK_LED_DEBUG_BLINK_LED_DEBUG_H_ */
