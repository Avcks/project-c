#include<stdio.h>
#include<string.h>
#define MAX_ITEMS 100
#define MAX_MEMBERS 100
#define MAX_CART 50

char ad_user[10]="admin";
char ad_pass[10]="admin";

//username: admin; passwored: admin
void mainManu();
void admin();
void adminDashboard();
void inventory();
void add();
void login();
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
void salesManu();
void salesSummary();
void productSales();
void discountReport();
void recordSale(int, char[], int, float);
float applyDiscount(int, float);
void billing(int finalAmount);

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

int main(){
    mainManu();
    return 0;
}

void mainManu(){
    int choice;
    while(1){
        printf("\nEnter 1 Customer Self-Checkout\n");
        printf("Enter 2 Staff / Admin Login\n");
        printf("Enter 3 View Store Info\n");
        printf("Enter 0 Exit Application\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                cust();
                break;
            case 2:
                admin();
                break;
            case 3:
                info();
                break;
            case 0:
                return;
            default:
                printf("Invalid Option!\n");
        }
    }
}

void admin(){
	login();
}

void login(){
	char username[10], password[10];
	printf("\n\nEnter Username: ");
	scanf("%s",username);
	printf("Enter Password: ");
	scanf("%s",password);
	
	if(strcmp(username,ad_user)==0 && strcmp(password,ad_pass)==0){
		printf("Login...\n\n");
		adminDashboard();
	}else{
		printf("Enter correct Username or Password");
	}
}

void adminDashboard() {
	printf("Admin Portal\n");
	int choice;
	while(1){
		printf("Enter 1 Inventory\n");
		printf("Enter 2 Members\n");
		printf("Enter 3 Sales Report\n");
		printf("Enter 4 Staff\n");
		printf("Enter 0 logout\n");
    	printf("------------------------------\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch(choice){
			case 1:
				inventory();
				break;
			case 2:
				members();
				break;
			case 3:
				salesManu();
				break;
			case 4:
				//staff();
				break;
			case 0:
				printf("\n\n\n");
				mainManu();
				return;
				break;
			default:
				printf("Invalid Option!\n");
				printf("\n\n\nEnter choice Again\n");
				adminDashboard();
				break;
		}
	}
}

void inventory(){
    int choice;
    while(1){
        printf("\nInventory\n");
        printf("Enter 1 Add\n");
        printf("Enter 2 Delete\n");
        printf("Enter 3 View\n");
        printf("Enter 0 Back\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                add();
                break;
            case 2:
                del();
                break;
            case 3:
                view();
                break;
            case 0:
                return;
            default:
                printf("Invalid Option!\n");
        }
    }
}

void add(){
	int count;
    printf("\n\nHow many products: "); 
	scanf("%d", &count);
    for (int i = 0; i < count; i++) {
        if (nProducts >= MAX_ITEMS){
        	printf("Product MaxOut!");
        	inventory();
		}else{
			printf("\nCode: "); 
			scanf("%d", &shop[nProducts].pCode);
        	printf("Name: "); 
			scanf(" %s", shop[nProducts].pName);
        	printf("Price: "); 
			scanf("%f", &shop[nProducts].pPrice);
        	printf("Stock: "); 
			scanf("%d", &shop[nProducts].pStock);
        	printf("Cat: "); 
			scanf("%s", shop[nProducts].pCategory);
        	nProducts++;
    	}
    }
    printf("\nProduct Added");
    inventory();
}

void view(){
	printf("\n\nAVAILABLE PRODUCTS \n");
    if (nProducts == 0) {
        printf("No products in shop.\n");
    }else{
    	printf("\nCODE \t NAME \t PRICE \t STOCK \t CATEGORY\n");
    	printf("------------------------------------------------\n");
    	for (int i = 0; i < nProducts; i++) {
        printf("%d \t %s \t %.2f \t %d \t %s\n",
			shop[i].pCode, shop[i].pName, shop[i].pPrice,
    		shop[i].pStock, shop[i].pCategory);
    	}
	}
	inventory();
}

void del(){
    int code, found = 0;
    printf("Enter code to delete: ");
    scanf("%d", &code);

    for(int i = 0; i < nProducts; i++){
        if(shop[i].pCode == code){
            for(int j = i; j < nProducts - 1; j++){
                shop[j] = shop[j + 1];
            }
            nProducts--;
            found = 1;
            printf("Product Deleted Successfully!\n");
            break;
        }
    }

    if(!found){
        printf("Product Code Not Found!\n");
    }

    inventory();
}

void info(){
	printf("\n\n==========================================\n");
	printf("\tSTORE INFORMATION\n");
	printf("==========================================\n");
	printf("Store Name: AYH Mart\n");
	printf("Location: Surat, India\n");
	printf("Timing: 9 AM to 10 PM\n");
	printf("Developed By: Aum, Yesh, HariKrushna\n\n\n");
	mainManu();
}

void cust(){
    int choice;
    while(1){
        printf("\nCUSTOMER MENU\n");
        printf("Enter 1 View Store Products\n");
        printf("Enter 2 Search for a Product\n");
        printf("Enter 3 Add Item to Basket\n");
        printf("Enter 4 View My Basket\n");
        printf("Enter 5 Final Checkout\n");
        printf("Enter 0 Return to Main Menu\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                custView();
                break;
            case 2:
    			search();
    			break;
			case 3:
    			addCart();
    			break;
			case 4:
    			viewCart();
    			break;
			case 5:
    			checkout();
    			break;
            case 0:
            	mainManu();
                break;
            default:
                printf("Invalid Option!\n");
        }
    }
}

void custView(){
	printf("\n\nAVAILABLE PRODUCTS \n");
    if (nProducts == 0) {
        printf("No products in shop.\n");
    }else{
    	printf("\nCODE \t NAME \t PRICE \t STOCK \t CATEGORY\n");
    	printf("------------------------------------------------\n");
    	for (int i = 0; i < nProducts; i++) {
        printf("%d \t %s \t %.2f \t %d \t %s\n",
			shop[i].pCode, 
			shop[i].pName, 
			shop[i].pPrice,
    		shop[i].pStock, 
			shop[i].pCategory);
    	}
	}
	cust();
}

void search(){
    int code, found = 0;
    printf("Enter Product Code to Search: ");
    scanf("%d", &code);

    for(int i = 0; i < nProducts; i++){
        if(shop[i].pCode == code){
            printf("Found: %s | %.2f | Stock: %d\n",
                   shop[i].pName,
                   shop[i].pPrice,
                   shop[i].pStock);
            found = 1;
            break;
        }
    }

    if(!found){
        printf("Product Not Found\n");
    }

    cust();
}

void addCart(){
    int code, qty, found = 0;
    
    if(cartCount >= MAX_CART){
    	printf("Cart is full!\n");
    	cust();
    	return;
	}

    printf("Enter Product Code: ");
    scanf("%d", &code);
    printf("Enter Quantity: ");
    scanf("%d", &qty);
	
    for(int i = 0; i < nProducts; i++){
        if(shop[i].pCode == code && shop[i].pStock >= qty){
            cart[cartCount].code = shop[i].pCode;
            strcpy(cart[cartCount].name, shop[i].pName);
            cart[cartCount].price = shop[i].pPrice;
            cart[cartCount].qty = qty;

            shop[i].pStock -= qty;
            cartCount++;
            found = 1;
            printf("Item Added to Cart\n");
            break;
        }
    }

    if(!found){
        printf("Product unavailable or insufficient stock\n");
    }

    cust();
}

void viewCart(){
    float total = 0;

    if(cartCount == 0){
        printf("Cart is empty\n");
    } else {
        printf("\nCODE\tNAME\tPRICE\tQTY\tTOTAL\n");
        for(int i = 0; i < cartCount; i++){
            float amt = cart[i].price * cart[i].qty;
            total += amt;
            printf("%d\t%s\t%.2f\t%d\t%.2f\n",
                   cart[i].code,
                   cart[i].name,
                   cart[i].price,
                   cart[i].qty,
                   amt);
        }
        printf("Grand Total: %.2f\n", total);
    }

    cust();
}

void checkout(){
    int mid;
    float total = 0, discount = 0, finalAmount;

    if(cartCount == 0){
        printf("Cart is empty. Add items first.\n");
        return;
    }

    for(int i = 0; i < cartCount; i++){
        total += cart[i].price * cart[i].qty;
	}

    for(int i = 0; i < cartCount; i++){
        recordSale(cart[i].code, cart[i].name, cart[i].qty, cart[i].price);
	}

    printf("Enter Member ID (0 if none): ");
    scanf("%d", &mid);

    discount = applyDiscount(mid, total);
    finalAmount = total - discount;

    printf("\nTotal: %.2f", total);
    printf("\nDiscount Amount: %.2f", discount);
    printf("\nPayable Amount: %.2f\n", finalAmount);
	
	payagain:
		billing(finalAmount);
    	totalBills++;
    	cartCount = 0;
}

void billing(int finalAmount){
	int payAmount;
	printf("Pay by Cash: ");
	scanf("%d", payAmount);
	
	if(payAmount==finalAmount){
		printf("Thank You Visit Again!");
		mainManu();
	}else{
		printf("Kindly Pay Approprate Amount");
		goto payagain;
	}
}                          

void members(){
    int choice;
    while(1){
        printf("\nMember Portal\n");
        printf("Enter 1 Add New Member\n");
        printf("Enter 2 Delete Old Members\n");
        printf("Enter 3 View all Members\n");
        printf("Enter 0 Back\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
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
                return;
            default:
                printf("Invalid Option!\n");
        }
    }
}

void addMembers(){
    int count;
    printf("\nHow many Members to be Added: ");
    scanf("%d", &count);

    for(int i = 0; i < count; i++){
        if(nMembers >= MAX_MEMBERS){
            printf("Member limit reached!\n");
            break;
        }

        printf("\nMember ID: ");
        scanf("%d", &Members[nMembers].mId);

        printf("Name: ");
        scanf("%s", Members[nMembers].mName);

        printf("Total Amount Spent: ");
        scanf("%f", &Members[nMembers].mSpent);

        printf("Tier (Silver/Gold/Platinum): ");
        scanf("%s", Members[nMembers].mTier);


        if(strcmp(Members[nMembers].mTier, "Silver") == 0)
            Members[nMembers].mDisc = 5;
        else if(strcmp(Members[nMembers].mTier, "Gold") == 0)
            Members[nMembers].mDisc = 10;
        else if(strcmp(Members[nMembers].mTier, "Platinum") == 0)
            Members[nMembers].mDisc = 15;
        else
            Members[nMembers].mDisc = 0;

        nMembers++;
    }

    printf("\nMember(s) Added Successfully!\n");
    members();
}

void deleteMembers(){
    int id, found = 0;
    printf("Enter Member ID to delete: ");
    scanf("%d", &id);

    
    for(int i = 0; i < nMembers; i++){
        if(Members[i].mId == id){
            for(int j = i; j < nMembers - 1; j++){
                Members[j] = Members[j + 1];
            }
            nMembers--;
            found = 1;
            printf("Member Deleted Successfully!\n");
            break;
        }
    }

	if(!found){
        printf("Member ID not found!\n");
    }
    
    members();
}

void viewMembers(){
    if(nMembers == 0){
        printf("\nNo Members Found.\n");
    } else {
        printf("\nID\tNAME\tSPENT\tTIER\tDISCOUNT\n");
        printf("------------------------------------------\n");
        for(int i = 0; i < nMembers; i++){
            printf("%d\t%s\t%.2f\t%s\t%.2f%%\n",
                   Members[i].mId,
                   Members[i].mName,
                   Members[i].mSpent,
                   Members[i].mTier,
                   Members[i].mDisc);
        }
    }
    members();
}

void recordSale(int code, char name[], int qty, float price){
	if(nSales >= 500){
		return;
	}else{ 
    	sales[nSales].pCode = code;
    	strcpy(sales[nSales].pName, name);
    	sales[nSales].qty = qty;
    	sales[nSales].price = price;
    	sales[nSales].total = qty * price;
    	totalRevenue += sales[nSales].total;
    	nSales++;
	}
}

float applyDiscount(int memberId, float billAmount){
    for(int i = 0; i < nMembers; i++){
        if(Members[i].mId == memberId){
            float discount = billAmount * (Members[i].mDisc / 100);
            Members[i].mSpent += (billAmount - discount);
            totalDiscount += discount;
            return discount;
        }
    }
    return 0;
}

void salesManu(){
    int choice;
    while(1){
        printf("\nSALES REPORT\n");
        printf("1. Summary Report\n");
        printf("2. Product-wise Sales\n");
        printf("3. Member Discount Report\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice){
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
                printf("Invalid option\n");
        }
    }
}

void salesSummary(){
    printf("\n===== SALES SUMMARY =====\n");
    printf("Total Bills Generated : %d\n", totalBills);
    printf("Total Items Sold      : %d\n", nSales);
    printf("Total Revenue         : %.2f\n", totalRevenue);
    printf("Total Discount Given  : %.2f\n", totalDiscount);
    printf("Net Revenue           : %.2f\n",totalRevenue - totalDiscount);
}

void productSales(){
    printf("\nPRODUCT SALES REPORT\n");
    printf("CODE\tNAME\tQTY\tTOTAL\n");
    printf("----------------------------------\n");

    for(int i = 0; i < nSales; i++){
        printf("%d\t%s\t%d\t%.2f\n",
               sales[i].pCode,
               sales[i].pName,
               sales[i].qty,
               sales[i].total);
    }
}

void discountReport(){
    printf("\nMEMBER DISCOUNT REPORT\n");
    printf("ID\tNAME\tSPENT\tDISCOUNT\n");
    printf("----------------------------------\n");

    for(int i = 0; i < nMembers; i++){
        printf("%d\t%s\t%.2f\t%.2f%%\n",
               Members[i].mId,
               Members[i].mName,
               Members[i].mSpent,
               Members[i].mDisc);
    }
}
