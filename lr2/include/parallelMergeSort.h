#ifndef PARALLELMERGESORT_H
#define PARALLELMERGESORT_H

#include <pthread.h>

typedef struct {
    int *array;
    int left;
    int right;
    int max_threads;
} ThreadData;

void parallel_merge_sort(int *array, int size, int max_threads);

#endif