//
// Created by safi azmi on 17/11/2019.
//

#ifndef MTMHW119_ORDER_H
#define MTMHW119_ORDER_H

#include "amount_set.h"




typedef struct order_t *Order;

/** allocates a new memory for new order if id<0 or allocation fail return null
 * in evrey other case the new order address
 */

Order createOrder(unsigned int id);

/**
* Deallocates an existing order. Clears all elements by using the
* stored free function.
*/

void destroyOrder(Order e);

/**
 * the function allocate a new memory and return new_order address if it success
 *if order to copy is a null it will return a null value also it will have a
 * null value when if the memory allocate fail in creat the new order **/

Order copyOrder(Order e);

/**
 * return the order id if a null argument was sent return 0 else the order id
 */
unsigned int getOrderId(Order e);

/**
 * return the order setamount if a null argument was sent return null else the
 * order setamount
 */
AmountSet getSetOrder(Order e);


double getProAmInOrder(Order e, unsigned int id);


#endif //MTMHW119_ORDER_H
