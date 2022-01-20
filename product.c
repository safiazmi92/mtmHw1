//
// Created by safi azmi on 16/11/2019.
//

#include "product.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**---------------------------------------------------------------------------------**/
/**-----------------------copyForString function------------------------------------**/
/**---------------------------------------------------------------------------------**/

/**
 * the function get a string arguments and copy it for other string
 * retrun values: if argument that was sent is null then return NULL
 * if the allocate fails it returns NULL
 * in every other case return a pointer to char(string)
 * */
char *   copyForString(const char *name) {
    if (name == NULL) {
        return NULL;
    }
    char *name_copy = malloc(strlen(name) + 1);
    if (name_copy == NULL) {
        return NULL;
    }
    return strcpy(name_copy, name);
}



/**----------------------------------------------------------------------------------**/
/**-----------------------------------Product struct---------------------------------**/
/**----------------------------------------------------------------------------------**/
struct product_t{
    char * name;
    unsigned int id;
    MatamazomAmountType type;
    MtmProductData data;
    double total_income;
    MtmCopyData copyProData;
    MtmFreeData freeProData;
    MtmGetProductPrice getProPrice;
};

/**----------------------------------------------------------------------------------**/
/**-----------------------------------ProductCreat---------------------------------**/
/**----------------------------------------------------------------------------------**/

Product ProductCreate(const unsigned int id, const char *name,
                      const MatamazomAmountType amountType,
                      const MtmProductData data, MtmCopyData copy,
                      MtmFreeData free
                      , MtmGetProductPrice getPrice){
    if(id<0 || !name || !data || !copy || !free || !getPrice){
        return NULL;
    }
    Product new_pro=malloc(sizeof(*new_pro));
    if(!new_pro){
        return NULL;
    }
    new_pro->name=copyForString(name);
    if(!new_pro->name){
        free(new_pro);
        return NULL;
    }
    new_pro->data=copy(data);
    if(!new_pro->data){
        free(new_pro->name);
        free(new_pro);
        return NULL;
    }
    new_pro->id=id;
    new_pro->type=amountType;
    new_pro->copyProData=copy;
    new_pro->freeProData=free;
    new_pro->getProPrice=getPrice;
    new_pro->total_income=0;
    return new_pro;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------ProductCopy---------------------------------**/
/**----------------------------------------------------------------------------------**/

Product ProductCopy(Product pro){
    if(!pro){
        return NULL;
    }
    Product copy=ProductCreate(pro->id,pro->name,pro->type,pro->data,
            pro->copyProData,pro->freeProData,pro->getProPrice);
    if(!copy){ return NULL;}
    copy->total_income=pro->total_income;
    return copy;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------destroyProduct---------------------------------**/
/**----------------------------------------------------------------------------------**/

void destroyProduct(Product pro){
    if(!pro){ return;}
    if(pro->name){
    free(pro->name);
    }
    if(pro->data) {
        pro->freeProData(pro->data);
    }
    free(pro);
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------getProductName---------------------------------**/
/**----------------------------------------------------------------------------------**/

char *getProductName(Product pro){
    if(!pro){ return NULL;}
    return pro->name;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------getProductId---------------------------------**/
/**----------------------------------------------------------------------------------**/

unsigned int getProductId(Product pro){
    if(!pro){ return -1;}
    return pro->id;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------getproductAmountType---------------------------------**/
/**----------------------------------------------------------------------------------**/

MatamazomAmountType getproductAmountType(Product pro){
    return pro->type;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------getProuductData---------------------------------**/
/**----------------------------------------------------------------------------------**/

MtmProductData getProuductData(Product pro){
    if(!pro){ return NULL;}
    return  pro->data;      //not sure maybe a copy we need
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------getProductPrice---------------------------------**/
/**----------------------------------------------------------------------------------**/



double getProductPrice(Product pro, double amount){
    if(!pro){ return -1;}
    return pro->getProPrice(pro->data,amount);
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------setTotalIncome---------------------------------**/
/**----------------------------------------------------------------------------------**/

void setTotalIncome(Product pro, double cost){
    pro->total_income+=cost;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------getTotalIncome---------------------------------**/
/**----------------------------------------------------------------------------------**/


double getTotalIncome(Product pro){
    return pro->total_income;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------compareProduct---------------------------------**/
/**----------------------------------------------------------------------------------**/


int compareProduct(Product p1,Product p2){
    if(getProductId(p1)>getProductId(p2)){
        return 1;
    }
    if(getProductId(p1)==getProductId(p2)){
        return 0;
    }
    return -1;
}

Product setProid(unsigned int id){
    Product tmp=malloc(sizeof(*tmp));
    if(!tmp){
        return NULL;
    }
    tmp->id=id;
    tmp->data=NULL;
    tmp->name=NULL;
    return tmp;
}

void setId(Product pro, unsigned int id){
    pro->id=id;
}