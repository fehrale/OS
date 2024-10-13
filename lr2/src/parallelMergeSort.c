#include "../include/parallelMergeSort.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t thread_count_mutex = PTHREAD_MUTEX_INITIALIZER;
int current_threads = 0;

void merge(int *array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = array[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void *threaded_merge_sort(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int left = data->left;
    int right = data->right;
    int *array = data->array;

    if (left < right) {
        int mid = left + (right - left) / 2;

        pthread_mutex_lock(&thread_count_mutex);
        current_threads++;
        pthread_mutex_unlock(&thread_count_mutex);

        ThreadData left_data = {array, left, mid};
        ThreadData right_data = {array, mid + 1, right};

        pthread_t left_thread, right_thread;
        pthread_create(&left_thread, NULL, threaded_merge_sort, &left_data);
        pthread_create(&right_thread, NULL, threaded_merge_sort, &right_data);

        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

        pthread_mutex_lock(&thread_count_mutex);
        current_threads--;
        pthread_mutex_unlock(&thread_count_mutex);

        merge(array, left, mid, right);
    }

    return NULL;
}

void parallel_merge_sort(int *array, int size, int max_threads) {
    pthread_t main_thread;
    ThreadData main_data = {array, 0, size - 1};

    pthread_mutex_lock(&thread_count_mutex);
    current_threads++;
    pthread_mutex_unlock(&thread_count_mutex);

    pthread_create(&main_thread, NULL, threaded_merge_sort, &main_data);
    pthread_join(main_thread, NULL);

    printf("Sorting completed with %d threads.\n", max_threads);
}