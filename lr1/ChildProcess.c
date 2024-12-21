#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        float numbers[10];
        int count = 0;
        char* token = strtok(buffer, " ");
        while (token != NULL && count < 10) {
            numbers[count++] = atof(token);
            token = strtok(NULL, " ");
        }

        if (count < 2) {
            printf("Неверный формат команды\n");
            continue;
        }

        float result = numbers[0];
        int error = 0;
        for (int i = 1; i < count; ++i) {
            if (numbers[i] == 0) {
                printf("Ошибка: деление на ноль.\n");
                error = 1;
                break; 
            }
            result /= numbers[i];
        }

        if (!error) {
            printf("Результат: %f\n", result);
        }
    }

    return 0;
}
