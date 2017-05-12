#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int usefullData = 0;

struct node{
	int nValue;
	char cValue;
	bool arrow[3];
	struct node* next;
	struct node* prev;
	struct node* down;
	struct node* up;
};
struct matrix{
	struct node* value;
	struct matrix* next;
	struct matrix* prev;
};

//First string
struct node *str1Head = NULL;
struct node *str1Curr = NULL;

//Second string
struct node *str2Head = NULL;
struct node *str2Curr = NULL;

//Aligned strings
struct matrix *alignRes1 = NULL;
struct matrix *alignRes2 = NULL;

//Table of values and arrows
struct matrix *table = NULL;

//Auxiliary variables for the table
struct matrix *tableCurrRow = NULL;
struct node *tableCurrVal = NULL;
struct node *tableUpVal = NULL;

int m = 1;		//match weight
int mm = -1;	//mismatch weight
int gap = -1;	//gap weight

//Assign match, mismatch and gapenalty values
void setWeight(int mValue, int mmValue, int gapValue){
	m = mValue;
	mm = mmValue;
	gap = gapValue;
}

//Print list as string
void printCharList(struct node *head){
	struct node *ptr = head;
	while(ptr != NULL) {
		printf("%c ", ptr->cValue);
		ptr = ptr->next;
	}
	printf("\n");
	free(ptr);
}
//Print list as values table row
void printValList(struct node *head) {
	struct node *ptr = head;
	while(ptr != NULL) {
		printf("(%i,{", ptr->nValue);
		if(ptr->arrow[0] == true){ printf("d "); }
		if (ptr->arrow[1] == true) { printf("u "); }
		if (ptr->arrow[2] == true) { printf("l"); }
		printf("}) * ");
		ptr = ptr->next;
	}
	printf("\n");
	free(ptr);
}
//Print values table
void printMatrix(struct matrix *head) {
	struct matrix *ptr = head;
	printf("Values table:\n");
	while (ptr != NULL) {
		printValList(ptr->value);
		ptr = ptr->next;
	}
	printf("\n");
	free(ptr);
}

//Min max functions
int min(int i1, int i2){
	if (i1 < i2){ return i1; }
	else { return i2; }
}
int max(int i1, int i2){
	if (i1 > i2){ return i1; }
	else { return i2; }
}
int max3(int i1, int i2, int i3){ return max(max(i1, i2), i3); }

//Insert values to first string
void insertStr1(char c) {
	struct node *aux = (struct node*) malloc(sizeof(struct node));
	aux->cValue = c;
	aux->nValue = 0;
	aux->arrow[0] = false;
	aux->arrow[1] = false;
	aux->arrow[2] = false;
	aux->next = NULL;
	aux->prev = NULL;
	aux->down = NULL;

	if(str1Head == NULL){
		//printf("Insert first\n");
		aux->next = str1Head;
		str1Head = aux;
		str1Curr = str1Head;
	} else {
		//printf("Insert any\n");
		aux->prev = str1Curr;
		str1Curr->next = aux;
		str1Curr = str1Curr->next;
	}
}
//Insert values to second string
void insertStr2(char c) {
	struct node *aux = (struct node*) malloc(sizeof(struct node));
	aux->cValue = c;
	aux->nValue = 0;
	aux->arrow[0] = false;
	aux->arrow[1] = false;
	aux->arrow[2] = false;
	aux->next = NULL;
	aux->prev = NULL;
	aux->down = NULL;
	aux->up = NULL;

	if(str2Head == NULL){
		//printf("Insert first\n");
		aux->next = str2Head;
		str2Head = aux;
		str2Curr = str2Head;
	} else {
		//printf("Insert any\n");
		aux->prev = str2Curr;
		str2Curr->next = aux;
		str2Curr = str2Curr->next;
	}
}

//Add a row to the table
void insertRow(bool arrows[3], int value){
	tableUpVal = tableCurrRow->value;

	struct node *aux = (struct node*) malloc(sizeof(struct node));
	aux->cValue = '\0';
	aux->nValue = value;
	aux->arrow[0] = arrows[0];
	aux->arrow[1] = arrows[1];
	aux->arrow[2] = arrows[2];
	aux->next = NULL;
	aux->prev = NULL;
	aux->down = NULL;
	aux->up = tableUpVal;

	struct matrix *nRow = (struct matrix*) malloc(sizeof(struct matrix));
	nRow->value = aux;
	nRow->next = NULL;
	nRow->prev = tableCurrRow;

	tableCurrRow->next = nRow;
	tableCurrRow = tableCurrRow->next;
	tableCurrVal = tableCurrRow->value;
}

void setTableValue2(bool arrows[3], int value, struct node *tempCurr) {
	tempCurr->next->nValue = value;
	tempCurr->next->arrow[0] = arrows[0];
	tempCurr->next->arrow[1] = arrows[1];
	tempCurr->next->arrow[2] = arrows[2];
}

//Set values to table
void setTableValue(bool arrows[3], int value) {
	struct node *aux = (struct node*) malloc(sizeof(struct node));
	aux->cValue = '\0';
	aux->nValue = value;
	aux->arrow[0] = arrows[0];
	aux->arrow[1] = arrows[1];
	aux->arrow[2] = arrows[2];
	aux->next = NULL;
	aux->prev = tableCurrVal;
	aux->down = NULL;

	if(table == NULL){
		printf("Create table\n");
		struct matrix *nTable = (struct matrix*) malloc(sizeof(struct matrix));
		nTable->value = aux;
		nTable->next = NULL;
		nTable->prev = NULL;

		table = nTable;
		tableCurrRow = table;
		tableCurrVal = tableCurrRow->value;
		tableUpVal = NULL;
	} else {
		if(tableUpVal != NULL) { tableUpVal = tableUpVal->next; tableUpVal->down = aux; }
		aux->up = tableUpVal;

		//aux->down = tableCurrVal;
		tableCurrVal->next = aux;
		tableCurrVal = tableCurrVal->next;
	}
}

//Check current string chars for match or mismatch
int checkChars(struct node *i, struct node *j){
	if (i->cValue == j->cValue) { return m; }
	else { return mm; }
}

void setValues(struct node *i, struct node *j, struct node *tempCurr, struct node *tempUp, bool nw){
	int v1 = tempUp->nValue + checkChars(i, j);
	int v2 = tempUp->next->nValue + gap;
	int v3 = tempCurr->nValue + gap;
	
	int maxVal = max3(v1, v2, v3);
	if (!nw) { maxVal = max(maxVal, 0); }

	bool arrows[3];
	//Diagonal arrow
	if (maxVal == v1) { arrows[0] = true; }
	else { arrows[0] = false; }

	//Upwards arrow
	if (maxVal == v2) { arrows[1] = true; }
	else { arrows[1] = false; }

	//Backwards arrow
	if (maxVal == v3) { arrows[2] = true; }
	else { arrows[2] = false; }

	setTableValue2(arrows, maxVal, tempCurr);
}

void fillDiagonal(struct node *i, struct node *j, bool nw){
	struct node *tempCurrVal = tableCurrVal;
	struct node *tempUpVal = tableUpVal;

	struct node *i2 = i;
	while(i2 != NULL){
		setValues(i2, j, tempCurrVal, tempUpVal, nw);
		i2 = i2->next;

		tempCurrVal = tempCurrVal->next;
		tempUpVal = tempUpVal->next;
	}
}

void fillCurrentRow(bool arrows[3], bool nw){
	int val = gap;
	bool temp[3];
	struct node *current = str1Head;

	while (current != NULL) {
		if (nw) {
			temp[0] = arrows[0];
			temp[1] = arrows[1];
			temp[2] = arrows[2];
			setTableValue(temp, val);
			val += gap;
		} else {
			temp[0] = false;
			temp[1] = false;
			temp[2] = false;
			setTableValue(temp, 0);
		}

		current = current->next;
	}
}

bool checkCurrentIsHead(){
	if(tableCurrVal->prev == NULL && tableCurrVal->up == NULL){
		return false;
	} else {
		return false;
	}
}

void insertAlign(char c, bool i){
	struct node *first = (struct node*) malloc(sizeof(struct node));
	first->cValue = c;
	first->nValue = 0;
	first->arrow[0] = false;
	first->arrow[1] = false;
	first->arrow[2] = false;
	
	first->prev = NULL;
	first->down = NULL;
	first->up = NULL;
	if (i) {
		first->next = alignRes1->value;
		alignRes1->value = first;
	} else {
		first->next = alignRes2->value;
		alignRes2->value = first;
	}
}

void setResults(bool nw){
	str1Curr = str1Head;
	while(str1Curr->next != NULL){ str1Curr = str1Curr->next; }

	str2Curr = str2Head;
	while(str2Curr->next != NULL){ str2Curr = str2Curr->next; }

	tableCurrRow = table;
	while(tableCurrRow->next != NULL) { tableCurrRow = tableCurrRow->next; }

	tableCurrVal = tableCurrRow->value;
	while(tableCurrVal->next != NULL) { tableCurrVal = tableCurrVal->next; }

	//printf("Last Cell: ");
	//printValList(tableCurrVal);

	alignRes1 = (struct matrix*) malloc(sizeof(struct matrix));
	alignRes1->next = NULL;
	alignRes1->prev = NULL;
	
	alignRes2 = (struct matrix*) malloc(sizeof(struct matrix));
	alignRes2->next = NULL;
	alignRes2->prev = NULL;

	while(str1Curr != NULL && str2Curr != NULL) {
		//printf("\nChars: %c, %c\n", str1Curr->cValue, str2Curr->cValue);
		//printf("CurrentCell: ");
		//printValList(tableCurrVal);
		if (nw){
			if(tableCurrVal->arrow[0] == true) {
				insertAlign(str1Curr->cValue, true);
				insertAlign(str2Curr->cValue, false);
				str1Curr = str1Curr->prev;
				str2Curr = str2Curr->prev;
				tableCurrVal = tableCurrVal->up->prev;
			} else if(tableCurrVal->arrow[1] == true) {
				insertAlign('_', true);
				insertAlign(str2Curr->cValue, false);
				str2Curr = str2Curr->prev;
				tableCurrVal = tableCurrVal->up;
			} else {
				insertAlign(str1Curr->cValue, true);
				insertAlign('_', false);
				str1Curr = str1Curr->prev;
				tableCurrVal = tableCurrVal->prev;
			}
		} else {

		}
	}
	printf("List 1: ");
	printCharList(alignRes1->value);
	printf("\nList 2: ");
	printCharList(alignRes2->value);
}

void alignStrings(bool nw){
	//Initialize table
	int value = 0;
	bool arrows[3] = {0, 0, 0};

	//Adds value of table[0][0]
	setTableValue(arrows, value);

	arrows[2] = true;
	fillCurrentRow(arrows, nw);

	int val = gap;
	str2Curr = str2Head;
	while (str2Curr != NULL) {
		if (nw) {
			value = val;
			arrows[0] = false;
			arrows[1] = true;
			arrows[2] = false;
			val += gap;
		} else {
			value = 0;
			arrows[0] = false;
			arrows[1] = false;
			arrows[2] = false;
		}
		insertRow(arrows, value);

		arrows[0] = false;
		arrows[1] = false;
		arrows[2] = false;
		fillCurrentRow(arrows, false);

		str2Curr = str2Curr->next;
	}

	str1Curr = str1Head;
	str2Curr = str2Head;

	tableCurrRow = table;
	
	while (str2Curr != NULL) {
		tableUpVal = tableCurrRow->value;
		tableCurrRow = tableCurrRow->next;
		tableCurrVal = tableCurrRow->value;
		fillDiagonal(str1Curr, str2Curr, nw);
		str1Curr = str1Head;
		str2Curr = str2Curr->next;
	}
	printMatrix(table);
	setResults(nw);
}