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


osal_status_t queue_create(osal_queue_t* queue_t, void** buffer, uint8_t list_size)
{

    my_os_queue_t* queue = (my_os_queue_t*)queue_t->queue;
    queue->head_idx = 0;
    queue->tail_idx = 0;
    queue->capacity = list_size;
    queue->count = 0;
    queue->buffer = buffer;
    return OSAL_OK;
}

osal_status_t queue_enqueue(osal_queue_t* queue_t, void* data)
{
    my_os_queue_t* queue = (my_os_queue_t*)queue_t->queue;
    if(queue->count >= queue->capacity)
    {
        return OSAL_ERROR_OVERFLOW;
    }

    queue->buffer[queue->tail_idx] = data;

    queue->tail_idx = (queue->tail_idx + 1) % queue->capacity;
    queue->count++;

    return OSAL_OK;
}

osal_status_t queue_dequeue(osal_queue_t* queue_t, void** data)
{
    my_os_queue_t* queue = (my_os_queue_t*)queue_t->queue;
    if(queue->count == 0)
    {
        return OSAL_ERROR_GENERAL;
    }

    *data = queue->buffer[queue->head_idx];
    queue->count--;
    queue->head_idx = (queue->head_idx + 1) % queue->capacity;
    return OSAL_OK;
}