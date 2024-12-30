/*
 * rtc_rv3149c3.c
 *
 *  Created on: Nov 20, 2024
 *      Author: CAO HIEU
 */

#include "rtc_rv3149c3.h"

volatile rtc_date_time_t pubRtcDateTime;
volatile uint32_t pubEpoch = 0;

static uint32_t prv_make_unix_time(void);
static rtc_date_time_t prv_make_date_time_from_epoch(uint32_t epoch);

static inline void prv_assert_cs(void) {
	LL_GPIO_SetOutputPin(CS_RTC_GPIO_Port, CS_RTC_Pin);
}

static inline void prv_deassert_cs(void) {
	LL_GPIO_ResetOutputPin(CS_RTC_GPIO_Port, CS_RTC_Pin);
}

void init_rtc(void) {

    //Initialize RTC
//    uint8_t init[5];
//    init[0] = COMMAND_WRITE(CONTROL_1); //Control page
//    init[1] = CONTROL_1_CLK_INT | CONTROL_1_TD_8 | CONTROL_1_SROn | CONTROL_1_EERE | CONTROL_1_WE; //Control_1
//    init[2] = CONTROL_INT_SRIE; //Control_INT
//    init[3] = 0; //Control_INT Flag
//    init[4] = 0; //Control_Status
//
//    //Enable 1 Hz Clkout
//    uint8_t clkout[2];
//    clkout[0] = COMMAND_WRITE(CONTROL_EEPROM);
//    clkout[1] = CONTROL_EEPROM_FD1 | CONTROL_EEPROM_FD0;
//
    pubRtcDateTime.second = 7;
    pubRtcDateTime.minute = 7;
    pubRtcDateTime.hour   = 7;
    pubRtcDateTime.day    = 7;
    pubRtcDateTime.month  = 7;
    pubRtcDateTime.year   = 77 + 2000;

//	uint8_t reset[2];
//	reset[0] = COMMAND_WRITE(CONTROL_RESET);
//	reset[1] = CONTROL_RESET_SysR;
//    prv_assert_cs();
//    SPI_LL_TransmitArray(reset, sizeof(reset));
//    prv_deassert_cs();

//    prv_assert_cs();
//    SPI_LL_TransmitArray(init, sizeof(init));
//    prv_deassert_cs();

    rtc_sync_ll();
    pubEpoch = prv_make_unix_time();
    // After this, start task, 10ms/1tick
}


bool rtc_sync_ll(void) {
    uint8_t time[10];
    memset(time, 0xFF, sizeof(time));
    time[0] = COMMAND_READ(CLOCK_SECONDS);

    prv_assert_cs();
    for (uint8_t i = 0; i < sizeof(time); i++) {
        time[i] = SPI_LL_Transmit(RTC_SPI, time[i]);
    }
    prv_deassert_cs();

   if (time[3] != 0xFF) {
			pubRtcDateTime.second   = SECTODEC(time[3]);
			pubRtcDateTime.minute    = MINTODEC(time[4]);
			pubRtcDateTime.hour = HOURTODEC(time[5]);
			pubRtcDateTime.day = DAYTODEC(time[6]);
			pubRtcDateTime.month  = MONTODEC(time[8]);
			pubRtcDateTime.year   = YEARTODEC(time[9]) + 2000;

        pubEpoch = prv_make_unix_time();
        return true;
    } else {
    	//Should change to default value
//        printf("RTC sync failed: Invalid response from RTC\r\n");
        return false;
    }
   return true;
}


rtc_date_time_t rtc_get_date_time(void) {
    return pubRtcDateTime;
}

uint32_t rtc_get_unix_time(void) {
    return pubEpoch;
}

bool rtc_set_date_time(rtc_date_time_t *dateTime) {
    uint8_t buffer[9];
    int8_t ret = -1;
    buffer[0] = COMMAND_WRITE(CLOCK_SECONDS);
    buffer[1] = DECTOSEC(dateTime->second);
    buffer[2] = DECTOMIN(dateTime->minute);
    buffer[3] = DECTOHOUR(dateTime->hour);
    buffer[4] = DECTODAY(dateTime->day);
    buffer[5] = 1; //Weekday
    buffer[6] = DECTOMON(dateTime->month);
    buffer[7] = DECTOYEAR((dateTime->year - 2000));
    buffer[8] = 0;

    prv_assert_cs();
    for (uint16_t i = 0; i < sizeof(buffer); i++) {
        ret = SPI_LL_Write(RTC_SPI, buffer[i]);
        if(ret != 0){
            prv_deassert_cs();
        	return false;
        }
    }
    prv_deassert_cs();

    pubRtcDateTime.second = dateTime->second;
    pubRtcDateTime.minute = dateTime->minute;
    pubRtcDateTime.hour   = dateTime->hour;
    pubRtcDateTime.day    = dateTime->day;
    pubRtcDateTime.month  = dateTime->month;
    pubRtcDateTime.year   = dateTime->year;
    pubEpoch = prv_make_unix_time();
    return true;
}

bool rtc_set_date_time_from_epoch(uint32_t epoch) {
    rtc_date_time_t dateTime;
    dateTime = prv_make_date_time_from_epoch(epoch);

    bool ret = rtc_set_date_time(&dateTime);

    return ret;
}

char* rtc_get_timestamp(void) {
    rtc_sync_ll();
    static char timestamp[28];
    snprintf(timestamp, sizeof(timestamp), "%04u-%02u-%02u_%02u-%02u-%02u\n\r", pubRtcDateTime.year,
            pubRtcDateTime.month, pubRtcDateTime.day, pubRtcDateTime.hour, pubRtcDateTime.minute, pubRtcDateTime.second);
    return timestamp;
}

static uint32_t prv_make_unix_time(void) {
    struct tm t;
    time_t epoch;

    t.tm_year = pubRtcDateTime.year-1900;
    t.tm_mon = pubRtcDateTime.month - 1;
    t.tm_mday = pubRtcDateTime.day;
    t.tm_hour = pubRtcDateTime.hour;
    t.tm_min = pubRtcDateTime.minute;
    t.tm_sec = pubRtcDateTime.second;
    t.tm_isdst = -1; //TODO DST flag should be stored in EEPROM
    epoch = mktime(&t);
    epoch -= TIMEZONE_GMT_PLUS_7;
    return (uint32_t) epoch;
}

static rtc_date_time_t prv_make_date_time_from_epoch(uint32_t epoch) {
    struct tm t;
    rtc_date_time_t dateTime;
    time_t unix = epoch + TIMEZONE_GMT_PLUS_7;
    localtime_r(&unix, &t);
    dateTime.year = t.tm_year + 1900;
    dateTime.month = t.tm_mon + 1;
    dateTime.day = t.tm_mday;
    dateTime.hour = t.tm_hour;
    dateTime.minute = t.tm_min;
    dateTime.second = t.tm_sec;
    return dateTime;
}

bool rtc_sync_time(void) {
    return rtc_sync_ll();
}

bool rtc_get_hard_time(rtc_date_time_t *hardTime) {
    uint8_t time[10];
    memset(time, 0xFF, sizeof(time));
    time[0] = COMMAND_READ(CLOCK_SECONDS);

    prv_assert_cs();
    for (uint8_t i = 0; i < sizeof(time); i++) {
        time[i] = SPI_LL_Transmit(RTC_SPI, time[i]);
    }
    prv_deassert_cs();

    if (time[3] != 0xFF) {
        hardTime->second = SECTODEC(time[3]);
        hardTime->minute = MINTODEC(time[4]);
        hardTime->hour = HOURTODEC(time[5]);
        hardTime->day = DAYTODEC(time[6]);
        hardTime->month = MONTODEC(time[8]);
        hardTime->year = YEARTODEC(time[9]) + 2000;
        return true;
    }
    return false;
}

void rtc_get_soft_time(rtc_date_time_t *softTime) {
    *softTime = rtc_get_date_time();
}

