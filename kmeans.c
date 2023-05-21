#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct Centroid
{
    double* selfVector;
    int numOfVectors;
    double** relatedVectors;
}Centroid;

typedef struct Cord
{
    double value;
    struct Cord *next;
}Cord;

typedef struct Vector
{
    struct Vector *next;
    Cord *cords;
}Vector;

/*
FUNCTIONS DECLARATIONS:
*/

/*Input functions*/
void deleteList(Vector*);
int isStrNumber(char*);
double** createMatrix(int*, int*);
int* verifyInput(int, char**, int);
void printMat(double**, int, int);
Vector* getInput(int*, int*);

/*Calculating functions*/
double** deepCopy2DArray(double**, int, int);
double* copyArray(double*, int);
double update(Centroid*, int);
Centroid* calcClosestCentroid(double*, Centroid*, int, int);
double euclidianDistance(double*, double*, int);
void zeroArray(double*, int);
double averageOf(Centroid*, int);
void freeRelatedVectors(Centroid*);
double maxDelta(double*, int);
double** getCentroidsSelfVectors(Centroid*, int);
double** kMeans(int, int, int, int, double, double**);


#ifdef DEBUG
void printVector(double, int);
#endif

/*
FUNCTIONS IMPLEMENTATION:
*/

double** createMatrix(int* numberOfVectors, int* vectorsLength){
    int i, j;
    Vector* input_vec = getInput(numberOfVectors, vectorsLength);
    double** mat;
    Cord* c;
    Vector* head = input_vec;
    Vector* curr_vec = head;
    int num = *numberOfVectors;
    int length = *vectorsLength;
    mat = (double**) malloc(num * sizeof(double*));
    for(i = 0; i < num; i++){
        mat[i] = (double*) malloc(length * sizeof(double));
        c = curr_vec -> cords;
        for(j = 0; j < length; j++){
            mat[i][j] = c -> value;
            if (j < length - 1) {
                c = c -> next;
            }
        }
        curr_vec = curr_vec -> next;
    }
    deleteList(head);
    return mat;
}


void deleteCords(Cord* head) {
    if (head != NULL) {
        deleteCords(head->next);
        free(head);
    }
}

void deleteList(Vector* vec){
    if (vec != NULL) {
        deleteList(vec->next);
        deleteCords(vec->cords);
        free(vec);
    }
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

int* verifyInput(int a, char **b, int numberOfVectors, int vectorsLength, double **mat){
    int* arr = calloc(2, sizeof(int));
    int k, iter, i;
    char* k1, *iter1;
    int thereIsAnError = 0;
    if (a < 2 || a > 3){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    k1 = b[1];
    if(!isStrNumber(k1)) {
        printf("invalid number of clusters!\n");
        thereIsAnError = 1;
    } else {
        k = atoi(k1);
        if(k <= 1 || k >= numberOfVectors) {
            printf("invalid number of clusters!\n");
            thereIsAnError = 1;
        }
    }
    if (a == 3){
        iter1 = b[2];
        if(!isStrNumber(iter1)){
            printf("Invalid maximum iteration!\n");
            thereIsAnError = 1;
        } else {
            iter = atoi(iter1);
            if(iter <= 1 || iter >= 1000){
                printf("Invalid maximum iteration!\n");
                thereIsAnError = 1;
            }
        }
    } else { iter = 200; }
    if (thereIsAnError) {
        for (i = 0; i < numberOfVectors; i++)
            free(mat[i]);
        free(mat);
        exit(1);
    }
    arr[0] = k;
    arr[1] = iter;
    return arr;
}

void printMat(double** mat, int vectorsLength, int numOfVectors){
    int i;
    int j;

    for(i=0; i<numOfVectors; i++) {
        for(j=0; j<vectorsLength; j++) {
            printf("%.4f", mat[i][j]);
            if (j < vectorsLength - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    double** input;
    int K, iter;
    int numberOfVectors = 0, vectorsLength = 0;
    double** kMeansResult;
    int* inputConsts;
    int i;
    
    input = createMatrix(&numberOfVectors, &vectorsLength);
    inputConsts = verifyInput(argc, argv, numberOfVectors, vectorsLength, input);
    K = inputConsts[0];
    iter = inputConsts[1];
    free(inputConsts);
    kMeansResult = kMeans(K, iter, numberOfVectors, vectorsLength, 0.01, input);
    printMat(kMeansResult, vectorsLength, K);
    
    for (i = 0; i < numberOfVectors; i++) {
        free(input[i]);
    }
    free(input);
    return 0;
}

Vector* getInput(int* numOfVectors, int* vectorsLength)
{
    Vector *head_vec, *curr_vec;
    Cord *head_cord, *curr_cord;
    int num = 0, length = 1;
    double n;
    char c;

    head_cord = (Cord*)malloc(sizeof(Cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = (Vector*)malloc(sizeof(Vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;

    while (scanf("%lf%c", &n, &c) == 2) {
        if (c == '\n') {
            num++;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = (Vector*)malloc(sizeof(Vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = (Cord*)malloc(sizeof(Cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }
        if (num == 0) {
            length++;
        }
        curr_cord->value = n;
        curr_cord->next = (Cord*)malloc(sizeof(Cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }
    *numOfVectors = num;
    *vectorsLength = length;
    return head_vec;
}

#ifdef DEBUG
void printVector(double *vec, int vectorsLength) {
    int i;
    for (i = 0;i < vectorsLength; i++) {
        printf("%f,", vec[i]);
    }
    printf("\n");
}
#endif

double** kMeans(int K, int maxIter, int numberOfVectors, int vectorsLength, double eps, double** vectorsList) {
    int i;
    int currentIteration = 0;
    double maxMiuK;
    Centroid *closestCentroid;
    double **result;
    Centroid* centroids = (Centroid*)malloc(K * sizeof(Centroid));
    double* deltas = (double*)calloc(numberOfVectors, sizeof(double));

    /*Create Centroids*/
    for (i = 0; i < K; i++) {
        centroids[i].selfVector = copyArray(vectorsList[i], vectorsLength);
        centroids[i].numOfVectors = 0;
        centroids[i].relatedVectors = (double**)malloc(numberOfVectors * sizeof(double*));
    }
    do {
        for (i = 0; i < numberOfVectors; i++) {
            #ifdef DEBUG
                printf("Inside for. Iteration #%d\nThe vectors:\n", i);
                printMat(vectorsList, vectorsLength, numberOfVectors);
                printf("The centroids:\n");
                printMat(getCentroidsSelfVectors(centroids, K), vectorsLength, K);
                printf("Before calcClosestCentroid.\n"); */
            #endif
            closestCentroid = calcClosestCentroid(vectorsList[i], centroids, K, vectorsLength);
            #ifdef DEBUG
                printf("After calcClosestCentroid.\n");
                printf("Centroid: ");
                printVector(closestCentroid->selfVector, vectorsLength);
                printf("Vector: ");
                printVector(vectorsList[i], vectorsLength);
            #endif
            closestCentroid->relatedVectors[closestCentroid->numOfVectors] = vectorsList[i];
            closestCentroid->numOfVectors++;
        }
        #ifdef DEBUG
            printf("The Centroids before updating:\n");
            printMat(getCentroidsSelfVectors(centroids, K), vectorsLength, K);
        #endif
        for (i = 0; i < K; i++) {
            deltas[i] = update(&centroids[i], vectorsLength);
        }
        #ifdef DEBUG
            printf("The deltas:\n");
            printVector(deltas, K);
            printf("The Centroids after updating:\n");
            printMat(getCentroidsSelfVectors(centroids, K), vectorsLength, K);
        #endif
        maxMiuK = maxDelta(deltas, numberOfVectors);
        zeroArray(deltas, vectorsLength);
        for (i = 0; i < K; i++) {
            centroids[i].numOfVectors = 0;
        }
        currentIteration++;
    } while (currentIteration < maxIter && maxMiuK >= eps);
    result = getCentroidsSelfVectors(centroids, K);
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
    double* oldCentroidVector;

    if (centroid->numOfVectors == 0) {
        return 0.0;
    }
    oldCentroidVector = copyArray(centroid->selfVector, vectorsLength);
    for (i = 0; i < vectorsLength; i++) {
        centroid->selfVector[i] = averageOf(centroid, i);
    }
    delta = euclidianDistance(oldCentroidVector, centroid->selfVector, vectorsLength);
    free(oldCentroidVector);
    centroid->numOfVectors = 0;
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
    free(centroid->relatedVectors);
    centroid->numOfVectors = 0;
}


    Centroid* calcClosestCentroid(double* vector, Centroid* centroids, int K, int vectorsLength) {
    int i;
    Centroid* closestCentroid = centroids;
    double distToClosest = euclidianDistance(vector, centroids[0].selfVector, vectorsLength);
    double currentDist;

    for (i = 1; i < K; i++) {
        currentDist = euclidianDistance(vector, centroids[i].selfVector, vectorsLength);
        if (currentDist < distToClosest) {
            closestCentroid = &centroids[i];
            distToClosest = currentDist;
        }
    }
    return closestCentroid;
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


void zeroArray(double* array, int arrayLength) {
    int i;

    for (i = 0; i < arrayLength; i++)
        array[i] = 0.0;
}
