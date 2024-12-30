/*
 * fake_exp.c
 *
 *  Created on: Dec 19, 2024
 *      Author: CAO HIEU
 */
#include "fake_exp.h"
#include <math.h>

#include <time.h>
#include "scheduler.h"
#include "stm32h7xx_hal.h"
#include "usbd_cdc_if.h"
#include "filesystem.h"

#include "../../BSP/System_IPC/ring_buffer.h"


TEC_Status_t tec_status = { .enabled = false, .temperature = 0.0, .currentTemp = 20.2 };
Laser_Status_t laser_status = { .laser_slot = 0, .current = 0, .laser_voltage = 0 , .photo_voltage = 0};
volatile uint8_t counter_fake = 0;
volatile uint8_t monitor = 0;
void cursor_move_to_fake(int x, int y);
static void FAKEEXP_update_task(void);
void Fake_Temperature_Update() ;
/*Private typedef*/
typedef struct FAKEEXP_TaskContextTypedef
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
}FAKEEXP_TaskContextTypedef;

static FAKEEXP_TaskContextTypedef		fakeexp_task_context =
{
		SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
		{
			SCH_TASK_SYNC,                      // taskType;
			SCH_TASK_PRIO_0,                    // taskPriority;
			3000,                               // taskPeriodInMS;
			FAKEEXP_update_task,					// taskFunction;
			0							//taskTick
		}

};

void FAKEEXP_create_task(void)
{
	SCH_TASK_CreateTask(&fakeexp_task_context.taskHandle, &fakeexp_task_context.taskProperty);
}

void cursor_move_to_fake(int x, int y){
	// ->x
	//   y
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "\033[%d;%dH", (y), (x));
    CDC_SendString(buffer);
}


static void FAKEEXP_update_task(void){
	if (tec_status.enabled == true){
		 counter_fake++;
		 Fake_Temperature_Update();
	}
	if(monitor){

        const char *content = "{\"i2c\":{\"ampe\":\"0\"}}";
        ringbuff_write(rb_cm7_to_cm4, content, strlen(content));


        rtc_date_time_t softTime = {0};
        rtc_get_soft_time(&softTime);
        char buffer[128];
        snprintf(buffer, sizeof(buffer),
                 "[%04u/%02u/%02u %02u:%02u:%02u]",
                 softTime.year, softTime.month, softTime.day,
                 softTime.hour, softTime.minute, softTime.second);
		cursor_move_to_fake(55, 1);
        CDC_SendString(buffer);

		cursor_move_to_fake(60, 7);
		snprintf(buffer, sizeof(buffer), "%s", tec_status.enabled ? "Yes" : "No");
        CDC_SendString(buffer);

		cursor_move_to_fake(60, 8);
		snprintf(buffer, sizeof(buffer), "%.1f°C",tec_status.temperature);
        CDC_SendString(buffer);

		cursor_move_to_fake(60, 9);
		snprintf(buffer, sizeof(buffer), "%.1f°C",tec_status.currentTemp);
        CDC_SendString(buffer);


        HAL_Delay(5);
        char contenxt[64];
        snprintf(contenxt, sizeof(contenxt), "{\"spi\":{\"read\":\"%d\"}}", laser_status.laser_slot);

        // Gửi nội dung qua ring buffer
        ringbuff_write(rb_cm7_to_cm4, contenxt, strlen(contenxt));

		cursor_move_to_fake(60, 13);
		snprintf(buffer, sizeof(buffer), "%d", laser_status.laser_slot);
        CDC_SendString(buffer);

		cursor_move_to_fake(60, 14);
		snprintf(buffer, sizeof(buffer), "%d mA",laser_status.current);
        CDC_SendString(buffer);

		cursor_move_to_fake(60, 15);
		snprintf(buffer, sizeof(buffer), "%.2f V",laser_status.laser_voltage);
        CDC_SendString(buffer);

		cursor_move_to_fake(60, 16);
		snprintf(buffer, sizeof(buffer), "%.2f V",laser_status.photo_voltage);
        CDC_SendString(buffer);


        char date[14]; // Định dạng: YYYY-MM-DD
        snprintf(date, sizeof(date), "%04u-%02u-%02u",
        		softTime.year, softTime.month, softTime.day);

        char time[12]; // Định dạng: HH:MM:SS
        snprintf(time, sizeof(time), "%02u:%02u:%02u",
        		softTime.hour, softTime.minute, softTime.second);

        // Tạo chuỗi JSON từ cấu trúc dữ liệu
        char json_message[128];
        snprintf(json_message, sizeof(json_message),
                 "{\"TEC\":{\"enabled\":%s,\"temp\":%.1f,\"currentTemp\":%.1f},"
                 "\"Laser\":{\"slot\":%u,\"current\":%u,\"laserVolt\":%.2f,\"photoVolt\":%.2f}}",
                 tec_status.enabled ? "true" : "false",
                 tec_status.temperature, tec_status.currentTemp,
                 laser_status.laser_slot, laser_status.current,
                 laser_status.laser_voltage, laser_status.photo_voltage);

        // Tạo log struct
        log_struct_t log = {0};
        strncpy(log.date, date, sizeof(log.date) - 1);
        strncpy(log.time, time, sizeof(log.time) - 1);
        strncpy(log.actTime, time, sizeof(log.actTime) - 1); // Bằng với thời gian hiện tại
        strncpy(log.message, json_message, sizeof(log.message) - 1);
        log.level = LOG_NOTICE;
        strncpy(log.core, "M7", sizeof(log.core) - 1);
        strncpy(log.function, "log_data", sizeof(log.function) - 1);
        log.line = 0;

        // Tạo chuỗi log
        char log_output[256];
        log_create(log, log_output);

        // Ghi log vào file
        if (SDFS_State == SDFS_READY) {
            if (Vim_SDFS("data.log", log_output) == 0) {
                // Nội dung ghi thành công
            } else {
                CDC_SendString("\r\nFailed to write content.\r\n");
            }
        }


	}

}



// Hàm cập nhật giá trị nhiệt độ hiện tại
void Fake_Temperature_Update() {
    static bool initialized = false;

    // Khởi tạo seed cho hàm random nếu chưa được khởi tạo
    if (!initialized) {
        uint32_t seed = HAL_GetTick(); // Lấy số tick từ khi hệ thống khởi động
        srand(seed);
    }

    // Tính khoảng chênh lệch giữa nhiệt độ thiết lập và nhiệt độ hiện tại
    float delta = tec_status.temperature - tec_status.currentTemp;

    // Tính toán bước tiến dần về nhiệt độ thiết lập
    float adjustment = delta * 0.05; // Tiến gần 5% mỗi lần gọi
    tec_status.currentTemp += adjustment;

    // Thêm nhiễu ngẫu nhiên trong khoảng ±0.5°C
    float random_noise = ((float)rand() / RAND_MAX) * 1.0 - 0.5;
    tec_status.currentTemp += random_noise;

    // Giới hạn giá trị nhiệt độ trong khoảng nhiệt độ thiết lập ±0.5°C để tránh vượt quá
    if (fabs(tec_status.currentTemp - tec_status.temperature) < 0.5) {
        tec_status.currentTemp = tec_status.temperature - random_noise;
    }
    if(counter_fake > 40){
		if (fabs(tec_status.currentTemp - tec_status.temperature) > 1) {
			tec_status.currentTemp = tec_status.temperature - random_noise;
		}
		if (fabs(tec_status.currentTemp - tec_status.temperature) > 2) {
			tec_status.currentTemp = tec_status.temperature + random_noise;
		}
		counter_fake = 41;
	}
}

