#include "hal.h"

const char* hal_log_level_to_string(hal_log_level_t level)
{
    switch (level)
    {
        case HAL_LOG_ERR:   return "ERROR";
        case HAL_LOG_WARN:  return "WARN";
        case HAL_LOG_INFO:  return "INFO";
        case HAL_LOG_DEBUG: return "DEBUG";
    
        default:            return "???";
    }
}
