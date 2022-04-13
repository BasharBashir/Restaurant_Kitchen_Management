#include "kitchenheader.h"

PProd CreateProducts(FILE* out) 
{
	PProd productlisthead = NULL, productlisttail = NULL, tempp;
	FILE* in;
	char tempname[51] = { '\n' };
	int tempquan, tempprice;
	in = fopen("Manot.txt", "rt"); // open text file to read
	if (in == NULL) {
		//FreeProductList(productlisthead);
		printf("\nfailed to open file!");
		exit(1);
	}
	while (!feof(in))
	{
		tempp = (PProd)malloc(sizeof(Product)); // new product
		if (tempp == NULL) {
			FreeProductList(productlisthead); // freeing memory to close program
			printf("\nfailed to allocate memory!");
			exit(1);
		}
		fscanf(in, "%s", tempname); // getting the info
		fscanf(in, "%d", &tempquan);
		fscanf(in, "%d", &tempprice);
		if (ProductExists(productlisthead, tempname)) { // if product exists, it wont add.
			if (productlisthead != NULL) {
				fputs("Product Exists in the list!\n", out);
				printf("product is on the list\n");
			}
		}
		else {
			if (tempquan <= 0) { // if quantity is negative, it wont add
				fputs("Quantity must be a posative number!\n", out);
			}
			else {
				if (tempprice <= 0) { // if price is negative it wont add
					fputs("Price must be a posative number!\n", out);
				}
				else {
					tempp->ProductName = (char*)malloc(strlen(tempname) + 1);
					strcpy(tempp->ProductName, tempname);  // adding info to struct if everything is OK
					tempp->price = tempprice;
					tempp->Quantity = tempquan;
					tempp->next = NULL;
					tempp->NumberOfTimesOrdered = 0;
					if (productlisthead == NULL) { // adding to the tail if list is empty
						productlisthead = tempp;
						productlisttail = tempp;
					}
					else { // if not empty
						productlisttail->next = tempp;
						productlisttail = tempp;
					}
				}
			}
		}
		strcpy(tempname, "\0");
	}
	fputs("The kitchen was created\n", out);
	printf("kitchen is created\n");
	fclose(in);
	return productlisthead; // return the head of the products list
}

int ProductExists(PProd prod_list, char* new_product) {
	PProd tempp = prod_list;
	while (tempp != NULL) {  // searching for the name
		if (strcmp(new_product, tempp->ProductName) == 0) // if they are the same
			return 1;
		tempp = tempp->next;
	}
	return 0;
}

void AddItem(PProd prodlisthead, char* prod_name, int prod_quantity, FILE* out) {
	PProd tempp = prodlisthead;
	if (ProductExists(prodlisthead, prod_name)) { // if the product is in the kitchen
		while (strcmp(tempp->ProductName, prod_name) != 0)  // find it
			tempp = tempp->next;
		tempp->Quantity += prod_quantity; // and add the quantity 
		if (prod_quantity == 1)
			fprintf(out, "%d %s was added to the kitchen\n", prod_quantity, prod_name);
		else fprintf(out, "%d %s were added to the kitchen\n", prod_quantity, prod_name);
		printf("added %d %s to the kitchen\n", prod_quantity, prod_name);
	}
	else {
		fprintf(out, "There is no %s in the kitchen\n", prod_name); // if not, tell the user
		printf("there is no %s in the kitchen\n", prod_name);
	}

}

void OrderItem(PProd ItemInKitchenHead, PITable* TablesArray, int TableNumber, char* ProductName, int Quantity, int arrlen, FILE* out) 
{

	PITable tempitemontable;
	PProd tempkitchenpointer = ItemInKitchenHead;
	if (ProductExists(ItemInKitchenHead, ProductName)) {
		while (strcmp(tempkitchenpointer->ProductName, ProductName))
			tempkitchenpointer = tempkitchenpointer->next;
		if (tempkitchenpointer->Quantity >= Quantity) {
			if (ProductIsOnTable(ProductName, TablesArray[TableNumber - 1])) {
				tempitemontable = TablesArray[TableNumber - 1];
				while (strcmp(tempitemontable->ItemName, ProductName) != 0)
					tempitemontable = tempitemontable->next;
				tempitemontable->Quantity += Quantity;
			}
			else {
				tempitemontable = (PITable)malloc(sizeof(Item_onTable));
				tempitemontable->ItemName = (char*)malloc(strlen(ProductName) + 1);
				tempitemontable->Quantity = Quantity;
				strcpy(tempitemontable->ItemName, ProductName);
				tempitemontable->next = TablesArray[TableNumber - 1];
				tempitemontable->prev = NULL;
				if (TablesArray[TableNumber - 1] != NULL)
					TablesArray[TableNumber - 1]->prev = tempitemontable;
				TablesArray[TableNumber - 1] = tempitemontable;
			}
			tempkitchenpointer->Quantity -= Quantity;
			tempkitchenpointer->NumberOfTimesOrdered += Quantity;
			if (Quantity == 1)
				fprintf(out, "%d %s was added to the table number %d\n", Quantity, ProductName, TableNumber);
			else fprintf(out, "%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber);
			printf("add %d %s to table %d\n", Quantity, ProductName, TableNumber);
		}
		else {
			fputs("The quantity of the product in the kitchen is not enough\n", out);
			printf("the quantity is big\n");
		}
	}
	else {
		fprintf(out, "We don't have %s, sorry!\n", ProductName);
		printf("there is no %s\n", ProductName);
	}
}

int ProductIsOnTable(char* prod_name, PITable Table) {
	PITable temp = Table; // start from begining
	while (temp != NULL) { // searching in evvery item on table
		if (strcmp(prod_name, temp->ItemName) == 0) // searching for the name of the product on table
			return 1;
		temp = temp->next;
	}
	return 0;
}

void RemoveItem(PProd ItemInKitchenHead, PITable* TablesArray, int TableNumber, char* ProductName, int Quantity, FILE* out) {
	PITable temptableitem = TablesArray[TableNumber - 1];
	PProd tempkitchenitem = ItemInKitchenHead;
	if (ProductIsOnTable(ProductName, TablesArray[TableNumber - 1])) {
		while (strcmp(temptableitem->ItemName, ProductName) != 0)
			temptableitem = temptableitem->next;
		while (strcmp(tempkitchenitem->ProductName, ProductName) != 0)
			tempkitchenitem = tempkitchenitem->next;
		if (temptableitem->Quantity >= Quantity) {
			temptableitem->Quantity -= Quantity;
			if (temptableitem->Quantity == 0) {
				if (temptableitem->prev != NULL) {
					if (temptableitem->next != NULL)
						temptableitem->next->prev = temptableitem->prev;
					temptableitem->prev->next = temptableitem->next;
					free(temptableitem);
				}
				else {
					TablesArray[TableNumber - 1] = TablesArray[TableNumber - 1]->next;
					if (TablesArray[TableNumber - 1] != NULL)
						TablesArray[TableNumber - 1]->prev = NULL;
					free(temptableitem);
				}
			}
			if (Quantity == 1)
				fprintf(out, "%d %s was returned to the kitchen from table number %d\n", Quantity, ProductName, TableNumber);
			else fprintf(out, "%d %s were returned to the kitchen from table number %d\n", Quantity, ProductName, TableNumber);
			printf("%d %s returned to kitchen from table %d\n", Quantity, ProductName, TableNumber);
		}
		else {
			fputs("The quantity to remove is bigger than on table\n", out);
			printf("the quantity to remove is too big\n");
		}
	}
	else {
		if (ProductExists(ItemInKitchenHead, ProductName)) {
			fputs("This product is not on the table!\n", out);
			printf("product is not on table\n");
		}
		else {
			fputs("This product is not on the menu!\n", out);
			printf("product is not on menu\n");
		}
	}
}

void RemoveTable(PProd KitchenProductsList, PITable* TablesArray, int TableNumber, int arrlen, FILE* out) {
	int maxtimes = 0, OverAllToPay;
	char tempmaxname[51] = { '\0' };
	char* maxname;
	PITable temptableitempointer = TablesArray[TableNumber - 1];
	if (TablesArray[TableNumber - 1] == NULL) {
		fprintf(out, "The table number %d is not ordered yet\n", TableNumber);
		printf("the table %d is not ordered yet\n", TableNumber);
	}
	else {
		while (temptableitempointer != NULL) {
			fprintf(out, "%d %s", temptableitempointer->Quantity, temptableitempointer->ItemName);
			printf("%d %s", temptableitempointer->Quantity, temptableitempointer->ItemName);
			temptableitempointer = temptableitempointer->next;
		}
		OverAllToPay = TableToPay(TablesArray[TableNumber - 1], KitchenProductsList);
		fprintf(out, ". %d nis, please!\n", OverAllToPay);
		printf(". %d nis, please!\n", OverAllToPay);
		FreeTable(&TablesArray[TableNumber - 1]);
		if (ResturantIsEmpty(TablesArray, arrlen)) {
			/**tempmaxname = CheckTheMostOrderd(TablesArray, KitchenProductsList, &maxtimes, arrlen);
			maxname = (char*)malloc(strlen(tempmaxname));
			fprintf(out, "The most popular dish today is %s! (Was ordered %d times)\n", maxname, maxtimes);
			printf("the most popular dish is %s was ordered %d times\n", maxname, maxtimes);*/
			CheckTheMostOrderd(TablesArray, KitchenProductsList, arrlen, out);
		}
	}
}
void FreeResturant(PITable* TablesArray, PProd ProductsInKitchen, int arrlen) {
	PITable temp = NULL;
	int i;
	FreeProductList(ProductsInKitchen);
	for (i = 0; i < arrlen; i++) {
		temp = TablesArray[i];
		while (temp != NULL) {
			TablesArray[i] = TablesArray[i]->next;
			temp->next = NULL;
			temp->prev = NULL;
			free(temp);
			temp = TablesArray[i];
		}
	}
	free(TablesArray);
}

void FreeProductList(PProd prodlisthead) {
	PProd temp = prodlisthead;
	while (temp != NULL) {
		free(prodlisthead->ProductName);
		temp = temp->next;
		prodlisthead->next = NULL;
		free(prodlisthead);
		prodlisthead = temp;
	}
}

void FreeTable(PITable* Table) {
	PITable temp;
	while (*Table != NULL) {
		temp = *Table;
		*Table = (*Table)->next;
		if ((*Table) != NULL)
			(*Table)->prev = NULL;
		free(temp->ItemName);
		free(temp);
	}
}

int ResturantIsEmpty(PITable* TablesArray, int arrlen) {
	PITable temp;
	int i;
	for (i = 0; i < arrlen; i++)
		if (TablesArray[i] != NULL)
			return 0;
	return 1;
}

void CheckTheMostOrderd(PITable* TablesArray, PProd KitchenList, int arrlen, FILE* out) {
	PProd temp = KitchenList;
	char* tempname;
	int temptimes;
	temptimes = temp->NumberOfTimesOrdered;
	tempname = (char*)malloc(strlen(temp->ProductName));
	if (tempname == NULL) {
		printf("\nfailed to allocate memory!");
		FreeResturant(TablesArray, KitchenList, arrlen);
		exit(1);
	}
	strcpy(tempname, temp->ProductName);
	temp = temp->next;
	while (temp != NULL) {
		if (temptimes < temp->NumberOfTimesOrdered) {
			temptimes = temp->NumberOfTimesOrdered;
			tempname = (char*)realloc(tempname, strlen(temp->ProductName));
			if (tempname == NULL) {
				printf("\nfailed to allocate memory!");
				FreeResturant(TablesArray, KitchenList, arrlen);
				exit(1);
			}
			strcpy(tempname, temp->ProductName);
		}
		temp = temp->next;
	}
	fprintf(out, "The most popular dish today is %s! (Was ordered %d times)\n", tempname, temptimes);
	printf("the most popular dish is %s was ordered %d times\n", tempname, temptimes);
}
int TableToPay(PITable Table, PProd kitchenlist)//returns how much the table pointed at should pay
{
	int sum = 0, tempq, tempp;
	PITable temp = Table;
	PProd tempk = kitchenlist;
	while (temp != NULL) {
		while (strcmp(temp->ItemName, tempk->ProductName) != 0)
			tempk = tempk->next;
		tempq = temp->Quantity;
		tempp = tempk->price;
		sum += tempq * tempp;
		temp = temp->next;
		tempk = kitchenlist;
	}
	return sum;
}
