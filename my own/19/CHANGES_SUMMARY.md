# AYH MART - Feature Updates Summary

## Changes Made

### 1. Product Name Validation (Prevents Duplicate Product Names)

#### New Function Added:
```c
int isProductNameExists(char name[], int excludeIndex)
```
- Checks if a product name already exists in the inventory
- Uses case-insensitive comparison (Rice = rice = RICE)
- The `excludeIndex` parameter allows excluding a specific product when editing
- Returns 1 if name exists, 0 if it doesn't

#### Modified Functions:

**add() function:**
- Added a validation loop when entering product name
- Prevents adding products with duplicate names
- User is prompted to enter a different name if duplicate detected

**editProduct() function:**
- Added validation when changing product name
- Checks if new name already exists (excluding current product)
- User can keep existing name by entering "0"
- Prevents renaming to a name that's already taken

### 2. Enhanced Customer Details

#### Updated Customer Structure:
```c
struct Customer {
    int  cId;
    char cName[50];
    char cPass[20];
    char cPhone[15];     // NEW: Phone number field
    char cEmail[50];     // NEW: Email address field
    int  cMemberId;
}
```

#### Modified Functions:

**addCustomer() function:**
- Now collects phone number from user
- Now collects email address from user
- Updated default customers in main() to include phone and email

**viewCustomers() function:**
- Updated display to show phone number and email
- Adjusted column widths for better formatting

**Default Customers Updated:**
- aum: 9876543210, aum@email.com
- hari: 9876543211, hari@email.com
- yash: 9876543212, yash@email.com

## How to Use the New Features

### Product Name Validation:
1. When adding a new product, if you enter a name that already exists:
   ```
   Product name already exists. Enter different name.
   ```
2. When editing a product, if you try to rename to an existing product:
   ```
   Product with this name already exists. Try different name.
   ```

### Customer Details:
1. When adding a customer, you'll be prompted for:
   - Customer ID
   - Name
   - Phone Number (new!)
   - Email Address (new!)
   - Password
   - Confirm Password
   - Member ID

2. When viewing customers, you'll see all details including phone and email

## Important Notes

1. **Case-Insensitive Validation**: The product name check uses `strcasecmp()` which is case-insensitive. This means "Rice", "rice", and "RICE" are all considered duplicates.

2. **Data Persistence**: The new customer fields (phone and email) are automatically saved to the data files when using the save/load functions.

3. **Backward Compatibility**: If you have existing customer data files, you'll need to either:
   - Delete old data files and start fresh, OR
   - Add phone/email to existing customers manually

## Testing Checklist

- [ ] Try adding a product with a duplicate name
- [ ] Try editing a product to have a duplicate name
- [ ] Add a new customer with phone and email
- [ ] View customer list to see phone and email displayed
- [ ] Save and reload data to ensure phone/email persists

## Files Modified
- Updated all sections handling product addition and editing
- Updated Customer structure and all related functions
- Updated file I/O to handle new customer fields
