#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

typedef struct linked_list {
    char* data;
    struct linked_list* next;
}linked_list;

typedef linked_list ELEMENT;
typedef ELEMENT* LINK;

int isStrNumber(char*);
double** createMatrix(void);
int* verifyInput(int, char**);
int getVectorSize(char*);
int countElements(LINK);
LINK createList(void);
void printMat(double**);


#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct Centroid
{
    double* selfVector;
    int numOfVectors;
    double** relatedVectors;
}Centroid;

double** deepCopy2DArray(double** inputArray, int rows, int columns);
double* copyArray(double* inputArray, int rows);
double update(Centroid* centroid, int vectorsLength);
Centroid* calcClosestCentroid(double *vector, Centroid **centroids, int K, int vectorsLength);
double euclidianDistance(double *vector1, double *vector2, int vectorsLength);
double* copyArray(double* inputArray, int rows);
void zeroArray(double* array, int arrayLength);
double averageOf(Centroid* centroid, int i);
void freeRelatedVectors(Centroid* centroid);
double maxDelta(double *deltas, int numberOfVectors);
double** getCentroidsSelfVectors(Centroid* centroids, int K);
int countDigitsOfWholePart(double value);
double** kMeans(int K, int iter, int numberOfVectors, int vectorsLength, double eps, double** vectorsList);

int getVectorSize(char* str){
    int i;
    int size = 1;
    int len = (int) strlen(str);
    for(i=0; i < len; i++){
        if (str[i] == ','){
            size++;
        }
    }
    return size;
}

LINK createList(void){
    LINK head = NULL, tail = NULL;
    size_t len;
    char* buffer = NULL;
    size_t buffer_size = 0;
    printf("create list\n");
    if (getline(&buffer, &buffer_size, stdin) != -1){
        printf("0");
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        printf("1");
        head = (ELEMENT*)malloc(sizeof(ELEMENT));
        printf("2");
        head->data = buffer;
        tail = head;
        while (getline(&buffer, &buffer_size, stdin) != -1) {
            len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            tail->next = (ELEMENT*)malloc(sizeof(ELEMENT));
            tail = tail -> next;
            tail -> data = buffer;
        }
        tail -> next = NULL;
    }
    return head;
}

int countElements(LINK head){
    int cnt = 0;
    for (;head !=NULL; head = head->next){
        ++cnt;
    }
    return cnt;
}

double** createMatrix(void){
    double num;
    int i, j;
    char *word, *token;
    LINK head = NULL;
    LINK head = createList();
    int numOfVectors = countElements(head);
    int vectorSize = getVectorSize(head->data);
    double** mat = (double**) malloc(numOfVectors * sizeof(double*));
    for(i = 0; i < numOfVectors; i++){
        mat[i] = (double*) malloc(vectorSize * sizeof(double));
        j = 0;
        word = head -> data;
        token = strtok(word, ",");
        while (token != NULL) {
            num = strtod(token, NULL);
            mat[i][j] = num;
            j++;
            token = strtok(NULL, "'");
        }
        free(head);
        head = head -> next;
    }
    return mat;
}

int isStrNumber(char* str){
    int i;
    int length = strlen(str);
    if (str[0] == '0'){
        return 0;
    }
    for (i=1;i<length; i++){
        if (str[i] < '0' || str[i] > '9'){
            return 0;
        }
    }

    return 1;

}

int* verifyInput(int a, char **b){
    int* arr = calloc(2, sizeof(int));
    int k, iter;
    char* k1, *iter1;
    if (a < 2 || a > 3){
        printf("An Error Has Occurred");
        exit(1);
    }
    k1 = b[1];
    if(!isStrNumber(k1)){
        printf("invalid number of clusters!");
        exit(1);
    } else {
        k = atoi(k1);
        if(k <= 1 /*|| argv[1] >= mat.GetLength(0)*/){
            printf("invalid number of clusters!");
            exit(1);
        }
    }
    if (a == 3){
        iter1 = b[2];
        if(!isStrNumber(iter1)){
            printf("Invalid maximum iteration!");
            exit(1);
        } else {
            iter = atoi(iter1);
            if(iter <= 1 || iter >= 1000){
                printf("Invalid maximum iteration!");
                exit(1);
            }
        }
    } else {
        iter = 200;
    }
    arr[0] = k;
    arr[1] = iter;
    return arr;
}

void printMat(double** mat){
    int i;
    int j;
    for(i=0; i<3; i++) {
        for(j=0; j<2; j++) {
            printf("%f ", mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv){
    double** mat;
    int* input;
    int K, iter;
    int numberOfVector;

    printf("x");
    mat = createMatrix();
    printf("y");
    input = verifyInput(argc, argv);
    K = input[0];
    iter = input[1];
    free(input);
    numberOfVector = LEN(mat);
    kMeans(K, iter, numberOfVector, 5, 0.01, mat);
    printMat(mat);
    return 0;
}

double** kMeans(int K, int iter, int numberOfVectors, int vectorsLength, double eps, double** vectorsList) {
    int i;
    int currentIteration = 0;
    double maxMiuK;
    Centroid *closestCentroid;
    double **result;
    Centroid* centroids = (Centroid*)malloc(K * sizeof(Centroid));
    double* deltas = (double*)malloc(numberOfVectors * sizeof(double));
    for (i = 0; i < K; i++) {
        centroids[i].selfVector = copyArray(vectorsList[i], vectorsLength);
        centroids[i].relatedVectors = (double**)malloc(numberOfVectors * sizeof(double*));
    }
    do
    {
        for (i = 0; i < numberOfVectors; i++) {
            closestCentroid = calcClosestCentroid(vectorsList[i], &centroids, K, vectorsLength);
            closestCentroid->relatedVectors[closestCentroid->numOfVectors] = vectorsList[i];
            closestCentroid->numOfVectors++;
        }

        zeroArray(deltas, vectorsLength);
        for (i = 0; i < K; i++) {
            deltas[i] = update(&centroids[i], vectorsLength);
        }
        maxMiuK = maxDelta(deltas, numberOfVectors);
        currentIteration++;
    } while (currentIteration < iter && maxMiuK >= eps);
    result = getCentroidsSelfVectors(centroids, K);
    for (i = 0; i < K; i++) {
        freeRelatedVectors(&(centroids[i]));
    }
    free(centroids);
    return result;
}

double maxDelta(double *deltas, int numberOfVectors) {
    int i;
    double maxVal = 0.0;
    for (i = 0; i < numberOfVectors; i++)
        maxVal = max(maxVal, deltas[i]);
    return maxVal;
}

double update(Centroid* centroid, int vectorsLength) {
    int i;
    double delta;
    double* oldCentroidVector = copyArray(centroid->selfVector, vectorsLength);
    for (i = 0; i < vectorsLength; i++) {
        centroid->selfVector[i] = averageOf(centroid, i);
    }
    delta = euclidianDistance(oldCentroidVector, centroid->selfVector, vectorsLength);
    freeRelatedVectors(centroid);
    free(oldCentroidVector);
    return delta;
}

double** getCentroidsSelfVectors(Centroid* centroids, int K) {
    int i;
    double** selfVectors = (double**) malloc(K * sizeof(double*));
    for (i = 0; i < K; i ++) {
        selfVectors[i] = centroids[i].selfVector;
    }
    return selfVectors;
}

double averageOf(Centroid* centroid, int i) {
    double sum = 0.0;
    int j;
    for (j = 0; j < centroid->numOfVectors; j++) {
        sum += centroid->relatedVectors[j][i];
    }
    return sum / centroid->numOfVectors;
}

void freeRelatedVectors(Centroid* centroid) {
    int i;
    for (i = 0; i < centroid->numOfVectors; i++) {
        free(centroid->relatedVectors[i]);
    }
    centroid->numOfVectors = 0;
}


Centroid* calcClosestCentroid(double* vector, Centroid** centroids, int K, int vectorsLength) {
    int i;
    Centroid* closestCentroid = centroids[0];
    double distToClosest = euclidianDistance(vector, centroids[0]->selfVector, vectorsLength);
    double currentDist;
    for (i = 1; i < K; i++) {
        currentDist = euclidianDistance(vector, centroids[i]->selfVector, vectorsLength);
        if (currentDist < distToClosest) {
            closestCentroid = centroids[i];
            distToClosest = currentDist;
        }
    }
    return closestCentroid;
}

char* roundedDouble(double* pDouble) {
    int numOfDigsInWholePart = countDigitsOfWholePart(*pDouble);
    char* rounded = (char*)malloc((1 + numOfDigsInWholePart + 1 + 4 + 1) * sizeof(char)); /* minus, radix, precision, null terminator */
    sprintf(rounded, "%.4f", *pDouble);
    return rounded;
}

char** roundedVector(double* vector, int vectorsLength) {
    int i;
    char** rounded = (char**)malloc(vectorsLength * sizeof(char*));
    for (i = 0; i < vectorsLength; i++) {
        rounded[i] = roundedDouble(&(vector[i]));
    }
    return rounded;
}

char*** roundedVectors(double** vectors, int vectorsLength, int numberOfVectors) {
    int i;
    char*** rounded = (char***) malloc(numberOfVectors * sizeof(char**));
    for (i = 0; i < numberOfVectors; i++) {
        rounded[i] = roundedVector(vectors[i], vectorsLength);
    }
    return rounded;
}

double euclidianDistance(double *vector1, double *vector2, int vectorsLength) {
    double sum = 0.0;
    int i;
    for (i = 0; i < vectorsLength; i++) {
        sum += pow(vector1[i] - vector2[i], 2);
    }
    return sqrt(sum);
}

double* copyArray(double* inputArray, int rows) {
    int i;
    double* arrayCopy = (double*)malloc(rows * sizeof(double));
    if (arrayCopy == NULL) 
        return NULL;
    for (i = 0; i < rows; i++)
        arrayCopy[i] = inputArray[i];
    return arrayCopy;
}

double** deepCopy2DArray(double** inputArray, int rows, int columns) {
    int i, j;
    /*Allocate memory*/
    double** arrayCopy = (double**)malloc(rows * sizeof(double*));
    if (arrayCopy == NULL)
        return NULL;
    for (i = 0; i < rows; i++) {
        arrayCopy[i] = (double*)malloc(columns * sizeof(double));
        if (arrayCopy[i] == NULL) {
            /*Error Handling*/
            for (j = 0; j < i; j++)
                free(arrayCopy[j]);
            free(arrayCopy);
            return NULL;
        }
        /*The copying*/
        for (j = 0; j < columns; j++)
            arrayCopy[i][j] = inputArray[i][j];
    }
    return arrayCopy;
}

void zeroArray(double* array, int arrayLength) {
    int i;
    for (i = 0; i < arrayLength; i++)
        array[i] = 0.0;
}

int countDigitsOfWholePart(double value) {
    int count = 1;
    if (value < 0)
        value = -value;
    while (value >= 10) {
        value /= 10;
        count++;
    }
    return count;
}