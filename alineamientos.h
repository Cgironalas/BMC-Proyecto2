#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define SIZE1 100
#define SIZE2 100

int usefullData = 0;

struct node{
	int arrow;
	int nValue;
	char cValue;
	struct node* next;
	struct node* prev;
};
struct matrix{
	struct node* value;
	struct matrix* next;
	struct matrix* prev;
};

struct node *string1Head = NULL;
struct node *string1Current = NULL;

struct node *string2Head = NULL;
struct node *string2Current = NULL;

struct node *alignmentResult = NULL;

struct matrix *valuesTable = NULL;
struct matrix *valuesTableCurrentRow = NULL;
struct node *valuesTableCurrentValue = NULL;
struct node *valuesTableCurrentValue2 = NULL;

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
	if (i->nValue == j->nValue) { return m; }
	else { return mm; }
}

void setValues(struct node *i, struct node *j, bool nw){
	//int v1 = table1[i - 1][j - 1] + checkChars(i, j);

	//int v2 = table1[i][j - 1] + gap; 	//backwards arrow
	
	//int v3 = table1[i - 1][j] + gap;	//upwards arrow
	
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

void alignStrings(bool nw){//da prioridad a diagonales
	//Initialize tables
	int val;

	tempM = (struct matrix*) malloc(sizeof(struct matrix));
	temp = (struct node*) malloc(sizeof(struct node));
	temp->cValue = '\0';
	temp->nValue = 0;
	temp->arrow = 76;
	temp->next = NULL;
	temp->prev = NULL;

	tempM->value = temp;
	tempM->next = NULL;
	tempM->prev = NULL;

	valuesTable = tempM;

	val = 0;
	string1Current = string1Head;
	valuesTableCurrentValue = valuesTable->value;
	
	while (string1Current != NULL) {
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
		valuesTableCurrentValue->next = temp;
		valuesTableCurrentValue = valuesTableCurrentValue->next;

		string1Current = string1Current->next;
	}

	val = 0;
	string2Current = string2Head;
	valuesTableCurrentRow = valuesTable;
	while (string2Current != NULL) {
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

		valuesTableCurrentRow->next = tempM;
		valuesTableCurrentRow = valuesTableCurrentRow->next;

		string2Current = string2Current->next;
	}

	//Begin parallel filling of diagonals
	string1Current = string1Head;
	string2Current = string2Head;

	valuesTableCurrentRow = valuesTable;
	valuesTableCurrentValue = valuesTableCurrentRow->value;
	valuesTableCurrentValue2 = valuesTableCurrentRow->next->value;

	while (string1Current != NULL && string2Current != NULL) {
		fillDiagonal(string1Current, string2Current, valuesTableCurrentValue, valuesTableCurrentValue2 nw);

		string1Current = string1Current->next;
		string2Current = string2Current->next;
		valuesTableCurrentValue = 
	}
}


