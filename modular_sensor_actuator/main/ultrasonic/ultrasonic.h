#ifndef __US_100_H__
#define __US_100_H__

#include <stdbool.h>

#include "../utils.h"
#include "../../components/my_hal/include/hal.h"
#include "../../components/osal/include/osal.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

typedef struct 
{
    uint8_t trig_pin;
    uint8_t echo_pin;
    volatile uint64_t start_time;
    volatile uint64_t end_time;
    osal_taskhandle_t task_to_notify;
    
} ultrasonic_config_t ;

typedef struct
{
    uint8_t sensor_id;
    uint8_t decimal_places;
    int16_t distance;
} sensor_msg_t;

void US100_setup();
int32_t US100_get_distance();

hal_intr_flag_t get_intr_flag();
static void echo_isr_handler(void*);
hal_status_t add_new_ultrasonic_sensor(ultrasonic_config_t*);
void ultrasonic_get_distance_cm(ultrasonic_config_t*, sensor_msg_t*);
void fire_ultrasonic_sensor(ultrasonic_config_t*);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__US_100_H