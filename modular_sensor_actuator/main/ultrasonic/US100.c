#include "ultrasonic.h"

static uint8_t gotten_intr_flag = false;

hal_intr_flag_t get_intr_flag()
{
    gotten_intr_flag = true;
    return HAL_INTR_FLAG_IRAM;
}

static int8_t is_intr_flag_retrieved()
{
    return gotten_intr_flag;
}

static void HAL_GPIO_ISR_ATTR echo_isr_handler(void* arg)
{
    ultrasonic_config_t* sensor = (ultrasonic_config_t*) arg;
    if(hal_gpio_get_level_func(sensor->echo_pin) == HAL_GPIO_HIGH)
    {
        sensor->start_time = hal_timer_get_time_func();
    }
    else
    {
        sensor->end_time = hal_timer_get_time_func();
        osal_task_notify_give_from_isr_func(sensor->task_to_notify);
    }
}

hal_status_t add_new_ultrasonic_sensor(ultrasonic_config_t* config)
{
    if(is_intr_flag_retrieved() == false)
        return HAL_ERROR_GENERAL;

    hal_gpio_set_direction_func(config->trig_pin, HAL_GPIO_MODE_OUTPUT);
    hal_gpio_set_direction_func(config->echo_pin, HAL_GPIO_MODE_INPUT);
    hal_gpio_set_level_func(config->trig_pin, HAL_GPIO_LOW);

    hal_status_t ret = hal_gpio_set_intr_type_func(config->echo_pin, HAL_GPIO_INTR_ANYEDGE);
    ret |= hal_gpio_isr_handler_add_func(config->echo_pin, echo_isr_handler, (void*) config);
    return ret;
}

void ultrasonic_get_distance_cm(ultrasonic_config_t* config, sensor_msg_t* msg)
{
    if(config->end_time < config->start_time)
        return;

    static int64_t avg_distance = 0;
    const int64_t sound_speed = 34300;
    int64_t distance = (sound_speed/* cm/s */ * ((config->end_time/* us */ - config->start_time/* us */)));
    distance/* cm/ms */ /= (int64_t)(1000000/* s/us */ * 2);

    if(avg_distance == 0)
    {
        avg_distance = distance;
    }
    else
    {
        avg_distance += (int64_t)((distance - avg_distance) / 2);
    }
    msg->distance = avg_distance;
}

void fire_ultrasonic_sensor(ultrasonic_config_t* config)
{
    hal_gpio_set_level_func(config->trig_pin, HAL_GPIO_HIGH);
    hal_rom_delay_us_func(10);
    hal_gpio_set_level_func(config->trig_pin, HAL_GPIO_LOW);
}

