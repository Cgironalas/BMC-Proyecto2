//Para recorrer tabla final
struct matrix *rowPtr = valuesTable;
while (rowPtr != NULL) {
	struct node *valuePtr = rowPtr->value;
	while(valuePtr != NULL){
		valuePtr->arrow; //valor de la flecha
		valuePtr->nValue; //valor de la casilla

		valuePtr = valuePtr->next; //siguiente valor de la fila actual
	}
	rowPtr = rowPtr->next;
}

int String1[100];
int String2[100];
int alignment[200];
int table1[101][101];	//contains values
int table2[101][101];	//contains arrows

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

void alignStrings(bool nw){//da prioridad a diagonales
	//Initialize tables
	int val;

	//table1[0][0] = 0;
	//table2[0][0] = 76;

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



//struct aNode{
	//	int value;
	//	struct aNode* next;
	//	struct aNode* prev;
	//}; //Arrow node

//struct amatrix{
	//	struct aNode* value;
	//	struct amatrix* next;
	//	struct amatrix* prev;
	//}; //Arrows matrix

//struct aNode *aTemp1;// = (struct aNode*) malloc(sizeof(struct aNode));

//struct amatrix *arrowsTable = NULL;

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

/*void printMatrix(struct Matrix *head){
	struct Matrix *ptr = head;
	printf("Values table:\n");
	while (ptr != NULL) {
		printCharList(ptr->value);
		ptr = ptr->next;
	}
	printf("\n");
}*/

void setValues(int i, int j, bool nw){
	int v1 = table1[i - 1][j - 1] + checkChars(i, j);
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