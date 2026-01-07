#include<stdio.h>
#include<string.h>
#define MAX_ITEMS 100
char ad_user[10]="admin";
char ad_pass[10]="admin";
//username: admin; passwored: admin
void main_manu();
void admin();
void adminDashboard();
void inventory();
void add();
void login();
void view();
void del();
void info();
void view();
void cust_view();

void main(){
	main_manu();
}

void main_manu(){
	int choice;
	printf("Enter 1 Customer Self-Checkout\n");
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
			break;
		default:
			printf("Invalid Option!\n");
			printf("\n\n\nEnter choice Again\n");
			main_manu();
			break;
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
	printf("Enter 1 Inventory\n");
	printf("Enter 2 Members\n");
	printf("Enter 3 Sales\n");
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
			//members();
			break;
		case 3:
			//sales();
			break;
		case 4:
			//staff();
			break;
		case 0:
			adminDashboard();
			break;
		default:
			printf("Invalid Option!\n");
			printf("\n\n\nEnter choice Again\n");
			adminDashboard();
			break;
	}	
}

void inventory(){
	printf("\n\nInventory\n");
	int choice;
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
			adminDashboard();
			break;
		default:
			printf("Invalid Option!\n");
			printf("\n\n\nEnter choice Again\n");
			inventory();
			break;
	}
}

struct Product {
    int pCode;
    char pName[50];
    float pPrice;
    int pStock;
    char pCategory[30];
} shop[MAX_ITEMS];
int nProducts = 0;

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
}

void del() {
    int code;
    printf("Enter code to delete: "); 
	scanf("%d", &code);
    for (int i = 0; i < nProducts; i++){
		if (shop[i].pCode == code) { 
			shop[i].pCode=shop[i+1].pCode;
        	strcpy(shop[i].pName, shop[i+1].pName);
        	shop[i].pPrice=shop[i+1].pPrice;
        	shop[i].pStock=shop[i+1].pStock;
        	strcpy(shop[i].pName, shop[i+1].pName);
			break; 
		}else{
			printf("code not found");
			inventory();
		}
    }
    printf("Product Deleted");
    nProducts--;
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
	main_manu();
}

void cust(){
	int choice;
	printf("\n\nCUSTOMER MENU");
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
			cust_view();
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 0:
			break;
		default:
			break;
	}
}

void cust_view(){
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

}
