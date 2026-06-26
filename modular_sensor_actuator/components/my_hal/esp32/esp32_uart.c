/******************************************************************************
 * @file: esp32_uart.c
 * @brief: Target-specific implementation of the UART HAL for the ESP32.
 *
 * This file implements the concrete hardware configurations and data transfer
 * mechanics for the ESP32 UART peripherals. It handles low-level driver calls
 * to the native ESP-IDF framework, manages hardware queues, and processes
 * asynchronous UART events via dedicated FreeRTOS worker tasks.
 *
 * @author: Michael Van Gool
 * @date: 2026-05-04
 * @license: MIT License (See header file for full license conditions)
 * * Copyright (c) 2026 Michael Van Gool. All rights reserved.
 ******************************************************************************/

#include "esp32_uart.h"

#define UART_PORT UART_NUM_2

hal_uart_init       hal_uart_init_func      = esp32_uart_init;
hal_uart_receive    hal_uart_receive_func   = esp32_uart_receive;
hal_uart_transmit   hal_uart_transmit_func  = esp32_uart_transmit;

#define RX_BUF_SIZE 2098

static QueueHandle_t uart_queue;
const char* TAG = "UART";

static hal_status_t convert_to_ESP32_word_length(hal_uart_data_bits_t hal_word, uart_word_length_t* word)
{
    switch (hal_word)
    {
        case HAL_UART_DATA_BITS_5:      *word = UART_DATA_5_BITS; break;
        case HAL_UART_DATA_BITS_6:      *word = UART_DATA_6_BITS; break;
        case HAL_UART_DATA_BITS_7:      *word = UART_DATA_7_BITS; break;
        case HAL_UART_DATA_BITS_8:      *word = UART_DATA_8_BITS; break;
        case HAL_UART_DATA_BITS_MAX:    *word = UART_DATA_BITS_MAX; break;
    
        default:                        return HAL_ERROR_INVALID_ARG;
    }
    return HAL_OK;
}

static hal_status_t convert_to_ESP32_parity(hal_uart_parity_t hal_parity, uart_parity_t* parity)
{
    switch (hal_parity)
    {
        case HAL_UART_PARITY_EVEN:      *parity = UART_PARITY_EVEN; break;
        case HAL_UART_PARITY_ODD:       *parity = UART_PARITY_ODD; break;
        case HAL_UART_PARITY_DISABLE:   *parity = UART_PARITY_DISABLE; break;

        default:                    return HAL_ERROR_INVALID_ARG;
    }
    return HAL_OK;
}

static hal_status_t convert_to_ESP32_stop_bits(hal_uart_stop_bits_t hal_stop_bits, uart_stop_bits_t* stop_bits)
{
    switch (hal_stop_bits)
    {
        case HAL_UART_STOP_BITS_1:      *stop_bits = UART_STOP_BITS_1; break;
        case HAL_UART_STOP_BITS_1_5:    *stop_bits = UART_STOP_BITS_1_5; break;
        case HAL_UART_STOP_BITS_2:      *stop_bits = UART_STOP_BITS_2; break;
    
        default:                        return HAL_ERROR_INVALID_ARG;
    }
    return HAL_OK;
}


static hal_status_t convert_to_ESP32_flow_ctrl(hal_uart_flow_control_t hal_flow_ctrl, uart_hw_flowcontrol_t* flow_ctrl)
{
    switch (hal_flow_ctrl)
    {
        case HAL_UART_FLOW_CONTROL_DISABLE: *flow_ctrl = UART_HW_FLOWCTRL_DISABLE; break;
        case HAL_UART_FLOW_CONTROL_RTS:     *flow_ctrl = UART_HW_FLOWCTRL_RTS; break;
        case HAL_UART_FLOW_CONTROL_CTS:     *flow_ctrl = UART_HW_FLOWCTRL_CTS; break;
        case HAL_UART_FLOW_CONTROL_RTS_CTS: *flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS; break;
        case HAL_UART_FLOW_CONTROL_MAX:     *flow_ctrl = UART_HW_FLOWCTRL_MAX; break;
    
        default:                    return HAL_ERROR_INVALID_ARG;
    }
    return HAL_OK;
}

hal_status_t esp32_uart_init(hal_uart_config_t* config, uint8_t transmit_pin, uint8_t receive_pin)
{
    uart_word_length_t word;
    uart_parity_t parity;
    uart_stop_bits_t stop_bits;
    uart_hw_flowcontrol_t flow_ctrl;

    hal_status_t ret = 
        convert_to_ESP32_word_length(config->data_bits, &word) |
        convert_to_ESP32_parity(config->parity, &parity) |
        convert_to_ESP32_stop_bits(config->stop_bits, &stop_bits) |
        convert_to_ESP32_flow_ctrl(config->flow_ctrl, &flow_ctrl);
    
    if(ret != HAL_OK)
        return HAL_ERROR_INVALID_ARG;
    
    uart_config_t uart_config = 
    {
        .baud_rate = config->baud_rate,
        .data_bits = word,
        .parity = parity,
        .stop_bits = stop_bits,
        .flow_ctrl = flow_ctrl
    };

    ESP_ERROR_CHECK(
        uart_driver_install(UART_PORT, RX_BUF_SIZE * 2, RX_BUF_SIZE * 2, 20, &uart_queue, 0)
    );

    ESP_ERROR_CHECK(
        uart_param_config(UART_PORT, &uart_config)
    );

    ESP_ERROR_CHECK(
        uart_set_pin(
            UART_PORT,
            transmit_pin,
            receive_pin,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
        )
    );
    return HAL_OK;
}

void esp32_uart_receive()
{
    uart_event_t event;
    uint8_t data[128];

    while(1)
    {
        if(xQueueReceive(uart_queue, &event, portMAX_DELAY))
        {
            switch(event.type)
            {
                case UART_DATA:
                    int len = uart_read_bytes(
                        UART_PORT,
                        data,
                        event.size,
                        portMAX_DELAY);

                data[len] = '\0';
                hal_log(HAL_LOG_INFO, TAG, "Received: %s", (char*)data);
                break;

            case UART_FIFO_OVF:
            case UART_BUFFER_FULL:
                hal_log(HAL_LOG_WARN, TAG, "UART queue overflow!");
                uart_flush_input(UART_PORT);
                xQueueReset(uart_queue);
                break;

            default:
                break;
            }
        }
    }
}

void esp32_uart_transmit(const char* msg)
{
    while(1)
    {
        uart_write_bytes(UART_PORT, msg, strlen(msg));
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}