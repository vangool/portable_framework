/******************************************************************************
 * @file: os_queue.h
 * @brief: Pointer-based queue implementation for the OSAL backend.
 *
 * Provides a circular queue for storing data pointers. The queue does not copy
 * queued objects; callers are responsible for managing the lifetime of stored
 * data.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-13
 * @license: MIT License
 *
 * Copyright (c) 2026 Michael Van Gool
 ******************************************************************************/

#ifndef __OS_QUEUE_H__
#define __OS_QUEUE_H__

#include <stdint.h>

#include "osal.h"


/*
 * Pointer-based circular queue.
 * Stores pointers only; does not copy data.
 * Caller owns the lifetime of queued objects.
 */
typedef struct 
{
    void* buffer;       // Array of item pointers
    uint8_t head_idx;   // Next item to dequeue
    uint8_t tail_idx;   // Next slot to enqueue
    uint8_t capacity;   // Maximum number of items
    uint8_t count;      // Current number of items
    uint64_t item_size; // Size of each item

} my_os_queue_t;


/*
 * Initialize queue with user-provided pointer storage.
 * No memory allocation is performed.
 */
osal_status_t queue_create(
    osal_queue_t *queue,
    uint8_t* buffer,
    osal_ubase_type queue_length,
    osal_ubase_type item_size);


/*
 * Add item pointer to queue.
 * Returns OSAL_ERROR_OVERFLOW if full.
 */
osal_status_t queue_enqueue(osal_queue_t* queue,
                            void* item);


/*
 * Remove item pointer from queue.
 * data receives the stored pointer.
 * Returns OSAL_ERROR_EMPTY if empty.
 */
osal_status_t queue_dequeue(osal_queue_t* queue,
                            void* data);


#endif // __OS_QUEUE_H__