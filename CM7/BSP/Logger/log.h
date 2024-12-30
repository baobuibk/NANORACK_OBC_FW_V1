/*
 * log.h
 *
 *  Created on: Dec 6, 2024
 *      Author: CAO HIEU
 */

#ifndef LOGGER_LOG_H_
#define LOGGER_LOG_H_


#include "stdint.h"
#include <string.h>
#include "stdio.h"

#define LOG_DATE_LEN     12
#define LOG_TIME_LEN     9
#define LOG_ACTTIME_LEN  20
#define LOG_CORE_LEN     10
#define LOG_FUNC_LEN     32
#define LOG_MSG_LEN      128

typedef enum {
    LOG_INFOR = 0,
	LOG_DEBUG,
	LOG_NOTICE,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL
} log_level_t;

typedef struct {
    char date[LOG_DATE_LEN];
    char time[LOG_TIME_LEN];
    char actTime[LOG_ACTTIME_LEN];
    log_level_t level;
    char core[LOG_CORE_LEN];
    char function[LOG_FUNC_LEN];
    uint16_t line;
    char message[LOG_MSG_LEN];
} log_struct_t;

void log_create(log_struct_t log, char* output);

#endif /* LOGGER_LOG_H_ */
