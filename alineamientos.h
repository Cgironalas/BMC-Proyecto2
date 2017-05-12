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
struct node *tableCurrVal2 = NULL;

struct node *temp;
struct matrix *tempM;

int m = 1;		//match weight
int mm = -1;	//mismatch weight
int gap = -2;	//gap weight

//Assign match, mismatch and gapenalty values
void setWeight(int mValue, int mmValue, int gapValue){
	m = mValue;
	mm = mmValue;
	gap = gapValue;
}

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
	} else {

	}
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
		printf("%i, {", ptr->nValue);
		if(ptr->arrow[0] == true){ printf("d, "); }
		else { printf("n, "); }
		if (ptr->arrow[1] == true) { printf("l, "); }
		else { printf("n, "); }
		if (ptr->arrow[2] == true) { printf("u} - "); }
		else { printf("n} - "); }
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

int checkChars(struct node *i, struct node *j){
	if (i->cValue == j->cValue) { return m; }
	else { return mm; }
}

void setValues(struct node *i, struct node *j, struct node *tVal, bool nw){
	int v1 = tVal->nValue + checkChars(i, j);
	int v2 = tVal->next->nValue + gap;
	int v3 = tVal->down->nValue + gap;
	
	int maxVal = max3(v1, v2, v3);
	if (!nw) { maxVal = max(maxVal, 0); }

	bool arrows[3];
	//Diagonal arrow
	if (maxVal == v1) { arrows[0] = true; }
	else { arrows[0] = false; }

	//upwards arrow
	if (maxVal == v2) { arrows[1] = true; }
	else { arrows[1] = false; }

	//backwards arrow
	if (maxVal == v3) { arrows[2] = true; }
	else { arrows[2] = false; }

	setTableValue(arrows, maxVal);
}

void fillDiagonal(struct node *i, struct node *j, struct node *tVal, bool nw){
	setValues(i, j, tVal, nw);
	struct node *i2 = i;
	while(i2 != NULL){
		setValues(i2, j, tVal, nw);
		i2 = i2->next;
	}
	struct node *j2 = j;
	while (j2 != NULL) {
		setValues(i, j2, tVal, nw);
		j2 = j2->next;
	}
}


void fillCurrentRow(struct node *prev, struct node *row, bool arrows[3], bool nw){
	int val = 0;
	struct node *temp = (struct node*) malloc(sizeof(struct node));
	struct node *current = str1Head;
	struct node *tableVal = row;
	struct node *currentUp = prev;

	while (current != NULL) {
		temp->next = NULL;
		temp->prev = NULL;
		temp->down = NULL;
		temp->cValue = '\0';

		if (nw) {
			temp->nValue = val;
			temp->arrow[0] = arrows[0];
			temp->arrow[1] = arrows[1];
			temp->arrow[2] = arrows[2];
			val += mm;
			
		} else {
			temp->nValue = 0;
			temp->arrow[0] = 0;
			temp->arrow[1] = 0;
			temp->arrow[2] = 0;
		}

		prev->down = temp;
		if (currentUp != NULL ) { currentUp = currentUp->next; }

		tableVal->next = temp;
		tableVal = tableCurrVal->next;

		current = current->next;
	}
}

void alignStrings(bool nw){//da prioridad a diagonales
	//Initialize tables
	int val;
	int value = 0;
	bool arrows[3] = {0, 0, 0};

	setTableValue(arrows, value);
	printValList(table->value);

	val = 0;
	//str1Curr = str1Head;
	//tableCurrVal = table->value;
	arrows[2] = 1;
	fillCurrentRow(NULL, table->value, arrows, nw);
	/*
		while (string1Curr != NULL) {
			if (nw) {
				temp->nValue = val;
				temp->arrow = -1;
				val += mm;
			} else {
				temp->nValue = 0;
				temp->arrow = 76;
			}

			temp->next = NULL;
			temp->prev = NULL;
			tableCurrValue->next = temp;
			tableCurrValue = tableCurrValue->next;

			string1Curr = string1Curr->next;
		}
	*/

	val = 0;
	str2Curr = str2Head;
	tableCurrRow = table;
	while (str2Curr != NULL) {
		if (nw) {
			value = val;
			arrows[0] = false;
			arrows[1] = true;
			arrows[2] = false;
		} else {
			value = 0;
			arrows[0] = false;
			arrows[1] = false;
			arrows[2] = false;
		}

		temp->next = NULL;
		temp->prev = NULL;

		tempM->value = temp;
		tempM->next = NULL;
		tempM->prev = NULL;

		tableCurrRow->next = tempM;

		fillCurrentRow(tableCurrRow->next->value, tableCurrRow->value, arrows, nw);
		tableCurrRow = tableCurrRow->next;

		str2Curr = str2Curr->next;
	}

	//Begin filling of diagonals
	str1Curr = str1Head;
	str2Curr = str2Head;

	tableCurrRow = table;
	tableCurrVal = tableCurrRow->value;
	tableCurrVal2 = tableCurrRow->next->value;

	while (str1Curr != NULL && str2Curr != NULL) {
		fillDiagonal(str1Curr, str2Curr, tableCurrVal, nw);

		str1Curr = str1Curr->next;
		str2Curr = str2Curr->next;
		tableCurrVal = tableCurrVal->down->next;
		
	}
}