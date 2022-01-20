//
// Created by safi azmi on 16/11/2019.
//

#ifndef MTMHW119_PRODUCT_H
#define MTMHW119_PRODUCT_H

#include "matamazom.h"



typedef struct product_t *Product;

/**
 function that allocate memory if it fails to do so it returns a NULL value
 also NULL will be the return value if it gets a NUll argument or if one of
 the int values
 is smaller than zero expect the first int it must be greater than zero.
 in every other case it return a new PRODUCT addrees.
 * */

Product ProductCreate(const unsigned int id, const char *name,
        const MatamazomAmountType amountType,
        const MtmProductData, MtmCopyData, MtmFreeData, MtmGetProductPrice);

/**
 * the function allocate a new memory and return new_Product address if it success
 * * if Product to copy is a null it will return a null value also it will have a null value when
 * *if the memory allocate fail in creat the new product **/

Product ProductCopy(Product);


/**
* Deallocates an existing Product. Clears all elements by using the
* stored free function.
*/

void destroyProduct(Product);

/**
 * duplicate the Product name and return the address of the copy back if the copy process fails then return NULL
 * if the argument that sent was null then also it will return a null value
 */

char *getProductName(Product);

/**
 * the function gets the ID of the Product if the argument that was sent is null
 * then it will returns -1 in every another case it will return the int value of the Product ID
 */
unsigned int getProductId(Product);


/**
 * the function gets the amountType of the Product if the argument that was sent
 * is null then it will returns -1 in every another case it will return the
 * int value of the citizen age
 */
MatamazomAmountType getproductAmountType(Product);

/**
 * the function gets the productdata if the argument that was sent
 * is null then it will returns NULL in every another case it will return the
 *  address of the productdata
 */

MtmProductData getProuductData(Product);

/**
 * the function for calculating the price of a product
 * if null argument was passed then it will returns -1 in every another case
 * it will return the total price of the amount
 */

double getProductPrice(Product, double);



/**
 * the function set the value of the total income of product to new one
 */

void setTotalIncome(Product, double);

/**
 * the function get the value of the total income of product
 */

double getTotalIncome(Product);

/**
 * the function return 1 if p1>p2 0 if p1==p2 else -1 and it compare there id's
 */
int compareProduct(Product p1,Product p2);

Product setProid(unsigned int id);
void setId(Product pro, unsigned int id);
#endif //MTMHW119_PRODUCT_H
