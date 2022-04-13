#ifndef _kitchenheader
#define _kitchenheader

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct product {  // product in kitchen
	char* ProductName;  // name of product
	int price;  // price of product
	int Quantity; // quantity of product in kitchen
	int NumberOfTimesOrdered;  // number of times product is ordered
	struct product* next;  // pointer to next product
}Product, * PProd;


typedef struct ontable {
	char* ItemName;  // product name
	int Quantity; // quantity of product on table
	struct ontable* next; //pointer to next product on table
	struct ontable* prev;  // pointer to previus product on table
}Item_onTable, * PITable;
PProd CreateProducts(FILE* out);  // creates a list of products in resturant
int ProductExists(PProd prod_list, char* new_product); //checks if product exists in resturant by name (secondery function)
void AddItem(PProd prodlisthead, char* prod_name, int prod_quantity, FILE* out);  // add items to product if exists
void OrderItem(PProd ItemInKitchen, PITable* TablesArray, int TableNumber, char* ProductName, int Quantity, int arrlen, FILE* out);// order food to a table if exists
int ProductIsOnTable(char* prod_name, PITable Table);   // check if product exists in the table(secondery function) 
void RemoveItem(PProd ItemInKitchenHead, PITable* TablesArray, int TableNumber, char* ProductName, int Quantity, FILE* out);  // to remove an item from a table
void RemoveTable(PProd KitchenProductsList, PITable* TablesArray, int TableNumber, int arrlen, FILE* out);  // close table and remove it.
void FreeResturant(PITable* TablesArray, PProd ProductsInKitchen, int arrlen);
void FreeProductList(PProd prodlisthead);   // free the list of products in kitchen   
void FreeTable(PITable* Table);   // to free a the table pointed at
int ResturantIsEmpty(PITable* TablesArray, int arrlen);   // to check if there still tables in resturant or its empty
void CheckTheMostOrderd(PITable* TablesArray, PProd KitchenList, int arrlen, FILE* out); // returns the name of most ordered product and number of times
int TableToPay(PITable Table, PProd kitchenlist);

#endif