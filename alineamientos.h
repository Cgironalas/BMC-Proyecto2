#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int usefullData = 0;

struct node{
	int arrow;
	int nValue;
	char cValue;
	struct node* next;
	struct node* prev;
	struct node* down;
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
		printf("%i, ", ptr->nValue);
		if(ptr->arrow == 0){ printf("d - "); }
		else if (ptr->arrow == -1) { printf("l - "); }
		else if (ptr->arrow == 1) { printf("u - "); }
		else { printf("n - "); }
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

void setValues(struct node *i, struct node *j, bool nw){
	//int v1 = table1[i - 1][j - 1] + checkChars(i, j);
	int v1;

	//int v2 = table1[i][j - 1] + gap; 	//backwards arrow
	int v2;

	//int v3 = table1[i - 1][j] + gap;	//upwards arrow
	int v3;
	
	int maxVal = max3(v1,v2,v3);
	if (!nw) { maxVal = max(maxVal, 0); }
	//table1[i][j] = maxVal;

	//if (maxVal == v1) { table2[i][j] = 0; }
	//else if (maxVal == v2) { table2[i][j] = -1; }
	//else if (maxVal == v3) { table2[i][j] = +1; }
	//else { table2[i][j] = 76; } //solo puede llegar si sw
}

void fillDiagonal(struct node *i, struct node *j, bool nw){
	setValues(i, j, nw);
	struct node *i2 = i;
	while(i2 != NULL){
		setValues(i2, j, nw);
		i2 = i2->next;
	}
	struct node *j2 = j;
	while (j2 != NULL) {
		setValues(i, j2, nw);
		j2 = j2->next;
	}
}


void fillCurrentRow(struct matrix *prev, bool nw){
	int val = 0;
	struct node *temp = (struct node*) malloc(sizeof(struct node));

	while (str1Curr != NULL) {
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
		temp->down = NULL;
		temp->cValue = '\0';
		tableCurrVal->next = temp;
		tableCurrVal = tableCurrVal->next;

		str1Curr = str1Curr->next;
	}
}

void alignStrings(bool nw){//da prioridad a diagonales
	//Initialize tables
	int val;

	temp = (struct node*) malloc(sizeof(struct node));
	temp->cValue = '\0';
	temp->nValue = 0;
	temp->arrow = 76;
	temp->next = NULL;
	temp->prev = NULL;
	temp->down = NULL;

	tempM = (struct matrix*) malloc(sizeof(struct matrix));
	tempM->value = temp;
	tempM->next = NULL;
	tempM->prev = NULL;

	table = tempM;

	val = 0;
	str1Curr = str1Head;
	tableCurrVal = table->value;
	fillCurrentRow(NULL, nw);
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
			temp->nValue = val;
			temp->arrow = 1;
		} else {
			temp->nValue = 0;
			temp->arrow = 76;
		}

		temp->next = NULL;
		temp->prev = NULL;

		tempM->value = temp;
		tempM->next = NULL;
		tempM->prev = NULL;

		tableCurrRow->next = tempM;
		tableCurrRow = tableCurrRow->next;

		str2Curr = str2Curr->next;
	}

	//Begin parallel filling of diagonals
	str1Curr = str1Head;
	str2Curr = str2Head;

	tableCurrRow = table;
	tableCurrVal = tableCurrRow->value;
	tableCurrVal2 = tableCurrRow->next->value;

	while (str1Curr != NULL && str2Curr != NULL) {
		fillDiagonal(str1Curr, str2Curr, nw);

		str1Curr = str1Curr->next;
		str2Curr = str2Curr->next;
		
	}
}