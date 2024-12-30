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
#include "usbd_cdc_if.h"
#include "../Task/Fake_EXP/fake_exp.h"
#include "string.h"
void parse_and_update_laser_status(const char *content);
//#include "../BSP/JSMN/jsmn.h"

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
	while ((len = ringbuff_get_linear_block_read_length(rb_cm4_to_cm7)) > 0) {
	    addr = ringbuff_get_linear_block_read_address(rb_cm4_to_cm7);

//	    HAL_UART_Transmit(&huart1, addr, len, 1000);
	    char content[len + 1];
	    strncpy(content, (char *)addr, len);
	    content[len] = '\0';
	    if(monitor){
	    	parse_and_update_laser_status(content);
	    }else{
		    CDC_SendString(content);
	    }


//	    parse_and_print_json(content);
	    ringbuff_skip(rb_cm4_to_cm7, len);
	}

}


void parse_and_update_laser_status(const char *content) {
    if (strstr(content, "Laser Current")) {
        // Bóc giá trị current
        uint16_t current;
        if (sscanf(content, "\r\n --->[CM4]: Laser Current: %hu mA", &current) == 1) {
            laser_status.current = current;

        }
    } else if (strstr(content, "Photodiode Voltage")) {
        // Bóc giá trị photo_voltage
        float photo_voltage;
        if (sscanf(content, "\r\n --->[CM4]: *Photodiode Voltage: %f V", &photo_voltage) == 1 ||
            sscanf(content, "\r\n --->[CM4]: RPhotodiode Voltage: %f V", &photo_voltage) == 1) {
            laser_status.photo_voltage = photo_voltage;

        }
    } else {

    }
}


