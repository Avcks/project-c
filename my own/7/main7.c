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

#define MAX_ITEMS      100
#define MAX_MEMBERS    100
#define MAX_CART        50
#define MAX_CUSTOMERS  100

/* ── credentials ── */
char ad_user[10] = "admin";
char ad_pass[10] = "admin";
char sf_user[10] = "staff";
char sf_pass[10] = "staff";

/* ══════════════════ FORWARD DECLARATIONS ══════════════════ */
void mainManu();
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
void productSales();
void discountReport();
void recordSale(int, char[], int, float);
float applyDiscount(int, float);
void billing(float finalAmount, int isQR);
void staff();
void underDev();

/* ── Customer account forward declarations ── */
void custManage();
void addCustomer();
void deleteCustomer();
void viewCustomers();
int  customerLogin();
void viewMyCard();

int totalQRPayments = 0;
int totalCashPayments = 0;

/* ══════════════════ STRUCTURES ══════════════════ */
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
} sales[500];
int   nSales        = 0;
float totalRevenue  = 0;
float totalDiscount = 0;
int   totalBills    = 0;

/* ── Customer account structure ── */
struct Customer {
    int  cId;
    char cName[50];
    char cPass[20];
    int  cMemberId;
} customers[MAX_CUSTOMERS];
int nCustomers   = 0;
int custLoggedIn = 0;
int currentCustIdx = -1;

/* ══════════════════ STYLING HELPERS ══════════════════ */
void printLine() {
    printf("============================================================\n");
}
void printDash() {
    printf("------------------------------------------------------------\n");
}
void printHeader(const char *title) {
    printf("\n");
    printLine();
    printf("   %s\n", title);
    printLine();
}

/* ══════════════════ MAIN ══════════════════ */
int main() {
    SetConsoleOutputCP(CP_UTF8);
    shop[0] = (struct Product){101, "Rice",     45.00,  80, "Grocery"};
    shop[1] = (struct Product){102, "Milk",     25.00, 120, "Dairy"};
    shop[2] = (struct Product){103, "Bread",    30.00,  60, "Bakery"};
    shop[3] = (struct Product){104, "Shampoo",  90.00,  40, "Personal"};
    shop[4] = (struct Product){105, "PenDrive", 350.00, 15, "Electronics"};
    nProducts = 5;

    mainManu();
    return 0;
}

/* ══════════════════ MAIN MENU ══════════════════ */
void mainManu() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printf("  1. Customer Self-Checkout\n");
        printf("  2. Staff / Admin Login\n");
        printf("  3. View Store Info\n");
        printf("  0. Exit Application\n");
        printDash();
        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: cust();  break;
            case 2: login(); break;
            case 3: info();  break;
            case 0:
                printHeader("Thank you for visiting AYH Mart!");
                exit(0);
            default:
                printf("  [!] Invalid option. Try again.\n");
                system("pause");
        }
    }
}

/* ══════════════════ LOGIN ══════════════════ */
void login() {
    char username[10], password[10];
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("LOGIN");
    printf("  Username: "); scanf("%s", username);
    printf("  Password: "); scanf("%s", password);

    if (strcmp(username, ad_user) == 0 && strcmp(password, ad_pass) == 0) {
        printf("\n  [✓] Logged in as ADMIN\n");
        system("pause");
        adminDashboard();
    } else if (strcmp(username, sf_user) == 0 && strcmp(password, sf_pass) == 0) {
        printf("\n  [✓] Logged in as STAFF\n");
        system("pause");
        staff();
    } else {
        printf("\n  [✗] Incorrect username or password.\n");
        system("pause");
    }
}

/* ══════════════════ ADMIN DASHBOARD ══════════════════ */
void adminDashboard() {
    int choice;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("ADMIN PORTAL");
    printf("  1. Inventory\n");
    printf("  2. Members\n");
    printf("  3. Sales Report\n");
    printf("  4. Customer Accounts\n");
    printf("  5. Under Development\n");
    printf("  0. Logout\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: inventory();      break;
        case 2: members();        break;
        case 3: salesManu();      break;
        case 4: custManage();     break;
        case 5: underDev();       break;
        case 0:
            printf("\n  [✓] Logged out successfully.\n\n");
            system("pause");
            mainManu();
            break;
        default:
            printf("  [!] Invalid option.\n");
            system("pause");
            adminDashboard();
    }
}

/* ══════════════════ INVENTORY ══════════════════ */
void inventory() {
    int choice;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("INVENTORY MANAGEMENT");
    printf("  1. Add Product(s)\n");
    printf("  2. Edit Product\n");
    printf("  3. Delete Product\n");
    printf("  4. View All Products\n");
    printf("  0. Back\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: add();            break;
        case 2: editProduct();    break;
        case 3: del();            break;
        case 4: view();           break;
        case 0: adminDashboard(); break;
        default:
            printf("  [!] Invalid option.\n");
            system("pause");
            inventory();
    }
}

/* ── Add products ── */
void add() {
    int count, i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  How many products to add: ");
    scanf("%d", &count);

    for (i = 0; i < count; i++) {
        if (nProducts >= MAX_ITEMS) {
            printf("  [!] Product limit reached (%d max).\n", MAX_ITEMS);
            system("pause");
            inventory();
            return;
        }
        printf("\n  -- Product %d --\n", i + 1);
        printf("  Code    : "); scanf("%d",  &shop[nProducts].pCode);
        printf("  Name    : "); scanf(" %s",  shop[nProducts].pName);
        printf("  Price   : "); scanf("%f",  &shop[nProducts].pPrice);
        printf("  Stock   : "); scanf("%d",  &shop[nProducts].pStock);
        printf("  Category: "); scanf("%s",   shop[nProducts].pCategory);
        nProducts++;
    }
    printf("\n  [✓] %d product(s) added successfully.\n", count);
    system("pause");
    inventory();
}

/* ── Edit product ── */
void editProduct() {
    int code, found = 0, i;
    printf("\n  Enter Product Code to Edit: ");
    scanf("%d", &code);

    for (i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            found = 1;
            printf("\n  Editing: %s\n", shop[i].pName);
            printf("  Leave field blank / enter 0 to keep current value.\n\n");

            char  buf[50];
            float fval;
            int   ival;

            printf("  New Name  [%s]: ", shop[i].pName);
            scanf(" %49s", buf);
            if (strcmp(buf, "0") != 0) strcpy(shop[i].pName, buf);

            printf("  New Price [%.2f]: ", shop[i].pPrice);
            scanf("%f", &fval);
            if (fval > 0) shop[i].pPrice = fval;

            printf("  New Stock [%d]: ", shop[i].pStock);
            scanf("%d", &ival);
            if (ival >= 0) shop[i].pStock = ival;

            printf("  New Category [%s]: ", shop[i].pCategory);
            scanf(" %29s", buf);
            if (strcmp(buf, "0") != 0) strcpy(shop[i].pCategory, buf);

            printf("\n  [✓] Product updated successfully.\n");
            break;
        }
    }
    if (!found) printf("  [✗] Product code not found.\n");
    system("pause");
    inventory();
}

/* ── View products ── */
void view() {
    int i;
    system("cls");
    printHeader("AVAILABLE PRODUCTS");
    if (nProducts == 0) {
        printf("  No products in inventory.\n");
    } else {
        printf("  %-6s  %-18s  %-8s  %-6s  %s\n", "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
        printDash();
        for (i = 0; i < nProducts; i++) {
            printf("  %-6d  %-18s  %-8.2f  %-6d  %s\n",
                   shop[i].pCode, shop[i].pName, shop[i].pPrice,
                   shop[i].pStock, shop[i].pCategory);
        }
    }
    printf("\n");
    system("pause");
    inventory();
}

/* ── Delete product ── */
void del() {
    int code, found = 0, i, j;
    printf("\n  Enter Product Code to Delete: ");
    scanf("%d", &code);

    for (i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            for (j = i; j < nProducts - 1; j++) shop[j] = shop[j + 1];
            nProducts--;
            found = 1;
            printf("  [✓] Product deleted successfully.\n");
            break;
        }
    }
    if (!found) printf("  [✗] Product code not found.\n");
    system("pause");
    inventory();
}

/* ══════════════════ STORE INFO ══════════════════ */
void info() {
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("STORE INFORMATION");
    printf("  Store Name   : AYH Mart\n");
    printf("  Location     : Surat, India\n");
    printf("  Timing       : 9 AM - 10 PM (Mon-Sun)\n");
    printf("  Developed By : Aum, Yesh, HariKrushna\n");
    printLine();
    system("pause");
    mainManu();
}

/* ══════════════════ CUSTOMER LOGIN ══════════════════ */
int customerLogin() {
    int  id, i;
    char pass[20];
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("CUSTOMER LOGIN");
    printf("  Customer ID : "); scanf("%d",  &id);
    printf("  Password    : "); scanf("%s",   pass);

    for (i = 0; i < nCustomers; i++) {
        if (customers[i].cId == id && strcmp(customers[i].cPass, pass) == 0) {
            printf("\n  [✓] Welcome, %s!\n", customers[i].cName);
            system("pause");
            custLoggedIn   = 1;
            currentCustIdx = i;
            return i;
        }
    }
    printf("\n  [✗] Invalid Customer ID or Password.\n");
    system("pause");
    return -1;
}

/* ══════════════════ CUSTOMER MENU ══════════════════ */
void cust() {
    int choice, i;

    if (!custLoggedIn) {
        if (customerLogin() == -1) {
            mainManu();
            return;
        }
    }

    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("CUSTOMER SELF-CHECKOUT");
    printf("  Welcome, %s\n", customers[currentCustIdx].cName);
    printDash();
    printf("  1. View Store Products\n");
    printf("  2. Search for a Product\n");
    printf("  3. Add Item to Basket\n");
    printf("  4. View My Basket\n");
    printf("  5. Final Checkout\n");
    printf("  6. My Membership Card\n");
    printf("  0. Logout & Return to Main Menu\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: custView(); break;
        case 2: search();   break;
        case 3:
            system("cls");
            printHeader("ADD ITEM TO BASKET");
            if (nProducts == 0) {
                printf("  No products available.\n");
            } else {
                printf("  %-6s  %-18s  %-8s  %-6s  %s\n", "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
                printDash();
                for (i = 0; i < nProducts; i++)
                    printf("  %-6d  %-18s  %-8.2f  %-6d  %s\n",
                           shop[i].pCode, shop[i].pName, shop[i].pPrice,
                           shop[i].pStock, shop[i].pCategory);
            }
            addCart();
            break;
        case 4: viewCart();  break;
        case 5: checkout();  break;
        case 6: viewMyCard(); break;
        case 0:
            custLoggedIn   = 0;
            currentCustIdx = -1;
            mainManu();
            break;
        default:
            printf("  [!] Invalid option.\n");
            system("pause");
            cust();
    }
}

/* ── Customer view ── */
void custView() {
    int i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("AVAILABLE PRODUCTS");
    if (nProducts == 0) {
        printf("  No products available.\n");
    } else {
        printf("  %-6s  %-18s  %-8s  %-6s  %s\n", "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
        printDash();
        for (i = 0; i < nProducts; i++) {
            printf("  %-6d  %-18s  %-8.2f  %-6d  %s\n",
                   shop[i].pCode, shop[i].pName, shop[i].pPrice,
                   shop[i].pStock, shop[i].pCategory);
        }
    }
    printf("\n");
    system("pause");
    cust();
}

/* ── Search ── */
void search() {
    int code, found = 0, i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  Enter Product Code to Search: ");
    scanf("%d", &code);

    for (i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            printDash();
            printf("  Found:\n");
            printf("    Product Name     : %s\n",   shop[i].pName);
            printf("    Product Price    : %.2f\n", shop[i].pPrice);
            printf("    Product Stock    : %d\n",   shop[i].pStock);
            printf("    Product Category : %s\n",   shop[i].pCategory);
            printDash();
            found = 1;
            break;
        }
    }
    if (!found) printf("  [✗] Product not found.\n");
    printf("\n");
    system("pause");
    cust();
}

/* ── Add to cart ── */
void addCart() {
    int  code, qty, found = 0, i;
    char confirm[4];

    if (cartCount >= MAX_CART) {
        printf("  [!] Cart is full!\n");
        system("pause");
        cust();
        return;
    }

    printf("\n  Enter Product Code : ");
    scanf("%d", &code);
    printf("  Enter Quantity     : ");
    scanf("%d", &qty);

    for (i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            if (shop[i].pStock < qty) {
                printf("  [!] Insufficient stock (available: %d).\n", shop[i].pStock);
                found = 1;
                break;
            }

            printf("\n  +- Confirm Item ----------------------------+\n");
            printf("  |  Name  : %-31s|\n", shop[i].pName);
            printf("  |  Price : %-5.2f per unit                 |\n", shop[i].pPrice);
            printf("  |  Qty   : %-31d|\n", qty);
            printf("  |  Total : %-5.2f                           |\n", shop[i].pPrice * qty);
            printf("  +-------------------------------------------+\n");
            printf("  Add to cart? (yes/no): ");
            scanf(" %3s", confirm);

            if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) {
                cart[cartCount].code  = shop[i].pCode;
                strcpy(cart[cartCount].name, shop[i].pName);
                cart[cartCount].price = shop[i].pPrice;
                cart[cartCount].qty   = qty;
                shop[i].pStock       -= qty;
                cartCount++;
                printf("  [✓] Item added to basket.\n");
            } else {
                printf("  [-] Item not added.\n");
            }
            found = 1;
            break;
        }
    }
    if (!found) printf("  [✗] Product unavailable.\n");
    printf("\n");
    system("pause");
    cust();
}

/* ── View cart ── */
void viewCart() {
    float total = 0;
    int   i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("MY BASKET");
    if (cartCount == 0) {
        printf("  Basket is empty.\n");
    } else {
        printf("  %-6s  %-18s  %-8s  %-5s  %s\n", "CODE", "NAME", "PRICE", "QTY", "AMOUNT");
        printDash();
        for (i = 0; i < cartCount; i++) {
            float amt = cart[i].price * cart[i].qty;
            total    += amt;
            printf("  %-6d  %-18s  %-8.2f  %-5d  %.2f\n",
                   cart[i].code, cart[i].name, cart[i].price, cart[i].qty, amt);
        }
        printDash();
        printf("  %-40s  %.2f\n", "Grand Total:", total);
    }
    printf("\n");
    system("pause");
    cust();
}

/* ══════════════════ PRINT QR CODE ══════════════════ */
/*
 * Accurate ASCII representation of the UPI QR code
 * belonging to: Dhameliya Yash Mukeshbhai
 * UPI ID      : 6353999107@ptyes
 * Each ## = dark module   (space)(space) = light module
 */
void printQRCode() {
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
}

/* ── Checkout ── */
void checkout() {
    int   mid, i, payChoice;
    float subtotal = 0, discount = 0, finalAmount;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    if (cartCount == 0) {
        printf("  [!] Basket is empty.\n");
        system("pause");
        cust();
        return;
    }

    for (i = 0; i < cartCount; i++) {
        subtotal += cart[i].price * cart[i].qty;
    }

    printf("\n  Enter Member ID (0 if none): ");
    scanf("%d", &mid);

    discount    = applyDiscount(mid, subtotal);
    finalAmount = subtotal - discount;

    printDash();
    printf("  Subtotal        : %.2f\n", subtotal);
    printf("  Discount        : %.2f\n", discount);
    printf("  Payable Amount  : %.2f\n", finalAmount);
    printDash();

    printf("\n  SELECT PAYMENT METHOD:\n");
    printf("  1. Cash\n");
    printf("  2. QR Code (Digital)\n");
    printf("  Enter choice: ");
    scanf("%d", &payChoice);

    for (i = 0; i < cartCount; i++) {
        recordSale(cart[i].code, cart[i].name, cart[i].qty, cart[i].price);
    }

    if (payChoice == 2) {
        /* ══ UPDATED QR CODE — matches Dhameliya Yash Mukeshbhai's actual UPI QR ══ */
        printf("\n  [ SCAN QR CODE TO PAY — Dhameliya Yash Mukeshbhai ]\n\n");
        printQRCode();
        printf("\n  UPI ID : 6353999107@ptyes\n");
        printf("  Amount : Rs. %.2f\n\n", finalAmount);
        totalQRPayments++;
        billing(finalAmount, 1);
    } else {
        totalCashPayments++;
        billing(finalAmount, 0);
    }

    totalBills++;
    cartCount = 0;

    custLoggedIn   = 0;
    currentCustIdx = -1;
}

/* ── Billing ── */
void billing(float finalAmount, int isQR) {
    if (isQR) {
        printLine();
        printf("  Transaction ID: AYH%d\n", 1000 + totalBills);
        printf("  Thank you for shopping at AYH Mart!\n");
        printLine();
        system("pause");
        mainManu();
        return;
    }

    float payAmount;
    printf("  Pay by Cash (enter amount): ");
    scanf("%f", &payAmount);

    if (payAmount >= finalAmount) {
        float change = payAmount - finalAmount;
        printf("\n");
        printLine();
        printf("  [✓] Payment received! Change: %.2f\n", change);
        printf("  Thank you for shopping at AYH Mart!\n");
        printLine();
        system("pause");
        mainManu();
    } else {
        printf("  [!] Insufficient. Need %.2f more.\n", finalAmount - payAmount);
        billing(finalAmount, 0);
    }
}

/* ══════════════════ MEMBERS ══════════════════ */
void members() {
    int choice;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("MEMBER PORTAL");
    printf("  1. Add New Member\n");
    printf("  2. Delete Member\n");
    printf("  3. View All Members\n");
    printf("  0. Back\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: addMembers();     break;
        case 2: deleteMembers();  break;
        case 3: viewMembers();    break;
        case 0: adminDashboard(); break;
        default:
            printf("  [!] Invalid option.\n");
            system("pause");
            members();
    }
}

/* ── Add members ── */
void addMembers() {
    int  count, i;
    char tier[20];
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  How many members to add: ");
    scanf("%d", &count);

    for (i = 0; i < count; i++) {
        if (nMembers >= MAX_MEMBERS) {
            printf("  [!] Member limit reached.\n");
            break;
        }
        printf("\n  -- Member %d --\n", i + 1);
        printf("  Member ID   : "); scanf("%d",  &Members[nMembers].mId);
        printf("  Name        : "); scanf("%s",   Members[nMembers].mName);
        printf("  Amount Spent: "); scanf("%f",  &Members[nMembers].mSpent);

        while (1) {
            printf("  Tier (Silver/s | Gold/g | Platinum/p): ");
            scanf("%s", tier);

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
                printf("  [!] Invalid tier. Please enter Silver/s, Gold/g, or Platinum/p.\n");
            }
        }
        Members[nMembers].mUsed = 0;
        nMembers++;
    }
    printf("\n  [✓] Member(s) added successfully.\n");
    system("pause");
    members();
}

/* ── Delete member ── */
void deleteMembers() {
    int i, id, found = 0, j;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  Enter Member ID to Delete: ");
    scanf("%d", &id);

    for (i = 0; i < nMembers; i++) {
        if (Members[i].mId == id) {
            for (j = i; j < nMembers - 1; j++) Members[j] = Members[j + 1];
            nMembers--;
            found = 1;
            printf("  [✓] Member deleted.\n");
            break;
        }
    }
    if (!found) printf("  [✗] Member ID not found.\n");
    system("pause");
    members();
}

/* ── View members ── */
void viewMembers() {
    int i;
    system("cls");
    printHeader("ALL MEMBERS");
    if (nMembers == 0) {
        printf("  No members registered.\n");
    } else {
        printf("  %-5s  %-18s  %-10s  %-10s  %-9s  %s\n",
               "ID", "NAME", "SPENT", "TIER", "DISCOUNT", "CARD STATUS");
        printDash();
        for (i = 0; i < nMembers; i++) {
            printf("  %-5d  %-18s  %-10.2f  %-10s  %-9.0f%%  %s\n",
                   Members[i].mId,
                   Members[i].mName,
                   Members[i].mSpent,
                   Members[i].mTier,
                   Members[i].mDisc,
                   Members[i].mUsed ? "Used" : "Active");
        }
    }
    printf("\n");
    system("pause");
    members();
}

/* ══════════════════ CUSTOMER ACCOUNT MANAGEMENT ══════════════════ */

void custManage() {
    int choice;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("CUSTOMER ACCOUNT MANAGEMENT");
    printf("  1. Add New Customer Account\n");
    printf("  2. Delete Customer Account\n");
    printf("  3. View All Customer Accounts\n");
    printf("  0. Back\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: addCustomer();    break;
        case 2: deleteCustomer(); break;
        case 3: viewCustomers();  break;
        case 0: adminDashboard(); break;
        default:
            printf("  [!] Invalid option.\n");
            system("pause");
            custManage();
    }
}

/* ── Add customer accounts ── */
void addCustomer() {
    int  count, i;
    char passConfirm[20];
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  How many customer accounts to add: ");
    scanf("%d", &count);

    for (i = 0; i < count; i++) {
        if (nCustomers >= MAX_CUSTOMERS) {
            printf("  [!] Customer limit reached (%d max).\n", MAX_CUSTOMERS);
            break;
        }
        printf("\n  -- Customer Account %d --\n", i + 1);
        printf("  Customer ID  : "); scanf("%d",  &customers[nCustomers].cId);
        printf("  Name         : "); scanf("%s",   customers[nCustomers].cName);

        while (1) {
            printf("  Password     : "); scanf("%s", customers[nCustomers].cPass);
            printf("  Confirm Pwd  : "); scanf("%s", passConfirm);
            if (strcmp(customers[nCustomers].cPass, passConfirm) == 0) {
                break;
            } else {
                printf("  [!] Passwords do not match. Try again.\n");
            }
        }

        printf("  Member ID    : (enter 0 if no membership) ");
        scanf("%d", &customers[nCustomers].cMemberId);

        nCustomers++;
    }
    printf("\n  [✓] Customer account(s) created successfully.\n");
    system("pause");
    custManage();
}

/* ── Delete customer account ── */
void deleteCustomer() {
    int id, i, j, found = 0;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printf("\n  Enter Customer ID to Delete: ");
    scanf("%d", &id);

    for (i = 0; i < nCustomers; i++) {
        if (customers[i].cId == id) {
            for (j = i; j < nCustomers - 1; j++) customers[j] = customers[j + 1];
            nCustomers--;
            found = 1;
            printf("  [✓] Customer account deleted.\n");
            break;
        }
    }
    if (!found) printf("  [✗] Customer ID not found.\n");
    system("pause");
    custManage();
}

/* ── View all customer accounts ── */
void viewCustomers() {
    int i;
    system("cls");
    printHeader("ALL CUSTOMER ACCOUNTS");
    if (nCustomers == 0) {
        printf("  No customer accounts registered.\n");
    } else {
        printf("  %-6s  %-18s  %-12s  %s\n", "ID", "NAME", "PASSWORD", "MEMBER ID");
        printDash();
        for (i = 0; i < nCustomers; i++) {
            if (customers[i].cMemberId != 0) {
                printf("  %-6d  %-18s  %-12s  %d\n",
                       customers[i].cId,
                       customers[i].cName,
                       customers[i].cPass,
                       customers[i].cMemberId);
            } else {
                printf("  %-6d  %-18s  %-12s  None\n",
                       customers[i].cId,
                       customers[i].cName,
                       customers[i].cPass);
            }
        }
    }
    printf("\n");
    system("pause");
    custManage();
}

/* ══════════════════ VIEW MY MEMBERSHIP CARD ══════════════════ */
void viewMyCard() {
    int i, found = 0;
    int linkedId = customers[currentCustIdx].cMemberId;

    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("MY MEMBERSHIP CARD");

    printf("  Account Holder : %s\n",  customers[currentCustIdx].cName);
    printf("  Customer ID    : %d\n",  customers[currentCustIdx].cId);
    printDash();

    if (linkedId == 0) {
        printf("  [!] No membership card linked to your account.\n");
        printf("      Please contact Staff or Admin to get one assigned.\n");
    } else {
        for (i = 0; i < nMembers; i++) {
            if (Members[i].mId == linkedId) {
                found = 1;
                printf("  Member ID      : %d\n",       Members[i].mId);
                printf("  Member Name    : %s\n",       Members[i].mName);
                printf("  Tier           : %s\n",       Members[i].mTier);
                printf("  Discount Rate  : %.0f%%\n",   Members[i].mDisc);
                printf("  Total Spent    : Rs. %.2f\n", Members[i].mSpent);
                printf("  Card Status    : %s\n",
                       Members[i].mUsed ? "Used (discount already redeemed)"
                                        : "Active (discount available)");
                break;
            }
        }
        if (!found) {
            printf("  [!] Linked Member ID (%d) not found in records.\n", linkedId);
            printf("      Please contact Staff or Admin.\n");
        }
    }

    printDash();
    system("pause");
    cust();
}

/* ══════════════════ SALES ══════════════════ */
void recordSale(int code, char name[], int qty, float price) {
    if (nSales >= 500) return;
    sales[nSales].pCode = code;
    strcpy(sales[nSales].pName, name);
    sales[nSales].qty   = qty;
    sales[nSales].price = price;
    sales[nSales].total = qty * price;
    totalRevenue       += sales[nSales].total;
    nSales++;
}

/* ── applyDiscount: one-time card restriction ── */
float applyDiscount(int memberId, float billAmount) {
    int i;
    if (memberId == 0) return 0;

    for (i = 0; i < nMembers; i++) {
        if (Members[i].mId == memberId) {
            if (Members[i].mUsed == 1) {
                printf("\n  [!] Membership card already used. No discount applied.\n");
                return 0;
            }
            float discount = billAmount * (Members[i].mDisc / 100.0f);
            Members[i].mSpent += (billAmount - discount);
            totalDiscount     += discount;
            Members[i].mUsed   = 1;
            printf("  [✓] Membership discount applied (%.0f%%). Card is now marked as used.\n",
                   Members[i].mDisc);
            return discount;
        }
    }

    printf("  [!] Member ID not found. No discount applied.\n");
    return 0;
}

void salesManu() {
    int choice;
    while (1) {
        system("cls");
        printHeader("AYH MART  |  Surat, India");
        printHeader("SALES REPORT");
        printf("  1. Summary Report\n");
        printf("  2. Product-wise Sales\n");
        printf("  3. Member Discount Report\n");
        printf("  0. Back\n");
        printDash();
        printf("  Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: salesSummary();   break;
            case 2: productSales();   break;
            case 3: discountReport(); break;
            case 0: return;
            default:
                printf("  [!] Invalid option.\n");
                system("pause");
        }
    }
}

void salesSummary() {
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("SALES SUMMARY");
    printf("  Total Bills Generated : %d\n",   totalBills);
    printf("  -- Cash Payments      : %d\n",   totalCashPayments);
    printf("  -- QR Payments        : %d\n",   totalQRPayments);
    printf("  Total Items Sold      : %d\n",   nSales);
    printf("  Total Revenue         : %.2f\n", totalRevenue);
    printf("  Total Discount Given  : %.2f\n", totalDiscount);
    printf("  Net Revenue           : %.2f\n", totalRevenue - totalDiscount);
    printDash();
    system("pause");
}

void productSales() {
    int i;
    system("cls");
    printHeader("PRODUCT SALES REPORT");
    if (nSales == 0) {
        printf("  No sales recorded yet.\n");
    } else {
        printf("  %-6s  %-18s  %-6s  %s\n", "CODE", "NAME", "QTY", "TOTAL");
        printDash();
        for (i = 0; i < nSales; i++)
            printf("  %-6d  %-18s  %-6d  %.2f\n",
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
        printf("  No members recorded.\n");
    } else {
        printf("  %-5s  %-18s  %-10s  %-10s  %-9s  %s\n",
               "ID", "NAME", "SPENT", "TIER", "DISCOUNT", "CARD STATUS");
        printDash();
        for (i = 0; i < nMembers; i++)
            printf("  %-5d  %-18s  %-10.2f  %-10s  %-9.0f%%  %s\n",
                   Members[i].mId, Members[i].mName,
                   Members[i].mSpent, Members[i].mTier,
                   Members[i].mDisc,
                   Members[i].mUsed ? "Used" : "Active");
    }
    printf("\n");
    system("pause");
}

/* ══════════════════ STAFF PORTAL ══════════════════ */
void staff() {
    int choice;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("STAFF PORTAL");
    printf("  1. View Inventory\n");
    printf("  2. View All Members\n");
    printf("  3. Add New Member\n");
    printf("  4. Remove Member\n");
    printf("  5. View Sales Summary\n");
    printf("  6. Search Product\n");
    printf("  7. Customer Accounts\n");
    printf("  0. Logout\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            int i;
            system("cls");
            printHeader("INVENTORY (Staff View)");
            if (nProducts == 0) {
                printf("  No products.\n");
            } else {
                printf("  %-6s  %-18s  %-8s  %-6s  %s\n",
                       "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
                printDash();
                for (i = 0; i < nProducts; i++)
                    printf("  %-6d  %-18s  %-8.2f  %-6d  %s\n",
                           shop[i].pCode, shop[i].pName, shop[i].pPrice,
                           shop[i].pStock, shop[i].pCategory);
            }
            printf("\n");
            system("pause");
            staff();
            break;
        }
        case 2: {
            int i;
            system("cls");
            printHeader("ALL MEMBERS (Staff View)");
            if (nMembers == 0) {
                printf("  No members registered.\n");
            } else {
                printf("  %-5s  %-18s  %-10s  %-10s  %-9s  %s\n",
                       "ID", "NAME", "SPENT", "TIER", "DISCOUNT", "CARD STATUS");
                printDash();
                for (i = 0; i < nMembers; i++)
                    printf("  %-5d  %-18s  %-10.2f  %-10s  %-9.0f%%  %s\n",
                           Members[i].mId, Members[i].mName,
                           Members[i].mSpent, Members[i].mTier,
                           Members[i].mDisc,
                           Members[i].mUsed ? "Used" : "Active");
            }
            printf("\n");
            system("pause");
            staff();
            break;
        }
        case 3: addMembers();  break;
        case 4: deleteMembers(); break;
        case 5:
            salesSummary();
            staff();
            break;
        case 6: {
            int code, found = 0, i;
            system("cls");
            printHeader("AYH MART  |  Surat, India");
            printf("\n  Enter Product Code to Search: ");
            scanf("%d", &code);
            for (i = 0; i < nProducts; i++) {
                if (shop[i].pCode == code) {
                    printDash();
                    printf("  Found:\n");
                    printf("    Product Name     : %s\n",   shop[i].pName);
                    printf("    Product Price    : %.2f\n", shop[i].pPrice);
                    printf("    Product Stock    : %d\n",   shop[i].pStock);
                    printf("    Product Category : %s\n",   shop[i].pCategory);
                    printDash();
                    found = 1;
                    break;
                }
            }
            if (!found) printf("  [✗] Product not found.\n");
            printf("\n");
            system("pause");
            staff();
            break;
        }
        case 7: {
            int staffChoice;
            system("cls");
            printHeader("AYH MART  |  Surat, India");
            printHeader("CUSTOMER ACCOUNT MANAGEMENT");
            printf("  1. Add New Customer Account\n");
            printf("  2. Delete Customer Account\n");
            printf("  3. View All Customer Accounts\n");
            printf("  0. Back\n");
            printDash();
            printf("  Enter your choice: ");
            scanf("%d", &staffChoice);

            switch (staffChoice) {
                case 1: addCustomer();    break;
                case 2: deleteCustomer(); break;
                case 3: viewCustomers();  break;
                case 0: staff();          break;
                default:
                    printf("  [!] Invalid option.\n");
                    system("pause");
                    staff();
            }
            break;
        }
        case 0:
            printf("  [✓] Logged out.\n");
            system("pause");
            mainManu();
            break;
        default:
            printf("  [!] Invalid option.\n");
            system("pause");
            staff();
    }
}

/* ══════════════════ UNDER DEVELOPMENT MODULE ══════════════════ */
void underDev() {
    int choice;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("SYSTEM MODULES - UNDER DEVELOPMENT");

    printf("  1. Employee Management\n");
    printf("  2. Stock Alerts / Reorder Levels\n");
    printf("  3. Expense Tracking\n");
    printf("  4. Backup & Data Security\n");
    printf("  5. System Logs\n");
    printf("  0. Back to Admin Portal\n");
    printDash();

    printf("  Enter choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\n  [!] Status: UI layout complete. Logic pending.\n");
            break;
        case 2:
            printf("\n  [!] Status: Integrating with Inventory module.\n");
            break;
        case 3:
            printf("\n  [!] Status: Database schema under design.\n");
            break;
        case 4:
            printf("\n  [!] Status: Encryption protocols being implemented.\n");
            break;
        case 5:
            printf("\n  [!] Status: File handling logic in progress.\n");
            break;
        case 0:
            adminDashboard();
            return;
        default:
            printf("\n  [!] Invalid choice. Please select 0-5.\n");
            system("pause");
            underDev();
            return;
    }

    printf("\n");
    system("pause");
    underDev();
}
