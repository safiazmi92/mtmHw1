//
// Created by safi azmi on 17/11/2019.
//
#include "matamazom.h"
#include "product.h"
#include "order.h"
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include <string.h>
#include "matamazom_print.h"


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/        cast functions for the set/list    \||/  *******************
 *****   \/           ***************************       \/   **************** */

ASElement copyFunPro(ASElement e){
    return ProductCopy((Product)e);
}

void freeFunPro(ASElement e){
    if(!e){ return;}
    destroyProduct((Product)e);
}

int comparePro(ASElement e1,ASElement e2){
    return compareProduct((Product)e1,(Product)e2);
}

ListElement copyOrderInList(ListElement e){
    if(!e){ return NULL;}
    return copyOrder((Order)e);
}

void freeOrderInList(ListElement e){
    if(!e){ return;}
    destroyOrder((Order)e);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\    cast functions for the set/list        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/             static functions area         \||/  *******************
 *****   \/           ***************************       \/   **************** */

static Product getPro(AmountSet pro_set,Product pro){
    for(Product i=(Product)asGetFirst(pro_set);i;i=(Product)asGetNext(pro_set)){
        if(compareProduct(i,pro)==0){
            return i;
        }
    }
    return NULL;
}

static bool invalidAmount(Product p1, double amount){
    double r=amount-(int)amount;
    if(r<0){
        r=r*(-1); //make r positive
    }
    MatamazomAmountType amountType=getproductAmountType(p1);
    if((amountType==MATAMAZOM_INTEGER_AMOUNT && ((0<=r && r<=0.001)||(1>=r &&
    r>=0.999)))|| (amountType==MATAMAZOM_HALF_INTEGER_AMOUNT && ((0<=r &&
    r<=0.001)||(0.499<=r && r<=0.501)||(0.999<=r && r<=1)))
    ||amountType==MATAMAZOM_ANY_AMOUNT){
        return false;
    }
    return true;
}

static bool listContainOrder(List order_list, unsigned int id){
    if(!order_list){
        return false;
    }
    for(Order i=(Order)listGetFirst(order_list);i;i=(Order)listGetNext
            (order_list)){
        if(id==getOrderId(i)){
            return true;
        }
    }
    return false;
}

static Order getOrder(List order_list, unsigned int id ){
    if(!order_list){
        return NULL;
    }
    for(Order i=(Order)listGetFirst(order_list);i;i=(Order)listGetNext
            (order_list)){
        if(id==getOrderId(i)){
            return i;
        }
    }
    return NULL;
}

static void deleteOrder(List order_list, unsigned int id ){
    if(!order_list){
        return ;
    }
    for(Order i=(Order)listGetFirst(order_list);i;i=(Order)listGetNext
            (order_list)){
        if(id==getOrderId(i)){
            listRemoveCurrent(order_list);
            return;
        }
    }
    return;
}


int compareProForPrint(ASElement e1,ASElement e2){
    double total_1=getTotalIncome(e1);
    double total_2=getTotalIncome(e2);
    double diff=total_1-total_2;
    if(-0.001<=diff && diff<=0.001){        //the case that was in moodle
        if(getProductId(e1)<getProductId(e2)){
            return -1;
        }
        return 1;
    }
    if(getTotalIncome(e1)>getTotalIncome(e2)){
        return -1;
    }
    if(getTotalIncome(e1)==getTotalIncome(e2)){
        if(getProductId(e1)<getProductId(e2)){
            return -1;
        }
        if(getProductId(e1)>getProductId(e2)){
            return 1;
        }
        return 0;
    }
    return 1;
}

/**  *   /\           ***************************       /\   *******************
 *****  /||\              static functions area        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/               Matamazom struct            \||/  *******************
 *****   \/           ***************************       \/   **************** */

struct Matamazom_t{
    unsigned int nextOrderId;
    AmountSet products_set;
    List orders_list;
};

/**  *   /\           ***************************       /\   *******************
 *****  /||\                Matamazom struct           /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/               matamazomCreate             \||/  *******************
 *****   \/           ***************************       \/   **************** */

Matamazom matamazomCreate(){
    Matamazom new=malloc(sizeof(*new));
    if(!new){ return NULL;}
    new->nextOrderId=1;
    new->products_set=asCreate(copyFunPro,freeFunPro,comparePro);
    if(!new->products_set){
        free(new);
        return NULL;
    }
    new->orders_list=listCreate(copyOrderInList,freeOrderInList);
    if(!new->orders_list){
        asDestroy(new->products_set);
        free(new);
        return NULL;
    }
    return new;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                 matamazomCreate           /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                matamazomDestroy           \||/  *******************
 *****   \/           ***************************       \/   **************** */

void matamazomDestroy(Matamazom matamazom){
    if(!matamazom){ return;}
    asDestroy(matamazom->products_set);
    listDestroy(matamazom->orders_list);
    free(matamazom);
    return;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                matamazomDestroy           /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                   mtmNewProduct           \||/  *******************
 *****   \/           ***************************       \/   **************** */

MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id,
        const char *name,const double amount,
        const MatamazomAmountType amountType,
        const MtmProductData data, MtmCopyData copy_data, MtmFreeData
        free_data, MtmGetProductPrice get_price){
    if(!matamazom || !name || !data || !copy_data || !free_data || !get_price ){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    double tmp_amount=amount-(int)amount;
    if(strlen(name)==0 || (name[0]<'a' && name[0]>'Z') || name[0]>'z' ||
            (name[0]<'A' && name[0]>'9') || name[0]<'0' ){
        return MATAMAZOM_INVALID_NAME;
    }
    if(amount<0){
        return MATAMAZOM_INVALID_AMOUNT;
    }
    if(amountType==MATAMAZOM_INTEGER_AMOUNT && (0.001<tmp_amount &&
    tmp_amount<0.999)) {
        return MATAMAZOM_INVALID_AMOUNT;
    }
    if(amountType==MATAMAZOM_HALF_INTEGER_AMOUNT && ((0.001<tmp_amount &&
    tmp_amount<0.499) || (0.501<tmp_amount && tmp_amount<0.999))){
        return MATAMAZOM_INVALID_AMOUNT;
    }
    Product tmp=ProductCreate(id,name,amountType,data,copy_data,free_data ,
            get_price);
    if(!tmp){
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    switch (asRegister(matamazom->products_set,tmp)){
        case AS_NULL_ARGUMENT:
            destroyProduct(tmp);
            return MATAMAZOM_NULL_ARGUMENT;
        case AS_ITEM_ALREADY_EXISTS:
            destroyProduct(tmp);
            return MATAMAZOM_PRODUCT_ALREADY_EXIST;
        default:
            asChangeAmount(matamazom->products_set,tmp,amount);
            destroyProduct(tmp);
            return MATAMAZOM_SUCCESS;
    }
}

/**  *   /\           ***************************       /\   *******************
 *****  /||\                   mtmNewProduct           /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/             mtmChangeProductAmount        \||/  *******************
 *****   \/           ***************************       \/   **************** */

MatamazomResult mtmChangeProductAmount(Matamazom matamazom,const unsigned int id
        , const double amount){
    if(!matamazom){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Product tmp =setProid(id);
    if(!tmp){
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    if(asContains(matamazom->products_set,tmp)==false){
        destroyProduct(tmp);
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    Product tmp2=getPro(matamazom->products_set,tmp);
    if(invalidAmount(tmp2,amount)){
        destroyProduct(tmp);
        return MATAMAZOM_INVALID_AMOUNT;
    }
    switch (asChangeAmount(matamazom->products_set,tmp,amount)){
        case AS_INSUFFICIENT_AMOUNT:
            destroyProduct(tmp);
            return MATAMAZOM_INSUFFICIENT_AMOUNT;
        default:
            destroyProduct(tmp);
            return MATAMAZOM_SUCCESS;
    }
}

/**  *   /\           ***************************       /\   *******************
 *****  /||\             mtmChangeProductAmount        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                 mtmClearProduct           \||/  *******************
 *****   \/           ***************************       \/   **************** */

MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id)
{
    if (! matamazom || !matamazom->orders_list ||!matamazom->nextOrderId ) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Product iterator = setProid(id);
    if (! iterator) {
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    switch ( asDelete(matamazom->products_set,iterator) ){
        case AS_ITEM_DOES_NOT_EXIST:
        destroyProduct(iterator);
        return MATAMAZOM_PRODUCT_NOT_EXIST;
        case AS_NULL_ARGUMENT:
        destroyProduct(iterator);
        return  MATAMAZOM_NULL_ARGUMENT;
        default:
        {
            Order list_interator = listGetFirst(matamazom->orders_list);
            unsigned int tmp_id = 0;
            while (list_interator) {
                tmp_id = getProductId(iterator);
                asDelete(getSetOrder(list_interator), &tmp_id);
                list_interator = listGetNext(matamazom->orders_list);
            }
            destroyProduct(iterator);
            return MATAMAZOM_SUCCESS;
        }
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                 mtmClearProduct           /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmCreateNewOrder          \||/  *******************
 *****   \/           ***************************       \/   **************** */


unsigned int mtmCreateNewOrder(Matamazom matamazom){
    if(!matamazom){
        return 0;
    }
    Order new_order=createOrder(matamazom->nextOrderId);
    if(!new_order){
        return 0;
    }
    if(listInsertLast(matamazom->orders_list,new_order)!=LIST_SUCCESS){
        destroyOrder(new_order);
        return 0;
    }
    matamazom->nextOrderId++;
    destroyOrder(new_order);
    return matamazom->nextOrderId-1;
}

/**  *   /\           ***************************       /\   *******************
 *****  /||\                mtmCreateNewOrder          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/         mtmChangeProductAmountInOrder     \||/  *******************
 *****   \/           ***************************       \/   **************** */

MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom,
        const unsigned int orderId, const unsigned int productId,
        const double amount){
    unsigned int tmp_id=productId;
    if(!matamazom){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(!listContainOrder(matamazom->orders_list,orderId)){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    Product tmp=setProid(tmp_id);
    if(!tmp){
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    if(asContains(matamazom->products_set,tmp)==false){
        destroyProduct(tmp);
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    Order tmp_order=getOrder(matamazom->orders_list,orderId);
    if(invalidAmount(getPro(matamazom->products_set,tmp),amount)){
        destroyProduct(tmp);
        return MATAMAZOM_INVALID_AMOUNT;
    }
    destroyProduct(tmp);
    double tmp_amount;
    switch (asChangeAmount(getSetOrder(tmp_order),&tmp_id,amount)){
        case AS_NULL_ARGUMENT:
            return MATAMAZOM_NULL_ARGUMENT;
        case AS_INSUFFICIENT_AMOUNT:
          asDelete(getSetOrder(tmp_order),&tmp_id);
            return MATAMAZOM_SUCCESS;
        case AS_ITEM_DOES_NOT_EXIST:
            if(amount<=0){      //to ask today
                return MATAMAZOM_SUCCESS;
            }
            if(asRegister(getSetOrder(tmp_order),&tmp_id)!=AS_SUCCESS){
                return MATAMAZOM_OUT_OF_MEMORY;
            }
           asChangeAmount(getSetOrder(tmp_order),&tmp_id,amount);
            return MATAMAZOM_SUCCESS;
        default:
            asGetAmount(getSetOrder(tmp_order),&tmp_id,&tmp_amount);
            if(tmp_amount==0){ //the amount in order is 0 them we need to
                asDelete(getSetOrder(tmp_order),&tmp_id);// remove the product form the order
            }
            return MATAMAZOM_SUCCESS;
    }
}

/**  *   /\           ***************************       /\   *******************
 *****  /||\         mtmChangeProductAmountInOrder     /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                 mtmShipOrder              \||/  *******************
 *****   \/           ***************************       \/   **************** */

MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId) {
    if (!matamazom)
        return MATAMAZOM_NULL_ARGUMENT;
    Order order_to_ship=getOrder(matamazom->orders_list,orderId);
    if(!order_to_ship){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    unsigned int *tmp_pro_id=asGetFirst(getSetOrder(order_to_ship));
    if(!tmp_pro_id){
       deleteOrder(matamazom->orders_list,orderId);
       return MATAMAZOM_SUCCESS;
    }
    Product tmp_pro=setProid(1);
    if(!tmp_pro){
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    double pro_amount_in_order, pro_amount_in_warehouse;
    while (tmp_pro_id) {
        setId(tmp_pro,*tmp_pro_id);
        asGetAmount(matamazom->products_set, tmp_pro, &pro_amount_in_warehouse);
        pro_amount_in_order = getProAmInOrder(order_to_ship, *tmp_pro_id);
        if (pro_amount_in_order > pro_amount_in_warehouse) {
            destroyProduct(tmp_pro);
            return MATAMAZOM_INSUFFICIENT_AMOUNT;
        }
        tmp_pro_id=asGetNext(getSetOrder(order_to_ship));
    }//the amounts in order is not greater than the product amount in warehouse
    tmp_pro_id=asGetFirst(getSetOrder(order_to_ship));
    while (tmp_pro_id) {
        setId(tmp_pro,*tmp_pro_id);
        pro_amount_in_order = getProAmInOrder(order_to_ship, *tmp_pro_id);
        asChangeAmount(matamazom->products_set, tmp_pro, -pro_amount_in_order);
        Product z = getPro(matamazom->products_set, tmp_pro);
        setTotalIncome(z, getProductPrice(z, pro_amount_in_order));
        tmp_pro_id = asGetNext(getSetOrder(order_to_ship));
    }
    destroyProduct(tmp_pro);
    deleteOrder(matamazom->orders_list,orderId);
    return MATAMAZOM_SUCCESS;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                 mtmShipOrder              /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmCancelOrder             \||/  *******************
 *****   \/           ***************************       \/   **************** */

MatamazomResult mtmCancelOrder(Matamazom matamazom, const unsigned int orderId){

    if (!matamazom || !(matamazom->orders_list) || !(matamazom->products_set)){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(listContainOrder(matamazom->orders_list,orderId)==false){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    deleteOrder(matamazom->orders_list,orderId);
    return MATAMAZOM_SUCCESS;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                 mtmCancelOrder            /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmPrintInventory          \||/  *******************
 *****   \/           ***************************       \/   **************** */

MatamazomResult mtmPrintInventory(Matamazom matamazom, FILE *output){
    if(!matamazom || !output){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    fprintf(output,"Inventory Status:\n");
    for(Product i=(Product)asGetFirst(matamazom->products_set);i;
    i=(Product)asGetNext(matamazom->products_set)){
        char* tmp=getProductName(i);
        unsigned int tmp_id=getProductId(i);
        double tmp_amount=0;
        asGetAmount(matamazom->products_set,i,&tmp_amount);
        double  tmp_price=getProductPrice(i,1);//to ask the motherfucker
        mtmPrintProductDetails(tmp,tmp_id,tmp_amount,tmp_price,output);
    }
   //fclose(output);
    return MATAMAZOM_SUCCESS;

}

/**  *   /\           ***************************       /\   *******************
 *****  /||\                mtmPrintInventory          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmPrintOrder              \||/  *******************
 *****   \/           ***************************       \/   **************** */


MatamazomResult mtmPrintOrder(Matamazom matamazom, const unsigned int
orderId, FILE *output){
    if(!matamazom || !output){
     /*   if(output) {
            fclose(output);
        }*/
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(listContainOrder(matamazom->orders_list,orderId)==false){
       // fclose(output);
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    mtmPrintOrderHeading(orderId,output);
    Order tmp_order=getOrder(matamazom->orders_list,orderId);
    double total_income=0;
    for(unsigned int* i=asGetFirst(getSetOrder(tmp_order));i;i=asGetNext
            (getSetOrder(tmp_order))){
        unsigned int tmp_id=*i;
        double tmp_amount=getProAmInOrder(tmp_order,tmp_id);
        Product tmp_pro=setProid(tmp_id);
        Product z=getPro(matamazom->products_set,tmp_pro);
        char * tmp_name=getProductName(z);
        double z_price=getProductPrice(z,tmp_amount);
        mtmPrintProductDetails(tmp_name,tmp_id,tmp_amount,z_price,output);
        total_income+=z_price;
        destroyProduct(tmp_pro);
    }
    mtmPrintOrderSummary(total_income,output);
  //  fclose(output);
    return MATAMAZOM_SUCCESS;
}

/**  *   /\           ***************************       /\   *******************
 *****  /||\                mtmPrintOrder              /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmPrintBestSelling        \||/  *******************
 *****   \/           ***************************       \/   **************** */


MatamazomResult mtmPrintBestSelling(Matamazom matamazom, FILE *output){
    if(!matamazom||!output){
       /* if(output) {
            fclose(output);
        }*/
        return MATAMAZOM_NULL_ARGUMENT;
    }
    AmountSet printSet=asCreate(copyFunPro,freeFunPro,compareProForPrint);
    if(!printSet){
       // fclose(output);
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    for(Product i=asGetFirst(matamazom->products_set);i;i=asGetNext
            (matamazom->products_set)){
        if(asRegister(printSet,i)!=AS_SUCCESS){
            asDestroy(printSet);
            //fclose(output);
            return MATAMAZOM_OUT_OF_MEMORY;
        }
    }
    fprintf(output,"Best Selling Product:\n");
    double max_income=getTotalIncome(asGetFirst(printSet));
    char *tmp = getProductName(asGetFirst(printSet));
    unsigned int tmp_id = getProductId(asGetFirst(printSet));
    if (max_income==0){ //check if none
        asDestroy(printSet);
        fprintf(output,"none\n");
     //fclose(output);
        return MATAMAZOM_SUCCESS;
    }
    mtmPrintIncomeLine(tmp, tmp_id, max_income, output);
    asDestroy(printSet);
    //fclose(output);
    return MATAMAZOM_SUCCESS;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                mtmPrintBestSelling        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************


// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmPrintFiltered        \||/  *******************
 *****   \/           ***************************       \/   **************** */

MatamazomResult mtmPrintFiltered(Matamazom matamazom, MtmFilterProduct
customFilter, FILE *output){
    if(!matamazom || !customFilter || !output){
      /*  if(output) {
            fclose(output);
        }*/
        return MATAMAZOM_NULL_ARGUMENT;
    }
    for(Product i=(Product)asGetFirst(matamazom->products_set);i;
        i=(Product)asGetNext(matamazom->products_set)){
        char* tmp=getProductName(i);
        unsigned int tmp_id=getProductId(i);
        double tmp_amount=0;
        asGetAmount(matamazom->products_set,i,&tmp_amount);
        double  tmp_price=getProductPrice(i,1);//to ask the motherfucker
        if(customFilter(tmp_id,tmp,tmp_amount,getProuductData(i))){
        mtmPrintProductDetails(tmp,tmp_id,tmp_amount,tmp_price,output);
        }
    }
   // fclose(output);
    return MATAMAZOM_SUCCESS;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                mtmPrintFiltered        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************
