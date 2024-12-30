/*
 * log.c
 *
 *  Created on: Dec 6, 2024
 *      Author: CAO HIEU
 */

#include "log.h"

const char* log_level_to_string(log_level_t level) {
    switch (level) {
        case LOG_INFOR:  return "INFOR";
        case LOG_DEBUG:  return "DEBUG";
        case LOG_NOTICE: return "NOTICE";
        case LOG_WARN:   return "WARN";
        case LOG_ERROR:  return "ERROR";
        case LOG_FATAL:  return "FATAL";
        default:     return "UNKNOWN";
    }
}

void log_create(log_struct_t log, char* output) {
    const char* core = (strcmp(log.core, "M4") == 0 || strcmp(log.core, "M7") == 0) ? log.core : "Mx";

    snprintf(output, LOG_MSG_LEN + LOG_FUNC_LEN + 100,
             "%s %s %s [%s] [%s] %s() line:%d \"%s\"\n",
             log.date, log.time, log.actTime,
             log_level_to_string(log.level), core, log.function,
             log.line, log.message);
}

// int main() {
//     log_struct_t log = {
//         .date = "2024-02-22",
//         .time = "23:40:03",
//         .actTime = "0:05:03",
//         .level = LOG_NOTICE,
//         .core = "M4",
//         .function = "get_link",
//         .line = 23,
//         .message = "Cannot access memory"
//     };

//     char log_output[256];
//     log_create(log, log_output);
//     printf("%s\n", log_output);
//     return 0;
// }

