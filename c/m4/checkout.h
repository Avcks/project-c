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