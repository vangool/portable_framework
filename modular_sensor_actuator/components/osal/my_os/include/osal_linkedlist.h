#ifndef __OSAL_LINKEDLIST_H___
#define __OSAL_LINKEDLIST_H__

#include "../../include/osal.h"

typedef struct osal_list_node
{
    void* data;
    struct osal_list_node* next;
    struct osal_list_node* prev;
} osal_list_node_t;

typedef struct 
{
    osal_list_node_t* head;
    uint8_t size;
    uint8_t item_size;
    uint8_t count;
} osal_linkedlist;


osal_status_t linkedlist_create(osal_linkedlist* linkedlist, uint8_t item_size, uint8_t list_size);
osal_status_t linkedlist_insert_head(osal_linkedlist* linkedlist, osal_list_node_t* node);
void* linkedlist_remove_tail(osal_linkedlist* linkedlist);

#endif // __OSAL_LINKEDLIST_H__