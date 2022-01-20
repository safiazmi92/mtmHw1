//
// Created by safi azmi on 13/11/2019.
//

#include "amount_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
    ASElement data;
    double size;
    struct node* next;
} *Node;


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/              createNode                   \||/  *******************
 *****   \/           ***************************       \/   **************** */
static Node createNode(ASElement* data) {
    Node ptr = malloc(sizeof(*ptr));
    if(!ptr){
        return NULL;
    }

    ptr->data=data;
    ptr->size=0;
    ptr->next=NULL;
    return ptr;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              createNode                   /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

struct AmountSet_t {
    Node head;
    int set_size;
    Node current_element;

    CopyASElement copy_element;
    FreeASElement  free_element;
    CompareASElements compare_element;
};


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                    asCreate               \||/  *******************
 *****   \/           ***************************       \/   **************** */
AmountSet asCreate(CopyASElement copyElement,
                   FreeASElement freeElement,
                   CompareASElements compareElements)
 {
    if(!copyElement || !freeElement || !compareElements) {
        return NULL;
    }

     AmountSet new_list = malloc(sizeof(*new_list));
    if(!new_list) {
        return NULL;
    }

    new_list->set_size =0;
    new_list->head=NULL;
    new_list->current_element=NULL;
    new_list->copy_element = copyElement;
    new_list->free_element = freeElement;
    new_list->compare_element = compareElements;
    return new_list;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                    asCreate               /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                   asDestroy               \||/  *******************
 *****   \/           ***************************       \/   **************** */
void asDestroy(AmountSet set) {
    if(!set) {
        return;
    }
    asClear(set);
    free(set);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                   asDestroy               /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************
// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                   asCopy                  \||/  *******************
 *****   \/           ***************************       \/   **************** */
AmountSet asCopy(AmountSet set) {
    if(!set) {
        return NULL;
    }
    AmountSet new_copy = asCreate(set->copy_element,
                                  set->free_element,
                                  set->compare_element);
    if(!new_copy) {
        return NULL;
    }
    for(Node iterator = set->head ; iterator ; iterator = iterator->next) {
        switch(asRegister(new_copy,(iterator->data))){
            case AS_SUCCESS:
                asChangeAmount(new_copy,iterator->data,iterator->size);
                continue;
            default:
                asDestroy(new_copy);
                return NULL;
        }
    }
    return new_copy;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                    asCopy                 /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                   asGetSize               \||/  *******************
 *****   \/           ***************************       \/   **************** */
int asGetSize(AmountSet set) {
    if(!set) {
        return -1;
    }
    int tmp_size = set->set_size;
    return tmp_size;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                   asGetSize               /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                  asContains               \||/  *******************
 *****   \/           ***************************       \/   **************** */
bool asContains(AmountSet set, ASElement element) {
    if(!set || !element) {
        return false;
    }
    for(Node iterator = set->head ; iterator ; iterator = iterator->next) {
        if (set->compare_element(element, (iterator->data))==0) {
            return true;
        }
    }
    return false;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                   asContains              /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                   asGetAmount             \||/  *******************
 *****   \/           ***************************       \/   **************** */


AmountSetResult asGetAmount(AmountSet set, ASElement element, double
*outAmount){
    if(!set || !element || !outAmount){
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element)){
        for(Node iterator = set->head ; iterator ; iterator = iterator->next) {
            if (set->compare_element(element, (iterator->data))==0) {
                *(outAmount)=iterator->size;
                break;
            }
        }
        return AS_SUCCESS;
    }
    return AS_ITEM_DOES_NOT_EXIST;
}

/**  *   /\           ***************************       /\   *******************
 *****  /||\                  asGetAmount              /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                  asRegister               \||/  *******************
 *****   \/           ***************************       \/   **************** */
AmountSetResult asRegister(AmountSet set, ASElement element) {
    if(!set || !element) {
        return AS_NULL_ARGUMENT;
    }
    else if(asContains(set, element)) {
        return AS_ITEM_ALREADY_EXISTS;
    }
    else if(!(set->head)) { //if  empty list.
        set->head = createNode(set->copy_element(element));
        if(!(set->head)) {
            return AS_OUT_OF_MEMORY;   //need to update the "mitargial"
        }
        set->set_size++;
        return AS_SUCCESS;
    }
    else if(set->compare_element((set->head->data), element)>0) {
        Node tmp_previous = set->head;
        set->head = createNode(set->copy_element(element));
        if(!(set->head)) {
            set->head = tmp_previous;
            return AS_OUT_OF_MEMORY;
        }
        set->head->next = tmp_previous;
        set->set_size++;
        return AS_SUCCESS;
    }
    else {
        Node tmp_previous = set->head;
        for(Node iterator=set->head->next; iterator ;iterator=iterator->next) {
            if (set->compare_element((iterator->data), element)>0) {
                tmp_previous->next = createNode(set->copy_element(element));
                if(!(tmp_previous->next)) {
                    tmp_previous->next = iterator;
                    return AS_OUT_OF_MEMORY;
                }
                tmp_previous->next->next = iterator;
                set->set_size++;
                return AS_SUCCESS;
            }
            tmp_previous = iterator;
        }
        tmp_previous->next = createNode(set->copy_element(element));
        if(!(tmp_previous->next)) {
            return AS_OUT_OF_MEMORY;
        }
        set->set_size++;
        return AS_SUCCESS;
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                    asRegister             /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                 asChangeAmount            \||/  *******************
 *****   \/           ***************************       \/   **************** */
AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double
amount){
    if(!set || !element){
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element)){
        for(Node iterator = set->head ; iterator ; iterator = iterator->next) {
            if (set->compare_element(element, (iterator->data))==0) {
                if(iterator->size+amount<0){
                    return AS_INSUFFICIENT_AMOUNT;
                }
                iterator->size+=amount;
                break;
            }
        }
        return AS_SUCCESS;
    }
    return AS_ITEM_DOES_NOT_EXIST;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                asChangeAmount             /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                     asDelete              \||/  *******************
 *****   \/           ***************************       \/   **************** */
AmountSetResult asDelete(AmountSet set, ASElement element) {
    if(!set || !element) {
        return AS_NULL_ARGUMENT;
    }
    else if(!asContains(set, element)) {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    else if(set->compare_element((set->head->data), element)==0) {
        Node toDelete = set->head;
        set->head = toDelete->next;
        set->free_element(toDelete->data);
        free(toDelete);
        set->set_size--;
        return AS_SUCCESS;
    }
    else {
        Node tmp_previous = set->head;
        for(Node iterator=set->head->next; iterator ;iterator=iterator->next) {
            if (set->compare_element(element, (iterator->data))==0) {
                Node toDelete = iterator;
                tmp_previous->next = toDelete->next;
                set->free_element(toDelete->data);
                free(toDelete);
                set->set_size--;
                return AS_SUCCESS;
            }
            tmp_previous = iterator;
        }
    }
    return AS_ITEM_DOES_NOT_EXIST;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                    asDelete               /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                    asClear                \||/  *******************
 *****   \/           ***************************       \/   **************** */
AmountSetResult asClear(AmountSet set) {
    if(!set) {
        return AS_NULL_ARGUMENT ;
    }
    while(set->head) {
        Node toDelete = set->head;
        set->head = toDelete->next;
        set->free_element(toDelete->data);
        free(toDelete);
    }
    set->current_element = set->head;
    set->set_size = 0;
    return AS_SUCCESS;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                    asClear                /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                   asGetFirst              \||/  *******************
 *****   \/           ***************************       \/   **************** */
ASElement asGetFirst(AmountSet set) {
    if(!set || !(set->head)) {
        return NULL;
    }
    set->current_element = set->head;
    ASElement tmp = (set->current_element->data);
    return tmp;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                   asGetFirst              /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                   asGetNext               \||/  *******************
 *****   \/           ***************************       \/   **************** */
ASElement asGetNext(AmountSet set) {
    if(!set || !(set->current_element)){
        return NULL;
    }
    if(!(set->current_element->next)){
        return NULL;
    }
    set->current_element=set->current_element->next;
    ASElement tmp = (set->current_element->data);
    return tmp;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                   asGetNext               /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************