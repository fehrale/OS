#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "include/parallelMergeSort.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <array_size> <max_threads>\n", argv[0]);
        return 1;
    }

    int array_size = atoi(argv[1]);
    int max_threads = atoi(argv[2]);

    int *array = malloc(array_size * sizeof(int));

    // Заполнение массива случайными числами
    for (int i = 0; i < array_size; i++) {
        array[i] = rand() % 1000;
    }

    printf("Original array:\n");
    for (int i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    parallel_merge_sort(array, array_size, max_threads);

    printf("Sorted array:\n");
    for (int i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);
    return 0;
}