#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <sys/mman.h> 

#define SHARED_FILE "/tmp/mmapfile"
#define BUFFER_SIZE 256

int main() {
    int fd = open(SHARED_FILE, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Ошибка при создании файла");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, BUFFER_SIZE);

    char *shared_memory = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("Ошибка отображения файла в память");
        exit(EXIT_FAILURE);
    }
    close(fd);

    char filename[256];
    printf("Введите имя файла: ");
    scanf("%s", filename);

    char filepath[512];
    sprintf(filepath, "../%s.txt", filename);

    int file_fd = open(filepath, O_RDONLY);
    if (file_fd == -1) {
        perror("Ошибка открытия файла");
        munmap(shared_memory, BUFFER_SIZE);
        exit(EXIT_FAILURE);
    }

    pid_t cpid = fork();
    if (cpid == -1) {
        perror("Ошибка при fork");
        close(file_fd);
        munmap(shared_memory, BUFFER_SIZE);
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        dup2(file_fd, STDIN_FILENO);
        close(file_fd);

        execlp("./ChildProcess_exe", "ChildProcess_exe", NULL);
        perror("Ошибка при запуске дочернего процесса");
        exit(EXIT_FAILURE);
    } else {
        close(file_fd);

        while (1) {
            if (strncmp(shared_memory, "done", 4) == 0) {
                printf("Все данные обработаны. Завершение работы.\n");
                break;
            } else if (strncmp(shared_memory, "exit", 4) == 0) {
                printf("%s", shared_memory + 5);
                break;
            } else if (strlen(shared_memory) > 0) {
                printf("%s", shared_memory);
                memset(shared_memory, 0, BUFFER_SIZE);
            }
            usleep(100000);
        }

        wait(NULL);
        munmap(shared_memory, BUFFER_SIZE);
        unlink(SHARED_FILE);
        return 0;
    }
}
