/*
 ============================================================
  AYH MART - SELF CHECKOUT SYSTEM
  Developed By: Aum, Yash, HariKrushna
  Location: Surat, India
 ============================================================
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>       /* ★ CREATIVE: for date/time in receipt */

#define MAX_ITEMS      100
#define MAX_MEMBERS    100
#define MAX_CART        50
#define MAX_CUSTOMERS  100
#define MAX_SALES 1000
#define MAX_CATEGORIES  30

#define LOW_STOCK_THRESHOLD  10   /* ★ CREATIVE: warn when stock <= this */

#define FILE_PRODUCTS   "ayhmart_products.dat"
#define FILE_MEMBERS    "ayhmart_members.dat"
#define FILE_CUSTOMERS  "ayhmart_customers.dat"
#define FILE_SALES      "ayhmart_sales.dat"
#define FILE_COUNTERS   "ayhmart_counters.dat"
#define FILE_CATEGORIES "ayhmart_categories.dat"

/* ── credentials ── */
char ad_user[10] = "admin";
char ad_pass[10] = "admin";
char sf_user[10] = "staff";
char sf_pass[10] = "staff";

/* ★ CREATIVE: Windows console handle for color output */
HANDLE hConsole;

/* ══════════════════ FORWARD DECLARATIONS ══════════════════ */
void mainManu();
int isCustomerIdExists(int id);
int isProductCodeExists(int code);
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
/* ── Category forward declarations ── */
void manageCategories();
void addCategory();
void deleteCategory();
void viewCategories();
int  isCategoryExists(const char *name);
int  selectCategory(char *outCategory);
void saveCategories();
void loadCategories();
/* ── Customer account forward declarations ── */
void custManage();
void addCustomer();
void deleteCustomer();
void viewCustomers();
int  customerLogin();
void viewMyCard();
/* ── File handling forward declarations ── */
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
/* ★ CREATIVE: forward declarations for new helpers */
void setColor(int c);
void resetColor();
void typeWrite(const char *msg, int delayMs);
void splash();
void printReceipt(float subtotal, float discount, float finalAmt, int billNo, int payType);
void randomThankYou();

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
    int   custId;
};
struct Sale sales[MAX_SALES];
int nSales = 0;
float totalRevenue  = 0;
float totalDiscount = 0;
int totalBills = 0;

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

/* ── Category structure ── */
char categories[MAX_CATEGORIES][30];
int  nCategories = 0;

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

/* ★ CREATIVE: Color helper — Windows console colors
   Common codes: 7=white(default), 10=green, 12=red,
                 14=yellow, 11=cyan, 13=magenta, 9=blue */
void setColor(int c) {
    SetConsoleTextAttribute(hConsole, c);
}
void resetColor() {
    SetConsoleTextAttribute(hConsole, 7);
}

/* ★ CREATIVE: Typewriter effect — prints each char with a small delay */
void typeWrite(const char *msg, int delayMs) {
    int i;
    for (i = 0; msg[i]; i++) {
        putchar(msg[i]);
        fflush(stdout);
        Sleep(delayMs);
    }
    putchar('\n');
}

/* ★ CREATIVE: Splash screen with ASCII logo + animated boot sequence */
void splash() {
    system("cls");
    setColor(11); /* cyan */
    printf("\n\n");
    printf("    ___   __ ____  ___    __  ___   ___  ____  ____\n");
    printf("   / _ | / // / | / _ |  /  |/  /  / _ |/ __//_  /\n");
    printf("  / __ |/ // /| |/ __ | / /|_/ /  / __ / __/  / / \n");
    printf(" /_/ |_|\\_, /|___/_/ |_|/_/  /_/  /_/ |_/___/ /_/  \n");
    printf("        /___/                                        \n");
    printf("\n");
    resetColor();
    setColor(14); /* yellow */
    printf("            Self-Checkout System  |  Surat, India\n");
    printf("            Developed by: Aum, Yesh, HariKrushna\n");
    resetColor();
    printf("\n");
    printDash();
    typeWrite("  Initializing AYH Mart System...", 28);
    Sleep(250);
    typeWrite("  Loading inventory data...", 22);
    Sleep(200);
    typeWrite("  Loading member records...", 22);
    Sleep(200);
    typeWrite("  Loading customer accounts...", 22);
    Sleep(200);
    setColor(10); /* green */
    typeWrite("  System ready! Welcome to AYH Mart.", 20);
    resetColor();
    Sleep(400);
    printf("\n");
    system("pause");
}

/* ★ CREATIVE: Random farewell message at end of billing */
void randomThankYou() {
    const char *msgs[] = {
        "  Have a wonderful day ahead!",
        "  See you again soon — we'll miss you!",
        "  Happy shopping! Come back anytime.",
        "  You made great choices today!",
        "  Stay safe and see you next time!"
    };
    srand((unsigned int)time(NULL));
    int idx = rand() % 5;
    setColor(13); /* magenta */
    printf("%s\n", msgs[idx]);
    resetColor();
}

/* ★ CREATIVE: Formatted receipt — call BEFORE cart is cleared */
void printReceipt(float subtotal, float discount, float finalAmt, int billNo, int payType) {
    int i;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char dateStr[30];
    strftime(dateStr, sizeof(dateStr), "%d-%m-%Y  %H:%M:%S", tm_info);

    printf("\n");
    setColor(11); /* cyan border */
    printf("  +--------------------------------------------------+\n");
    printf("  |              *** AYH MART ***                    |\n");
    printf("  |              Surat, India                        |\n");
    printf("  +--------------------------------------------------+\n");
    resetColor();

    printf("  Date     : %s\n", dateStr);
    printf("  Bill No  : #AYH%04d\n", 1000 + billNo);
    if (currentCustIdx >= 0)
        printf("  Customer : %s\n", customers[currentCustIdx].cName);
    printf("  Payment  : %s\n", payType == 2 ? "QR / Digital" : "Cash");
    printDash();

    printf("  %-22s  %-5s  %-8s  %s\n", "ITEM", "QTY", "RATE", "AMOUNT");
    printDash();

    for (i = 0; i < cartCount; i++) {
        printf("  %-22s  %-5d  %-8.2f  %.2f\n",
               cart[i].name, cart[i].qty,
               cart[i].price, cart[i].price * cart[i].qty);
    }
    printDash();

    printf("  %-38s  %.2f\n", "Subtotal :", subtotal);

    if (discount > 0) {
        setColor(10); /* green for discount line */
        printf("  %-38s -%.2f\n", "Membership Discount :", discount);
        resetColor();
    }

    setColor(14); /* yellow for total */
    printf("  %-38s  %.2f\n", "TOTAL PAYABLE :", finalAmt);
    resetColor();

    setColor(11);
    printf("  +--------------------------------------------------+\n");
    resetColor();
    printf("  Thank you for shopping at AYH Mart!\n");
    printLine();
}

/* ══════════════════ MAIN ══════════════════ */
int main() {
    /* ★ CREATIVE: grab console handle for color output */
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	/* ── Default Products ── */
    SetConsoleOutputCP(CP_UTF8);
    shop[0] = (struct Product){101, "Rice",     45.00,  80, "Grocery"};
    shop[1] = (struct Product){102, "Milk",     25.00, 120, "Dairy"};
    shop[2] = (struct Product){103, "Bread",    30.00,  60, "Bakery"};
    shop[3] = (struct Product){104, "Shampoo",  90.00,  40, "Personal"};
    shop[4] = (struct Product){105, "PenDrive", 350.00, 15, "Electronics"};
    nProducts = 5;

    /* ── Default categories ── */
    strcpy(categories[0], "Grocery");
    strcpy(categories[1], "Dairy");
    strcpy(categories[2], "Bakery");
    strcpy(categories[3], "Personal");
    strcpy(categories[4], "Electronics");
    nCategories = 5;

	/* ── Default customers ── */
	customers[0] = (struct Customer){1, "aum",  "123", 0};
	customers[1] = (struct Customer){2, "hari", "123", 0};
	customers[2] = (struct Customer){3, "yash", "123", 0};
	nCustomers = 3;

    loadAllData();

    splash();   /* ★ CREATIVE: show splash after data is loaded */

    mainManu();
    return 0;
}

/* ══════════════════ MAIN MENU ══════════════════ */
void mainManu() {
    int choice;
    while (1) {
        system("cls");
        /* ★ CREATIVE: cyan header line */
        setColor(11);
        printHeader("AYH MART  |  Surat, India");
        resetColor();
        printf("  1. Customer Self-Checkout\n");
        printf("  2. Staff / Admin Login\n");
        printf("  3. View Store Info\n");
        printf("  0. Exit Application\n");
        printDash();
        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
				cust();
				break;
            case 2:
				login();
				break;
            case 3:
				info();
				break;
            case 0:
                setColor(11);
                printHeader("Thank you for visiting AYH Mart!");
                resetColor();
                exit(0);
            default:
                setColor(12); /* ★ CREATIVE: red for errors */
                printf("  [!] Invalid option. Try again.\n");
                resetColor();
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
    printf("  Username: ");
	scanf("%s", username);
    printf("  Password: ");
	inputPassword(password);

    if (strcmp(username, ad_user) == 0 && strcmp(password, ad_pass) == 0) {
        setColor(10); /* ★ CREATIVE: green for success */
        printf("\n  [✓] Logged in as ADMIN\n");
        resetColor();
        system("pause");
        adminDashboard();
    } else if (strcmp(username, sf_user) == 0 && strcmp(password, sf_pass) == 0) {
        setColor(10);
        printf("\n  [✓] Logged in as STAFF\n");
        resetColor();
        system("pause");
        staff();
    } else {
        setColor(12); /* ★ CREATIVE: red for failure */
        printf("\n  [✗] Incorrect username or password.\n");
        resetColor();
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
    printf("  2. Customer Accounts\n");
    printf("  3. Sales Report\n");
    printf("  4. Members Ship\n");
    printf("  5. Backup & Restore Data\n");
    printf("  0. Logout\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
			inventory();
			break;
        case 2:
			custManage();
			break;
        case 3:
			salesManu();
			break;
        case 4:
			members();
			break;
        case 5:
			backupRestore();
			break;
        case 0:
            setColor(10);
            printf("\n  [✓] Logged out successfully.\n\n");
            resetColor();
            system("pause");
            mainManu();
            break;
        default:
            setColor(12);
            printf("  [!] Invalid option.\n");
            resetColor();
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
    printf("  5. Manage Categories\n");
    printf("  0. Back\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
			add();
			break;
        case 2:
			editProduct();
			break;
        case 3:
			del();
			break;
        case 4:
			view();
			break;
        case 5:
            manageCategories();
            break;
        case 0:
			adminDashboard();
			break;
        default:
            setColor(12);
            printf("  [!] Invalid option.\n");
            resetColor();
            system("pause");
            inventory();
    }
}

/* ══════════════════ CATEGORY MANAGEMENT ══════════════════ */

/* ── Check if category name already exists ── */
int isCategoryExists(const char *name) {
    for (int i = 0; i < nCategories; i++) {
        if (strcasecmp(categories[i], name) == 0)
            return 1;
    }
    return 0;
}

/* ── Display numbered category list and let user pick one ── */
int selectCategory(char *outCategory) {
    if (nCategories == 0) {
        setColor(12);
        printf("  [!] No categories available. Please ask admin to add categories first.\n");
        resetColor();
        return 0;
    }
    printf("\n  ── Available Categories ──\n");
    for (int i = 0; i < nCategories; i++) {
        printf("  %2d. %s\n", i + 1, categories[i]);
    }
    printf("   0. Cancel\n");
    printf("  Select category number: ");
    int sel;
    scanf("%d", &sel);
    if (sel < 1 || sel > nCategories) {
        printf("  [-] Category selection cancelled.\n");
        return 0;
    }
    strcpy(outCategory, categories[sel - 1]);
    setColor(10);
    printf("  [✓] Category set to: %s\n", outCategory);
    resetColor();
    return 1;
}

/* ── Manage Categories Menu ── */
void manageCategories() {
    int choice;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("MANAGE CATEGORIES");
    printf("  1. Add Category\n");
    printf("  2. Delete Category\n");
    printf("  3. View All Categories\n");
    printf("  0. Back to Inventory\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: addCategory();    break;
        case 2: deleteCategory(); break;
        case 3: viewCategories(); break;
        case 0: inventory();      break;
        default:
            setColor(12);
            printf("  [!] Invalid option.\n");
            resetColor();
            system("pause");
            manageCategories();
    }
}

/* ── Add Category ── */
void addCategory() {
    int count, i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("ADD CATEGORIES");

    printf("  How many categories to add: ");
    scanf("%d", &count);

    for (i = 0; i < count; i++) {
        if (nCategories >= MAX_CATEGORIES) {
            setColor(12);
            printf("  [!] Category limit reached (%d max).\n", MAX_CATEGORIES);
            resetColor();
            break;
        }
        char catName[30];
        printf("  Category %d name: ", i + 1);
        scanf(" %29s", catName);

        if (isCategoryExists(catName)) {
            setColor(14);
            printf("  [!] Category '%s' already exists. Skipping.\n", catName);
            resetColor();
            i--;
            continue;
        }
        strcpy(categories[nCategories], catName);
        nCategories++;
        setColor(10);
        printf("  [✓] Category '%s' added.\n", catName);
        resetColor();
    }
    saveCategories();
    system("pause");
    manageCategories();
}

/* ── Delete Category ── */
void deleteCategory() {
    int i, j;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("DELETE CATEGORY");

    if (nCategories == 0) {
        printf("  No categories to delete.\n");
        system("pause");
        manageCategories();
        return;
    }

    printf("  %-4s  %s\n", "NO.", "CATEGORY");
    printDash();
    for (i = 0; i < nCategories; i++) {
        printf("  %-4d  %s\n", i + 1, categories[i]);
    }
    printDash();

    int sel;
    printf("  Enter category number to delete (0 to cancel): ");
    scanf("%d", &sel);

    if (sel < 1 || sel > nCategories) {
        printf("  [-] Cancelled.\n");
        system("pause");
        manageCategories();
        return;
    }

    /* ── Check if any product uses this category ── */
    char *target = categories[sel - 1];
    int inUse = 0;
    for (i = 0; i < nProducts; i++) {
        if (strcasecmp(shop[i].pCategory, target) == 0) {
            inUse = 1;
            break;
        }
    }
    if (inUse) {
        setColor(12);
        printf("  [!] Cannot delete '%s' — it is assigned to one or more products.\n", target);
        printf("      Re-assign those products first.\n");
        resetColor();
        system("pause");
        manageCategories();
        return;
    }

    printf("  Delete category '%s'? (yes/no): ", target);
    char confirm[5];
    scanf(" %4s", confirm);
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) {
        for (j = sel - 1; j < nCategories - 1; j++) {
            strcpy(categories[j], categories[j + 1]);
        }
        nCategories--;
        saveCategories();
        setColor(10);
        printf("  [✓] Category deleted.\n");
        resetColor();
    } else {
        printf("  [-] Cancelled.\n");
    }
    system("pause");
    manageCategories();
}

/* ── View Categories ── */
void viewCategories() {
    int i;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("ALL CATEGORIES");

    if (nCategories == 0) {
        printf("  No categories defined yet.\n");
    } else {
        printf("  %-4s  %s\n", "NO.", "CATEGORY NAME");
        printDash();
        for (i = 0; i < nCategories; i++) {
            printf("  %-4d  %s\n", i + 1, categories[i]);
        }
    }
    printf("\n");
    system("pause");
    manageCategories();
}

/* ── Save Categories ── */
void saveCategories() {
    FILE *fp = fopen(FILE_CATEGORIES, "wb");
    if (!fp) {
        setColor(12);
        printf("  [✗] Error: Could not save categories.\n");
        resetColor();
        return;
    }
    fwrite(&nCategories, sizeof(int), 1, fp);
    fwrite(categories, sizeof(categories[0]), nCategories, fp);
    fclose(fp);
}

/* ── Load Categories ── */
void loadCategories() {
    FILE *fp = fopen(FILE_CATEGORIES, "rb");
    if (!fp) return;
    fread(&nCategories, sizeof(int), 1, fp);
    fread(categories, sizeof(categories[0]), nCategories, fp);
    fclose(fp);
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
            setColor(12);
            printf("  [!] Product limit reached (%d max).\n", MAX_ITEMS);
            resetColor();
            system("pause");
            inventory();
            return;
        }
        printf("\n  -- Product %d --\n", i + 1);
        int code;
        while (1) {
            printf("  Code    : ");
            scanf("%d", &code);
            if (isProductCodeExists(code)) {
                setColor(14);
                printf("  [!] Product code already exists. Enter different code.\n");
                resetColor();
            } else {
                shop[nProducts].pCode = code;
                break;
            }
        }
        printf("  Name    : ");
		scanf(" %s", shop[nProducts].pName);
        printf("  Price   : ");
		scanf("%f", &shop[nProducts].pPrice);
        printf("  Stock   : ");
		scanf("%d", &shop[nProducts].pStock);
        if (!selectCategory(shop[nProducts].pCategory)) {
            setColor(12);
            printf("  [!] Product adding cancelled (no category selected).\n");
            resetColor();
            system("pause");
            inventory();
            return;
        }
        nProducts++;
    }
    setColor(10);
    printf("\n  [✓] %d product(s) added successfully.\n", count);
    resetColor();
    saveProducts();
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
            if (strcmp(buf, "0") != 0) {
				strcpy(shop[i].pName, buf);
			}

            printf("  New Price [%.2f]: ", shop[i].pPrice);
            scanf("%f", &fval);
            if (fval > 0){
				shop[i].pPrice = fval;
			}

            printf("  New Stock [%d]: ", shop[i].pStock);
            scanf("%d", &ival);
            if (ival >= 0){
				shop[i].pStock = ival;
			}

            printf("  New Category [%s]: ", shop[i].pCategory);
            printf("\n  Press ENTER to keep current, or select a new one:\n");
            char keepCat[5];
            printf("  Change category? (yes/no): ");
            scanf(" %4s", keepCat);
            if (strcmp(keepCat, "yes") == 0 || strcmp(keepCat, "y") == 0) {
                char newCat[30];
                if (selectCategory(newCat)) {
                    strcpy(shop[i].pCategory, newCat);
                }
            }

            setColor(10);
            printf("\n  [✓] Product updated successfully.\n");
            resetColor();
            saveProducts();
            break;
        }
    }
    if (!found){
        setColor(12);
        printf("  [✗] Product code not found.\n");
        resetColor();
    }
    system("pause");
    inventory();
}

/* ── View products ── */
/* ★ CREATIVE: low stock items highlighted in yellow, out-of-stock in red */
void view() {
    int i;
    system("cls");
    printHeader("AVAILABLE PRODUCTS");

    /* ★ CREATIVE: legend for stock colors */
    setColor(14);
    printf("  [*] Yellow = Low Stock (<= %d)  ", LOW_STOCK_THRESHOLD);
    setColor(12);
    printf("[*] Red = Out of Stock\n");
    resetColor();

    if (nProducts == 0) {
        printf("  No products in inventory.\n");
    } else {
        printf("  %-6s  %-18s  %-8s  %-6s  %s\n", "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
        printDash();
        for (i = 0; i < nProducts; i++) {
            /* ★ CREATIVE: color rows based on stock level */
            if (shop[i].pStock == 0) {
                setColor(12); /* red */
            } else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) {
                setColor(14); /* yellow */
            }
            printf("  %-6d  %-18s  %-8.2f  %-6d  %s",
                   shop[i].pCode, shop[i].pName, shop[i].pPrice,
                   shop[i].pStock, shop[i].pCategory);
            if (shop[i].pStock == 0)
                printf("  [OUT OF STOCK]");
            else if (shop[i].pStock <= LOW_STOCK_THRESHOLD)
                printf("  [LOW STOCK]");
            printf("\n");
            resetColor();
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
            for (j = i; j < nProducts - 1; j++){
				shop[j] = shop[j + 1];
			}
            nProducts--;
            found = 1;
            setColor(10);
            printf("  [✓] Product deleted successfully.\n");
            resetColor();
            saveProducts();
            break;
        }
    }
    if (!found){
        setColor(12);
        printf("  [✗] Product code not found.\n");
        resetColor();
    }
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
    printf("  Customer ID : ");
	scanf("%d", &id);
    printf("  Password    : ");
	inputPassword(pass);

    for (i = 0; i < nCustomers; i++) {
        if (customers[i].cId == id && strcmp(customers[i].cPass, pass) == 0) {
            setColor(10);
            printf("\n  [✓] Welcome, %s!\n", customers[i].cName);
            resetColor();
            system("pause");
            custLoggedIn   = 1;
            currentCustIdx = i;
            return i;
        }
    }
    setColor(12);
    printf("\n  [✗] Invalid Customer ID or Password.\n");
    resetColor();
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

    /* ★ CREATIVE: cart badge — show item count next to basket option */
    setColor(11);
    printf("  Cart: %d item(s) in basket\n", cartCount);
    resetColor();

    printDash();
    printf("  1. View Store Products\n");
    printf("  2. Search for a Product\n");
    printf("  3. Add Item to Basket\n");

    /* ★ CREATIVE: highlight basket option in cyan if items are in cart */
    if (cartCount > 0) {
        setColor(11);
        printf("  4. View My Basket  [%d item(s)]\n", cartCount);
        resetColor();
    } else {
        printf("  4. View My Basket\n");
    }

    printf("  5. Final Checkout\n");
    printf("  6. My Membership Card\n");
    printf("  0. Logout & Return to Main Menu\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
			custView();
			break;
        case 2:
			search();
			break;
        case 3:
            system("cls");
            printHeader("ADD ITEM TO BASKET");
            if (nProducts == 0) {
                printf("  No products available.\n");
            } else {
                printf("  %-6s  %-18s  %-8s  %-6s  %s\n", "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
                printDash();
                for (i = 0; i < nProducts; i++){
                    /* ★ CREATIVE: stock color in add-to-basket view too */
                    if (shop[i].pStock == 0) setColor(12);
                    else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) setColor(14);
                    printf("  %-6d  %-18s  %-8.2f  %-6d  %s\n",
                           shop[i].pCode, shop[i].pName, shop[i].pPrice,
                           shop[i].pStock, shop[i].pCategory);
                    resetColor();
                }
            }
            addCart();
            break;
        case 4:
			viewCart();
			break;
        case 5:
			checkout();
			break;
        case 6:
			viewMyCard();
			break;
        case 0:
            custLoggedIn   = 0;
            currentCustIdx = -1;
            mainManu();
            break;
        default:
            setColor(12);
            printf("  [!] Invalid option.\n");
            resetColor();
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
            /* ★ CREATIVE: color stock in customer view */
            if (shop[i].pStock == 0) setColor(12);
            else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) setColor(14);
            printf("  %-6d  %-18s  %-8.2f  %-6d  %s\n",
                   shop[i].pCode, shop[i].pName, shop[i].pPrice,
                   shop[i].pStock, shop[i].pCategory);
            resetColor();
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
            setColor(10);
            printf("  Found:\n");
            resetColor();
            printf("    Product Name     : %s\n",   shop[i].pName);
            printf("    Product Price    : %.2f\n", shop[i].pPrice);

            /* ★ CREATIVE: color stock value based on level */
            printf("    Product Stock    : ");
            if (shop[i].pStock == 0) setColor(12);
            else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) setColor(14);
            printf("%d", shop[i].pStock);
            resetColor();
            if (shop[i].pStock == 0) printf("  [OUT OF STOCK]");
            else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) printf("  [LOW STOCK]");
            printf("\n");

            printf("    Product Category : %s\n",   shop[i].pCategory);
            printDash();
            found = 1;
            break;
        }
    }
    if (!found){
        setColor(12);
        printf("  [✗] Product not found.\n");
        resetColor();
    }
    printf("\n");
    system("pause");
    cust();
}

/* ── Add to cart ── */
void addCart() {
    int  code, qty, found = 0, i;
    char confirm[4];

    if (cartCount >= MAX_CART) {
        setColor(12);
        printf("  [!] Cart is full!\n");
        resetColor();
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
                setColor(12);
                printf("  [!] Insufficient stock (available: %d).\n", shop[i].pStock);
                resetColor();
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
                saveProducts();
                cartCount++;
                setColor(10);
                printf("  [✓] Item added to basket.\n");
                resetColor();
            } else {
                printf("  [-] Item not added.\n");
            }
            found = 1;
            break;
        }
    }
    if (!found){
        setColor(12);
        printf("  [✗] Product unavailable.\n");
        resetColor();
    }
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
        setColor(14); /* ★ CREATIVE: yellow for grand total */
        printf("  %-40s  %.2f\n", "Grand Total:", total);
        resetColor();
    }
    printf("\n");
    system("pause");
    cust();
}

/* ══════════════════ PRINT QR CODE ══════════════════ */
void printQRCode() {
    /* ★ CREATIVE: print QR in cyan for visual pop */
    setColor(11);
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
    resetColor();
}

/* ── Checkout ── */
void checkout() {
    int   mid, i, payChoice;
    float subtotal = 0, discount = 0, finalAmount;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    if (cartCount == 0) {
        setColor(12);
        printf("  [!] Basket is empty.\n");
        resetColor();
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
    if (discount > 0) {
        setColor(10);
        printf("  Discount        : %.2f\n", discount);
        resetColor();
    } else {
        printf("  Discount        : %.2f\n", discount);
    }
    setColor(14);
    printf("  Payable Amount  : %.2f\n", finalAmount);
    resetColor();
    printDash();

    printf("\n  SELECT PAYMENT METHOD:\n");
    printf("  1. Cash\n");
    setColor(11);
    printf("  2. QR Code (Digital)\n");
    resetColor();
    printf("  Enter choice: ");
    scanf("%d", &payChoice);

    for (i = 0; i < cartCount; i++) {
        recordSale(cart[i].code, cart[i].name, cart[i].qty, cart[i].price, customers[currentCustIdx].cId);
    }

    /* ★ CREATIVE: print receipt BEFORE cart is cleared so items are visible */
    totalBills++;
    printReceipt(subtotal, discount, finalAmount, totalBills, payChoice);
    printf("\n");
    system("pause");

    cartCount      = 0;
    custLoggedIn   = 0;
    currentCustIdx = -1;
    saveAllData();

    if (payChoice == 2) {
        printf("\n  [ SCAN QR CODE TO PAY — Dhameliya Yash Mukeshbhai ]\n\n");
        printQRCode();
        setColor(14);
        printf("\n  UPI ID : 6353999107@ptyes\n");
        printf("  Amount : Rs. %.2f\n\n", finalAmount);
        resetColor();
        totalQRPayments++;
        saveCounters();
        billing(finalAmount, 1);
    } else {
        totalCashPayments++;
        saveCounters();
        billing(finalAmount, 0);
    }
}

void showCustomerHistory(int custId) {
    int i, found = 0;
    float totalSpent = 0;

    printf("\n  ===== CUSTOMER PURCHASE HISTORY =====\n");
    printf("  %-6s  %-18s  %-5s  %s\n", "CODE", "NAME", "QTY", "TOTAL");
    printDash();

    for (i = 0; i < nSales; i++) {
        if (sales[i].custId == custId) {
            printf("  %-6d  %-18s  %-5d  %.2f\n",
                   sales[i].pCode,
                   sales[i].pName,
                   sales[i].qty,
                   sales[i].total);
            totalSpent += sales[i].total;
            found = 1;
        }
    }

    if (!found) {
        printf("  No purchase history found.\n");
    } else {
        printDash();
        setColor(14);
        printf("  Total Spent: %.2f\n", totalSpent);
        resetColor();
    }

    printDash();
}

/* ── Billing ── */
void billing(float finalAmount, int isQR) {
    if (isQR) {
        setColor(10);
        printLine();
        printf("  Transaction ID: AYH%d\n", 1000 + totalBills);
        printf("  Thank you for shopping at AYH Mart!\n");
        printLine();
        resetColor();
        randomThankYou(); /* ★ CREATIVE */
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
        setColor(10);
        printLine();
        printf("  [✓] Payment received! Change: %.2f\n", change);
        printf("  Thank you for shopping at AYH Mart!\n");
        printLine();
        resetColor();
        randomThankYou(); /* ★ CREATIVE */
        system("pause");
        mainManu();
    } else {
        setColor(12);
        printf("  [!] Insufficient. Need %.2f more.\n", finalAmount - payAmount);
        resetColor();
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
        case 1:
			addMembers();
			break;
        case 2:
			deleteMembers();
			break;
        case 3:
			viewMembers();
			break;
        case 0:
			adminDashboard();
			break;
        default:
            setColor(12);
            printf("  [!] Invalid option.\n");
            resetColor();
            system("pause");
            members();
    }
}

/* ── Add members ── */
void addMembers() {
    int count, i;

    system("cls");
    printHeader("AYH MART  |  Surat, India");

    printf("\n  ===== EXISTING CUSTOMER LIST =====\n");

    if (nCustomers == 0) {
        printf("  No customers available. Please create customer first.\n");
        system("pause");
        members();
        return;
    }

    printf("  %-6s  %-18s  %s\n", "ID", "NAME", "MEMBER ID");
    printDash();

    for (i = 0; i < nCustomers; i++) {
        if (customers[i].cMemberId == 0)
            printf("  %-6d  %-18s  None\n", customers[i].cId, customers[i].cName);
        else
            printf("  %-6d  %-18s  %d\n", customers[i].cId, customers[i].cName, customers[i].cMemberId);
    }

    printDash();

    printf("\n  How many members to add: ");
    scanf("%d", &count);

    for (i = 0; i < count; i++) {
        if (nMembers >= MAX_MEMBERS) {
            setColor(12);
            printf("  [!] Member limit reached.\n");
            resetColor();
            break;
        }

        int custId, custIndex = -1;
        char tier[20];

        printf("\n  -- Member %d --\n", i + 1);
        printf("  Enter Customer ID to assign membership: ");
        scanf("%d", &custId);

        for (int j = 0; j < nCustomers; j++) {
            if (customers[j].cId == custId) {
                custIndex = j;
                break;
            }
        }

        if (custIndex == -1) {
            setColor(12);
            printf("  [✗] Customer not found.\n");
            resetColor();
            i--;
            continue;
        } else {
            showCustomerHistory(customers[custIndex].cId);
            printf("\n  Continue to assign membership? (yes/no): ");
            char confirm[5];
            scanf("%s", confirm);
            if (strcmp(confirm, "yes") != 0 && strcmp(confirm, "y") != 0) {
                printf("  Skipped.\n");
                i--;
                continue;
            }
        }

        if (customers[custIndex].cMemberId != 0) {
            setColor(14);
            printf("  [!] This customer already has a membership.\n");
            resetColor();
            i--;
            continue;
        }

        Members[nMembers].mId = 1000 + nMembers;
        strcpy(Members[nMembers].mName, customers[custIndex].cName);

        printf("  Assigned to: %s\n", customers[custIndex].cName);
        setColor(11);
        printf("  Generated Member ID: %d\n", Members[nMembers].mId);
        resetColor();

        printf("  Amount Spent: ");
        scanf("%f", &Members[nMembers].mSpent);

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
                setColor(12);
                printf("  [!] Invalid tier. Try again.\n");
                resetColor();
            }
        }

        Members[nMembers].mUsed = 0;
        customers[custIndex].cMemberId = Members[nMembers].mId;
        nMembers++;
    }

    setColor(10);
    printf("\n  [✓] Member(s) added and linked successfully.\n");
    resetColor();
    saveMembers();
    saveCustomers();
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
            setColor(10);
            printf("  [✓] Member deleted.\n");
            resetColor();
            saveMembers();
            break;
        }
    }
    if (!found){
        setColor(12);
        printf("  [✗] Member ID not found.\n");
        resetColor();
    }
    system("pause");
    members();
}

/* ── View members ── */
/* ★ CREATIVE: tier names colored — Silver=white, Gold=yellow, Platinum=cyan */
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
            /* ★ CREATIVE: color by tier */
            if (strcmp(Members[i].mTier, "Platinum") == 0)      setColor(11); /* cyan */
            else if (strcmp(Members[i].mTier, "Gold") == 0)     setColor(14); /* yellow */
            else                                                  setColor(7);  /* white/default */
            printf("  %-5d  %-18s  %-10.2f  %-10s  %-9.0f%%  %s\n",
                   Members[i].mId,
                   Members[i].mName,
                   Members[i].mSpent,
                   Members[i].mTier,
                   Members[i].mDisc,
                   Members[i].mUsed ? "Used" : "Active");
            resetColor();
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
        case 1:
			addCustomer();
			break;
        case 2:
			deleteCustomer();
			break;
        case 3:
			viewCustomers();
			break;
        case 0:
			adminDashboard();
			break;
        default:
            setColor(12);
            printf("  [!] Invalid option.\n");
            resetColor();
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
            setColor(12);
            printf("  [!] Customer limit reached (%d max).\n", MAX_CUSTOMERS);
            resetColor();
            break;
        }
        printf("\n  -- Customer Account %d --\n", i + 1);
        int id;
        while (1) {
            printf("  Customer ID  : ");
            scanf("%d", &id);
            if (isCustomerIdExists(id)) {
                setColor(14);
                printf("  [!] Customer ID already exists. Try again.\n");
                resetColor();
            } else {
                customers[nCustomers].cId = id;
                break;
            }
        }
        printf("  Name         : ");
		scanf("%s", customers[nCustomers].cName);

        while (1) {
            printf("  Password     : ");
			inputPassword(customers[nCustomers].cPass);
			printf("\n");
            printf("  Confirm Pwd  : ");
			inputPassword(passConfirm);
            if (strcmp(customers[nCustomers].cPass, passConfirm) == 0) {
                break;
            } else {
                setColor(12);
                printf("  [!] Passwords do not match. Try again.\n");
                resetColor();
            }
        }
        printf("\n");
        printf("  Member ID    : (enter 0 if no membership) ");
        scanf("%d", &customers[nCustomers].cMemberId);

        nCustomers++;
    }
    setColor(10);
    printf("\n  [✓] Customer account(s) created successfully.\n");
    resetColor();
    saveCustomers();
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
            setColor(10);
            printf("  [✓] Customer account deleted.\n");
            resetColor();
            saveCustomers();
            break;
        }
    }
    if (!found) {
        setColor(12);
        printf("  [✗] Customer ID not found.\n");
        resetColor();
    }
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
                setColor(11); /* ★ CREATIVE: cyan for members */
                printf("  %-6d  %-18s  %-12s  %d\n",
                       customers[i].cId,
                       customers[i].cName,
                       customers[i].cPass,
                       customers[i].cMemberId);
                resetColor();
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
        setColor(14);
        printf("  [!] No membership card linked to your account.\n");
        printf("      Please contact Staff or Admin to get one assigned.\n");
        resetColor();
    } else {
        for (i = 0; i < nMembers; i++) {
            if (Members[i].mId == linkedId) {
                found = 1;

                /* ★ CREATIVE: card color by tier */
                if (strcmp(Members[i].mTier, "Platinum") == 0)      setColor(11);
                else if (strcmp(Members[i].mTier, "Gold") == 0)     setColor(14);
                else                                                  setColor(7);

                printf("  Member ID      : %d\n",       Members[i].mId);
                printf("  Member Name    : %s\n",       Members[i].mName);
                printf("  Tier           : %s\n",       Members[i].mTier);
                printf("  Discount Rate  : %.0f%%\n",   Members[i].mDisc);
                printf("  Total Spent    : Rs. %.2f\n", Members[i].mSpent);
                printf("  Card Status    : %s\n",
                       Members[i].mUsed ? "Used (discount already redeemed)"
                                        : "Active (discount available)");
                resetColor();
                break;
            }
        }
        if (!found) {
            setColor(12);
            printf("  [!] Linked Member ID (%d) not found in records.\n", linkedId);
            printf("      Please contact Staff or Admin.\n");
            resetColor();
        }
    }

    printDash();
    system("pause");
    cust();
}

/* ══════════════════ SALES ══════════════════ */
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

/* ── applyDiscount: one-time card restriction ── */
float applyDiscount(int memberId, float billAmount) {
    int i;
    if (memberId == 0) return 0;

    for (i = 0; i < nMembers; i++) {
        if (Members[i].mId == memberId) {
            if (Members[i].mUsed == 1) {
                setColor(14);
                printf("\n  [!] Membership card already used. No discount applied.\n");
                resetColor();
                return 0;
            }
            float discount = billAmount * (Members[i].mDisc / 100.0f);
            Members[i].mSpent += (billAmount - discount);
            totalDiscount     += discount;
            Members[i].mUsed   = 1;
            setColor(10);
            printf("  [✓] Membership discount applied (%.0f%%). Card is now marked as used.\n",
                   Members[i].mDisc);
            resetColor();
            return discount;
        }
    }

    setColor(14);
    printf("  [!] Member ID not found. No discount applied.\n");
    resetColor();
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
            case 1:
				salesSummary();
				break;
            case 2:
				productSales();
				break;
            case 3:
				discountReport();
				break;
            case 0:
				return;
            default:
                setColor(12);
                printf("  [!] Invalid option.\n");
                resetColor();
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
    setColor(11);
    printf("  -- QR  Payments       : %d\n",   totalQRPayments);
    resetColor();
    printf("  Total Items Sold      : %d\n",   nSales);
    setColor(10);
    printf("  Total Revenue         : %.2f\n", totalRevenue);
    resetColor();
    setColor(14);
    printf("  Total Discount Given  : %.2f\n", totalDiscount);
    resetColor();
    setColor(10);
    printf("  Net Revenue           : %.2f\n", totalRevenue - totalDiscount);
    resetColor();
    printDash();
    system("pause");
}

/* ── Staff Sales Summary ── */
void staffSalesSummary() {
    int i;
    int totalItemsSold = 0;
    float revenue = 0.0f;

    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("SALES SUMMARY");

    for (i = 0; i < nSales; i++) {
        totalItemsSold += sales[i].qty;
        revenue        += sales[i].total;
    }

    setColor(10);
    printf("  Total Items Sold  : %d\n",        totalItemsSold);
    printf("  Total Revenue     : Rs. %.2f\n",  revenue);
    resetColor();
    printDash();

    printf("\n  ITEMS SOLD:\n");
    if (nSales == 0) {
        printf("  No sales recorded yet.\n");
    } else {
        printf("\n  %-6s  %-18s  %-6s  %s\n", "CODE", "NAME", "QTY", "TOTAL");
        printDash();
        for (i = 0; i < nSales; i++)
            printf("  %-6d  %-18s  %-6d  Rs. %.2f\n",
                   sales[i].pCode, sales[i].pName,
                   sales[i].qty,  sales[i].total);
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
        for (i = 0; i < nMembers; i++) {
            /* ★ CREATIVE: tier color in discount report too */
            if (strcmp(Members[i].mTier, "Platinum") == 0)      setColor(11);
            else if (strcmp(Members[i].mTier, "Gold") == 0)     setColor(14);
            printf("  %-5d  %-18s  %-10.2f  %-10s  %-9.0f%%  %s\n",
                   Members[i].mId, Members[i].mName,
                   Members[i].mSpent, Members[i].mTier,
                   Members[i].mDisc,
                   Members[i].mUsed ? "Used" : "Active");
            resetColor();
        }
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

            /* ★ CREATIVE: stock legend for staff too */
            setColor(14);
            printf("  [*] Yellow = Low Stock (<= %d)  ", LOW_STOCK_THRESHOLD);
            setColor(12);
            printf("[*] Red = Out of Stock\n");
            resetColor();

            if (nProducts == 0) {
                printf("  No products.\n");
            } else {
                printf("  %-6s  %-18s  %-8s  %-6s  %s\n",
                       "CODE", "NAME", "PRICE", "STOCK", "CATEGORY");
                printDash();
                for (i = 0; i < nProducts; i++) {
                    if (shop[i].pStock == 0) setColor(12);
                    else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) setColor(14);
                    printf("  %-6d  %-18s  %-8.2f  %-6d  %s",
                           shop[i].pCode, shop[i].pName, shop[i].pPrice,
                           shop[i].pStock, shop[i].pCategory);
                    if (shop[i].pStock == 0) printf("  [OUT OF STOCK]");
                    else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) printf("  [LOW STOCK]");
                    printf("\n");
                    resetColor();
                }
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
                for (i = 0; i < nMembers; i++) {
                    if (strcmp(Members[i].mTier, "Platinum") == 0)  setColor(11);
                    else if (strcmp(Members[i].mTier, "Gold") == 0) setColor(14);
                    printf("  %-5d  %-18s  %-10.2f  %-10s  %-9.0f%%  %s\n",
                           Members[i].mId, Members[i].mName,
                           Members[i].mSpent, Members[i].mTier,
                           Members[i].mDisc,
                           Members[i].mUsed ? "Used" : "Active");
                    resetColor();
                }
            }
            printf("\n");
            system("pause");
            staff();
            break;
        }
        case 3:
			addMembers();
			break;
        case 4:
			deleteMembers();
			break;
        case 5:
            staffSalesSummary();
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
                    setColor(10);
                    printf("  Found:\n");
                    resetColor();
                    printf("    Product Name     : %s\n",   shop[i].pName);
                    printf("    Product Price    : %.2f\n", shop[i].pPrice);
                    printf("    Product Stock    : ");
                    if (shop[i].pStock == 0) setColor(12);
                    else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) setColor(14);
                    printf("%d", shop[i].pStock);
                    resetColor();
                    if (shop[i].pStock == 0) printf("  [OUT OF STOCK]");
                    else if (shop[i].pStock <= LOW_STOCK_THRESHOLD) printf("  [LOW STOCK]");
                    printf("\n");
                    printf("    Product Category : %s\n",   shop[i].pCategory);
                    printDash();
                    found = 1;
                    break;
                }
            }
            if (!found){
                setColor(12);
                printf("  [✗] Product not found.\n");
                resetColor();
            }
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
                case 1:
					addCustomer();
					break;
                case 2:
					deleteCustomer();
					break;
                case 3:
					viewCustomers();
					break;
                case 0:
					staff();
					break;
                default:
                    setColor(12);
                    printf("  [!] Invalid option.\n");
                    resetColor();
                    system("pause");
                    staff();
            }
            break;
        }
        case 0:
            setColor(10);
            printf("  [✓] Logged out.\n");
            resetColor();
            system("pause");
            mainManu();
            break;
        default:
            setColor(12);
            printf("  [!] Invalid option.\n");
            resetColor();
            system("pause");
            staff();
    }
}

/* ── Save Products ── */
void saveProducts() {
    FILE *fp = fopen(FILE_PRODUCTS, "wb");
    if (!fp) {
        setColor(12);
        printf("  [✗] Error: Could not open products file for writing.\n");
        resetColor();
        return;
    }
    fwrite(&nProducts, sizeof(int), 1, fp);
    fwrite(shop, sizeof(struct Product), nProducts, fp);
    fclose(fp);
}

/* ── Load Products ── */
void loadProducts() {
    FILE *fp = fopen(FILE_PRODUCTS, "rb");
    if (!fp){
		return;
	}
    fread(&nProducts, sizeof(int), 1, fp);
    fread(shop, sizeof(struct Product), nProducts, fp);
    fclose(fp);
}

/* ── Save Members ── */
void saveMembers() {
    FILE *fp = fopen(FILE_MEMBERS, "wb");
    if (!fp) {
        setColor(12);
        printf("  [✗] Error: Could not open members file for writing.\n");
        resetColor();
        return;
    }
    fwrite(&nMembers, sizeof(int), 1, fp);
    fwrite(Members, sizeof(struct Member), nMembers, fp);
    fclose(fp);
}

/* ── Load Members ── */
void loadMembers() {
    FILE *fp = fopen(FILE_MEMBERS, "rb");
    if (!fp){
		return;
	}
    fread(&nMembers, sizeof(int), 1, fp);
    fread(Members, sizeof(struct Member), nMembers, fp);
    fclose(fp);
}

/* ── Save Customers ── */
void saveCustomers() {
    FILE *fp = fopen(FILE_CUSTOMERS, "wb");
    if (!fp) {
        setColor(12);
        printf("  [✗] Error: Could not open customers file for writing.\n");
        resetColor();
        return;
    }
    fwrite(&nCustomers, sizeof(int), 1, fp);
    fwrite(customers, sizeof(struct Customer), nCustomers, fp);
    fclose(fp);
}

/* ── Load Customers ── */
void loadCustomers() {
    FILE *fp = fopen(FILE_CUSTOMERS, "rb");
    if (!fp){
		return;
	}
    fread(&nCustomers, sizeof(int), 1, fp);
    fread(customers, sizeof(struct Customer), nCustomers, fp);
    fclose(fp);
}

/* ── Save Sales ── */
void saveSales() {
    FILE *fp = fopen(FILE_SALES, "wb");
    if (!fp) {
        setColor(12);
        printf("  [✗] Error: Could not open sales file for writing.\n");
        resetColor();
        return;
    }
    fwrite(&nSales, sizeof(int), 1, fp);
    fwrite(sales, sizeof(struct Sale), nSales, fp);
    fclose(fp);
}

/* ── Load Sales ── */
void loadSales() {
    FILE *fp = fopen(FILE_SALES, "rb");
    if (!fp){
		return;
	}
    fread(&nSales, sizeof(int), 1, fp);
    fread(sales, sizeof(struct Sale), nSales, fp);
    fclose(fp);
}

/* ── Save global counters ── */
void saveCounters() {
    FILE *fp = fopen(FILE_COUNTERS, "wb");
    if (!fp){
		return;
	}
    fwrite(&totalRevenue,     sizeof(float), 1, fp);
    fwrite(&totalDiscount,    sizeof(float), 1, fp);
    fwrite(&totalBills,       sizeof(int),   1, fp);
    fwrite(&totalQRPayments,  sizeof(int),   1, fp);
    fwrite(&totalCashPayments,sizeof(int),   1, fp);
    fclose(fp);
}

/* ── Load global counters ── */
void loadCounters() {
    FILE *fp = fopen(FILE_COUNTERS, "rb");
    if (!fp) {
		return;
	}
    fread(&totalRevenue,      sizeof(float), 1, fp);
    fread(&totalDiscount,     sizeof(float), 1, fp);
    fread(&totalBills,        sizeof(int),   1, fp);
    fread(&totalQRPayments,   sizeof(int),   1, fp);
    fread(&totalCashPayments, sizeof(int),   1, fp);
    fclose(fp);
}

/* ── Save all data at once ── */
void saveAllData() {
    saveProducts();
    saveMembers();
    saveCustomers();
    saveSales();
    saveCounters();
    saveCategories();
}

/* ── Load all data at once ── */
void loadAllData() {
    loadProducts();
    loadMembers();
    loadCustomers();
    loadSales();
    loadCounters();
    loadCategories();
}

/* ── Backup & Restore Menu (Admin) ── */
void backupRestore() {
    int choice;
    system("cls");
    printHeader("AYH MART  |  Surat, India");
    printHeader("BACKUP & RESTORE DATA");
    printf("  Files are stored in the program directory.\n\n");
    printf("  1. Save All Data (Backup)\n");
    printf("  2. Load All Data (Restore)\n");
    printf("  3. View Backup File Status\n");
    printf("  0. Back to Admin Portal\n");
    printDash();
    printf("  Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            saveAllData();
            setColor(10);
            printf("\n  [✓] All data saved successfully.\n");
            resetColor();
            printf("      Products    -> %s\n", FILE_PRODUCTS);
            printf("      Members     -> %s\n", FILE_MEMBERS);
            printf("      Customers   -> %s\n", FILE_CUSTOMERS);
            printf("      Sales       -> %s\n", FILE_SALES);
            printf("      Counters    -> %s\n", FILE_COUNTERS);
            printf("      Categories  -> %s\n", FILE_CATEGORIES);
            printf("\n");
            system("pause");
            backupRestore();
            break;
        case 2: {
            char confirm[5];
            setColor(14);
            printf("\n  [!] This will overwrite current in-memory data.\n");
            resetColor();
            printf("  Are you sure? (yes/no): ");
            scanf("%s", confirm);
            if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) {
                loadAllData();
                setColor(10);
                printf("\n  [✓] Data restored from files.\n");
                resetColor();
                printf("      Products  : %d loaded\n",  nProducts);
                printf("      Members   : %d loaded\n",  nMembers);
                printf("      Customers : %d loaded\n",  nCustomers);
                printf("      Sales     : %d loaded\n",  nSales);
            } else {
                printf("  [-] Restore cancelled.\n");
            }
            printf("\n");
            system("pause");
            backupRestore();
            break;
        }
        case 3: {
            FILE *fp;
            system("cls");
            printHeader("BACKUP FILE STATUS");
            printf("  %-30s  %s\n", "File", "Status");
            printDash();

            /* ★ CREATIVE: green for found, red for missing */
            fp = fopen(FILE_PRODUCTS,  "rb");
            if (fp) setColor(10); else setColor(12);
            printf("  %-30s  %s\n", FILE_PRODUCTS,  fp ? "EXISTS" : "NOT FOUND");
            resetColor(); if (fp) fclose(fp);

            fp = fopen(FILE_MEMBERS,   "rb");
            if (fp) setColor(10); else setColor(12);
            printf("  %-30s  %s\n", FILE_MEMBERS,   fp ? "EXISTS" : "NOT FOUND");
            resetColor(); if (fp) fclose(fp);

            fp = fopen(FILE_CUSTOMERS, "rb");
            if (fp) setColor(10); else setColor(12);
            printf("  %-30s  %s\n", FILE_CUSTOMERS, fp ? "EXISTS" : "NOT FOUND");
            resetColor(); if (fp) fclose(fp);

            fp = fopen(FILE_SALES,     "rb");
            if (fp) setColor(10); else setColor(12);
            printf("  %-30s  %s\n", FILE_SALES,     fp ? "EXISTS" : "NOT FOUND");
            resetColor(); if (fp) fclose(fp);

            fp = fopen(FILE_COUNTERS,  "rb");
            if (fp) setColor(10); else setColor(12);
            printf("  %-30s  %s\n", FILE_COUNTERS,  fp ? "EXISTS" : "NOT FOUND");
            resetColor(); if (fp) fclose(fp);

            fp = fopen(FILE_CATEGORIES,"rb");
            if (fp) setColor(10); else setColor(12);
            printf("  %-30s  %s\n", FILE_CATEGORIES,fp ? "EXISTS" : "NOT FOUND");
            resetColor(); if (fp) fclose(fp);

            printf("\n");
            system("pause");
            backupRestore();
            break;
        }
        case 0:
            adminDashboard();
            break;
        default:
            setColor(12);
            printf("  [!] Invalid option.\n");
            resetColor();
            system("pause");
            backupRestore();
    }
}

void inputPassword(char *password) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) {
            password[i] = '\0';
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
}

int isProductCodeExists(int code) {
    for (int i = 0; i < nProducts; i++) {
        if (shop[i].pCode == code) {
            return 1;
        }
    }
    return 0;
}

int isCustomerIdExists(int id) {
    for (int i = 0; i < nCustomers; i++) {
        if (customers[i].cId == id) {
            return 1;
        }
    }
    return 0;
}
