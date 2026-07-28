/******************************************************************************
 * @file: osal_queue.c
 * @brief: Pointer-based circular queue implementation for the OSAL backend.
 *
 * Implements queue creation, enqueue, and dequeue operations using a circular
 * buffer. Queue entries store data pointers only; the queue does not allocate
 * memory or copy the data being queued.
 *
 * The caller is responsible for managing the lifetime of queued objects and
 * ensuring that stored pointers remain valid until they are removed.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-13
 * @license: MIT License
 *
 * Copyright (c) 2026 Michael Van Gool
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED.
 ******************************************************************************/

#include "osal_queue.h"

#include "scheduler.h"

static void queue_copy(uint8_t *dst, const uint8_t *src, uint32_t size)
{
    while(size--)
    {
        *dst++ = *src++;
    }
}

osal_status_t queue_create(
    osal_queue_t *queue,
    uint8_t* buffer,
    osal_ubase_type queue_length,
    osal_ubase_type item_size)
{

    my_os_queue_t* my_queue = (my_os_queue_t*)queue->queue;
    my_queue->head_idx = 0;
    my_queue->tail_idx = 0;
    my_queue->capacity = queue_length;
    my_queue->item_size = item_size;
    my_queue->count = 0;
    my_queue->buffer = buffer;
    return OSAL_OK;
}

osal_status_t queue_enqueue(osal_queue_t* queue, void* data)
{
    scheduler_enter_critical();
    my_os_queue_t* my_queue = (my_os_queue_t*)queue->queue;
    if(my_queue->count >= my_queue->capacity)
    {
        return OSAL_ERROR_OVERFLOW;
    }

    queue_copy(
        my_queue->buffer + my_queue->tail_idx * my_queue->item_size,
        data,
        my_queue->item_size
    );

    my_queue->tail_idx = (my_queue->tail_idx + 1) % my_queue->capacity;
    my_queue->count++;

    scheduler_exit_critical();

    return OSAL_OK;
}

osal_status_t queue_dequeue(osal_queue_t* queue, void* data)
{
    scheduler_enter_critical();
    my_os_queue_t* my_queue = (my_os_queue_t*)queue->queue;
    if(my_queue->count == 0)
    {
        return OSAL_ERROR_GENERAL;
    }

    queue_copy(
        data,
        my_queue->buffer + my_queue->head_idx * my_queue->item_size,
        my_queue->item_size
    );
    my_queue->count--;
    my_queue->head_idx = (my_queue->head_idx + 1) % my_queue->capacity;

    scheduler_exit_critical();

    return OSAL_OK;
}