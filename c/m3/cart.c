//cart.c
#include <stdio.h>
#include <string.h>
#include "cart.h"
#include "inventory.h"

/* Cart array */
struct CartItem cart[MAX_CART_ITEMS];

/* Number of items in cart */
int cartItemCount = 0;

/* Initialize cart */
void initCart()
{
    cartItemCount = 0;
}

/* Check if cart is empty */
int isCartEmpty()
{
    if (cartItemCount == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Add item to cart */
void addItemToCart()
{
    int code;
    int quantity;
    int i;
    int found = 0;

    showAllProducts();

    printf("\nEnter product code to add: ");
    scanf("%d", &code);

    for (i = 0; i < getProductCount(); i++)
    {
        if (products[i].code == code)
        {
            found = 1;

            printf("Enter quantity: ");
            scanf("%d", &quantity);

            if (quantity <= 0)
            {
                printf("\nInvalid quantity.\n");
                return;
            }

            if (quantity > products[i].stock)
            {
                printf("\nNot enough stock available.\n");
                return;
            }

            cart[cartItemCount].productCode = products[i].code;
            strcpy(cart[cartItemCount].productName, products[i].name);
            cart[cartItemCount].price = products[i].price;
            cart[cartItemCount].quantity = quantity;

            cartItemCount++;

            products[i].stock = products[i].stock - quantity;

            printf("\nItem added to cart successfully.\n");
            return;
        }
    }

    if (found == 0)
    {
        printf("\nProduct not found.\n");
    }
}

/* View cart items */
void viewCart()
{
    int i;
    float total;

    if (isCartEmpty())
    {
        printf("\nCart is empty.\n");
        return;
    }

    printf("\n----------------------------------------------\n");
    printf("ITEM\t\tQTY\tPRICE\tTOTAL\n");
    printf("----------------------------------------------\n");

    for (i = 0; i < cartItemCount; i++)
    {
        total = cart[i].price * cart[i].quantity;

        printf("%-12s\t%d\t%.2f\t%.2f\n",
               cart[i].productName,
               cart[i].quantity,
               cart[i].price,
               total);
    }

    printf("----------------------------------------------\n");
    printf("Subtotal: %.2f\n", calculateSubtotal());
}

/* Calculate subtotal */
float calculateSubtotal()
{
    int i;
    float subtotal = 0.0;

    for (i = 0; i < cartItemCount; i++)
    {
        subtotal = subtotal + (cart[i].price * cart[i].quantity);
    }

    return subtotal;
}