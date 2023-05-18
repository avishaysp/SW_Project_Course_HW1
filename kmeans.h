#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

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