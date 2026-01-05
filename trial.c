#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTS 50
#define MAX_CART 100
#define MAX_MEMBERS 20
#define ADMIN_PASSWORD "admin123"

/* ---------- STRUCTURES ---------- */
struct Product {
    int code;
    char name[50];
    float price;
    int stock;
};

struct CartItem {
    int code;
    char name[50];
    float price;
    int qty;
};

struct Member {
    int id;
    char name[50];
    float discount;
};

/* ---------- GLOBAL DATA ---------- */
struct Product products[MAX_PRODUCTS];
struct CartItem cart[MAX_CART];
struct Member members[MAX_MEMBERS];

int productCount = 0, cartCount = 0, memberCount = 0;
int billNo = 1001;

/* SALES REPORT */
float totalRevenue = 0;
float totalTax = 0;
float totalDiscount = 0;
int totalBills = 0;

/* ---------- INITIAL DATA ---------- */
void initProducts() {
    products[0] = (struct Product){101, "Milk Pack", 45, 100};
    products[1] = (struct Product){102, "Bread", 30, 80};
    products[2] = (struct Product){103, "Rice 5kg", 350, 40};
    products[3] = (struct Product){104, "Cooking Oil", 180, 60};
    products[4] = (struct Product){105, "Eggs 12", 70, 90};
    productCount = 5;
}

void initMembers() {
    members[0] = (struct Member){1, "Silver", 0.05};
    members[1] = (struct Member){2, "Gold", 0.10};
    members[2] = (struct Member){3, "Platinum", 0.15};
    members[3] = (struct Member){4, "Diamond", 0.20};
    members[4] = (struct Member){5, "VIP", 0.30};
    memberCount = 5;
}

/* ---------- UTILITIES ---------- */
int findProduct(int code) {
    for (int i = 0; i < productCount; i++)
        if (products[i].code == code)
            return i;
    return -1;
}

float calculateSubtotal() {
    float sum = 0;
    for (int i = 0; i < cartCount; i++)
        sum += cart[i].price * cart[i].qty;
    return sum;
}

/* ---------- DISPLAY ---------- */
void showProducts() {
    printf("\nCODE\tNAME\t\tPRICE\tSTOCK\n");
    for (int i = 0; i < productCount; i++)
        printf("%d\t%-15s %.2f\t%d\n",
               products[i].code,
               products[i].name,
               products[i].price,
               products[i].stock);
}

void showMembers() {
    printf("\nID\tMEMBERSHIP\tDISCOUNT\n");
    for (int i = 0; i < memberCount; i++)
        printf("%d\t%-12s\t%.0f%%\n",
               members[i].id,
               members[i].name,
               members[i].discount * 100);
}

/* ---------- CART ---------- */
void addToCart() {
    int code, qty;
    showProducts();

    printf("Product code: ");
    scanf("%d", &code);

    int idx = findProduct(code);
    if (idx == -1) {
        printf("Invalid product\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d", &qty);

    if (qty <= 0 || qty > products[idx].stock) {
        printf("Invalid quantity\n");
        return;
    }

    cart[cartCount++] = (struct CartItem){
        products[idx].code, "", products[idx].price, qty
    };
    strcpy(cart[cartCount - 1].name, products[idx].name);

    products[idx].stock -= qty;
    printf("Item added successfully\n");
}

float applyMembership(float subtotal) {
    int id;
    showMembers();
    printf("Membership ID (0 = none): ");
    scanf("%d", &id);

    for (int i = 0; i < memberCount; i++)
        if (members[i].id == id)
            return subtotal * members[i].discount;

    return 0;
}

/* ---------- PAYMENT ---------- */
int processPayment(float total) {
    int choice;
    float amount;
    int pin;

    printf("\nPayment Method:\n1 Cash\n2 Card\nChoice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter cash amount: ");
            scanf("%f", &amount);
            if (amount < total) {
                printf("Insufficient cash\n");
                return 0;
            }
            printf("Change: %.2f\n", amount - total);
            return 1;

        case 2:
            printf("Enter 4-digit PIN: ");
            scanf("%d", &pin);
            printf("Payment successful (Card)\n");
            return 1;

        default:
            printf("Invalid payment method\n");
            return 0;
    }
}

/* ---------- CHECKOUT ---------- */
void checkout() {
    if (cartCount == 0) {
        printf("Cart empty\n");
        return;
    }

    float subtotal = calculateSubtotal();
    float discount = applyMembership(subtotal);
    float tax = subtotal * 0.05;
    float total = subtotal + tax - discount;

    printf("\nSubtotal: %.2f\nTax: %.2f\nDiscount: %.2f\nTotal: %.2f\n",
           subtotal, tax, discount, total);

    if (!processPayment(total)) {
        printf("Transaction cancelled\n");
        cartCount = 0;
        return;
    }

    printf("\n===== RECEIPT #%d =====\n", billNo++);
    for (int i = 0; i < cartCount; i++)
        printf("%s x%d = %.2f\n",
               cart[i].name,
               cart[i].qty,
               cart[i].price * cart[i].qty);

    printf("Total Paid: %.2f\nThank you for shopping!\n", total);

    totalRevenue += total;
    totalTax += tax;
    totalDiscount += discount;
    totalBills++;

    cartCount = 0;
}

/* ---------- ADMIN ---------- */
int adminLogin() {
    char pass[20];
    int attempts = 3;

    while (attempts--) {
        printf("Enter admin password: ");
        scanf("%s", pass);
        if (strcmp(pass, ADMIN_PASSWORD) == 0)
            return 1;
        printf("Wrong password (%d attempts left)\n", attempts);
    }
    return 0;
}

void salesReport() {
    printf("\n===== SALES REPORT =====\n");
    printf("Total Bills: %d\n", totalBills);
    printf("Total Revenue: %.2f\n", totalRevenue);
    printf("Total Tax Collected: %.2f\n", totalTax);
    printf("Total Discount Given: %.2f\n", totalDiscount);
}

void addMembership() {
    printf("Membership ID: ");
    scanf("%d", &members[memberCount].id);
    printf("Name: ");
    scanf(" %[^\n]", members[memberCount].name);
    printf("Discount (%%): ");
    scanf("%f", &members[memberCount].discount);
    members[memberCount].discount /= 100;
    memberCount++;
    printf("Membership added\n");
}

void adminPanel() {
    int choice;
    do {
        printf("\n--- ADMIN PANEL ---");
        printf("\n1 View Products");
        printf("\n2 Add Membership");
        printf("\n3 View Sales Report");
        printf("\n4 Exit Admin");
        printf("\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: showProducts(); break;
            case 2: addMembership(); break;
            case 3: salesReport(); break;
        }
    } while (choice != 4);
}

/* ---------- MAIN ---------- */
int main() {
    initProducts();
    initMembers();

    int choice;
    do {
        printf("\n===== SELF CHECKOUT SYSTEM =====");
        printf("\n1 Customer");
        printf("\n2 Admin");
        printf("\n3 Exit");
        printf("\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                do {
                    printf("\n1 Scan Item\n2 Checkout\n3 Exit\nChoice: ");
                    scanf("%d", &choice);
                    if (choice == 1) addToCart();
                    else if (choice == 2) checkout();
                } while (choice != 3);
                break;

            case 2:
                if (adminLogin())
                    adminPanel();
                else
                    printf("Access denied\n");
                break;
        }
    } while (choice != 3);

    return 0;
}
