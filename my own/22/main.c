/*
 ============================================================
  AYH MART - SELF CHECKOUT SYSTEM
  Developed By: Aum, Yesh, HariKrushna
  Location: Surat, India
 ============================================================
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define MAX_ITEMS      100
#define MAX_MEMBERS    100
#define MAX_CART        50
#define MAX_CUSTOMERS  100
#define MAX_SALES 1000

#define MEMBER_ID_BASE  1000
#define TXID_BASE       1000

#define FILE_PRODUCTS  "ayhmart_products.dat"
#define FILE_MEMBERS   "ayhmart_members.dat"
#define FILE_CUSTOMERS "ayhmart_customers.dat"
#define FILE_SALES     "ayhmart_sales.dat"
#define FILE_COUNTERS  "ayhmart_counters.dat"

/* ------------------ ANSI COLOR CODES ------------------ */
#define RESET       "\033[0m"
#define BOLD        "\033[1m"

/* Text colors */
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

/* Bright variants */
#define BRED        "\033[91m"
#define BGREEN      "\033[92m"
#define BYELLOW     "\033[93m"
#define BBLUE       "\033[94m"
#define BMAGENTA    "\033[95m"
#define BCYAN       "\033[96m"
#define BWHITE      "\033[97m"

/* Background colors */
#define BG_BLUE     "\033[44m"
#define BG_GREEN    "\033[42m"
#define BG_RED      "\033[41m"

/* Convenience macros */
#define OK(msg)     printf("  " BGREEN "[OK]" RESET " %s\n", msg)
#define ERR(msg)    printf("  " BRED   "[X]"  RESET " %s\n", msg)
#define WARN(msg)   printf("  " BYELLOW "[!]" RESET " %s\n", msg)

unsigned long ad_user_hash = 193488787UL;
unsigned long ad_pass_hash = 193488787UL;
unsigned long sf_user_hash = 193500721UL;
unsigned long sf_pass_hash = 193500721UL;

/* ------------------ FORWARD DECLARATIONS ------------------ */
void mainManu();
int  isCustomerIdExists(int id);
int  isProductCodeExists(int code);
int  isProductNameExists(char name[], int excludeIndex);
int  strEqualCI(const char *a, const char *b);
unsigned long djb2Hash(const char *str);
void flushInput();
int  readInt(int *out);
void admin();
void adminDashboard();
void inventory();
void add();
void login();
void view();
void del();
void editProduct();
void info();
void cust();
void custView();
void members();
void addMembers();
void deleteMembers();
void viewMembers();
void search();
void addCart();
void viewCart();
void checkout();
void salesManu();
void salesSummary();
void staffSalesSummary();
void productSales();
void discountReport();
void recordSale(int code, char name[], int qty, float price, int custId);
float applyDiscount(int, float);
void billing(float finalAmount, int isQR);
void staff();
void inputPassword(char *password);
void showCustomerHistory(int custId);
void custManage();
void addCustomer();
void deleteCustomer();
void viewCustomers();
int  customerLogin();
void viewMyCard();
void saveProducts();
void loadProducts();
void saveMembers();
void loadMembers();
void saveCustomers();
void loadCustomers();
void saveSales();
void loadSales();
void saveCounters();
void loadCounters();
void saveAllData();
void loadAllData();
void backupRestore();

int totalQRPayments  = 0;
int totalCashPayments = 0;

/* ------------------ STRUCTURES ------------------ */
struct Product {
    int   pCode;
    char  pName[50];
    float pPrice;
    int   pStock;
    char  pCategory[30];
} shop[MAX_ITEMS];
int nProducts = 0;

struct Member {
    int   mId;
    char  mName[50];
    float mSpent;
    char  mTier[20];
    float mDisc;
    int   mUsed;
} Members[MAX_MEMBERS];
int nMembers = 0;

struct CartItem {
    int   code;
    char  name[50];
    float price;
    int   qty;
} cart[MAX_CART];
int cartCount = 0;

struct Sale {
    int   pCode;
    char  pName[50];
    int   qty;
    float price;
    float total;
    int   custId;
};
struct Sale sales[MAX_SALES];
int nSales       = 0;
float totalRevenue  = 0;
float totalDiscount = 0;
int totalBills   = 0;

struct Customer {
    int  cId;
    char cName[50];
    char cPass[20];
    char cPhone[15];
    char cEmail[50];
    int  cMemberId;
} customers[MAX_CUSTOMERS];
int nCustomers   = 0;
int custLoggedIn = 0;
int currentCustIdx = -1;

/* ------------------ STYLING HELPERS ------------------ */
void printLine() {
    printf(CYAN "============================================================" RESET "\n");
}
void printDash() {
    printf(BLUE "------------------------------------------------------------" RESET "\n");
}
void printHeader(const char *title) {
    printf("\n");
    printLine();
    printf(BOLD BWHITE "   %s\n" RESET, title);
    printLine();
}

/* ------------------ UTILITY HELPERS ------------------ */
void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int readInt(int *out) {
    int result = scanf("%d", out);
    if (result != 1) {
        flushInput();
        return 0;
    }
    return 1;
}

int strEqualCI(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

unsigned long djb2Hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/* ------------------ MAIN ------------------ */
int main() {
    SetConsoleOutputCP(CP_UTF8);

    /* Enable ANSI escape codes on Windows 10+ */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    shop[0] = (struct Product){101, "Rice",     45.00,  80, "Grocery"};
    shop[1] = (struct Product){102, "Milk",     25.00, 120, "Dairy"};
    shop[2] = (struct Product){103, "Bread",    30.00,  60, "Bakery"};
    shop[3] = (struct Product){104, "Shampoo",  90.00,  40, "Personal"};
    shop[4] = (struct Product){105, "PenDrive", 350.00, 15, "Electronics"};
    nProducts = 5;

    customers[0] = (struct Customer){1, "aum",  "123", "9876543210", "aum@email.com",  0};
    customers[1] = (struct Customer){2, "hari", "123", "9876543211", "hari@email.com", 0};
    customers[2] = (struct Customer){3, "yash", "123", "9876543212", "yash@email.com", 0};
    nCustomers = 3;

    loadAllData();
    mainManu();
    return 0;
}

/* ------------------ MAIN MENU ------------------ */
void mainManu() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printf("  " BGREEN "1." RESET " Customer Self-Checkout\n");
        printf("  " BCYAN  "2." RESET " Staff / Admin Login\n");
        printf("  " BYELLOW "3." RESET " View Store Info\n");
        printf("  " BRED   "0." RESET " Exit Application\n");
        printDash();
        printf("  " BOLD "Enter your choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input. Try again.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1: cust();  break;
            case 2: login(); break;
            case 3: info();  break;
            case 0:
                printHeader("Thank you for visiting AYH Mart!");
                exit(0);
            default:
                WARN("Invalid option. Try again.");
                system("pause");
        }
    }
}

/* ------------------ LOGIN ------------------ */
void login() {
    char username[20], password[20];
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("LOGIN");
    printf("  " BCYAN "Username: " RESET);
    scanf("%19s", username);
    printf("  " BCYAN "Password: " RESET);
    inputPassword(password);

    unsigned long uHash = djb2Hash(username);
    unsigned long pHash = djb2Hash(password);

    if (uHash == ad_user_hash && pHash == ad_pass_hash) {
        printf("\n");
        OK("Logged in as " BMAGENTA "ADMIN");
        system("pause");
        adminDashboard();
    } else if (uHash == sf_user_hash && pHash == sf_pass_hash) {
        printf("\n");
        OK("Logged in as " BCYAN "STAFF");
        system("pause");
        staff();
    } else {
        printf("\n");
        ERR("Incorrect username or password.");
        system("pause");
    }
}

/* ------------------ ADMIN DASHBOARD ------------------ */
void adminDashboard() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("ADMIN PORTAL");
        printf("  " BGREEN  "1." RESET " Inventory\n");
        printf("  " BCYAN   "2." RESET " Members\n");
        printf("  " BYELLOW "3." RESET " Sales Report\n");
        printf("  " BBLUE   "4." RESET " Customer Accounts\n");
        printf("  " BMAGENTA "5." RESET " Backup & Restore Data\n");
        printf("  " BRED    "0." RESET " Logout\n");
        printDash();
        printf("  " BOLD "Enter your choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1: inventory();    break;
            case 2: members();      break;
            case 3: salesManu();    break;
            case 4: custManage();   break;
            case 5: backupRestore(); break;
            case 0:
                printf("\n");
                OK("Logged out successfully.");
                printf("\n");
                system("pause");
                return;
            default:
                WARN("Invalid option.");
                system("pause");
        }
    }
}

/* ------------------ INVENTORY ------------------ */
void inventory() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("INVENTORY MANAGEMENT");
        printf("  " BGREEN  "1." RESET " Add Product(s)\n");
        printf("  " BYELLOW "2." RESET " Edit Product\n");
        printf("  " BRED    "3." RESET " Delete Product\n");
        printf("  " BCYAN   "4." RESET " View All Products\n");
        printf("  " BWHITE  "0." RESET " Back\n");
        printDash();
        printf("  " BOLD "Enter your choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1: add();         break;
            case 2: editProduct(); break;
            case 3: del();         break;
            case 4: view();        break;
            case 0: return;
            default:
                WARN("Invalid option.");
                system("pause");
        }
    }
}

/* -- Add products -- */
void add() {
    int count, i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  How many products to add: ");

    if (!readInt(&count) || count <= 0) {
        WARN("Invalid count.");
        system("pause");
        return;
    }

    for (i = 0; i < count; i++) {
        if (nProducts >= MAX_ITEMS) {
            WARN("Product limit reached (" BYELLOW "100" RESET " max).");
            system("pause");
            return;
        }
        printf("\n  " BOLD BCYAN "-- Product %d --" RESET "\n", i + 1);

        int code;
        while (1) {
            printf("  Code    : ");
            if (!readInt(&code)) {
                WARN("Invalid code. Try again.");
                continue;
            }
            if (isProductCodeExists(code)) {
                WARN("Product code already exists. Enter different code.");
            } else {
                shop[nProducts].pCode = code;
                break;
            }
        }

        char tempName[50];
        while (1) {
            printf("  Name    : ");
            scanf(" %49s", tempName);
            if (isProductNameExists(tempName, -1)) {
                WARN("Product with this name already exists. Enter different name.");
            } else {
                strcpy(shop[nProducts].pName, tempName);
                break;
            }
        }

        printf("  Price   : ");
        scanf("%f", &shop[nProducts].pPrice);
        printf("  Stock   : ");
        if (!readInt(&shop[nProducts].pStock)) shop[nProducts].pStock = 0;
        printf("  Category: ");
        scanf("%29s", shop[nProducts].pCategory);
        nProducts++;
    }
    printf("\n");
    printf("  " BGREEN "[OK]" RESET " " BGREEN "%d" RESET " product(s) added successfully.\n", count);
    saveProducts();
    system("pause");
}

/* -- Edit product -- */
void editProduct() {
    int code, found = 0, i;
    printf("\n  Enter Product Code to Edit: ");
    if (!readInt(&code)) {
        WARN("Invalid code.");
        system("pause");
        return;
    }

    for (i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            found = 1;
            printf("\n  Editing: " BYELLOW "%s" RESET "\n", shop[i].pName);
            printf("  Leave field blank / enter 0 to keep current value.\n\n");

            char  buf[50];
            float fval;
            int   ival;

            while (1) {
                printf("  New Name  [" BCYAN "%s" RESET "]: ", shop[i].pName);
                scanf(" %49s", buf);
                if (strcmp(buf, "0") == 0) break;
                if (isProductNameExists(buf, i)) {
                    WARN("Product with this name already exists. Try different name.");
                } else {
                    strcpy(shop[i].pName, buf);
                    break;
                }
            }

            printf("  New Price [" BCYAN "%.2f" RESET "]: ", shop[i].pPrice);
            scanf("%f", &fval);
            if (fval > 0) shop[i].pPrice = fval;

            printf("  New Stock [" BCYAN "%d" RESET "]: ", shop[i].pStock);
            if (readInt(&ival) && ival >= 0) shop[i].pStock = ival;

            printf("  New Category [" BCYAN "%s" RESET "]: ", shop[i].pCategory);
            scanf(" %29s", buf);
            if (strcmp(buf, "0") != 0) strcpy(shop[i].pCategory, buf);

            printf("\n");
            OK("Product updated successfully.");
            saveProducts();
            break;
        }
    }
    if (!found) ERR("Product code not found.");
    system("pause");
}

/* -- View products -- */
void view() {
    int i;
    system("cls");
    printHeader("AVAILABLE PRODUCTS");
    if (nProducts == 0) {
        WARN("No products in inventory.");
    } else {
        printf("  " BOLD BYELLOW "%-6s  %-18s  %-8s  %-6s  %s" RESET "\n",
               "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
        printDash();
        for (i = 0; i < nProducts; i++) {
            printf("  " BCYAN "%-6d" RESET "  %-18s  " BGREEN "%-8.2f" RESET "  ",
                   shop[i].pCode, shop[i].pName, shop[i].pPrice);
            if (shop[i].pStock <= 5)
                printf(BRED "%-6d" RESET "  %s\n", shop[i].pStock, shop[i].pCategory);
            else
                printf(BGREEN "%-6d" RESET "  %s\n", shop[i].pStock, shop[i].pCategory);
        }
    }
    printf("\n");
    system("pause");
}

/* -- Delete product -- */
void del() {
    int code, found = 0, i, j;
    printf("\n  Enter Product Code to Delete: ");
    if (!readInt(&code)) {
        WARN("Invalid code.");
        system("pause");
        return;
    }

    for (i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            for (j = i; j < nProducts - 1; j++) shop[j] = shop[j + 1];
            nProducts--;
            found = 1;
            OK("Product deleted successfully.");
            saveProducts();
            break;
        }
    }
    if (!found) ERR("Product code not found.");
    system("pause");
}

/* ------------------ STORE INFO ------------------ */
void info() {
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("STORE INFORMATION");
    printf("  " BCYAN   "Store Name   : " RESET "AYH Mart\n");
    printf("  " BCYAN   "Location     : " RESET "Surat, India\n");
    printf("  " BCYAN   "Timing       : " RESET "9 AM - 10 PM (Mon-Sun)\n");
    printf("  " BMAGENTA "Developed By : " RESET "Aum, Yesh, HariKrushna\n");
    printLine();
    system("pause");
}

/* ------------------ CUSTOMER LOGIN ------------------ */
int customerLogin() {
    int  id, i;
    char pass[20];
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("CUSTOMER LOGIN");
    printf("  " BCYAN "Customer ID : " RESET);
    if (!readInt(&id)) {
        printf("\n");
        ERR("Invalid ID.");
        system("pause");
        return -1;
    }
    printf("  " BCYAN "Password    : " RESET);
    inputPassword(pass);

    for (i = 0; i < nCustomers; i++) {
        if (customers[i].cId == id && strcmp(customers[i].cPass, pass) == 0) {
            printf("\n");
            printf("  " BGREEN "[OK]" RESET " Welcome, " BGREEN BOLD "%s" RESET "!\n",
                   customers[i].cName);
            system("pause");
            custLoggedIn   = 1;
            currentCustIdx = i;
            return i;
        }
    }
    printf("\n");
    ERR("Invalid Customer ID or Password.");
    system("pause");
    return -1;
}

/* ------------------ CUSTOMER MENU ------------------ */
void cust() {
    int choice, i;

    if (!custLoggedIn) {
        if (customerLogin() == -1) return;
    }

    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("CUSTOMER SELF-CHECKOUT");
        printf("  Welcome, " BGREEN BOLD "%s" RESET "\n", customers[currentCustIdx].cName);
        printDash();
        printf("  " BCYAN   "1." RESET " View Store Products\n");
        printf("  " BCYAN   "2." RESET " Search for a Product\n");
        printf("  " BGREEN  "3." RESET " Add Item to Basket\n");
        printf("  " BYELLOW "4." RESET " View My Basket\n");
        printf("  " BMAGENTA "5." RESET " Final Checkout\n");
        printf("  " BBLUE   "6." RESET " My Membership Card\n");
        printf("  " BRED    "0." RESET " Logout & Return to Main Menu\n");
        printDash();
        printf("  " BOLD "Enter your choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1: custView(); break;
            case 2: search();   break;
            case 3:
                system("cls");
                printHeader("ADD ITEM TO BASKET");
                if (nProducts == 0) {
                    WARN("No products available.");
                } else {
                    printf("  " BOLD BYELLOW "%-6s  %-18s  %-8s  %-6s  %s" RESET "\n",
                           "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
                    printDash();
                    for (i = 0; i < nProducts; i++) {
                        printf("  " BCYAN "%-6d" RESET "  %-18s  " BGREEN "%-8.2f" RESET "  %-6d  %s\n",
                               shop[i].pCode, shop[i].pName, shop[i].pPrice,
                               shop[i].pStock, shop[i].pCategory);
                    }
                }
                addCart();
                break;
            case 4: viewCart(); break;
            case 5: checkout(); return;
            case 6: viewMyCard(); break;
            case 0:
                custLoggedIn   = 0;
                currentCustIdx = -1;
                return;
            default:
                WARN("Invalid option.");
                system("pause");
        }
    }
}

/* -- Customer view -- */
void custView() {
    int i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("AVAILABLE PRODUCTS");
    if (nProducts == 0) {
        WARN("No products available.");
    } else {
        printf("  " BOLD BYELLOW "%-6s  %-18s  %-8s  %-6s  %s" RESET "\n",
               "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
        printDash();
        for (i = 0; i < nProducts; i++) {
            printf("  " BCYAN "%-6d" RESET "  %-18s  " BGREEN "%-8.2f" RESET "  %-6d  %s\n",
                   shop[i].pCode, shop[i].pName, shop[i].pPrice,
                   shop[i].pStock, shop[i].pCategory);
        }
    }
    printf("\n");
    system("pause");
}

/* -- Search -- */
void search() {
    int code, found = 0, i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  Enter Product Code to Search: ");
    if (!readInt(&code)) {
        WARN("Invalid code.");
        system("pause");
        return;
    }

    for (i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            printDash();
            printf("  " BGREEN "Found:\n" RESET);
            printf("    " BCYAN "Product Name     : " RESET "%s\n",   shop[i].pName);
            printf("    " BCYAN "Product Price    : " RESET BGREEN "%.2f\n" RESET, shop[i].pPrice);
            printf("    " BCYAN "Product Stock    : " RESET "%d\n",   shop[i].pStock);
            printf("    " BCYAN "Product Category : " RESET "%s\n",   shop[i].pCategory);
            printDash();
            found = 1;
            break;
        }
    }
    if (!found) ERR("Product not found.");
    printf("\n");
    system("pause");
}

/* -- Add to cart -- */
void addCart() {
    int  code, qty, found = 0, i;
    char confirm[4];

    if (cartCount >= MAX_CART) {
        WARN("Cart is full!");
        system("pause");
        return;
    }

    printf("\n  Enter Product Code : ");
    if (!readInt(&code)) {
        WARN("Invalid code.");
        system("pause");
        return;
    }
    printf("  Enter Quantity     : ");
    if (!readInt(&qty) || qty <= 0) {
        WARN("Invalid quantity.");
        system("pause");
        return;
    }

    for (i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            if (shop[i].pStock < qty) {
                printf("  " BRED "[!]" RESET " Insufficient stock (available: " BYELLOW "%d" RESET ").\n",
                       shop[i].pStock);
                found = 1;
                break;
            }

            printf("\n  " BOLD BCYAN "+- Confirm Item ----------------------------+" RESET "\n");
            printf("  " BCYAN "|" RESET "  Name  : " BWHITE "%-31s" RESET BCYAN "|" RESET "\n", shop[i].pName);
            printf("  " BCYAN "|" RESET "  Price : " BGREEN "%-5.2f" RESET " per unit                 " BCYAN "|" RESET "\n", shop[i].pPrice);
            printf("  " BCYAN "|" RESET "  Qty   : %-31d" BCYAN "|" RESET "\n", qty);
            printf("  " BCYAN "|" RESET "  Total : " BYELLOW "%-5.2f" RESET "                           " BCYAN "|" RESET "\n", shop[i].pPrice * qty);
            printf("  " BCYAN "+-------------------------------------------+" RESET "\n");
            printf("  Add to cart? (yes/no): ");
            scanf(" %3s", confirm);

            if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) {
                cart[cartCount].code  = shop[i].pCode;
                strcpy(cart[cartCount].name, shop[i].pName);
                cart[cartCount].price = shop[i].pPrice;
                cart[cartCount].qty   = qty;
                shop[i].pStock       -= qty;
                saveProducts();
                cartCount++;
                OK("Item added to basket.");
            } else {
                printf("  " BYELLOW "[-]" RESET " Item not added.\n");
            }
            found = 1;
            break;
        }
    }
    if (!found) ERR("Product unavailable.");
    printf("\n");
    system("pause");
}

/* -- View cart -- */
void viewCart() {
    float total = 0;
    int   i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("MY BASKET");
    if (cartCount == 0) {
        WARN("Basket is empty.");
    } else {
        printf("  " BOLD BYELLOW "%-6s  %-18s  %-8s  %-5s  %s" RESET "\n",
               "CODE", "NAME", "PRICE", "QTY", "AMOUNT");
        printDash();
        for (i = 0; i < cartCount; i++) {
            float amt = cart[i].price * cart[i].qty;
            total    += amt;
            printf("  " BCYAN "%-6d" RESET "  %-18s  " BGREEN "%-8.2f" RESET "  %-5d  " BGREEN "%.2f" RESET "\n",
                   cart[i].code, cart[i].name, cart[i].price, cart[i].qty, amt);
        }
        printDash();
        printf("  " BOLD "%-40s  " BYELLOW "%.2f" RESET "\n", "Grand Total:", total);
    }
    printf("\n");
    system("pause");
}

/* ------------------ PRINT QR CODE ------------------ */
void printQRCode() {
    printf(BWHITE);
    printf("  +----------------------------------------------------------+\n");
    printf("  |                                                          |\n");
    printf("  |  ##############  ##  ##  ####  ####  ##############     |\n");
    printf("  |  ##          ##    ####    ##  ####  ##          ##     |\n");
    printf("  |  ##  ######  ##  ##  ########  ####  ##  ######  ##     |\n");
    printf("  |  ##  ######  ##  ####  ##  ##    ##  ##  ######  ##     |\n");
    printf("  |  ##  ######  ##  ##  ##  ####  ####  ##  ######  ##     |\n");
    printf("  |  ##          ##  ####  ######  ####  ##          ##     |\n");
    printf("  |  ##############  ##  ##  ##  ##  ##  ##############     |\n");
    printf("  |                  ##  ####  ##  ####                     |\n");
    printf("  |  ####  ########  ####  ######  ####  ####  ######       |\n");
    printf("  |  ##  ####    ##    ##  ##  ##  ##  ######      ##       |\n");
    printf("  |  ######  ##  ####  ##  ##  ##    ####  ##  ##  ##       |\n");
    printf("  |  ##  ####  ######  ######  ##  ######  ######  ##       |\n");
    printf("  |  ##    ##  ##  ######  ##  ##  ##  ##  ####  ####       |\n");
    printf("  |  ######  ########    ####  ##  ####  ######  ##         |\n");
    printf("  |  ##  ##  ##  ####  ######  ##  ####  ####  ######       |\n");
    printf("  |  ####  ######  ##  ##  ##  ##  ##  ########  ##  ##     |\n");
    printf("  |  ##    ##  ########  ####  ######  ##    ##  ####       |\n");
    printf("  |  ######  ######  ##  ##  ######  ####  ##  ########     |\n");
    printf("  |  ##  ##  ##    ####  ####  ##  ##  ######  ##  ##       |\n");
    printf("  |  ####  ######  ##  ######  ####  ####  ##  ####  ##     |\n");
    printf("  |                  ######  ####  ##  ##  ##  ##           |\n");
    printf("  |  ##############  ####  ##  ######  ##  ##  ######       |\n");
    printf("  |  ##          ##  ##  ######  ####    ######  ##  ##     |\n");
    printf("  |  ##  ######  ##  ####  ##  ######  ################     |\n");
    printf("  |  ##  ######  ##  ##  ######  ##  ########  ######       |\n");
    printf("  |  ##  ######  ##  ######  ##  ##  ##  ##  ##  ####       |\n");
    printf("  |  ##          ##  ####  ##  ######  ####  ##    ##       |\n");
    printf("  |  ##############  ##  ######  ##  ########  ##  ##       |\n");
    printf("  |                                                          |\n");
    printf("  +----------------------------------------------------------+\n");
    printf(RESET);
}

/* -- Checkout -- */
void checkout() {
    int   mid, i, payChoice;
    float subtotal = 0, discount = 0, finalAmount;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    if (cartCount == 0) {
        WARN("Basket is empty.");
        system("pause");
        return;
    }

    for (i = 0; i < cartCount; i++) subtotal += cart[i].price * cart[i].qty;

    printf("\n  Enter Member ID (0 if none): ");
    if (!readInt(&mid)) mid = 0;

    discount    = applyDiscount(mid, subtotal);
    finalAmount = subtotal - discount;

    printDash();
    printf("  " BCYAN "Subtotal        : " RESET "%.2f\n", subtotal);
    printf("  " BGREEN "Discount        : " RESET BGREEN "%.2f\n" RESET, discount);
    printf("  " BOLD BYELLOW "Payable Amount  : %.2f\n" RESET, finalAmount);
    printDash();

    printf("\n  " BOLD "SELECT PAYMENT METHOD:\n" RESET);
    printf("  " BWHITE "1." RESET " Cash\n");
    printf("  " BCYAN  "2." RESET " QR Code (Digital)\n");
    printf("  Enter choice: ");
    if (!readInt(&payChoice)) payChoice = 1;

    for (i = 0; i < cartCount; i++) {
        recordSale(cart[i].code, cart[i].name, cart[i].qty, cart[i].price,
                   customers[currentCustIdx].cId);
    }

    totalBills++;
    cartCount      = 0;
    custLoggedIn   = 0;
    currentCustIdx = -1;
    saveAllData();

    if (payChoice == 2) {
        printf("\n  " BCYAN "[ SCAN QR CODE TO PAY - Dhameliya Yash Mukeshbhai ]\n\n" RESET);
        printQRCode();
        printf("\n  " BCYAN "UPI ID : " RESET "6353999107@ptyes\n");
        printf("  " BYELLOW "Amount : Rs. %.2f\n\n" RESET, finalAmount);
        totalQRPayments++;
        saveCounters();
        billing(finalAmount, 1);
    } else {
        totalCashPayments++;
        saveCounters();
        billing(finalAmount, 0);
    }
}

/* -- Show customer purchase history -- */
void showCustomerHistory(int custId) {
    int i, found = 0;
    float totalSpent = 0;

    printf("\n  " BOLD BYELLOW "===== CUSTOMER PURCHASE HISTORY =====" RESET "\n");
    printf("  " BOLD "%-6s  %-18s  %-5s  %s\n" RESET, "CODE", "NAME", "QTY", "TOTAL");
    printDash();

    for (i = 0; i < nSales; i++) {
        if (sales[i].custId == custId) {
            printf("  " BCYAN "%-6d" RESET "  %-18s  %-5d  " BGREEN "%.2f\n" RESET,
                   sales[i].pCode, sales[i].pName, sales[i].qty, sales[i].total);
            totalSpent += sales[i].total;
            found = 1;
        }
    }

    if (!found) {
        WARN("No purchase history found.");
    } else {
        printDash();
        printf("  " BOLD "Total Spent: " BYELLOW "%.2f\n" RESET, totalSpent);
    }
    printDash();
}

/* -- Billing -- */
void billing(float finalAmount, int isQR) {
    if (isQR) {
        printLine();
        printf("  " BCYAN "Transaction ID: " RESET BWHITE "AYH%d\n" RESET, TXID_BASE + totalBills);
        printf("  " BGREEN BOLD "Thank you for shopping at AYH Mart!\n" RESET);
        printLine();
        system("pause");
        return;
    }

    float payAmount;
    while (1) {
        printf("  Pay by Cash (enter amount): ");
        scanf("%f", &payAmount);

        if (payAmount >= finalAmount) {
            float change = payAmount - finalAmount;
            printf("\n");
            printLine();
            printf("  " BGREEN "[OK]" RESET " Payment received! Change: " BYELLOW "%.2f\n" RESET, change);
            printf("  " BGREEN BOLD "Thank you for shopping at AYH Mart!\n" RESET);
            printLine();
            system("pause");
            return;
        } else {
            printf("  " BRED "[!]" RESET " Insufficient. Need " BYELLOW "%.2f" RESET " more.\n",
                   finalAmount - payAmount);
        }
    }
}

/* ------------------ MEMBERS ------------------ */
void members() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("MEMBER PORTAL");
        printf("  " BGREEN "1." RESET " Add New Member\n");
        printf("  " BRED   "2." RESET " Delete Member\n");
        printf("  " BCYAN  "3." RESET " View All Members\n");
        printf("  " BWHITE "0." RESET " Back\n");
        printDash();
        printf("  " BOLD "Enter your choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1: addMembers();    break;
            case 2: deleteMembers(); break;
            case 3: viewMembers();   break;
            case 0: return;
            default:
                WARN("Invalid option.");
                system("pause");
        }
    }
}

/* -- Add members -- */
void addMembers() {
    int count, i, j;
    system("cls");
    printHeader("AYH MART  |  Surat, India");

    printf("\n  " BOLD BYELLOW "===== EXISTING CUSTOMER LIST =====" RESET "\n");

    if (nCustomers == 0) {
        WARN("No customers available. Please create customer first.");
        system("pause");
        return;
    }

    printf("  " BOLD "%-6s  %-18s  %s\n" RESET, "ID", "NAME", "MEMBER ID");
    printDash();

    for (i = 0; i < nCustomers; i++) {
        if (customers[i].cMemberId == 0)
            printf("  " BCYAN "%-6d" RESET "  %-18s  " BYELLOW "None\n" RESET,
                   customers[i].cId, customers[i].cName);
        else
            printf("  " BCYAN "%-6d" RESET "  %-18s  " BGREEN "%d\n" RESET,
                   customers[i].cId, customers[i].cName, customers[i].cMemberId);
    }
    printDash();

    printf("\n  How many members to add: ");
    if (!readInt(&count) || count <= 0) {
        WARN("Invalid count.");
        system("pause");
        return;
    }

    int added = 0;
    while (added < count) {
        if (nMembers >= MAX_MEMBERS) {
            WARN("Member limit reached.");
            break;
        }

        int custId, custIndex = -1;
        char tier[20];

        printf("\n  " BOLD BCYAN "-- Member %d --" RESET "\n", added + 1);
        printf("  Enter Customer ID to assign membership: ");
        if (!readInt(&custId)) {
            WARN("Invalid ID. Try again.");
            continue;
        }

        for (j = 0; j < nCustomers; j++) {
            if (customers[j].cId == custId) { custIndex = j; break; }
        }

        if (custIndex == -1) {
            ERR("Customer not found.");
            continue;
        }

        showCustomerHistory(customers[custIndex].cId);

        printf("\n  Continue to assign membership? (yes/no): ");
        char confirm[5];
        scanf("%4s", confirm);

        if (strcmp(confirm, "yes") != 0 && strcmp(confirm, "y") != 0) {
            printf("  " BYELLOW "Skipped.\n" RESET);
            continue;
        }

        if (customers[custIndex].cMemberId != 0) {
            WARN("This customer already has a membership.");
            continue;
        }

        Members[nMembers].mId = MEMBER_ID_BASE + nMembers;
        strcpy(Members[nMembers].mName, customers[custIndex].cName);

        printf("  Assigned to: " BGREEN "%s\n" RESET, customers[custIndex].cName);
        printf("  Generated Member ID: " BCYAN "%d\n" RESET, Members[nMembers].mId);

        printf("  Amount Spent: ");
        scanf("%f", &Members[nMembers].mSpent);

        while (1) {
            printf("  Tier (" BYELLOW "Silver/s" RESET " | " BMAGENTA "Gold/g" RESET " | " BCYAN "Platinum/p" RESET "): ");
            scanf("%19s", tier);
            char c = tolower((unsigned char)tier[0]);
            if (c == 's') {
                strcpy(Members[nMembers].mTier, "Silver");
                Members[nMembers].mDisc = 5.0f;
                break;
            } else if (c == 'g') {
                strcpy(Members[nMembers].mTier, "Gold");
                Members[nMembers].mDisc = 10.0f;
                break;
            } else if (c == 'p') {
                strcpy(Members[nMembers].mTier, "Platinum");
                Members[nMembers].mDisc = 15.0f;
                break;
            } else {
                WARN("Invalid tier. Try again.");
            }
        }

        Members[nMembers].mUsed = 0;
        customers[custIndex].cMemberId = Members[nMembers].mId;
        nMembers++;
        added++;
    }

    printf("\n");
    OK("Member(s) added and linked successfully.");
    saveMembers();
    saveCustomers();
    system("pause");
}

/* -- Delete member -- */
void deleteMembers() {
    int i, id, found = 0, j;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  Enter Member ID to Delete: ");
    if (!readInt(&id)) {
        WARN("Invalid ID.");
        system("pause");
        return;
    }

    for (i = 0; i < nMembers; i++) {
        if (Members[i].mId == id) {
            for (j = i; j < nMembers - 1; j++) Members[j] = Members[j + 1];
            nMembers--;
            found = 1;
            OK("Member deleted.");
            saveMembers();
            break;
        }
    }
    if (!found) ERR("Member ID not found.");
    system("pause");
}

/* -- View members -- */
void viewMembers() {
    int i;
    system("cls");
    printHeader("ALL MEMBERS");
    if (nMembers == 0) {
        WARN("No members registered.");
    } else {
        printf("  " BOLD BYELLOW "%-5s  %-18s  %-10s  %-10s  %-9s  %s\n" RESET,
               "ID", "NAME", "SPENT", "TIER", "DISCOUNT", "CARD STATUS");
        printDash();
        for (i = 0; i < nMembers; i++) {
            /* Color-code the tier */
            const char *tierColor =
                (Members[i].mTier[0] == 'P') ? BCYAN :
                (Members[i].mTier[0] == 'G') ? BYELLOW : WHITE;
            const char *statusColor = Members[i].mUsed ? BRED : BGREEN;

            printf("  " BCYAN "%-5d" RESET "  %-18s  %-10.2f  %s%-10s" RESET "  %-9.0f%%  %s%s" RESET "\n",
                   Members[i].mId, Members[i].mName, Members[i].mSpent,
                   tierColor, Members[i].mTier,
                   Members[i].mDisc,
                   statusColor, Members[i].mUsed ? "Used" : "Active");
        }
    }
    printf("\n");
    system("pause");
}

/* ------------------ CUSTOMER ACCOUNT MANAGEMENT ------------------ */
void custManage() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("CUSTOMER ACCOUNT MANAGEMENT");
        printf("  " BGREEN "1." RESET " Add New Customer Account\n");
        printf("  " BRED   "2." RESET " Delete Customer Account\n");
        printf("  " BCYAN  "3." RESET " View All Customer Accounts\n");
        printf("  " BWHITE "0." RESET " Back\n");
        printDash();
        printf("  " BOLD "Enter your choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1: addCustomer();    break;
            case 2: deleteCustomer(); break;
            case 3: viewCustomers();  break;
            case 0: return;
            default:
                WARN("Invalid option.");
                system("pause");
        }
    }
}

/* -- Add customer accounts -- */
void addCustomer() {
    int  count, i;
    char passConfirm[20];
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  How many customer accounts to add: ");
    if (!readInt(&count) || count <= 0) {
        WARN("Invalid count.");
        system("pause");
        return;
    }

    for (i = 0; i < count; i++) {
        if (nCustomers >= MAX_CUSTOMERS) {
            WARN("Customer limit reached (100 max).");
            break;
        }
        printf("\n  " BOLD BCYAN "-- Customer Account %d --" RESET "\n", i + 1);

        int id;
        while (1) {
            printf("  Customer ID  : ");
            if (!readInt(&id)) {
                WARN("Invalid ID. Try again.");
                continue;
            }
            if (isCustomerIdExists(id)) {
                WARN("Customer ID already exists. Try again.");
            } else {
                customers[nCustomers].cId = id;
                break;
            }
        }

        printf("  Name         : "); scanf("%49s", customers[nCustomers].cName);
        printf("  Phone Number : "); scanf("%14s", customers[nCustomers].cPhone);
        printf("  Email Address: "); scanf("%49s", customers[nCustomers].cEmail);

        while (1) {
            printf("  Password     : ");
            inputPassword(customers[nCustomers].cPass);
            printf("\n  Confirm Pwd  : ");
            inputPassword(passConfirm);
            if (strcmp(customers[nCustomers].cPass, passConfirm) == 0) {
                break;
            } else {
                WARN("Passwords do not match. Try again.");
            }
        }
        printf("\n");

        printf("  Member ID    : (enter 0 if no membership) ");
        if (!readInt(&customers[nCustomers].cMemberId))
            customers[nCustomers].cMemberId = 0;
        nCustomers++;
    }
    printf("\n");
    OK("Customer account(s) created successfully.");
    saveCustomers();
    system("pause");
}

/* -- Delete customer account -- */
void deleteCustomer() {
    int id, i, j, found = 0;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  Enter Customer ID to Delete: ");
    if (!readInt(&id)) {
        WARN("Invalid ID.");
        system("pause");
        return;
    }

    for (i = 0; i < nCustomers; i++) {
        if (customers[i].cId == id) {
            for (j = i; j < nCustomers - 1; j++) customers[j] = customers[j + 1];
            nCustomers--;
            found = 1;
            OK("Customer account deleted.");
            saveCustomers();
            break;
        }
    }
    if (!found) ERR("Customer ID not found.");
    system("pause");
}

/* -- View all customer accounts -- */
void viewCustomers() {
    int i;
    system("cls");
    printHeader("ALL CUSTOMER ACCOUNTS");
    if (nCustomers == 0) {
        WARN("No customer accounts registered.");
    } else {
        printf("  " BOLD BYELLOW "%-6s  %-15s  %-12s  %-15s  %-25s  %s\n" RESET,
               "ID", "NAME", "PASSWORD", "PHONE", "EMAIL", "MEMBER ID");
        printDash();
        for (i = 0; i < nCustomers; i++) {
            char masked[20];
            int  k, plen = (int)strlen(customers[i].cPass);
            for (k = 0; k < plen && k < 12; k++) masked[k] = '*';
            masked[plen < 12 ? plen : 12] = '\0';

            if (customers[i].cMemberId != 0) {
                printf("  " BCYAN "%-6d" RESET "  %-15s  " BYELLOW "%-12s" RESET "  %-15s  %-25s  " BGREEN "%d\n" RESET,
                       customers[i].cId, customers[i].cName, masked,
                       customers[i].cPhone, customers[i].cEmail, customers[i].cMemberId);
            } else {
                printf("  " BCYAN "%-6d" RESET "  %-15s  " BYELLOW "%-12s" RESET "  %-15s  %-25s  None\n",
                       customers[i].cId, customers[i].cName, masked,
                       customers[i].cPhone, customers[i].cEmail);
            }
        }
    }
    printf("\n");
    system("pause");
}

/* ------------------ VIEW MY MEMBERSHIP CARD ------------------ */
void viewMyCard() {
    int i, found = 0;
    int linkedId = customers[currentCustIdx].cMemberId;

    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("MY MEMBERSHIP CARD");

    printf("  " BCYAN "Account Holder : " RESET BWHITE BOLD "%s\n" RESET,
           customers[currentCustIdx].cName);
    printf("  " BCYAN "Customer ID    : " RESET "%d\n", customers[currentCustIdx].cId);
    printDash();

    if (linkedId == 0) {
        WARN("No membership card linked to your account.");
        printf("      Please contact Staff or Admin to get one assigned.\n");
    } else {
        for (i = 0; i < nMembers; i++) {
            if (Members[i].mId == linkedId) {
                found = 1;
                const char *tierColor =
                    (Members[i].mTier[0] == 'P') ? BCYAN :
                    (Members[i].mTier[0] == 'G') ? BYELLOW : WHITE;

                printf("  " BCYAN "Member ID      : " RESET "%d\n",       Members[i].mId);
                printf("  " BCYAN "Member Name    : " RESET "%s\n",       Members[i].mName);
                printf("  " BCYAN "Tier           : " RESET "%s%s" RESET "\n", tierColor, Members[i].mTier);
                printf("  " BCYAN "Discount Rate  : " RESET BGREEN "%.0f%%\n" RESET, Members[i].mDisc);
                printf("  " BCYAN "Total Spent    : " RESET "Rs. %.2f\n", Members[i].mSpent);
                printf("  " BCYAN "Card Status    : " RESET "%s\n",
                       Members[i].mUsed
                           ? BRED "Used (discount already redeemed)" RESET
                           : BGREEN "Active (discount available)" RESET);
                break;
            }
        }
        if (!found) {
            printf("  " BRED "[!]" RESET " Linked Member ID (" BYELLOW "%d" RESET ") not found in records.\n", linkedId);
            printf("      Please contact Staff or Admin.\n");
        }
    }
    printDash();
    system("pause");
}

/* ------------------ SALES ------------------ */
void recordSale(int code, char name[], int qty, float price, int custId) {
    if (nSales >= MAX_SALES) return;
    sales[nSales].pCode  = code;
    sales[nSales].custId = custId;
    strcpy(sales[nSales].pName, name);
    sales[nSales].qty   = qty;
    sales[nSales].price = price;
    sales[nSales].total = qty * price;
    totalRevenue += sales[nSales].total;
    nSales++;
}

float applyDiscount(int memberId, float billAmount) {
    int i;
    if (memberId == 0) return 0;

    for (i = 0; i < nMembers; i++) {
        if (Members[i].mId == memberId) {
            if (Members[i].mUsed == 1) {
                WARN("Membership card already used. No discount applied.");
                return 0;
            }
            float discount = billAmount * (Members[i].mDisc / 100.0f);
            Members[i].mSpent += (billAmount - discount);
            totalDiscount     += discount;
            Members[i].mUsed   = 1;
            printf("  " BGREEN "[OK]" RESET " Membership discount applied ("
                   BGREEN "%.0f%%" RESET "). Card is now marked as " BRED "used" RESET ".\n",
                   Members[i].mDisc);
            return discount;
        }
    }
    WARN("Member ID not found. No discount applied.");
    return 0;
}

/* ------------------ SALES MENU ------------------ */
void salesManu() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("SALES REPORT");
        printf("  " BCYAN   "1." RESET " Summary Report\n");
        printf("  " BYELLOW "2." RESET " Product-wise Sales\n");
        printf("  " BMAGENTA "3." RESET " Member Discount Report\n");
        printf("  " BWHITE  "0." RESET " Back\n");
        printDash();
        printf("  " BOLD "Enter choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1: salesSummary();    break;
            case 2: productSales();    break;
            case 3: discountReport();  break;
            case 0: return;
            default:
                WARN("Invalid option.");
                system("pause");
        }
    }
}

void salesSummary() {
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("SALES SUMMARY");
    printf("  " BCYAN "Total Bills Generated : " RESET BWHITE "%d\n" RESET,  totalBills);
    printf("  " BCYAN "-- Cash Payments      : " RESET "%d\n",               totalCashPayments);
    printf("  " BCYAN "-- QR Payments        : " RESET "%d\n",               totalQRPayments);
    printf("  " BCYAN "Total Items Sold      : " RESET "%d\n",               nSales);
    printf("  " BCYAN "Total Revenue         : " RESET BGREEN "%.2f\n" RESET, totalRevenue);
    printf("  " BCYAN "Total Discount Given  : " RESET BYELLOW "%.2f\n" RESET, totalDiscount);
    printf("  " BOLD BCYAN "Net Revenue           : " RESET BOLD BGREEN "%.2f\n" RESET,
           totalRevenue - totalDiscount);
    printDash();
    system("pause");
}

void staffSalesSummary() {
    int i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("SALES SUMMARY");
    printf("  " BCYAN "Total Items Sold  : " RESET "%d\n",               nSales);
    printf("  " BCYAN "Total Revenue     : " RESET BGREEN "Rs. %.2f\n" RESET, totalRevenue);
    printDash();

    printf("\n  " BOLD BYELLOW "ITEMS SOLD:\n" RESET);
    if (nSales == 0) {
        WARN("No sales recorded yet.");
    } else {
        printf("\n  " BOLD "%-6s  %-18s  %-6s  %s\n" RESET, "CODE", "NAME", "QTY", "TOTAL");
        printDash();
        for (i = 0; i < nSales; i++)
            printf("  " BCYAN "%-6d" RESET "  %-18s  %-6d  " BGREEN "Rs. %.2f\n" RESET,
                   sales[i].pCode, sales[i].pName, sales[i].qty, sales[i].total);
        printDash();
    }
    printf("\n");
    system("pause");
}

void productSales() {
    int i;
    system("cls");
    printHeader("PRODUCT SALES REPORT");
    if (nSales == 0) {
        WARN("No sales recorded yet.");
    } else {
        printf("  " BOLD BYELLOW "%-6s  %-18s  %-6s  %s\n" RESET, "CODE", "NAME", "QTY", "TOTAL");
        printDash();
        for (i = 0; i < nSales; i++)
            printf("  " BCYAN "%-6d" RESET "  %-18s  %-6d  " BGREEN "%.2f\n" RESET,
                   sales[i].pCode, sales[i].pName, sales[i].qty, sales[i].total);
    }
    printf("\n");
    system("pause");
}

void discountReport() {
    int i;
    system("cls");
    printHeader("MEMBER DISCOUNT REPORT");
    if (nMembers == 0) {
        WARN("No members recorded.");
    } else {
        printf("  " BOLD BYELLOW "%-5s  %-18s  %-10s  %-10s  %-9s  %s\n" RESET,
               "ID", "NAME", "SPENT", "TIER", "DISCOUNT", "CARD STATUS");
        printDash();
        for (i = 0; i < nMembers; i++) {
            const char *tierColor =
                (Members[i].mTier[0] == 'P') ? BCYAN :
                (Members[i].mTier[0] == 'G') ? BYELLOW : WHITE;
            const char *statusColor = Members[i].mUsed ? BRED : BGREEN;
            printf("  " BCYAN "%-5d" RESET "  %-18s  %-10.2f  %s%-10s" RESET "  %-9.0f%%  %s%s" RESET "\n",
                   Members[i].mId, Members[i].mName, Members[i].mSpent,
                   tierColor, Members[i].mTier, Members[i].mDisc,
                   statusColor, Members[i].mUsed ? "Used" : "Active");
        }
    }
    printf("\n");
    system("pause");
}

/* ------------------ STAFF PORTAL ------------------ */
void staff() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("STAFF PORTAL");
        printf("  " BCYAN    "1." RESET " View Inventory\n");
        printf("  " BBLUE    "2." RESET " View All Members\n");
        printf("  " BGREEN   "3." RESET " Add New Member\n");
        printf("  " BRED     "4." RESET " Remove Member\n");
        printf("  " BYELLOW  "5." RESET " View Sales Summary\n");
        printf("  " BMAGENTA "6." RESET " Search Product\n");
        printf("  " BWHITE   "7." RESET " Customer Accounts\n");
        printf("  " BRED     "0." RESET " Logout\n");
        printDash();
        printf("  " BOLD "Enter your choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1: {
                int i;
                system("cls");
                printHeader("INVENTORY (Staff View)");
                if (nProducts == 0) {
                    WARN("No products.");
                } else {
                    printf("  " BOLD BYELLOW "%-6s  %-18s  %-8s  %-6s  %s\n" RESET,
                           "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
                    printDash();
                    for (i = 0; i < nProducts; i++)
                        printf("  " BCYAN "%-6d" RESET "  %-18s  " BGREEN "%-8.2f" RESET "  %-6d  %s\n",
                               shop[i].pCode, shop[i].pName, shop[i].pPrice,
                               shop[i].pStock, shop[i].pCategory);
                }
                printf("\n"); system("pause"); break;
            }
            case 2: {
                int i;
                system("cls");
                printHeader("ALL MEMBERS (Staff View)");
                if (nMembers == 0) {
                    WARN("No members registered.");
                } else {
                    printf("  " BOLD BYELLOW "%-5s  %-18s  %-10s  %-10s  %-9s  %s\n" RESET,
                           "ID", "NAME", "SPENT", "TIER", "DISCOUNT", "CARD STATUS");
                    printDash();
                    for (i = 0; i < nMembers; i++) {
                        const char *sc = Members[i].mUsed ? BRED : BGREEN;
                        printf("  " BCYAN "%-5d" RESET "  %-18s  %-10.2f  %-10s  %-9.0f%%  %s%s" RESET "\n",
                               Members[i].mId, Members[i].mName,
                               Members[i].mSpent, Members[i].mTier,
                               Members[i].mDisc,
                               sc, Members[i].mUsed ? "Used" : "Active");
                    }
                }
                printf("\n"); system("pause"); break;
            }
            case 3: addMembers();    break;
            case 4: deleteMembers(); break;
            case 5: staffSalesSummary(); break;
            case 6: {
                int code, found = 0, i;
                system("cls");
                printHeader("AYH MART  |  Surat, India");
                printf("\n  Enter Product Code to Search: ");
                if (!readInt(&code)) {
                    WARN("Invalid code.");
                    system("pause");
                    break;
                }
                for (i = 0; i < nProducts; i++) {
                    if (shop[i].pCode == code) {
                        printDash();
                        printf("  " BGREEN "Found:\n" RESET);
                        printf("    " BCYAN "Product Name     : " RESET "%s\n",   shop[i].pName);
                        printf("    " BCYAN "Product Price    : " RESET BGREEN "%.2f\n" RESET, shop[i].pPrice);
                        printf("    " BCYAN "Product Stock    : " RESET "%d\n",   shop[i].pStock);
                        printf("    " BCYAN "Product Category : " RESET "%s\n",   shop[i].pCategory);
                        printDash();
                        found = 1;
                        break;
                    }
                }
                if (!found) ERR("Product not found.");
                printf("\n"); system("pause"); break;
            }
            case 7: {
                int staffChoice;
                while (1) {
                    system("cls");
                    printHeader("AYH MART  |  Surat, India");
                    printHeader("CUSTOMER ACCOUNT MANAGEMENT");
                    printf("  " BGREEN "1." RESET " Add New Customer Account\n");
                    printf("  " BRED   "2." RESET " Delete Customer Account\n");
                    printf("  " BCYAN  "3." RESET " View All Customer Accounts\n");
                    printf("  " BWHITE "0." RESET " Back\n");
                    printDash();
                    printf("  " BOLD "Enter your choice: " RESET);

                    if (!readInt(&staffChoice)) {
                        WARN("Invalid input.");
                        system("pause");
                        continue;
                    }

                    switch (staffChoice) {
                        case 1: addCustomer();    break;
                        case 2: deleteCustomer(); break;
                        case 3: viewCustomers();  break;
                        case 0: goto staffCustomerDone;
                        default:
                            WARN("Invalid option.");
                            system("pause");
                    }
                }
                staffCustomerDone:;
                break;
            }
            case 0:
                OK("Logged out.");
                system("pause");
                return;
            default:
                WARN("Invalid option.");
                system("pause");
        }
    }
}

/* ------------------ FILE HANDLING ------------------ */
void saveProducts() {
    FILE *fp = fopen(FILE_PRODUCTS, "wb");
    if (!fp) { ERR("Could not save products."); return; }
    fwrite(&nProducts, sizeof(int), 1, fp);
    fwrite(shop, sizeof(struct Product), nProducts, fp);
    fclose(fp);
}
void loadProducts() {
    FILE *fp = fopen(FILE_PRODUCTS, "rb");
    if (!fp) return;
    fread(&nProducts, sizeof(int), 1, fp);
    if (nProducts > MAX_ITEMS) nProducts = MAX_ITEMS;
    fread(shop, sizeof(struct Product), nProducts, fp);
    fclose(fp);
}
void saveMembers() {
    FILE *fp = fopen(FILE_MEMBERS, "wb");
    if (!fp) { ERR("Could not save members."); return; }
    fwrite(&nMembers, sizeof(int), 1, fp);
    fwrite(Members, sizeof(struct Member), nMembers, fp);
    fclose(fp);
}
void loadMembers() {
    FILE *fp = fopen(FILE_MEMBERS, "rb");
    if (!fp) return;
    fread(&nMembers, sizeof(int), 1, fp);
    if (nMembers > MAX_MEMBERS) nMembers = MAX_MEMBERS;
    fread(Members, sizeof(struct Member), nMembers, fp);
    fclose(fp);
}
void saveCustomers() {
    FILE *fp = fopen(FILE_CUSTOMERS, "wb");
    if (!fp) { ERR("Could not save customers."); return; }
    fwrite(&nCustomers, sizeof(int), 1, fp);
    fwrite(customers, sizeof(struct Customer), nCustomers, fp);
    fclose(fp);
}
void loadCustomers() {
    FILE *fp = fopen(FILE_CUSTOMERS, "rb");
    if (!fp) return;
    fread(&nCustomers, sizeof(int), 1, fp);
    if (nCustomers > MAX_CUSTOMERS) nCustomers = MAX_CUSTOMERS;
    fread(customers, sizeof(struct Customer), nCustomers, fp);
    fclose(fp);
}
void saveSales() {
    FILE *fp = fopen(FILE_SALES, "wb");
    if (!fp) { ERR("Could not save sales."); return; }
    fwrite(&nSales, sizeof(int), 1, fp);
    fwrite(sales, sizeof(struct Sale), nSales, fp);
    fclose(fp);
}
void loadSales() {
    FILE *fp = fopen(FILE_SALES, "rb");
    if (!fp) return;
    fread(&nSales, sizeof(int), 1, fp);
    if (nSales > MAX_SALES) nSales = MAX_SALES;
    fread(sales, sizeof(struct Sale), nSales, fp);
    fclose(fp);
}
void saveCounters() {
    FILE *fp = fopen(FILE_COUNTERS, "wb");
    if (!fp) return;
    fwrite(&totalRevenue,      sizeof(float), 1, fp);
    fwrite(&totalDiscount,     sizeof(float), 1, fp);
    fwrite(&totalBills,        sizeof(int),   1, fp);
    fwrite(&totalQRPayments,   sizeof(int),   1, fp);
    fwrite(&totalCashPayments, sizeof(int),   1, fp);
    fclose(fp);
}
void loadCounters() {
    FILE *fp = fopen(FILE_COUNTERS, "rb");
    if (!fp) return;
    fread(&totalRevenue,      sizeof(float), 1, fp);
    fread(&totalDiscount,     sizeof(float), 1, fp);
    fread(&totalBills,        sizeof(int),   1, fp);
    fread(&totalQRPayments,   sizeof(int),   1, fp);
    fread(&totalCashPayments, sizeof(int),   1, fp);
    fclose(fp);
}
void saveAllData() {
    saveProducts(); saveMembers(); saveCustomers(); saveSales(); saveCounters();
}
void loadAllData() {
    loadProducts(); loadMembers(); loadCustomers(); loadSales(); loadCounters();
}

/* -- Backup & Restore Menu -- */
void backupRestore() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("BACKUP & RESTORE DATA");
        printf("  Files are stored in the program directory.\n\n");
        printf("  " BGREEN  "1." RESET " Save All Data (Backup)\n");
        printf("  " BCYAN   "2." RESET " Load All Data (Restore)\n");
        printf("  " BYELLOW "3." RESET " View Backup File Status\n");
        printf("  " BWHITE  "0." RESET " Back to Admin Portal\n");
        printDash();
        printf("  " BOLD "Enter your choice: " RESET);

        if (!readInt(&choice)) {
            WARN("Invalid input.");
            system("pause");
            continue;
        }

        switch (choice) {
            case 1:
                saveAllData();
                printf("\n");
                OK("All data saved successfully.");
                printf("       Products  -> " BCYAN "%s\n" RESET, FILE_PRODUCTS);
                printf("       Members   -> " BCYAN "%s\n" RESET, FILE_MEMBERS);
                printf("       Customers -> " BCYAN "%s\n" RESET, FILE_CUSTOMERS);
                printf("       Sales     -> " BCYAN "%s\n" RESET, FILE_SALES);
                printf("       Counters  -> " BCYAN "%s\n" RESET, FILE_COUNTERS);
                printf("\n"); system("pause"); break;
            case 2: {
                char confirm[5];
                printf("\n  " BRED "[!]" RESET " This will overwrite current in-memory data.\n");
                printf("  Are you sure? (yes/no): ");
                scanf("%4s", confirm);
                if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) {
                    loadAllData();
                    OK("Data restored from files.");
                    printf("       Products  : " BGREEN "%d loaded\n" RESET, nProducts);
                    printf("       Members   : " BGREEN "%d loaded\n" RESET, nMembers);
                    printf("       Customers : " BGREEN "%d loaded\n" RESET, nCustomers);
                    printf("       Sales     : " BGREEN "%d loaded\n" RESET, nSales);
                } else {
                    printf("  " BYELLOW "[-]" RESET " Restore cancelled.\n");
                }
                printf("\n"); system("pause"); break;
            }
            case 3: {
                FILE *fp;
                system("cls");
                printHeader("BACKUP FILE STATUS");
                printf("  " BOLD "%-30s  %s\n" RESET, "File", "Status");
                printDash();
                #define CHECK_FILE(f) \
                    fp = fopen(f, "rb"); \
                    printf("  %-30s  %s\n", f, fp ? BGREEN "EXISTS" RESET : BRED "NOT FOUND" RESET); \
                    if (fp) fclose(fp);
                CHECK_FILE(FILE_PRODUCTS)
                CHECK_FILE(FILE_MEMBERS)
                CHECK_FILE(FILE_CUSTOMERS)
                CHECK_FILE(FILE_SALES)
                CHECK_FILE(FILE_COUNTERS)
                #undef CHECK_FILE
                printf("\n"); system("pause"); break;
            }
            case 0: return;
            default:
                WARN("Invalid option.");
                system("pause");
        }
    }
}

/* -- Password input with masking -- */
void inputPassword(char *password) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) {
            password[i] = '\0';
            break;
        } else if (ch == 8) {
            if (i > 0) { i--; printf("\b \b"); }
        } else {
            password[i++] = ch;
            printf(BMAGENTA "*" RESET);
        }
    }
}

/* -- Check if product code exists -- */
int isProductCodeExists(int code) {
    int i;
    for (i = 0; i < nProducts; i++)
        if (shop[i].pCode == code) return 1;
    return 0;
}

/* -- Check if product name exists (case-insensitive) -- */
int isProductNameExists(char name[], int excludeIndex) {
    int i;
    for (i = 0; i < nProducts; i++) {
        if (i == excludeIndex) continue;
        if (strEqualCI(shop[i].pName, name)) return 1;
    }
    return 0;
}

/* -- Check if customer ID exists -- */
int isCustomerIdExists(int id) {
    int i;
    for (i = 0; i < nCustomers; i++)
        if (customers[i].cId == id) return 1;
    return 0;
}