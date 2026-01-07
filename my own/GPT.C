#include<stdio.h>
#include<string.h>

#define MAX_ITEMS 100
#define MAX_MEMBERS 100
#define MAX_CART 50

char ad_user[10]="admin";
char ad_pass[10]="admin";

/* ---------- FUNCTION DECLARATIONS ---------- */
void mainMenu();
void admin();
void login();
void adminDashboard();
void inventory();
void add();
void view();
void del();
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
void salesMenu();
void salesSummary();
void productSales();
void discountReport();
void recordSale(int, char[], int, float);
float applyDiscount(int, float);
void staff();

/* ---------- DATA STRUCTURES ---------- */
struct Product {
    int pCode;
    char pName[50];
    float pPrice;
    int pStock;
    char pCategory[30];
} shop[MAX_ITEMS];
int nProducts = 0;

struct Member {
    int mId;
    char mName[50];
    float mSpent;
    char mTier[20];
    float mDisc;
} Members[MAX_MEMBERS];
int nMembers = 0;

struct CartItem {
    int code;
    char name[50];
    float price;
    int qty;
} cart[MAX_CART];
int cartCount = 0;

struct Sale {
    int pCode;
    char pName[50];
    int qty;
    float price;
    float total;
} sales[500];
int nSales = 0;

float totalRevenue = 0;
float totalDiscount = 0;
int totalBills = 0;

/* ---------- MAIN ---------- */
int main(){
    mainMenu();
    return 0;
}

/* ---------- MAIN MENU ---------- */
void mainMenu(){
    int choice;
    while(1){
        printf("\n1. Customer Self-Checkout");
        printf("\n2. Staff / Admin Login");
        printf("\n3. View Store Info");
        printf("\n0. Exit\nChoice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1: cust(); break;
            case 2: admin(); break;
            case 3: info(); break;
            case 0: return;
            default: printf("Invalid choice\n");
        }
    }
}

/* ---------- ADMIN ---------- */
void admin(){ login(); }

void login(){
    char u[10], p[10];
    printf("Username: "); scanf("%s",u);
    printf("Password: "); scanf("%s",p);

    if(strcmp(u,ad_user)==0 && strcmp(p,ad_pass)==0)
        adminDashboard();
    else
        printf("Invalid Login\n");
}

void adminDashboard(){
    int ch;
    while(1){
        printf("\nADMIN DASHBOARD");
        printf("\n1. Inventory");
        printf("\n2. Members");
        printf("\n3. Sales Report");
        printf("\n4. Staff");
        printf("\n0. Logout\nChoice: ");
        scanf("%d",&ch);

        switch(ch){
            case 1: inventory(); break;
            case 2: members(); break;
            case 3: salesMenu(); break;
            case 4: staff(); break;
            case 0: return;
            default: printf("Invalid\n");
        }
    }
}

/* ---------- INVENTORY ---------- */
void inventory(){
    int ch;
    while(1){
        printf("\nINVENTORY");
        printf("\n1. Add Product");
        printf("\n2. Delete Product");
        printf("\n3. View Products");
        printf("\n0. Back\nChoice: ");
        scanf("%d",&ch);

        switch(ch){
            case 1: add(); break;
            case 2: del(); break;
            case 3: view(); break;
            case 0: return;
        }
    }
}

void add(){
    int c;
    printf("How many products: ");
    scanf("%d",&c);

    for(int i=0;i<c && nProducts<MAX_ITEMS;i++){
        scanf("%d %s %f %d %s",
              &shop[nProducts].pCode,
              shop[nProducts].pName,
              &shop[nProducts].pPrice,
              &shop[nProducts].pStock,
              shop[nProducts].pCategory);
        nProducts++;
    }
}

void view(){
    printf("\nCODE NAME PRICE STOCK CATEGORY\n");
    for(int i=0;i<nProducts;i++)
        printf("%d %s %.2f %d %s\n",
               shop[i].pCode, shop[i].pName,
               shop[i].pPrice, shop[i].pStock,
               shop[i].pCategory);
}

void del(){
    int code;
    scanf("%d",&code);
    for(int i=0;i<nProducts;i++){
        if(shop[i].pCode==code){
            shop[i]=shop[--nProducts];
            printf("Deleted\n");
            return;
        }
    }
}

/* ---------- CUSTOMER ---------- */
void cust(){
    int ch;
    while(1){
        printf("\nCUSTOMER");
        printf("\n1. View Products");
        printf("\n2. Search");
        printf("\n3. Add to Cart");
        printf("\n4. View Cart");
        printf("\n5. Checkout");
        printf("\n0. Back\nChoice: ");
        scanf("%d",&ch);

        switch(ch){
            case 1: custView(); break;
            case 2: search(); break;
            case 3: addCart(); break;
            case 4: viewCart(); break;
            case 5: checkout(); break;
            case 0: return;
        }
    }
}

void custView(){ view(); }

void search(){
    int c;
    scanf("%d",&c);
    for(int i=0;i<nProducts;i++)
        if(shop[i].pCode==c)
            printf("%s %.2f\n",shop[i].pName,shop[i].pPrice);
}

void addCart(){
    int c,q;
    scanf("%d %d",&c,&q);
    for(int i=0;i<nProducts;i++){
        if(shop[i].pCode==c && shop[i].pStock>=q){
            cart[cartCount].code=c;
            strcpy(cart[cartCount].name,shop[i].pName);
            cart[cartCount].price=shop[i].pPrice;
            cart[cartCount].qty=q;
            shop[i].pStock-=q;
            cartCount++;
        }
    }
}

void viewCart(){
    float t=0;
    for(int i=0;i<cartCount;i++){
        t+=cart[i].price*cart[i].qty;
        printf("%s %.2f\n",cart[i].name,cart[i].price);
    }
    printf("Total: %.2f\n",t);
}

void checkout(){
    float total=0;
    int mid;
    for(int i=0;i<cartCount;i++){
        total+=cart[i].price*cart[i].qty;
        recordSale(cart[i].code,cart[i].name,cart[i].qty,cart[i].price);
    }
    scanf("%d",&mid);
    float d=applyDiscount(mid,total);
    printf("Pay: %.2f\n",total-d);
    totalBills++;
    cartCount=0;
}

/* ---------- MEMBERS ---------- */
void members(){
    int ch;
    while(1){
        printf("\n1.Add 2.Delete 3.View 0.Back: ");
        scanf("%d",&ch);
        if(ch==0) return;
        if(ch==1) addMembers();
        if(ch==2) deleteMembers();
        if(ch==3) viewMembers();
    }
}

void addMembers(){
    scanf("%d %s %f %s",
          &Members[nMembers].mId,
          Members[nMembers].mName,
          &Members[nMembers].mSpent,
          Members[nMembers].mTier);
    Members[nMembers].mDisc =
        strcmp(Members[nMembers].mTier,"Gold")==0?10:
        strcmp(Members[nMembers].mTier,"Platinum")==0?15:5;
    nMembers++;
}

void deleteMembers(){
    int id; scanf("%d",&id);
    for(int i=0;i<nMembers;i++)
        if(Members[i].mId==id)
            Members[i]=Members[--nMembers];
}

void viewMembers(){
    for(int i=0;i<nMembers;i++)
        printf("%d %s %.2f %.2f%%\n",
               Members[i].mId,Members[i].mName,
               Members[i].mSpent,Members[i].mDisc);
}

/* ---------- SALES ---------- */
void recordSale(int c,char n[],int q,float p){
    sales[nSales].pCode=c;
    strcpy(sales[nSales].pName,n);
    sales[nSales].qty=q;
    sales[nSales].price=p;
    sales[nSales].total=q*p;
    totalRevenue+=sales[nSales].total;
    nSales++;
}

float applyDiscount(int id,float amt){
    for(int i=0;i<nMembers;i++)
        if(Members[i].mId==id){
            float d=amt*(Members[i].mDisc/100);
            totalDiscount+=d;
            Members[i].mSpent+=amt-d;
            return d;
        }
    return 0;
}

void salesMenu(){
    int ch;
    while(1){
        printf("\n1.Summary 2.Product 3.Discount 0.Back: ");
        scanf("%d",&ch);
        if(ch==0) return;
        if(ch==1) salesSummary();
        if(ch==2) productSales();
        if(ch==3) discountReport();
    }
}

void salesSummary(){
    printf("Bills:%d Revenue:%.2f Discount:%.2f\n",
           totalBills,totalRevenue,totalDiscount);
}

void productSales(){
    for(int i=0;i<nSales;i++)
        printf("%s %.2f\n",sales[i].pName,sales[i].total);
}

void discountReport(){
    for(int i=0;i<nMembers;i++)
        printf("%s %.2f%%\n",Members[i].mName,Members[i].mDisc);
}

/* ---------- STAFF ---------- */
void staff(){
    printf("Staff module placeholder\n");
}
