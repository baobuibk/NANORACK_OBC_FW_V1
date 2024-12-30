/*
 * rtc_update.c
 *
 *  Created on: Nov 20, 2024
 *      Author: CAO HIEU
 */
#include "rtc_update.h"
#include "stdio.h"
#include "usbd_cdc_if.h"

static void RTC_update_task(void);

extern volatile rtc_date_time_t pubRtcDateTime;
extern volatile uint32_t pubEpoch;

/*Private typedef*/
typedef struct RTC_TaskContextTypedef
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
}RTC_TaskContextTypedef;

static RTC_TaskContextTypedef		rtc_task_context =
{
		SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
		{
			SCH_TASK_SYNC,                      // taskType;
			SCH_TASK_PRIO_0,                    // taskPriority;
			100,                               // taskPeriodInMS;
			RTC_update_task,					// taskFunction;
			0							//taskTick
		}

};

volatile uint16_t current_ms;

void update_time(void){
    current_ms += 100;
    if (current_ms >= 1000)
    {
    	current_ms = 0;
    	pubRtcDateTime.second++;
        if (pubRtcDateTime.second >= 60)
        {
            pubRtcDateTime.second = 0;
            pubRtcDateTime.minute++;

            if (pubRtcDateTime.minute >= 60)
            {
                pubRtcDateTime.minute = 0;
                pubRtcDateTime.hour++;

                if (pubRtcDateTime.hour >= 24)
                {
                    pubRtcDateTime.hour = 0;
                    pubRtcDateTime.day++;

                    if ((pubRtcDateTime.month == 2 && pubRtcDateTime.day > 28) ||
                        ((pubRtcDateTime.month == 4 || pubRtcDateTime.month == 6 || pubRtcDateTime.month == 9 || pubRtcDateTime.month == 11) && pubRtcDateTime.day > 30) ||
                        (pubRtcDateTime.day > 31))
                    {
                        pubRtcDateTime.day = 1;
                        pubRtcDateTime.month++;

                        if (pubRtcDateTime.month > 12)
                        {
                            pubRtcDateTime.month = 1;
                            pubRtcDateTime.year++;
                        }
                    }
                }
            }
        }
    }
}



uint32_t timing = 0;
void RTC_create_task(void)
{
	init_rtc();
	rtc_date_time_t DEFAULT_TIME;
	DEFAULT_TIME.second = 35;
	DEFAULT_TIME.minute = 59;
	DEFAULT_TIME.hour 	= 23;
	DEFAULT_TIME.day 	= 31;
	DEFAULT_TIME.month	= 12;
	DEFAULT_TIME.year	= 2024;
	if(rtc_set_date_time(&DEFAULT_TIME)){
	    CDC_SendString("RTC set date time successfully: 23:59:35 31/12/2024\r\n");
	}else{
		CDC_SendString("Fail to set date&time\r\n");
	}

	SCH_TASK_CreateTask(&rtc_task_context.taskHandle, &rtc_task_context.taskProperty);
}

static void RTC_update_task(void){
	timing++;
	update_time();
//    	(void)rtc_sync_ll();
//        CDC_SendString("%02d:%02d:%02d %02d/%02d/%04d --->>> Unix: %lu\r\n",
//               pubRtcDateTime.hour,
//               pubRtcDateTime.minute,
//               pubRtcDateTime.second,
//               pubRtcDateTime.day,
//               pubRtcDateTime.month,
//               pubRtcDateTime.year,
//			   pubEpoch);

//        float temperature = read_temperature();
//        CDC_SendString("Temperature: %.2f*C\r\n", temperature);
//        uint8_t o18 = SPI4_ReadRegister(0x18);
//        uint8_t o19 = SPI4_ReadRegister(0x19);
//        uint8_t o20 = SPI4_ReadRegister(0x20);
//        CDC_SendString("18: %d, 19: %d, 20: %d\r\n", TEMPTODEC(o18), TEMPTODEC(o19), TEMPTODEC(o20));
	if(timing > 18000){
		timing = 0;
		rtc_sync_ll();
	}

}

