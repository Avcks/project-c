//checkout.h
#ifndef CHECKOUT_H
#define CHECKOUT_H

/* Function declarations */
void checkoutProcess();
void showBillSummary();
float calculateTax(float amount);
void processCashPayment(float totalAmount);
void processCardPayment(float totalAmount);
void clearCartAfterPayment();

#endif


//checkout.c
#include <stdio.h>
#include "checkout.h"
#include "cart.h"

/* Checkout main function */
void checkoutProcess()
{
    int paymentChoice;
    float subtotal;
    float tax;
    float total;

    if (isCartEmpty())
    {
        printf("\nCannot checkout. Cart is empty.\n");
        return;
    }

    subtotal = calculateSubtotal();
    tax = calculateTax(subtotal);
    total = subtotal + tax;

    showBillSummary();

    printf("\nSelect Payment Method\n");
    printf("1. Cash Payment\n");
    printf("2. Card Payment\n");
    printf("Enter choice: ");
    scanf("%d", &paymentChoice);

    switch (paymentChoice)
    {
        case 1:
            processCashPayment(total);
            break;

        case 2:
            processCardPayment(total);
            break;

        default:
            printf("\nInvalid payment option.\n");
    }
}

/* Show bill summary */
void showBillSummary()
{
    float subtotal;
    float tax;
    float total;

    subtotal = calculateSubtotal();
    tax = calculateTax(subtotal);
    total = subtotal + tax;

    printf("\n=========== BILL SUMMARY ===========\n");
    viewCart();
    printf("Tax (5%%): %.2f\n", tax);
    printf("Total Amount: %.2f\n", total);
    printf("===================================\n");
}

/* Calculate tax */
float calculateTax(float amount)
{
    float taxRate = 0.05;
    float tax;

    tax = amount * taxRate;
    return tax;
}

/* Cash payment */
void processCashPayment(float totalAmount)
{
    float cashPaid;

    printf("\nEnter cash amount: ");
    scanf("%f", &cashPaid);

    if (cashPaid < totalAmount)
    {
        printf("\nInsufficient cash. Payment failed.\n");
        return;
    }

    printf("\nPayment successful.\n");
    printf("Change returned: %.2f\n", cashPaid - totalAmount);

    clearCartAfterPayment();
}

/* Card payment */
void processCardPayment(float totalAmount)
{
    int pin;

    printf("\nEnter 4-digit PIN: ");
    scanf("%d", &pin);

    printf("\nProcessing card payment...\n");
    printf("Payment successful.\n");
    printf("Amount charged: %.2f\n", totalAmount);

    clearCartAfterPayment();
}

/* Clear cart after payment */
void clearCartAfterPayment()
{
    initCart();
    printf("\nCart cleared. Thank you for shopping!\n");
}
