#include "contracts.h"
#include <stdio.h>
#include <stdlib.h>

void process_commands();

int main() {
    process_commands();
    return 0;
}

void process_commands() {
    int command;
    while (scanf("%d", &command) != EOF) {
        if (command == 1) {
            int x;
            scanf("%d", &x);
            printf("E(%d) = %f\n", x, E(x));
        } else if (command == 2) {
            int size;
            scanf("%d", &size);
            int array[size];
            for (int i = 0; i < size; i++) {
                scanf("%d", &array[i]);
            }
            Sort(array, size);
            printf("Sorted array: ");
            for (int i = 0; i < size; i++) {
                printf("%d ", array[i]);
            }
            printf("\n");
        } else if (command == 3) {
            printf("Exiting program.\n");
            exit(0);
        }
    }
}
