
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
