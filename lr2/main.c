#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "include/parallelMergeSort.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <array_size> <max_threads>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    int array_size = atoi(argv[1]);
    int max_threads = atoi(argv[2]);

    int *array = malloc(array_size * sizeof(int));

    for (int i = 0; i < array_size; i++) {
        array[i] = rand() % 1000;
    }

    // printf("Original array:\n");
    // for (int i = 0; i < array_size; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");

    struct timespec start_real, end_real;
    clock_gettime(CLOCK_MONOTONIC, &start_real);

    parallel_merge_sort(array, array_size, max_threads);

    clock_gettime(CLOCK_MONOTONIC, &end_real);

    // printf("Sorted array:\n");
    // for (int i = 0; i < array_size; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");

    double time_taken_real = (end_real.tv_sec - start_real.tv_sec) +
                             (end_real.tv_nsec - start_real.tv_nsec) / 1e9;
    printf("Real time: %.6f seconds\n", time_taken_real);

    free(array);
    return 0;
}