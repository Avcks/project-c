//sales.h
#ifndef SALES_H
#define SALES_H

/* Function declarations */
void recordSale();
void showSalesReport();
void resetSalesReport();

#endif




//sales.c
#include <stdio.h>
#include "sales.h"
#include "inventory.h"
#include "cart.h"

/* Global arrays to track sales per product */
int itemsSold[MAX_PRODUCTS] = {0};
float revenuePerProduct[MAX_PRODUCTS] = {0.0};
float totalRevenue = 0.0;
int totalTransactions = 0;

/* Record a sale after checkout */
void recordSale()
{
    int i;
    float subtotal;

    if (isCartEmpty())
        return;

    subtotal = calculateSubtotal();

    for (i = 0; i < cartItemCount; i++)
    {
        int code = cart[i].productCode;
        int j;
        /* Find product index in inventory */
        for (j = 0; j < productCount; j++)
        {
            if (products[j].code == code)
            {
                itemsSold[j] += cart[i].quantity;
                revenuePerProduct[j] += cart[i].quantity * cart[i].price;
            }
        }
    }

    totalRevenue += subtotal;
    totalTransactions++;
}

/* Show sales report */
void showSalesReport()
{
    int i;

    printf("\n============= SALES REPORT =============\n");
    printf("Total Transactions: %d\n", totalTransactions);
    printf("Total Revenue: %.2f\n", totalRevenue);
    printf("----------------------------------------\n");
    printf("Product\t\tQty Sold\tRevenue\n");
    printf("----------------------------------------\n");

    for (i = 0; i < productCount; i++)
    {
        printf("%-12s\t%d\t\t%.2f\n",
               products[i].name,
               itemsSold[i],
               revenuePerProduct[i]);
    }

    printf("----------------------------------------\n");
}

/* Reset sales report */
void resetSalesReport()
{
    int i;
    for (i = 0; i < MAX_PRODUCTS; i++)
    {
        itemsSold[i] = 0;
        revenuePerProduct[i] = 0.0;
    }
    totalRevenue = 0.0;
    totalTransactions = 0;
    printf("\nSales report reset successfully.\n");
}
