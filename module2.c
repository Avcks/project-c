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






//inventory.c
#include <stdio.h>
#include <string.h>
#include "inventory.h"

/* Global product array */
struct Product products[MAX_PRODUCTS];

/* Number of products */
int productCount = 0;

/* Initialize default products */
void initProducts()
{
    products[0].code = 101;
    strcpy(products[0].name, "Milk Pack");
    products[0].price = 45.0;
    products[0].stock = 100;

    products[1].code = 102;
    strcpy(products[1].name, "Bread");
    products[1].price = 30.0;
    products[1].stock = 80;

    products[2].code = 103;
    strcpy(products[2].name, "Rice 5kg");
    products[2].price = 350.0;
    products[2].stock = 40;

    products[3].code = 104;
    strcpy(products[3].name, "Cooking Oil");
    products[3].price = 180.0;
    products[3].stock = 60;

    products[4].code = 105;
    strcpy(products[4].name, "Eggs 12");
    products[4].price = 70.0;
    products[4].stock = 90;

    productCount = 5;
}

/* Display all products */
void showAllProducts()
{
    int i;

    printf("\n---------------------------------------------\n");
    printf("CODE\tNAME\t\tPRICE\tSTOCK\n");
    printf("---------------------------------------------\n");

    for (i = 0; i < productCount; i++)
    {
        printf("%d\t%-12s\t%.2f\t%d\n",
               products[i].code,
               products[i].name,
               products[i].price,
               products[i].stock);
    }

    printf("---------------------------------------------\n");
}

/* Add new product (Admin only) */
void addNewProduct()
{
    if (productCount >= MAX_PRODUCTS)
    {
        printf("\nInventory full. Cannot add more products.\n");
        return;
    }

    printf("\nEnter product code: ");
    scanf("%d", &products[productCount].code);

    printf("Enter product name: ");
    scanf(" %[^\n]", products[productCount].name);

    printf("Enter product price: ");
    scanf("%f", &products[productCount].price);

    printf("Enter product stock: ");
    scanf("%d", &products[productCount].stock);

    productCount++;

    printf("\nProduct added successfully.\n");
}

/* Return product count */
int getProductCount()
{
    return productCount;
}
