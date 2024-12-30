/*
 * dualcore_cmd.c
 *
 *  Created on: Dec 19, 2024
 *      Author: CAO HIEU
 */


/*
 * rtc_update.c
 *
 *  Created on: Nov 20, 2024
 *      Author: CAO HIEU
 */
#include "scheduler.h"
#include "ring_buffer.h"
#include "main.h"
#include "stdio.h"
#include "../BSP/JSMN/jsmn.h"

static void DUALCORECMD_update_task(void);

/*Private typedef*/
typedef struct DUALCORECMD_TaskContextTypedef
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
}DUALCORECMD_TaskContextTypedef;

static DUALCORECMD_TaskContextTypedef		dualcorecmd_task_context =
{
		SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
		{
			SCH_TASK_SYNC,                      // taskType;
			SCH_TASK_PRIO_0,                    // taskPriority;
			10,                               // taskPeriodInMS;
			DUALCORECMD_update_task,					// taskFunction;
			0							//taskTick
		}

};

void DUALCORECMD_create_task(void)
{
	SCH_TASK_CreateTask(&dualcorecmd_task_context.taskHandle, &dualcorecmd_task_context.taskProperty);
}

static void DUALCORECMD_update_task(void){
    size_t len = 0;
    void* addr;
	while ((len = ringbuff_get_linear_block_read_length(rb_cm7_to_cm4)) > 0) {
	    addr = ringbuff_get_linear_block_read_address(rb_cm7_to_cm4);

//	    HAL_UART_Transmit(&huart1, addr, len, 1000);
	    printf("Length: %d, Content: %.*s\r\n", len, len, (char *)addr);
	    char content[len + 1];
	    strncpy(content, (char *)addr, len);
	    content[len] = '\0';

	    parse_and_print_json(content);

	    printf("\r\nSent: Roger!\r\n");
	    ringbuff_write(rb_cm4_to_cm7, "\r\n[CM4]: Roger!\r\n" , strlen("\r\n[CM4]: Roger!\r\n"));

	    ringbuff_skip(rb_cm7_to_cm4, len);
	}

}

