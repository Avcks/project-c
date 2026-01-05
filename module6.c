//membership.h
#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#define MAX_MEMBERS 20

/* Member structure */
struct Member
{
    int id;
    char name[50];
    float discount; // e.g., 0.05 for 5%
    float totalSpent; // track total spending for auto-upgrade
};

/* Function declarations */
void initMembers();
void showAllMembers();
int applyMembershipDiscount();
void addNewMember();
void autoUpgradeMembership();

#endif


//  membership.c
#include <stdio.h>
#include <string.h>
#include "membership.h"
#include "cart.h"

/* Global array of members */
struct Member members[MAX_MEMBERS];

/* Number of members */
int memberCount = 0;

/* Initialize 5 default members */
void initMembers()
{
    members[0].id = 1;
    strcpy(members[0].name, "Gold Member");
    members[0].discount = 0.10;
    members[0].totalSpent = 0.0;

    members[1].id = 2;
    strcpy(members[1].name, "Silver Member");
    members[1].discount = 0.05;
    members[1].totalSpent = 0.0;

    members[2].id = 3;
    strcpy(members[2].name, "Platinum Member");
    members[2].discount = 0.15;
    members[2].totalSpent = 0.0;

    members[3].id = 4;
    strcpy(members[3].name, "Diamond Member");
    members[3].discount = 0.20;
    members[3].totalSpent = 0.0;

    members[4].id = 5;
    strcpy(members[4].name, "Basic Member");
    members[4].discount = 0.02;
    members[4].totalSpent = 0.0;

    memberCount = 5;
}

/* Show all members */
void showAllMembers()
{
    int i;
    printf("\nID\tNAME\t\tDISCOUNT\tTOTAL SPENT\n");
    printf("-----------------------------------------------\n");

    for (i = 0; i < memberCount; i++)
    {
        printf("%d\t%-12s\t%.2f\t\t%.2f\n",
               members[i].id,
               members[i].name,
               members[i].discount * 100,
               members[i].totalSpent);
    }

    printf("-----------------------------------------------\n");
}

/* Add a new member (Admin only) */
void addNewMember()
{
    if (memberCount >= MAX_MEMBERS)
    {
        printf("\nCannot add more members. Max limit reached.\n");
        return;
    }

    int id;
    char name[50];
    float discount;

    printf("\nEnter Member ID: ");
    scanf("%d", &id);

    printf("Enter Member Name: ");
    scanf(" %[^\n]", name);

    printf("Enter discount (e.g., 0.05 for 5%%): ");
    scanf("%f", &discount);

    members[memberCount].id = id;
    strcpy(members[memberCount].name, name);
    members[memberCount].discount = discount;
    members[memberCount].totalSpent = 0.0;

    memberCount++;

    printf("\nMember added successfully.\n");
}

/* Apply membership discount at checkout */
int applyMembershipDiscount()
{
    int id;
    int i;
    float subtotal;
    float discountAmount;

    if (isCartEmpty())
    {
        printf("\nCannot apply discount. Cart is empty.\n");
        return 0;
    }

    subtotal = calculateSubtotal();

    printf("\nEnter Membership ID (0 if none): ");
    scanf("%d", &id);

    if (id == 0)
        return 0;

    for (i = 0; i < memberCount; i++)
    {
        if (members[i].id == id)
        {
            discountAmount = subtotal * members[i].discount;
            printf("Membership applied: %.2f discount\n", discountAmount);

            /* Track total spent for auto-upgrade */
            members[i].totalSpent += subtotal;

            return discountAmount;
        }
    }

    printf("Invalid Membership ID.\n");
    return 0;
}

/* Auto-upgrade membership based on total spent */
void autoUpgradeMembership()
{
    int i;

    for (i = 0; i < memberCount; i++)
    {
        if (members[i].totalSpent > 5000.0 && members[i].discount < 0.20)
        {
            members[i].discount = 0.20;
            strcpy(members[i].name, "Diamond Member");
            printf("\nMember ID %d upgraded to Diamond!\n", members[i].id);
        }
        else if (members[i].totalSpent > 2000.0 && members[i].discount < 0.15)
        {
            members[i].discount = 0.15;
            strcpy(members[i].name, "Platinum Member");
            printf("\nMember ID %d upgraded to Platinum!\n", members[i].id);
        }
        else if (members[i].totalSpent > 1000.0 && members[i].discount < 0.10)
        {
            members[i].discount = 0.10;
            strcpy(members[i].name, "Gold Member");
            printf("\nMember ID %d upgraded to Gold!\n", members[i].id);
        }
    }
}
