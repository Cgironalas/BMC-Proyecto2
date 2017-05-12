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
struct node *alignmentResult1 = NULL;
struct node *alignmentResult2 = NULL;

//Table of values and arrows
struct matrix *table = NULL;

//Auxiliary variables for the table
struct matrix *tableCurrRow = NULL;
struct node *tableCurrVal = NULL;
struct node *tableUpVal = NULL;

int m = 1;		//match weight
int mm = -1;	//mismatch weight
int gap = -2;	//gap weight

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
	struct node *aux = (struct node*) malloc(sizeof(struct node));
	aux->cValue = '\0';
	aux->nValue = value;
	aux->arrow[0] = arrows[0];
	aux->arrow[1] = arrows[1];
	aux->arrow[2] = arrows[2];
	aux->next = NULL;
	aux->prev = NULL;
	aux->down = NULL;
	aux->up = NULL;

	struct matrix *nRow = (struct matrix*) malloc(sizeof(struct matrix));
	nRow->value = aux;
	nRow->next = NULL;
	nRow->prev = tableCurrRow;

	tableCurrRow->next = nRow;
	tableUpVal = tableCurrRow->value;
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
	aux->prev = NULL;
	aux->down = NULL;
	aux->up = NULL;

	if(table == NULL){
		printf("Create table\n");
		struct matrix *nTable = (struct matrix*) malloc(sizeof(struct matrix));
		nTable->value = aux;
		nTable->next = NULL;
		nTable->prev = NULL;

		table = nTable;
		tableCurrRow = table;
		tableCurrVal = tableCurrRow->value;
	} else {
		if(tableUpVal != NULL) { tableUpVal->down = aux; }
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

	//struct node *tempCurrVal2 = tableCurrVal->down;
	//struct node *tempUpVal2 = tableUpVal->down;

	struct node *i2 = i;
	while(i2 != NULL){
		setValues(i2, j, tempCurrVal, tempUpVal, nw);
		i2 = i2->next;

		tempCurrVal = tempCurrVal->next;
		tempUpVal = tempUpVal->next;
	}
	printMatrix(table);/*
	printf("HERE\n");

	struct node *j2 = j->next;
	while (j2 != NULL) {
		printf("%c, %c\n", i->cValue, j2->cValue);
		setValues(i, j2, tempCurrVal2, tempUpVal2, nw);
		j2 = j2->next;

		tempCurrVal2 = tempCurrVal2->down;
		tempUpVal2 = tempUpVal2->down;
	}
	printMatrix(table);
	printf("HERE2\n");*/
}

void fillCurrentRow(bool arrows[3], bool nw){
	int val = -2;
	bool temp[3];
	temp[0] = arrows[0];
	temp[1] = arrows[1];
	temp[2] = arrows[2];
	struct node *currentUp;
	struct node *current = str1Head;

	if(tableUpVal != NULL) { currentUp = tableUpVal; }
	else { currentUp = NULL; }

	while (current != NULL) {
		if (nw) {
			setTableValue(temp, val);
			val += gap;
		} else {
			temp[0] = false;
			temp[1] = false;
			temp[2] = false;
			setTableValue(temp, 0);
		}

		if (currentUp != NULL ) { currentUp = currentUp->next; }

		current = current->next;
	}
}

void alignStrings(bool nw){
	//Initialize table
	int value = 0;
	bool arrows[3] = {0, 0, 0};

	//Adds value of table[0][0]
	setTableValue(arrows, value);
	//printMatrix(table);

	arrows[2] = true;
	fillCurrentRow(arrows, nw);
	//printMatrix(table);

	int val = -2;
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
	printMatrix(table);

	//Begin filling of diagonals
	str1Curr = str1Head;
	str2Curr = str2Head;

	tableCurrRow = table;
	//tableUpVal = tableCurrRow->value;

	//tableCurrRow = tableCurrRow->next;
	//tableCurrVal = tableCurrRow->value;

	printf("Here\n");
	
	while (str2Curr != NULL) {
		tableUpVal = tableCurrRow->value;

		tableCurrRow = tableCurrRow->next;
		tableCurrVal = tableCurrRow->value;
		printf("This1\n");
		fillDiagonal(str1Curr, str2Curr, nw);
		printf("This2\n");
		str1Curr = str1Head;
		str2Curr = str2Curr->next;
		printf("This3\n");
		//tableUpVal = tableCurrRow->value;
		//tableCurrRow = tableCurrRow->next;
		//tableCurrVal = tableCurrRow->value;
		//printf("This4\n");
	}
}