#include "kitchenheader.h"

int main()
{
	FILE* out, * in;
	PProd TheKitchenList;
	PITable* Tables;
	int NumOfTables = 0, ToDo = 0, tempquantity = 0, temptablenum = 0, i;
	char tempname[51] = { '\0' }, tempname2[51] = { '\0' };
	in = fopen("Instructions.txt", "rt");//opening file to read
	if (in == NULL) {
		printf("\nfailed to open file!");
		exit(1);
	}
	out = fopen("output.txt", "wt");//opening file to write
	if (out == NULL) {
		printf("\nfailed to open file!");
		exit(1);
	}
	fscanf(in, "%d", &NumOfTables);//getting number of tabels
	Tables = (PITable*)malloc(NumOfTables * sizeof(PITable));//creating the tables
	if (Tables == NULL) {
		printf("\nfailed to allocate memory!");
		exit(1);
	}
	for (i = 0; i < NumOfTables; i++) {
		Tables[i] = NULL;
	}

	fscanf(in, "%d", &ToDo);
	TheKitchenList = CreateProducts(out);//calling function

	while (!feof(in)) {
		fscanf(in, "%d", &ToDo);
		if (ToDo == 2) {
			fscanf(in, "%s %d", tempname, &tempquantity);
			AddItem(TheKitchenList, tempname, tempquantity, out);//calling function
		}
		else {
			if (ToDo == 3) {
				fscanf(in, "%d %s %d", &temptablenum, tempname, &tempquantity);
				OrderItem(TheKitchenList, Tables, temptablenum, tempname, tempquantity, NumOfTables, out);//call function
			}
			else {
				if (ToDo == 4) {
					fscanf(in, "%d %s %d", &temptablenum, tempname, &tempquantity);
					RemoveItem(TheKitchenList, Tables, temptablenum, tempname, tempquantity, out);//calling function
				}
				else {
					if (ToDo == 5) {
						fscanf(in, "%d", &temptablenum);
						RemoveTable(TheKitchenList, Tables, temptablenum, NumOfTables, out);//calling function
					}
				}
			}
		}
		strcpy(tempname, tempname2);
		tempquantity = 0;
		temptablenum = 0;
	}
	FreeResturant(Tables, TheKitchenList, NumOfTables);//calling function
	fclose(in);
	fclose(out);
	return 0;
}