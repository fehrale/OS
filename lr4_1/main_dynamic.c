#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void* handle = NULL;
float (*E_func)(int);
int* (*Sort_func)(int*, int);

void load_library(const char* path) {
    if (handle) dlclose(handle);

    handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    E_func = dlsym(handle, "E");
    Sort_func = dlsym(handle, "Sort");
    if (!E_func || !Sort_func) {
        fprintf(stderr, "Failed to find symbols: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }
}

void process_commands(const char* lib1, const char* lib2) {
    load_library(lib1);

    int command;
    while (scanf("%d", &command) != EOF) {
        if (command == 0) {
            load_library((handle == dlopen(lib1, RTLD_LAZY)) ? lib2 : lib1);
        } else if (command == 1) {
            int x;
            scanf("%d", &x);
            printf("E(%d) = %f\n", x, E_func(x));
        } else if (command == 2) {
            int size;
            scanf("%d", &size);
            int array[size];
            for (int i = 0; i < size; i++) {
                scanf("%d", &array[i]);
            }
            Sort_func(array, size);
            printf("Sorted array: ");
            for (int i = 0; i < size; i++) {
                printf("%d ", array[i]);
            }
            printf("\n");
        } else if (command == 3) {
            printf("Exiting program.\n");
            dlclose(handle);
            exit(0);
        }
    }

    dlclose(handle);
}

int main() {
    process_commands("/workspaces/OS/lr4_1/build/libmath_lib1.so", "/workspaces/OS/lr4_1/build/libmath_lib2.so");
    return 0;
}
