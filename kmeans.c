#define _GNU_SOURCE
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

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

struct input_list
{
    struct vector *vectors;
    int vectorLength;
    int numOfVectors;

};

struct inputMat
{
    double** mat;
    int numOfVectors;
    int vectorsLength;
};


void deleteList(struct vector* vec);
int isStrNumber(char*);
struct inputMat createMatrix(void);
int* verifyInput(int, char**);
void printMat(double** mat, int vectorsLength, int numOfVectors);
struct input_list getInput(void);
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


struct inputMat createMatrix(void){
    int i, j;
    struct input_list input = getInput();
    struct inputMat structinputMat;
    double** mat;
    struct vector head = input.vectors[0];
    struct vector vec = head;
    struct cord c;
    int numOfVectors = input.numOfVectors;
    int vectorsLength = input.vectorLength;
    printf("%d\n", vectorsLength);
    mat = (double**) malloc(numOfVectors * sizeof(double*));
    for(i = 0; i < numOfVectors; i++){
        mat[i] = (double*) malloc(vectorsLength * sizeof(double));
        c = vec.cords[0];
        for(j = 0; j < vectorsLength; j++){
            mat[i][j] = c.value;
            if (j < vectorsLength - 1) {
                c = *c.next;
            }
        }
        vec = *(vec.next);
    }
    deleteList(head.next);
    printf("%d\n", 9);
    structinputMat.mat = mat;
    structinputMat.vectorsLength = vectorsLength;
    structinputMat.numOfVectors = numOfVectors;
    return structinputMat;
}



void deleteCords(struct cord* head) {
    if (head != NULL) {
        printf("%d\n", 14);
        deleteCords(head->next);
        printf("%d\n", 15);
        free(head);
        printf("%d\n", 16);
    }
}

void deleteList(struct vector* vec){
    if (vec != NULL) {
        printf("%d\n", 10);
        deleteList(vec->next);
        printf("%d\n", 11);
        deleteCords(&(vec->cords[0]));
        printf("%d\n", 12);
        free(vec);
        printf("%d\n", 13);
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

void printMat(double** mat, int vectorsLength, int numOfVectors){
    int i;
    int j;
    for(i=0; i<numOfVectors; i++) {
        for(j=0; j<vectorsLength; j++) {
            printf("%f,", mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    struct inputMat input;
    int K, iter, vectorsLength, numOfVectors;
    double** kMeansResult;
    int* inputConsts;
    input = createMatrix();
    inputConsts = verifyInput(argc, argv);
    printf("%d\n", 20);
    K = inputConsts[0];
    iter = inputConsts[1];
    numOfVectors = input.numOfVectors;
    vectorsLength = input.vectorsLength;
    printf("%d\n", 21);
    free(inputConsts);
    printf("%d\n", 22);
    kMeansResult = kMeans(K, iter, numOfVectors, vectorsLength, 0.01, input.mat);
    printf("%f", kMeansResult[0][0]);
    return 0;
}

struct input_list getInput(void)
{
    struct input_list input;
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    double n;
    char c;
    int numOfVectors = 0, vectorsLength = 1;


    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;


    while (scanf("%lf%c", &n, &c) == 2)
    {

        if (c == '\n')
        {
            numOfVectors++;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }

        if (numOfVectors == 0) {
            vectorsLength++;
        }
        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }

    input.numOfVectors = numOfVectors;
    input.vectorLength = vectorsLength;
    input.vectors = head_vec;

    return input;
}
void printVector(double *vec, int vectorsLength) {
    int i;
    for (i = 0;i < vectorsLength; i++) {
        printf("%f,", vec[i]);
    }
    printf("\n");
}
double** kMeans(int K, int iter, int numberOfVectors, int vectorsLength, double eps, double** vectorsList) {
    int i;
    int currentIteration = 0;
    double maxMiuK;
    Centroid *closestCentroid;
    double **result;
    Centroid* centroids = (Centroid*)malloc(K * sizeof(Centroid));
    double* deltas = (double*)malloc(numberOfVectors * sizeof(double));
    printf("%d\n", 30);
    for (i = 0; i < K; i++) {
        printf("%d\n", 31);
        centroids[i].selfVector = copyArray(vectorsList[i], vectorsLength);
        printf("Centroid num: %d", i);
        printVector(centroids[i].selfVector, vectorsLength);
        printf("%d\n", 32);
        centroids[i].relatedVectors = (double**)malloc(numberOfVectors * sizeof(double*));
        printf("%d\n", 33);
    }
    do
    {
        for (i = 0; i < numberOfVectors; i++) {
            printf("%d\n", 34);
            closestCentroid = calcClosestCentroid(vectorsList[i], &centroids, K, vectorsLength);
            printf("%d\ni: %d\n", 35, i);
            printf("Centroid:");
            printVector(closestCentroid->selfVector, vectorsLength);
            printf("Vector:");
            printVector(vectorsList[i], vectorsLength);
            closestCentroid->relatedVectors[closestCentroid->numOfVectors] = vectorsList[i];
            printf("%d\n", 36);
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
        printf("Centroid num: &d", i);
        printVector(centroids[i]->selfVector, vectorsLength);
        printf("Same Vector");
        printVector(vector, vectorsLength);
        currentDist = euclidianDistance(vector, centroids[i]->selfVector, vectorsLength);
        printf("distToClosest: %0.4f\n", distToClosest);
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
        printf("1: %.4f 2: %.4f\n", vector1[i], vector2[i]);
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