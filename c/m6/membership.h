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