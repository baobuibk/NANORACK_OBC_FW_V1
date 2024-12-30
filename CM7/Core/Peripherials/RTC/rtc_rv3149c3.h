/*
 * rtc_rv3149c3.h
 *
 *  Created on: Nov 20, 2024
 *      Author: CAO HIEU
 */

#ifndef PERIPHERIALS_RTC_RTC_RV3149C3_H_
#define PERIPHERIALS_RTC_RTC_RV3149C3_H_
#include "main.h"
#include "stm32h745xx.h"
#include "stdbool.h"
#include "time.h"
#include "stdio.h"
#include "spi.h"
#include <string.h>
/*!
 * @def RTC_SPI
 * Define the SPI peripheral used by the RTC
 */
#define RTC_SPI SPI4

#define TIMEZONE_GMT_PLUS_7 (7 * 3600)

#define CONTROL_1               (0x00)
#define CONTROL_1_CLK_INT       (1 << 7)
#define CONTROL_1_TD_32         (0 << 5)
#define CONTROL_1_TD_8          (1 << 5)
#define CONTROL_1_TD_1          (2 << 5)
#define CONTROL_1_TD_05         (3 << 5)
#define CONTROL_1_SROn          (1 << 4)
#define CONTROL_1_EERE          (1 << 3)
#define CONTROL_1_TAR           (1 << 2)
#define CONTROL_1_TE            (1 << 1)
#define CONTROL_1_WE            (1 << 0)

#define CONTROL_INT             (0x01)
#define CONTROL_INT_SRIE        (1 << 4)
#define CONTROL_INT_V2IE        (1 << 3)
#define CONTROL_INT_V1IE        (1 << 2)
#define CONTROL_INT_TIE         (1 << 1)
#define CONTROL_INT_AIE         (1 << 0)

#define CONTROL_INT_FLAG        (0x02)
#define CONTROL_INT_FLAG_SRF    (1 << 4)
#define CONTROL_INT_FLAG_V2IF   (1 << 3)
#define CONTROL_INT_FLAG_V1IF   (1 << 2)
#define CONTROL_INT_FLAG_TF     (1 << 1)
#define CONTROL_INT_FLAG_AF     (1 << 0)

#define CONTROL_STATUS          (0x03)
#define CONTROL_STATUS_EEbusy   (1 << 7)
#define CONTROL_STATUS_PON      (1 << 5)
#define CONTROL_STATUS_SR       (1 << 4)
#define CONTROL_STATUS_V2F      (1 << 3)
#define CONTROL_STATUS_V1F      (1 << 2)

#define CONTROL_EEPROM          (0x30)
#define CONTROL_EEPROM_R80k     (1 << 7)
#define CONTROL_EEPROM_R20k     (1 << 6)
#define CONTROL_EEPROM_R5k      (1 << 5)
#define CONTROL_EEPROM_R1k      (1 << 4)
#define CONTROL_EEPROM_FD0      (0 << 3)
#define CONTROL_EEPROM_FD1      (0 << 2)
#define CONTROL_EEPROM_ThE      (1 << 1)
#define CONTROL_EEPROM_ThP      (1 << 0)

#define CONTROL_RESET           (0x04)
#define CONTROL_RESET_SysR      (1 << 4)

#define CLOCK_SECONDS           (0x08)
#define CLOCK_MINUTES           (0x09)
#define CLOCK_HOURS             (0x0A)
#define CLOCK_DAYS              (0x0B)
#define CLOCK_WEEKDAYS          (0x0C)
#define CLOCK_MONTHS            (0x0D)
#define CLOCK_YEARS             (0x0E)
#define CLOCK_12_HOUR_MODE      (1 << 6)
#define CLOCK_AM_PM             (1 << 5)

#define ALARM_SECOND            (0x10)
#define ALARM_MINUTE            (0x11)
#define ALARM_HOUR              (0x12)
#define ALARM_DAYS              (0x13)
#define ALARM_WEEKDAYS          (0x14)
#define ALARM_MONTHS            (0x15)
#define ALARM_YEARS             (0x16)
#define ALARM_AE                (1 << 7)

#define TIMER_LOW               (0x18)
#define TIMER_HIGH              (0x19)

#define TEMPERATURE_K           (0x20)

#define COMMAND_READ(x) (0x80 | x) //x = command
#define COMMAND_WRITE(x) (x) //x = command

#define SECTODEC(x)  ( ( (x & 0x70) >> 4 ) * 10 + (x & 0x0F) ) //Convert BCD coded second to uint8
#define MINTODEC(x) (SECTODEC(x)) //Convert BCD coded minute to uint8
#define HOURTODEC(x) ( ( (x & 0x30) >> 4)  * 10 + (x & 0x0F) ) //Convert BCD coded hour to uint8
#define DAYTODEC(x) (HOURTODEC(x)) //Convert BCD coded day to uint8
#define MONTODEC(x)  ( ( (x & 0x10) >> 4) * 10 +  (x & 0x0F) ) //Convert BCD coded month to uint8
#define YEARTODEC(x) ( ( (x & 0xF0) >> 4) * 10 +  (x & 0x0F) ) //Convert BCD coded year to uint8

#define DECTOSEC(x) ((((x / 10) & 0x7) << 4) | ((x % 10) & 0xF))
#define DECTOMIN(x) (DECTOSEC(x))
#define DECTOHOUR(x) ((((x / 10) & 0x3) << 4) | ((x % 10) & 0xF))
#define DECTODAY(x) (DECTOHOUR(x))
#define DECTOMON(x) ((((x / 10) & 0x1) << 4) | ((x % 10) & 0xF))
#define DECTOYEAR(x) ((((x / 10) & 0xF) << 4) | ((x % 10) & 0xF))

#define TEMPTODEC(x) ((int16_t)(x) - 60)

/*!
 * @struct rtc_date_time_t
 * @brief Data type for the date and time
 */
typedef struct {
	uint8_t day;    //!< Day: Starting at 1 for the first day
	uint8_t month;  //!< Month: Starting at 1 for January
	uint16_t year;  //!< Year in format YYYY
	uint8_t hour;   //!< Hour
	uint8_t minute; //!< Minute
	uint8_t second; //!< Second
} rtc_date_time_t;


/*!
 *  @brief Initializes the RTC and assigns the tick hook function.
 *  @param rtcTickHook Tick hook function. The tick hook must have the function body shown in @ref rtc_tick_hook_t
 *  After initialization, the local date time variable is synchronized with the RTC
 *  @note This function must be called before any other function in this module can be used!
 */
void init_rtc(void);

/*!
 *  @brief Synchronizes the local date time variable with the RTC.
 *  The new value is available through the rtc_get_date_time() function.
 *  @return true on success, false on failure
 */
bool rtc_sync_ll(void);

/*!
 * @brief Return the local date time variable.
 * @note The timestamp is NOT updated automatically.
 * Call rtc_sync() before if the current timestamp is needed or use the rtc_get_unix_time()
 * function to get a constantly updated timestamp.
 */
rtc_date_time_t rtc_get_date_time(void);

/*!
 * @brief Returns the current date and time as unix timestamp.
 * @note This value is updated automatically.
 */
uint32_t rtc_get_unix_time(void);


/*!
 * @brief Set the RTC.
 * @param dateTime Pointer to a variable of type @ref rtc_date_time_t with the values to set.
 * @return true on success, false on failure
 */
bool rtc_set_date_time(rtc_date_time_t *dateTime);

/*!
 * @brief Set the RTC with a unix timestamp.
 * @param epoch Unix timestamp
 * @return true on success, false on failure
 */
bool rtc_set_date_time_from_epoch(uint32_t epoch);

/*!
 * @brief Return the local date time as string.
 * @note The timestamp is NOT updated automatically.
 * Call rtc_sync() before if the current timestamp is needed.
 * @return Pointer to a string in the format YYYY-MM-DD_hh-mm-ss. The string is 26 bytes long.
 */
char* rtc_get_timestamp(void);

//float read_temperature(void);
//uint8_t SPI4_ReadRegister(uint8_t regAddress);

extern volatile rtc_date_time_t pubRtcDateTime;
extern volatile uint32_t pubEpoch;
bool rtc_sync_time(void);
bool rtc_get_hard_time(rtc_date_time_t *hardTime);
void rtc_get_soft_time(rtc_date_time_t *softTime);

#endif /* PERIPHERIALS_RTC_RTC_RV3149C3_H_ */
