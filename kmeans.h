#include <stdio.h>

typedef struct Centroid
{
    double* selfVector;
    int numOfVectors;
    double** relatedVectors;
}Centroid;

double** deepCopy2DArray(double** inputArray, int rows, int columns);
double* copyArray(double* inputArray, int rows);
double update(Centroid* centroid, int vectorsLength);
Centroid* calcClosestCentroid(double* vector, Centroid** centroids);
double euclidianDistance(double *vector1, double *vector2, int vectorsLength);
double* copyArray(double* inputArray, int rows);
void zeroArray(double* array, int arrayLength);
double averageOf(Centroid* centroid, int i);
void freeRelatedVectors(Centroid* centroid);