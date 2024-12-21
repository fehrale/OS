#include "contracts.h"
#include <math.h>
#include <stdlib.h>

static int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

float E(int x) {
    float sum = 0.0;
    for (int n = 0; n <= x; n++) {
        sum += 1.0 / factorial(n);
    }
    return sum;
}

int* Sort(int* array, int size) {
    if (size < 2) return array;

    int pivot = array[size / 2];
    int* left = malloc(size * sizeof(int));
    int* right = malloc(size * sizeof(int));
    int lsize = 0, rsize = 0;

    for (int i = 0; i < size; i++) {
        if (i == size / 2) continue;
        if (array[i] <= pivot) left[lsize++] = array[i];
        else right[rsize++] = array[i];
    }

    left = Sort(left, lsize);
    right = Sort(right, rsize);

    int index = 0;
    for (int i = 0; i < lsize; i++) array[index++] = left[i];
    array[index++] = pivot;
    for (int i = 0; i < rsize; i++) array[index++] = right[i];

    free(left);
    free(right);
    return array;
}