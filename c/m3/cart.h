//cart.h
#ifndef CART_H
#define CART_H

#define MAX_CART_ITEMS 100

/* Cart item structure */
struct CartItem
{
    int productCode;
    char productName[50];
    float price;
    int quantity;
};

/* Function declarations */
void initCart();
void addItemToCart();
void viewCart();
float calculateSubtotal();
int isCartEmpty();

#endif