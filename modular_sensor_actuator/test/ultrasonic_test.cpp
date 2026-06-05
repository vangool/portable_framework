#include "ultrasonic_test.hpp"

static MockHAL* ultra_mock = nullptr;

TEST(UltrasonicFireTest, FireTest)
{
    MockHAL mock;
    MockHAL::instance = &mock;

    hal_gpio_set_level_func = MockHAL::gpio_set_level_trampoline;
    hal_rom_delay_us_func = MockHAL::rom_delay_us_trampoline;
    hal_timer_get_time_func = MockHAL::timer_get_time_trampoline;

    {
        testing::InSequence seq;
        EXPECT_CALL(mock, hal_gpio_set_level(::testing::_, 1))
            .Times(1);
        EXPECT_CALL(mock, hal_rom_delay_us(10))
            .Times(1);
        EXPECT_CALL(mock, hal_gpio_set_level(::testing::_, 0))
            .Times(1);
        EXPECT_CALL(mock, hal_timer_get_time())
            .Times(1);
    }

    ultrasonic_config_t ultrasonic_config = 
        { .trig_pin = 0, .echo_pin = 0, .start_time = 0, .end_time = 20000 };

    fire_ultrasonic_sensor(&ultrasonic_config);
}

TEST(DistanceTest, ValidDistanceTest)
{
    ultrasonic_config_t ultrasonic_config = 
        { .trig_pin = 0, .echo_pin = 0, .start_time = 0, .end_time = 20000 };

    sensor_msg_t sensor_msg;

    ultrasonic_get_distance_cm(&ultrasonic_config, &sensor_msg);

    EXPECT_EQ(343, sensor_msg.distance);

    ultrasonic_config.start_time = 20000;
    ultrasonic_config.end_time = 80000;

    ultrasonic_get_distance_cm(&ultrasonic_config, &sensor_msg);

    EXPECT_EQ(686, sensor_msg.distance);

    ultrasonic_config.start_time = 100000;
    ultrasonic_config.end_time = 134000;

    ultrasonic_get_distance_cm(&ultrasonic_config, &sensor_msg);

    EXPECT_EQ(635, sensor_msg.distance);

    ultrasonic_config.start_time = 134000;
    ultrasonic_config.end_time = 145000;

    ultrasonic_get_distance_cm(&ultrasonic_config, &sensor_msg);

    EXPECT_EQ(412, sensor_msg.distance);
}

TEST(AddSensor, NoISRFlagSet)
{   
    ultrasonic_config_t ultrasonic_config = 
        { .trig_pin = 0, .echo_pin = 0, .start_time = 0, .end_time = 20000 };
    
    hal_status_t ret = add_new_ultrasonic_sensor(&ultrasonic_config);

    EXPECT_EQ(HAL_ERROR_GENERAL, ret);
}

TEST(AddSensor, ISRFlagSet)
{
    ultrasonic_config_t ultrasonic_config = 
        { .trig_pin = 0, .echo_pin = 0, .start_time = 0, .end_time = 20000 };
}