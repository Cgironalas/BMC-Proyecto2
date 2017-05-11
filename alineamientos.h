#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define SIZE1 100
#define SIZE2 100

FILE * fileTableData;
char buffer[9];
int totalGenCount;
int usefullData = 0;

struct sNode{
	char value;
	struct sNode* next;
	struct sNode* prev;
}; //Value node

struct sMatrix{
	struct sNode* value;
	struct sMatrix* next;
	struct sMatrix* prev;
}; //Values matrix

struct aNode{
	int value;
	struct aNode* next;
	struct aNode* prev;
}; //Arrow node

struct aMatrix{
	struct aNode* value;
	struct aMatrix* next;
	struct aMatrix* prev;
}; //Arrows matrix

struct sNode *string1Head = NULL;
struct sNode *string2Head = NULL;
struct sNode *alignmentResult = NULL;

struct sNode *sTemp1;// = (struct sNode*) malloc(sizeof(struct sNode));
struct sNode *sTemp2;// = (struct sNode*) malloc(sizeof(struct sNode));
struct aNode *aTemp1;// = (struct aNode*) malloc(sizeof(struct aNode));
struct aNode *aTemp2;// = (struct aNode*) malloc(sizeof(struct aNode));

struct sMatrix *valuesTable = NULL;
struct aMatrix *arrowsTable = NULL;

int m = 1;			//match weight
int mm = -1;		//mismatch weight
int gap = -2;		//gap weight

int String1[100];
int String2[100];
int alignment[200];
int table1[101][101];	//contains values
int table2[101][101];	//contains arrows

void alignStrings(bool nw){//da prioridad a diagonales
	//Initialize tables
	int val;
	struct sMatrix *firstSM = (struct sMatrix*) malloc(sizeof(struct sMatrix));
	sTemp1 = (struct sNode*) malloc(sizeof(struct sNode));
	sTemp1->value = 0;
	sTemp1->next = NULL;
	sTemp1->prev = NULL;

	firstSM->value = sTemp1;
	firstSM->next = valuesTable;
	firstSM->prev = NULL;

	valuesTable = firstSM;

	struct aMatrix *firstAM = (struct aMatrix*) malloc(sizeof(struct aMatrix));
	aTemp1 = (struct aNode*) malloc(sizeof(struct aNode));
	aTemp1->value = 76;
	aTemp1->next = NULL;
	aTemp1->prev = NULL;

	firstAM->value = aTemp1;
	firstAM->next = arrowsTable;
	firstAM->prev = NULL;

	arrowsTable = firstAM;

	table1[0][0] = 0;
	table2[0][0] = 76;

	val = 0;
	for (int i = 1; i <= SIZE1; i++) {
		if (nw) {
			table1[i][0] = val;
			table2[i][0] = 1; 	//upwards arrows
			val += mm;
		} else {
			table1[i][0] = 0;
			table2[i][0] = 76; 	//no arrow
		}
	}
	val = 0;
	for (int j = 1; j <= SIZE2; j++) {
		if (nw) {
			table1[0][j] = val;
			table2[0][j] = -1; 	//backwards arrow
			val += mm;
		} else {
			table1[0][j] = 0;
			table2[0][j] = 76; 	//no arrow
		}
	}

	//Begin parallel filling of diagonals
	int minSize = min(SIZE1, SIZE2);
	for(int diagonal = 1; diagonal <= minSize; diagonal++){
		fillDiagonal(diagonal, nw);
	}
}

/*Read Files*/
void fillBuffer(int _val) {
	if (strlen(buffer) == 0) {
		char dato= (char)_val;
		char auxiliar[] = {dato,'\0'};
		strcat(buffer,auxiliar);
	} else{
		char dato = (char)_val;
		char auxiliar[]={dato,'\0'};
		strcat(buffer,auxiliar);
	}
}

void cleanBuffer(){ memset(buffer,'\0',strlen(buffer)); }

int escaner() {
	cleanBuffer();

	int ch;

	while (feof(fileTableData)==0) {   
		ch = fgetc(fileTableData);
		if (ch=='\n') {
			return 0;
		}
		if (ch==';') {
		
			return 1;
		}
		fillBuffer(ch);

	}
	return 0;
}

int countObjectsFiles(char * address){
	fileTableData = fopen(address,"r");
	int ch;


	while(feof(fileTableData) == 0) {  
		ch = fgetc(fileTableData);
		if (ch == '\n'){
			
			break;
		}
		if (ch==';'){
			totalGenCount ++;
		}
	}

	
	
	fclose(fileTableData);
	return totalGenCount;
}

void readFile(char Data[totalGenCount][totalGenCount][5],char * address,char header[totalGenCount][30]){
	/*fileTableData = fopen(address,"r");
	int startData = 0;
	int c;
	int columns = 0;
	int rows= 0;
	while (rows<totalGenCount+1 && feof(fileTableData) == 0){
		while (columns < totalGenCount+1){
			c = escaner();
			if (startData==1 && c == 1){
				char * data = malloc(5);
	 			strcpy(data,buffer);
				strcpy(Data[rows-1][columns],data);		
			}
			if (startData==0 && c==1){
				strcpy(header[columns],buffer);
			}
			if (startData==0 && c == 0){
				startData = 1;
			}
			columns ++;

		}
		
		columns = 0;
		rows ++;	
	}*/
}

void printString(char string[]){
	int i = 0;
	while(string[i] != '\0'){
		printf("%c", string[i]);
		i++;
	}
}

void copyString(char str1[], char str2[]){
    for (int i = 0; i < SIZE1; i++){
        str1[i] = str2[i];
    }
}

void copyString2(char str1[], const char str2[]){
	for (int i = 0; i < SIZE1; i++) {
		str1[i] = str2[i];
	}
}

int compareDoubles(const void * a, const void * b){
	return ( *(double*)a - *(double*)b );
}

/* Compare structs
	int compareRelations(const void * a, const void *b){
		relation f = *((relation*)a);
		relation s = *((relation*)b);
		if(f.value > s.value) return -1;
		if(f.value < s.value) return 1;
		return 0;
	}
*/

void printSList(struct sNode *head){
	struct sNode *ptr = head;

	while(ptr != NULL) {
		printf("%c - ", ptr->value);
		ptr = ptr->next;
	}
	printf("\n");
}
void printSMatrix(struct sMatrix *head){
	struct sMatrix *ptr = head;
	printf("Values table:\n");
	while (ptr != NULL) {
		printSList(ptr->value);
		ptr = ptr->next;
	}
	printf("\n");
}
void printAList(struct aNode *head){
	struct aNode *ptr = head;

	while (ptr != NULL) {
		if(ptr->value == 0){ printf("d - "); }
		else if (ptr->value == -1) { printf("l - "); }
		else if (ptr->value == 1) { printf("u - "); }
		else { printf("n - "); }
		ptr = ptr->next;
	}
	printf("\n");
}
void printAMatrix(struct aMatrix *head) {
	struct aMatrix *ptr = head;
	printf("Arrow table:\n");
	while (ptr != NULL) {
		printAList(ptr->value);
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

int checkChars(int i, int j){
	if (String1[i] == String2[j]) { return m; }
	else { return mm; }
}

void setValues(int i, int j, bool nw){
	int v1 = table1[i - 1][j - 1] + checkChars(i - 1, j - 1);
	int v2 = table1[i][j - 1] + gap; 	//backwards arrow
	int v3 = table1[i - 1][j] + gap;	//upwards arrow
	
	int maxVal = max3(v1,v2,v3);
	if (!nw) { maxVal = max(maxVal, 0); }
	table1[i][j] = maxVal;

	if (maxVal == v1) { table2[i][j] = 0; }
	else if (maxVal == v2) { table2[i][j] = -1; }
	else if (maxVal == v3) { table2[i][j] = +1; }
	else { table2[i][j] = 76; } //solo puede llegar si sw
}

void fillDiagonal(int diagonal, bool nw){
	int i = diagonal;
	int j = diagonal;

	setValues(i, j, nw);
	for(int i2 = i; i2 <= SIZE1; i2++){
		setValues(i2, j, nw);
	}
	for(int j2 = j; j2 <= SIZE2; j2++){
		setValues(i, j2, nw);
	}
}

