#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ITEMS 100
#define MAX_CART 50
#define MAX_MEMBERS 100
#define MAX_RECORDS 500
#define MAX_COUPONS 50
#define MAX_STAFF 20

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define PRODUCT_DB  "products.txt"
#define MEMBER_DB   "members.txt"
#define SALES_DB    "sales.txt"
#define COUPON_DB   "coupons.txt"
#define STAFF_DB    "staff.txt"

struct DateTime {
    int d;
    int m;
    int y;
    int h;
    int min;
    int s;
};

struct Product {
    int pCode;
    char pName[50];
    float pPrice;
    int pStock;
    char pCategory[30];
};

struct Cart {
    int cCode;
    char cName[50];
    float cPrice;
    int cQuantity;
};

struct Member {
    int mId;
    char mName[50];
    float mSpent;
    char mTier[20];
    float mDisc;
};

struct Coupon {
    char coCode[15];
    float coValue;
    int coType;
    int coActive;
};

struct SaleHistory {
    int sId;
    struct DateTime sTime;
    float sSub;
    float sTax;
    float sDisc;
    float sTotal;
    char sPayMode[20];
};

struct Staff {
    int stId;
    char stUser[30];
    char stPass[30];
    int stRole;
};

struct Product shopProducts[MAX_ITEMS];
int nProducts = 0;
struct Cart myCart[MAX_CART];
int nCartItems = 0;
struct Member shopMembers[MAX_MEMBERS];
int nMembers = 0;
struct Coupon shopCoupons[MAX_COUPONS];
int nCoupons = 0;
struct SaleHistory salesLog[MAX_RECORDS];
int nSales = 0;
struct Staff staffList[MAX_STAFF];
int nStaff = 0;
struct Staff* currentUser = NULL;
char adminMasterPass[30] = "1234";

void clrscr();
void header(char* title);
void wait();
struct DateTime getSystemTime();

void loadProducts();
void saveProducts();
void loadMembers();
void saveMembers();
void loadCoupons();
void saveCoupons();
void loadSales();
void saveSales();
void loadStaff();
void saveStaff();

void addItems();
void listItems();
void findItems();
void deleteItems();

void addCustomer();
void listCustomers();
void findCustomers();

void addToBasket();
void showBasket();
void checkoutBill();
float applyVoucher(float amt);

void showReport();
void manageTeam();
void authLogin();

void customerDashboard();
void adminDashboard();

int main() {
    loadProducts();
    loadMembers();
    loadCoupons();
    loadSales();
    loadStaff();

    int choice;
    while (1) {
        clrscr();
        printf("\n");
        printf("******************************************\n");
        printf("*      WELCOME TO COSTCO SHOP SYSTEM     *\n");
        printf("******************************************\n");
        printf("1. Customer Self-Checkout\n");
        printf("2. Staff / Admin Login\n");
        printf("3. View Store Info\n");
        printf("0. Exit Application\n");
        printf("------------------------------------------\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            getchar();
            wait();
            continue;
        }

        if (choice == 1) {
            customerDashboard();
        } else if (choice == 2) {
            authLogin();
        } else if (choice == 3) {
            clrscr();
            header("STORE INFORMATION");
            printf("Store Name: Costco Mart\n");
            printf("Location: Main Street, New York\n");
            printf("Timing: 9 AM to 10 PM\n");
            printf("Developed By: Year 1 Student\n");
            wait();
        } else if (choice == 0) {
            printf("\nSaving data and closing program...\n");
            saveProducts();
            saveMembers();
            saveCoupons();
            saveSales();
            saveStaff();
            printf("Thank you. Goodbye!\n");
            break;
        } else {
            printf("Invalid choice! Try again.\n");
            wait();
        }
    }
    return 0;
}

void clrscr() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void header(char* title) {
    printf(CYAN);
    printf("\n==========================================\n");
    printf("  %s  \n", title);
    printf("==========================================\n");
    printf(RESET);
}

void wait() {
    printf("\nPress any key to continue...");
    getchar();
    getchar();
}

struct DateTime getSystemTime() {
    struct DateTime dt;
    time_t raw;
    struct tm* info;
    time(&raw);
    info = localtime(&raw);
    dt.d = info->tm_mday;
    dt.m = info->tm_mon + 1;
    dt.y = info->tm_year + 1900;
    dt.h = info->tm_hour;
    dt.min = info->tm_min;
    dt.s = info->tm_sec;
    return dt;
}

void loadProducts() {
    FILE* f = fopen(PRODUCT_DB, "r");
    if (f == NULL) {
        nProducts = 0;
        return;
    }
    nProducts = 0;
    while (nProducts < MAX_ITEMS) {
        if (fscanf(f, "%d|%[^|]|%f|%d|%[^\n]\n", 
            &shopProducts[nProducts].pCode,
            shopProducts[nProducts].pName,
            &shopProducts[nProducts].pPrice,
            &shopProducts[nProducts].pStock,
            shopProducts[nProducts].pCategory) != 5) {
            break;
        }
        nProducts++;
    }
    fclose(f);
}

void saveProducts() {
    FILE* f = fopen(PRODUCT_DB, "w");
    if (f == NULL) return;
    for (int i = 0; i < nProducts; i++) {
        fprintf(f, "%d|%s|%.2f|%d|%s\n",
            shopProducts[i].pCode,
            shopProducts[i].pName,
            shopProducts[i].pPrice,
            shopProducts[i].pStock,
            shopProducts[i].pCategory);
    }
    fclose(f);
}

void loadMembers() {
    FILE* f = fopen(MEMBER_DB, "r");
    if (f == NULL) {
        nMembers = 0;
        return;
    }
    nMembers = 0;
    while (nMembers < MAX_MEMBERS) {
        if (fscanf(f, "%d|%[^|]|%f|%[^|]|%f\n",
            &shopMembers[nMembers].mId,
            shopMembers[nMembers].mName,
            &shopMembers[nMembers].mSpent,
            shopMembers[nMembers].mTier,
            &shopMembers[nMembers].mDisc) != 5) {
            break;
        }
        nMembers++;
    }
    fclose(f);
}

void saveMembers() {
    FILE* f = fopen(MEMBER_DB, "w");
    if (f == NULL) return;
    for (int i = 0; i < nMembers; i++) {
        fprintf(f, "%d|%s|%.2f|%s|%.2f\n",
            shopMembers[i].mId,
            shopMembers[i].mName,
            shopMembers[i].mSpent,
            shopMembers[i].mTier,
            shopMembers[i].mDisc);
    }
    fclose(f);
}

void customerDashboard() {
    int cmd;
    while (1) {
        clrscr();
        header("CUSTOMER MENU");
        printf("1. View Store Products\n");
        printf("2. Search for a Product\n");
        printf("3. Add Item to Basket\n");
        printf("4. View My Basket\n");
        printf("5. Final Checkout\n");
        printf("0. Return to Main Menu\n");
        printf("------------------------------\n");
        printf("Enter option: ");
        scanf("%d", &cmd);

        if (cmd == 1) listItems();
        else if (cmd == 2) findItems();
        else if (cmd == 3) addToBasket();
        else if (cmd == 4) showBasket();
        else if (cmd == 5) checkoutBill();
        else if (cmd == 0) break;
        else {
            printf("Invalid Option!\n");
            wait();
        }
    }
}

void listItems() {
    clrscr();
    header("AVAILABLE PRODUCTS");
    if (nProducts == 0) {
        printf("No products in shop.\n");
        wait();
        return;
    }
    printf("%-5s %-20s %-10s %-10s %-15s\n", "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < nProducts; i++) {
        if (shopProducts[i].pStock < 10) printf(RED);
        printf("%-5d %-20s $%-9.2f %-10d %-15s\n",
            shopProducts[i].pCode, shopProducts[i].pName, shopProducts[i].pPrice,
            shopProducts[i].pStock, shopProducts[i].pCategory);
        printf(RESET);
    }
    wait();
}

void findItems() {
    int searchCode;
    clrscr();
    header("SEARCH PRODUCT");
    printf("Enter Product Code: ");
    scanf("%d", &searchCode);
    int found = 0;
    for (int i = 0; i < nProducts; i++) {
        if (shopProducts[i].pCode == searchCode) {
            printf("\nProduct Found: %s\nPrice: $%.2f\nStock: %d\nCategory: %s\n", 
                shopProducts[i].pName, shopProducts[i].pPrice, shopProducts[i].pStock, shopProducts[i].pCategory);
            found = 1;
            break;
        }
    }
    if (!found) printf("\nProduct not found.\n");
    wait();
}

void addToBasket() {
    int code, qty;
    clrscr();
    header("ADD TO BASSET");
    printf("Enter Product ID: ");
    scanf("%d", &code);
    int pi = -1;
    for (int i = 0; i < nProducts; i++) {
        if (shopProducts[i].pCode == code) { pi = i; break; }
    }
    if (pi == -1) { printf("Invalid Product Code.\n"); wait(); return; }
    if (shopProducts[pi].pStock <= 0) { printf("Out of stock!\n"); wait(); return; }
    printf("Product Found: %s ($%.2f)\nQty available: %d. Buy how many? ", shopProducts[pi].pName, shopProducts[pi].pPrice, shopProducts[pi].pStock);
    scanf("%d", &qty);
    if (qty <= 0) { printf("Invalid quantity!\n"); wait(); return; }
    if (qty > shopProducts[pi].pStock) qty = shopProducts[pi].pStock;
    int ci = -1;
    for (int i = 0; i < nCartItems; i++) {
        if (myCart[i].cCode == code) { ci = i; break; }
    }
    if (ci != -1) myCart[ci].cQuantity += qty;
    else {
        if (nCartItems >= MAX_CART) { printf("Cart is full!\n"); wait(); return; }
        myCart[nCartItems].cCode = code;
        strcpy(myCart[nCartItems].cName, shopProducts[pi].pName);
        myCart[nCartItems].cPrice = shopProducts[pi].pPrice;
        myCart[nCartItems].cQuantity = qty;
        nCartItems++;
    }
    shopProducts[pi].pStock -= qty;
    printf(GREEN "\nAdded to bill!\n" RESET);
    wait();
}

void showBasket() {
    clrscr();
    header("YOUR SHOPPING CART");
    if (nCartItems == 0) { printf("Basket is empty.\n"); wait(); return; }
    float sub = 0;
    printf("%-5s %-20s %-10s %-10s %-10s\n", "NO", "PRODUCT", "PRICE", "QUANTITY", "TOTAL");
    for (int i = 0; i < nCartItems; i++) {
        float t = myCart[i].cPrice * myCart[i].cQuantity;
        printf("%-5d %-20s $%-9.2f x %-8d $%-9.2f\n", i + 1, myCart[i].cName, myCart[i].cPrice, myCart[i].cQuantity, t);
        sub += t;
    }
    printf("CURRENT SUBTOTAL: $%.2f\n", sub);
    wait();
}

void checkoutBill() {
    if (nCartItems == 0) { wait(); return; }
    float subTotal = 0;
    for (int i = 0; i < nCartItems; i++) subTotal += (myCart[i].cPrice * myCart[i].cQuantity);
    clrscr();
    header("BILLING CHECKOUT");
    printf("Subtotal: $%.2f\n", subTotal);
    int mid;
    float discPercent = 0;
    printf("Member ID (0 if none): ");
    scanf("%d", &mid);
    int mi = -1;
    if (mid > 0) {
        for (int i = 0; i < nMembers; i++) if (shopMembers[i].mId == mid) { mi = i; break; }
    }
    if (mi != -1) {
        printf("Member: %s (%s)\n", shopMembers[mi].mName, shopMembers[mi].mTier);
        discPercent = shopMembers[mi].mDisc;
        printf("Discount: %.1f%%\n", discPercent);
    }
    float finalTotal = subTotal - (subTotal * (discPercent / 100.0f));
    finalTotal = applyVoucher(finalTotal);
    float taxVal = finalTotal * 0.05f;
    float totalWithTax = finalTotal + taxVal;
    printf("\nGRAND TOTAL: $%.2f\n", totalWithTax);
    int pm;
    printf("1. Cash, 2. Card: ");
    scanf("%d", &pm);
    if (nSales < MAX_RECORDS) {
        salesLog[nSales].sId = 1000 + nSales;
        salesLog[nSales].sTime = getSystemTime();
        salesLog[nSales].sSub = subTotal;
        salesLog[nSales].sTax = taxVal;
        salesLog[nSales].sDisc = subTotal - finalTotal;
        salesLog[nSales].sTotal = totalWithTax;
        strcpy(salesLog[nSales].sPayMode, (pm == 1 ? "Cash" : "Card"));
        nSales++;
    }
    if (mi != -1) {
        shopMembers[mi].mSpent += totalWithTax;
        if (shopMembers[mi].mSpent > 5000) { strcpy(shopMembers[mi].mTier, "Platinum"); shopMembers[mi].mDisc = 15.0; }
        else if (shopMembers[mi].mSpent > 2000) { strcpy(shopMembers[mi].mTier, "Gold"); shopMembers[mi].mDisc = 10.0; }
        else if (shopMembers[mi].mSpent > 500) { strcpy(shopMembers[mi].mTier, "Silver"); shopMembers[mi].mDisc = 5.0; }
    }
    nCartItems = 0;
    printf("\nPayment complete. Thank you!\n");
    wait();
}

float applyVoucher(float amt) {
    char code[15];
    printf("Voucher code ('none' to skip): ");
    scanf("%s", code);
    if (strcmp(code, "none") == 0) return amt;
    int ci = -1;
    for (int i = 0; i < nCoupons; i++) {
        if (strcmp(shopCoupons[i].coCode, code) == 0 && shopCoupons[i].coActive == 1) { ci = i; break; }
    }
    if (ci != -1) {
        float saving = (shopCoupons[ci].coType == 1 ? amt * (shopCoupons[ci].coValue / 100.0f) : shopCoupons[ci].coValue);
        if (saving > amt) saving = amt;
        printf(GREEN "Discount Applied: $%.2f\n" RESET, saving);
        return amt - saving;
    }
    printf(RED "Invalid voucher.\n" RESET);
    return amt;
}

void authLogin() {
    char u[30], p[30];
    clrscr();
    header("LOGIN");
    printf("User: "); scanf("%s", u);
    printf("Pass: "); scanf("%s", p);
    if (strcmp(u, "admin") == 0 && strcmp(p, adminMasterPass) == 0) { adminDashboard(); return; }
    int si = -1;
    for (int i = 0; i < nStaff; i++) {
        if (strcmp(staffList[i].stUser, u) == 0 && strcmp(staffList[i].stPass, p) == 0) { si = i; break; }
    }
    if (si != -1) {
        if (staffList[si].stRole == 1) adminDashboard();
        else {
            int op;
            while(1) {
                clrscr(); header("STAFF");
                printf("1. Inv, 2. Member, 3. Shop, 0. Logout: ");
                scanf("%d", &op);
                if (op == 1) listItems();
                else if (op == 2) listCustomers();
                else if (op == 3) customerDashboard();
                else if (op == 0) break;
            }
        }
    } else { printf(RED "Login failed.\n" RESET); wait(); }
}

void adminDashboard() {
    int cmd;
    while (1) {
        clrscr();
        header("ADMIN");
        printf("1. Inventory, 2. Members, 3. Sales, 4. Staff, 0. Logout: ");
        scanf("%d", &cmd);
        if (cmd == 1) {
            int pcmd;
            while(1) {
                clrscr(); header("INV");
                printf("1. Add, 2. Del, 3. View, 0. Back: ");
                scanf("%d", &pcmd);
                if (pcmd == 1) addItems();
                else if (pcmd == 2) deleteItems();
                else if (pcmd == 3) listItems();
                else if (pcmd == 0) break;
            }
        }
        else if (cmd == 2) listCustomers();
        else if (cmd == 3) showReport();
        else if (cmd == 4) manageTeam();
        else if (cmd == 0) break;
    }
}

void addItems() {
    int count;
    printf("How many products: "); scanf("%d", &count);
    for (int i = 0; i < count; i++) {
        if (nProducts >= MAX_ITEMS) break;
        printf("Code: "); scanf("%d", &shopProducts[nProducts].pCode);
        printf("Name: "); scanf(" %[^\n]", shopProducts[nProducts].pName);
        printf("Price: "); scanf("%f", &shopProducts[nProducts].pPrice);
        printf("Stock: "); scanf("%d", &shopProducts[nProducts].pStock);
        printf("Cat: "); scanf("%s", shopProducts[nProducts].pCategory);
        nProducts++;
    }
    saveProducts();
    wait();
}

void deleteItems() {
    int code;
    printf("Enter code to delete: "); scanf("%d", &code);
    int idx = -1;
    for (int i = 0; i < nProducts; i++) if (shopProducts[i].pCode == code) { idx = i; break; }
    if (idx != -1) {
        for (int i = idx; i < nProducts - 1; i++) shopProducts[i] = shopProducts[i+1];
        nProducts--;
        saveProducts();
    }
    wait();
}

void listCustomers() {
    clrscr(); header("MEMBERS");
    for (int i = 0; i < nMembers; i++) 
        printf("%d | %s | $%.2f | %s\n", shopMembers[i].mId, shopMembers[i].mName, shopMembers[i].mSpent, shopMembers[i].mTier);
    wait();
}

void showReport() {
    clrscr(); header("REPORT");
    float rev = 0;
    for (int i = 0; i < nSales; i++) {
        printf("TX %d | $%.2f\n", salesLog[i].sId, salesLog[i].sTotal);
        rev += salesLog[i].sTotal;
    }
    printf("Total Volume: %d | Total Rev: $%.2f\n", nSales, rev);
    wait();
}

void manageTeam() {
    int op;
    while(1) {
        clrscr(); header("TEAM");
        printf("1. Add, 2. List, 0. Back: "); scanf("%d", &op);
        if (op == 1) {
            staffList[nStaff].stId = 500 + nStaff;
            printf("User: "); scanf("%s", staffList[nStaff].stUser);
            printf("Pass: "); scanf("%s", staffList[nStaff].stPass);
            printf("Role (1:Admin, 0:Staff): "); scanf("%d", &staffList[nStaff].stRole);
            nStaff++; saveStaff();
        } else if (op == 2) {
            for (int j = 0; j < nStaff; j++) printf("%s | %s\n", staffList[j].stUser, (staffList[j].stRole == 1 ? "Admin" : "Staff"));
            wait();
        } else if (op == 0) break;
    }
}

void loadCoupons() {
    FILE* f = fopen(COUPON_DB, "r"); if (f == NULL) return; nCoupons = 0;
    while(nCoupons < MAX_COUPONS && fscanf(f, "%[^|]|%f|%d|%d\n", shopCoupons[nCoupons].coCode, &shopCoupons[nCoupons].coValue, &shopCoupons[nCoupons].coType, &shopCoupons[nCoupons].coActive) == 4) nCoupons++;
    fclose(f);
}

void saveCoupons() {
    FILE* f = fopen(COUPON_DB, "w"); if (f == NULL) return;
    for(int i=0; i<nCoupons; i++) fprintf(f, "%s|%.2f|%d|%d\n", shopCoupons[i].coCode, shopCoupons[i].coValue, shopCoupons[i].coType, shopCoupons[i].coActive);
    fclose(f);
}

void loadSales() {
    FILE* f = fopen(SALES_DB, "r"); if (f == NULL) return; nSales = 0;
    while(nSales < MAX_RECORDS && fscanf(f, "%d|%d-%d-%d|%d:%d:%d|%f|%f|%f|%f|%s\n", &salesLog[nSales].sId, &salesLog[nSales].sTime.d, &salesLog[nSales].sTime.m, &salesLog[nSales].sTime.y, &salesLog[nSales].sTime.h, &salesLog[nSales].sTime.min, &salesLog[nSales].sTime.s, &salesLog[nSales].sSub, &salesLog[nSales].sTax, &salesLog[nSales].sDisc, &salesLog[nSales].sTotal, salesLog[nSales].sPayMode) == 12) nSales++;
    fclose(f);
}

void saveSales() {
    FILE* f = fopen(SALES_DB, "w"); if (f == NULL) return;
    for(int i=0; i<nSales; i++) fprintf(f, "%d|%02d-%02d-%d|%02d:%02d:%02d|%.2f|%.2f|%.2f|%.2f|%s\n", salesLog[i].sId, salesLog[i].sTime.d, salesLog[i].sTime.m, salesLog[i].sTime.y, salesLog[i].sTime.h, salesLog[i].sTime.min, salesLog[i].sTime.s, salesLog[i].sSub, salesLog[i].sTax, salesLog[i].sDisc, salesLog[i].sTotal, salesLog[i].sPayMode);
    fclose(f);
}

void loadStaff() {
    FILE* f = fopen(STAFF_DB, "r"); if (f == NULL) return; nStaff = 0;
    while(nStaff < MAX_STAFF && fscanf(f, "%d|%[^|]|%[^|]|%d\n", &staffList[nStaff].stId, staffList[nStaff].stUser, staffList[nStaff].stPass, &staffList[nStaff].stRole) == 4) nStaff++;
    fclose(f);
}

void saveStaff() {
    FILE* f = fopen(STAFF_DB, "w"); if (f == NULL) return;
    for(int i=0; i<nStaff; i++) fprintf(f, "%d|%s|%s|%d\n", staffList[i].stId, staffList[i].stUser, staffList[i].stPass, staffList[i].stRole);
    fclose(f);
}
