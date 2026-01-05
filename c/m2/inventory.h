//inventory.h
#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_PRODUCTS 100

/* Product structure */
struct Product
{
    int code;
    char name[50];
    float price;
    int stock;
};

/* Function declarations */
void initProducts();
void showAllProducts();
void addNewProduct();
int getProductCount();

#endif