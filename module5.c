//admin.h
#ifndef ADMIN_H
#define ADMIN_H

/* Function declarations */
int adminLogin();
void setAdminPassword();
int checkPassword(int inputPassword);

#endif



//admin.c
#include <stdio.h>
#include "admin.h"

/* Store password globally (default 1234) */
int adminPassword = 1234;

/* Number of wrong attempts */
int wrongAttempts = 0;
int maxAttempts = 3;

/* Admin login function */
int adminLogin()
{
    int input;
    wrongAttempts = 0;

    while (wrongAttempts < maxAttempts)
    {
        printf("\nEnter Admin 4-digit password: ");
        scanf("%d", &input);

        if (checkPassword(input))
        {
            printf("\nLogin successful!\n");
            return 1;
        }
        else
        {
            wrongAttempts++;
            printf("Incorrect password. Attempts left: %d\n", maxAttempts - wrongAttempts);
        }
    }

    printf("\nMaximum attempts reached. Access denied.\n");
    return 0;
}

/* Check password */
int checkPassword(int inputPassword)
{
    if (inputPassword == adminPassword)
        return 1;
    else
        return 0;
}

/* Set a new password */
void setAdminPassword()
{
    int newPassword;
    printf("\nEnter new 4-digit password: ");
    scanf("%d", &newPassword);

    if (newPassword < 1000 || newPassword > 9999)
    {
        printf("Invalid password. Must be 4 digits.\n");
        return;
    }

    adminPassword = newPassword;
    printf("Admin password updated successfully.\n");
}
