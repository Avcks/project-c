//main.c
#include <stdio.h>
#include <stdlib.h>

/* Function declarations */
void showWelcomeScreen();
void showMainMenu();
void customerMenu();
void adminMenu();

/* Entry point */
int main()
{
    int choice = 0;

    showWelcomeScreen();

    while (1)
    {
        showMainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                customerMenu();
                break;

            case 2:
                adminMenu();
                break;

            case 3:
                printf("\nThank you for using the system.\n");
                printf("Program terminated successfully.\n");
                exit(0);

            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }

    return 0;
}

/* Welcome screen */
void showWelcomeScreen()
{
    printf("=========================================\n");
    printf("   SMART SELF CHECKOUT SYSTEM (C PROJECT) \n");
    printf("=========================================\n");
    printf("University Level Mini Project\n");
    printf("First Year – C Programming\n\n");
}

/* Main menu */
void showMainMenu()
{
    printf("\n------------- MAIN MENU -------------\n");
    printf("1. Customer Mode\n");
    printf("2. Admin Mode\n");
    printf("3. Exit Program\n");
    printf("-------------------------------------\n");
}

/* Customer menu */
void customerMenu()
{
    int choice = 0;

    while (1)
    {
        printf("\n--------- CUSTOMER MENU ---------\n");
        printf("1. Scan Product (Not implemented yet)\n");
        printf("2. View Cart (Not implemented yet)\n");
        printf("3. Checkout (Not implemented yet)\n");
        printf("4. Go Back to Main Menu\n");
        printf("---------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("\n[INFO] Scan Product feature will be added in next module.\n");
                break;

            case 2:
                printf("\n[INFO] View Cart feature will be added in next module.\n");
                break;

            case 3:
                printf("\n[INFO] Checkout feature will be added in next module.\n");
                break;

            case 4:
                printf("\nReturning to Main Menu...\n");
                return;

            default:
                printf("\nInvalid option. Try again.\n");
        }
    }
}

/* Admin menu */
void adminMenu()
{
    int choice = 0;

    while (1)
    {
        printf("\n----------- ADMIN MENU -----------\n");
        printf("1. View Products (Coming Soon)\n");
        printf("2. Add Product (Coming Soon)\n");
        printf("3. Sales Report (Coming Soon)\n");
        printf("4. Go Back to Main Menu\n");
        printf("---------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("\n[INFO] View Products will be implemented later.\n");
                break;

            case 2:
                printf("\n[INFO] Add Product will be implemented later.\n");
                break;

            case 3:
                printf("\n[INFO] Sales Report will be implemented later.\n");
                break;

            case 4:
                printf("\nReturning to Main Menu...\n");
                return;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }
}












//menu_system.c

#include <stdio.h>

/* This file will later handle:
   - Menu formatting
   - Input validation
   - Navigation helpers
   For now, it is intentionally simple.
*/

void printLine()
{
    printf("----------------------------------------\n");
}

void pauseScreen()
{
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

void clearScreen()
{
    int i;
    for (i = 0; i < 30; i++)
    {
        printf("\n");
    }
}
