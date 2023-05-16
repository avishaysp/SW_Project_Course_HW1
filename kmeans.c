#include "kmeans.h"

int main() {
    return 0;
}

double** kMeans(int K, int iter, int numberOfVectors, int vectorsLength, double eps, double** vectorsList) {
    int i;
    int currentIteration = 0;
    double maxMiuK;
    Centroid* closestCentroid;
    double** vectorsListCopy = deepCopy2DArray(vectorsList, numberOfVectors, vectorsLength);
    Centroid* centroids = (Centroid*)malloc(K * sizeof(Centroid));
    double* deltas = (double*)malloc(numberOfVectors * sizeof(double));
    for (i = 0; i < K; i++) {
        centroids[i].selfVector = copyArray(vectorsListCopy[i], vectorsLength);
        centroids[i].relatedVectors = (double**)malloc(numberOfVectors * sizeof(double*));
    }
    do
    {
        for (i = 0; i < numberOfVectors; i ++) {
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
    return getCentroidsSelfVectors(centroids, K);
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