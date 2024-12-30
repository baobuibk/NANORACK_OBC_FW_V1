/*
 * blink_led_debug.c
 *
 *  Created on: Nov 20, 2024
 *      Author: CAO HIEU
 */

#include "blink_led_debug.h"

/* Private define ------------------------------------------------------------*/
#define	POWERUP_PERIOD	1000
#define	POWER_NORMAL_OFF_PERIOD	3000
#define	POWER_NORMAL_ON_PERIOD	500
void	status_led_update(void);
static	void	status_led_normal(void);

/* Private typedef -----------------------------------------------------------*/
typedef struct Led_TaskContextTypedef
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
	uint32_t                      taskTick;
} Led_TaskContextTypedef;

typedef	struct StatusLed_CurrentStateTypedef
{
	uint8_t				led:1;
	Led_DisplayStateTypedef	state:7;
}StatusLed_CurrentStateTypedef;

/* Private variables ---------------------------------------------------------*/

static	StatusLed_CurrentStateTypedef	s_led_display_status = {0, POWERUP};
static Led_TaskContextTypedef           s_task_context =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		500,                                // taskPeriodInMS;
		status_led_update,                // taskFunction;
		483							//taskTick
	},
};

static void status_led_on(void);
static void status_led_off(void);
static void status_led_powerup(void);

void	status_led_set_status(Led_DisplayStateTypedef status)
{
	s_led_display_status.state = status;
}


static void status_led_off(void)
{

	//LL_GPIO_ResetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
}

static void status_led_on(void)
{

	//LL_GPIO_SetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
}



void	status_led_update(void)
{

	switch (s_led_display_status.state) {
	case POWERUP:
		status_led_powerup();
		break;
	case NORMAL:
		status_led_normal();
		break;
	case POWERING_SUB:
		break;
	case OVERCURRENT:
		break;
	}
}


static void status_led_powerup(void)
{
    if (s_led_display_status.led == 1) // LED is ON
    {
        if (SCH_TIM_HasCompleted(SCH_TIM_LED))
        {
            s_led_display_status.led = 0;
            status_led_off();
            SCH_TIM_Start(SCH_TIM_LED, POWERUP_PERIOD); // restart

        }
    }
    else if (s_led_display_status.led == 0) // LED is OFF
    {
        if (SCH_TIM_HasCompleted(SCH_TIM_LED))
        {
            s_led_display_status.led = 1;
            status_led_on();
            SCH_TIM_Start(SCH_TIM_LED, POWERUP_PERIOD); // restart

        }

    }

}




static void status_led_normal(void)
{

}

void	status_led_create_task(void)
{
    s_led_display_status.led = 0;
    s_led_display_status.state = POWERUP;
    status_led_on();
	SCH_TASK_CreateTask(&s_task_context.taskHandle, &s_task_context.taskProperty);
}

