#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    char buffer[256];

    // Чтение команд из стандартного потока ввода
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Удаление символа новой строки
        buffer[strcspn(buffer, "\n")] = 0;

        // Разбираем команду и выполняем деление
        float numbers[10];
        int count = 0;
        char* token = strtok(buffer, " ");
        while (token != NULL && count < 10) {
            numbers[count++] = atof(token);
            token = strtok(NULL, " ");
        }

        // Проверяем наличие хотя бы двух чисел
        if (count < 2) {
            printf("Неверный формат команды\n");
            continue;
        }

        // Выполняем деление
        float result = numbers[0];
        for (int i = 1; i < count; ++i) {
            if (numbers[i] == 0) {
                printf("Ошибка: деление на ноль. Завершение программы.\n");
                exit(EXIT_FAILURE);
            }
            result /= numbers[i];
        }

        // Отправляем результат в stdout
        printf("Результат: %f\n", result);
    }

    return 0;
}
