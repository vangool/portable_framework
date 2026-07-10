#include "../include/osal_linkedlist.h"


/*osal_status_t queue_create(osal_queue_t* queue, uint8_t item_size, uint8_t list_size)
{
    linklist->head = NULL;
    linklist->size = list_size;
    linklist->item_size = item_size;
    linklist->count = 0;
    return OSAL_OK;
}

osal_status_t queue_send(osal_queue_t* linklist, osal_list_node_t* node)
{
    if(linklist->count >= linklist->size)
    {
        return OSAL_ERROR_OVERFLOW;
    }

    node->next = NULL;
    if(linklist->head == NULL)
    {
        linklist->head = node;
        linklist->head->next = node;
        linklist->head->prev = node;
        linklist->count++;
        return OSAL_OK;
    }

    node->prev = linklist->head->prev;
    linklist->head->prev->next = node;
    node->next = linklist->head;
    linklist->head->prev = node;
    
    linklist->count++;
    return OSAL_OK;
}

void* queue_receive(osal_queue_t* linklist)
{
    if(linklist->count == 0)
    {
        return NULL;
    }

    if(linklist->count == 1)
    {
        void* ndata = linklist->head->data;
        linklist->head = NULL;
        return ndata;
    }

    osal_list_node_t* node = linklist->head;
    void* ndata = node->data;
    linklist->head->prev->next = linklist->head->next;
    linklist->head->next->prev = linklist->head->prev;

    linklist->head = node->next;

    node->next = NULL;
    node->prev = NULL;

    linklist->count--;
    return ndata;
}*/