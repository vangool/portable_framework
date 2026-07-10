#ifndef __OS_QUEUE_H__
#define __OS_QUEUE_H__

#include <stdint.h>

#include "../../include/osal.h"


typedef struct 
{
    uint8_t* buffer;

    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t count;
    uint32_t item_size;
} osal_queue;

osal_status_t queue_create(osal_queue* queue, uint8_t item_size, uint8_t queue_size);
osal_status_t queue_send(osal_queue* queue, void* item);
void* queue_receive(osal_queue* queue);

#endif // __OS_QUEUE_H__