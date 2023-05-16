#include <stdio.h>

typedef struct Centroid
{
    int* selfVector;
    int numOfVectors;
    int** relatedVectors; 
}Centroid;

int** deepCopy2DArray(int** inputArray, int rows, int columns);
int* copyArray(int* inputArray, int rows);
double update(Centroid* centroid, int vectorsLength);
Centroid* calcClosestCentroid(int* vector, Centroid** centroids);
double euclidianDistance(int *vector1, int *vector2, int vectorsLength);
int* copyArray(int* inputArray, int rows);
void zeroArray(double* array, int arrayLength);
double averageOf(Centroid* centroid, int i);
void freeRelatedVectors(Centroid* centroid);