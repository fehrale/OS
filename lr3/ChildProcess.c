#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHARED_FILE "/tmp/mmapfile"
#define BUFFER_SIZE 256

int main() {
    int fd = open(SHARED_FILE, O_RDWR);
    if (fd == -1) {
        perror("Ошибка при открытии отображаемого файла");
        exit(EXIT_FAILURE);
    }

    char *shared_memory = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("Ошибка отображения файла в память");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);

    char buffer[BUFFER_SIZE];

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
            snprintf(shared_memory, BUFFER_SIZE, "Неверный формат команды\n");
            sleep(1);
            continue;
        }

        float result = numbers[0];
        for (int i = 1; i < count; ++i) {
            if (numbers[i] == 0) {
                snprintf(shared_memory, BUFFER_SIZE, "exit Ошибка: деление на ноль.\n");
                munmap(shared_memory, BUFFER_SIZE);
                exit(EXIT_FAILURE);
            }
            result /= numbers[i];
        }

        snprintf(shared_memory, BUFFER_SIZE, "Результат: %f\n", result);
        sleep(1);
    }

    strncpy(shared_memory, "done", 4);
    munmap(shared_memory, BUFFER_SIZE);
    return 0;
}
