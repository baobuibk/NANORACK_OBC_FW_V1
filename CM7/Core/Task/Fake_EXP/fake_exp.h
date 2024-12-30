/*
 * fake_exp.h
 *
 *  Created on: Dec 19, 2024
 *      Author: CAO HIEU
 */

#ifndef TASK_FAKE_EXP_FAKE_EXP_H_
#define TASK_FAKE_EXP_FAKE_EXP_H_
#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"
#include "../../BSP/CMDLine_M7/vt100.h"
#include "../Peripherials/RTC/rtc_rv3149c3.h"
typedef struct {
    bool enabled;      // Trạng thái bật/tắt TEC
    float temperature; // Nhiệt độ thiết lập
    float currentTemp; // Nhiệt độ hiện tại (dùng cho "get")
} TEC_Status_t;
typedef struct {
	uint8_t laser_slot;
    uint16_t current;
    float laser_voltage;
    float photo_voltage;
} Laser_Status_t;

extern TEC_Status_t tec_status ;
extern Laser_Status_t laser_status ;
extern volatile uint8_t counter_fake;
extern volatile uint8_t monitor;

void FAKEEXP_create_task(void);
#endif /* TASK_FAKE_EXP_FAKE_EXP_H_ */
