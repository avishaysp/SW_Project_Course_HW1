#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kmeans.h"



int** kmeans(int K, int iter, int numberOfVectors, int vectorsLength, double eps, int **vectorsList) {
    int i;
    int currentIteration = 0;
    double maxMiuK = 0.0;
    Centroid* closestCentroid;
    int** vectorsListCopy = deepCopy2DArray(vectorsList, numberOfVectors, vectorsLength);
    Centroid* centroids = (Centroid*)malloc(K * sizeof(Centroid));
    double* deltas = (double*)malloc(numberOfVectors * sizeof(double));
    for (i = 0; i < K; i++) {
        centroids[i].selfVector = copyArray(vectorsListCopy[i], vectorsLength);
        centroids[i].relatedVectors = (int**)malloc(numberOfVectors * sizeof(int*));
    }
    do
    {
        for (i = 0; i < numberOfVectors; i ++) {
            closestCentroid = calcClosestCentroid(vectorsList[i], &centroids);
            closestCentroid->relatedVectors[closestCentroid->numOfVectors] = vectorsList[i];
            closestCentroid->numOfVectors++;
        }

        zeroArray(deltas, vectorsLength);
        for (i = 0; i < K; i++) {
            deltas[i] = update(&centroids[i], vectorsLength);
        }
        currentIteration++;
    } while (currentIteration < iter && maxMiuK >= eps);
}

double update(Centroid* centroid, int vectorsLength) {
    int i;
    int** vectors_list = deepCopy2DArray(centroid->relatedVectors, centroid->numOfVectors, vectorsLength);
    int* updatingCentroidVector = copyArray(centroid->selfVector, vectorsLength);
    for (i = 0; i < vectorsLength; i++) {
        centroid->selfVector[i] = avargeOf(centroid, i);
    }
    freeRelatedVectors(centroid);
}

double avargeOf(Centroid* centroid, int i) {
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
}

Centroid* calcClosestCentroid(int* vector, Centroid** centroids) {

}

double euclidianDistance(int *vector1, int *vector2, int vectorsLength) {
    double sum = 0.0;
    int i;
    for (i = 0; i < vectorsLength; i++) {
        sum += pow(vector1[i] - vector2[i], 2);
    }
    return sqrt(sum);
}

int* copyArray(int* inputArray, int rows) {
    int i;
    int* arrayCopy = (int*)malloc(rows * sizeof(int));
    if (arrayCopy == NULL) 
        return NULL;
    for (i = 0; i < rows; i++)
        arrayCopy[i] = inputArray[i];
    return arrayCopy;
}

int** deepCopy2DArray(int** inputArray, int rows, int columns) {
    int i, j;
    /*Allocate memory*/
    int** arrayCopy = (int**)malloc(rows * sizeof(int*));
    if (arrayCopy == NULL)
        return NULL;
    for (i = 0; i < rows; i++) {
        arrayCopy[i] = (int*)malloc(columns * sizeof(int));
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