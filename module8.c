//main.c
#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"
#include "cart.h"
#include "checkout.h"
#include "admin.h"
#include "membership.h"
#include "sales.h"

/* Main function */
int main()
{
    int mainChoice;
    int customerChoice;
    int adminChoice;

    /* Initialize all modules */
    initProducts();
    initCart();
    initMembers();

    printf("==========================================\n");
    printf("  WELCOME TO COSTCO SELF CHECKOUT POS\n");
    printf("==========================================\n");

    while (1)
    {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Customer Mode\n");
        printf("2. Admin Mode\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &mainChoice);

        switch (mainChoice)
        {
            case 1: /* Customer Mode */
                while (1)
                {
                    printf("\n--- CUSTOMER MENU ---\n");
                    printf("1. Scan Item / Add to Cart\n");
                    printf("2. View Cart\n");
                    printf("3. Checkout\n");
                    printf("4. Exit Customer Mode\n");
                    printf("Enter choice: ");
                    scanf("%d", &customerChoice);

                    switch (customerChoice)
                    {
                        case 1:
                            addItemToCart();
                            break;

                        case 2:
                            viewCart();
                            break;

                        case 3:
                        {
                            float discount = applyMembershipDiscount();
                            checkoutProcess();
                            autoUpgradeMembership();
                            recordSale();
                            break;
                        }

                        case 4:
                            printf("\nExiting Customer Mode...\n");
                            break;

                        default:
                            printf("Invalid choice.\n");
                    }

                    if (customerChoice == 4)
                        break;
                }
                break;

            case 2: /* Admin Mode */
                if (!adminLogin())
                    break;

                while (1)
                {
                    printf("\n--- ADMIN MENU ---\n");
                    printf("1. Add Product\n");
                    printf("2. View Inventory\n");
                    printf("3. Add Member\n");
                    printf("4. View Members\n");
                    printf("5. Sales Report\n");
                    printf("6. Reset Sales Report\n");
                    printf("7. Change Admin Password\n");
                    printf("8. Exit Admin Mode\n");
                    printf("Enter choice: ");
                    scanf("%d", &adminChoice);

                    switch (adminChoice)
                    {
                        case 1:
                            addNewProduct();
                            break;

                        case 2:
                            showAllProducts();
                            break;

                        case 3:
                            addNewMember();
                            break;

                        case 4:
                            showAllMembers();
                            break;

                        case 5:
                            showSalesReport();
                            break;

                        case 6:
                            resetSalesReport();
                            break;

                        case 7:
                            setAdminPassword();
                            break;

                        case 8:
                            printf("\nExiting Admin Mode...\n");
                            break;

                        default:
                            printf("Invalid choice.\n");
                    }

                    if (adminChoice == 8)
                        break;
                }
                break;

            case 3:
                printf("\nExiting POS system. Goodbye!\n");
                exit(0);
                break;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}






