#include <stdio.h>
#include <stdlib.h> //функции общего назначения (exit, ftruncate)
#include <unistd.h> //функции POSIX API (close, fork, usleep
#include <string.h>
#include <sys/wait.h> //управление процессами (например, wait
#include <fcntl.h> //управление файлами (open)
#include <sys/mman.h> // отображение файлов в память

#define SHARED_FILE "/tmp/mmapfile" //путь к файлу, который будет использоваться для отображения в память
#define BUFFER_SIZE 256 //размер отображаемой памяти (256 байт).

int main() {
    // Создаем отображаемый файл
    int fd = open(SHARED_FILE, O_CREAT | O_RDWR, 0666); //open: открываем (или создаем) файл SHARED_FILE с флагами O_CREAT (создать, если не существует) и O_RDWR (чтение/запись). Права доступа — 0666 (чтение и запись для всех)
    if (fd == -1) {
        perror("Ошибка при создании файла");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, BUFFER_SIZE); //изменяем размер файла до BUFFER_SIZE

    // Отображаем файл в память
    char *shared_memory = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //mmap: отображает файл в память, NULL: система сама выберет адрес памяти, BUFFER_SIZE: размер отображения, PROT_READ | PROT_WRITE: права на чтение и запись, MAP_SHARED: изменения памяти видны другим процессам
    if (shared_memory == MAP_FAILED) {
        perror("Ошибка отображения файла в память");
        exit(EXIT_FAILURE);
    }
    close(fd); //Закрываем файловый дескриптор fd

    char filename[256]; //Создаем строку filename для хранения имени файла.
    printf("Введите имя файла: ");
    scanf("%s", filename);

    char filepath[512]; //Объявляем строку filepath для хранения полного пути к файлу.
    sprintf(filepath, "../%s.txt", filename);

    int file_fd = open(filepath, O_RDONLY); //открываем файл по пути filepath с флагом O_RDONLY (только чтение)
    if (file_fd == -1) {
        perror("Ошибка открытия файла");
        munmap(shared_memory, BUFFER_SIZE);
        exit(EXIT_FAILURE);
    }

    pid_t cpid = fork(); //fork: создает дочерний процесс. Возвращает 0 в дочернем процессе, а в родительском — PID дочернего
    if (cpid == -1) {
        perror("Ошибка при fork");
        close(file_fd);
        munmap(shared_memory, BUFFER_SIZE);
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        // Дочерний процесс
        dup2(file_fd, STDIN_FILENO); //перенаправляем файловый дескриптор file_fd на стандартный ввод (STDIN_FILENO)
        close(file_fd); //Закрываем file_fd

        execlp("./ChildProcess_exe", "ChildProcess_exe", NULL); //запускаем программу ChildProcess_exe
        perror("Ошибка при запуске дочернего процесса");
        exit(EXIT_FAILURE);
    } else {
        // Родительский процесс
        close(file_fd);

        while (1) {
            // Чтение результатов из отображаемой памяти
            if (strncmp(shared_memory, "done", 4) == 0) {
                printf("Все данные обработаны. Завершение работы.\n");
                break;
            } else if (strncmp(shared_memory, "exit", 4) == 0) {
                printf("%s", shared_memory + 5); // Пропускаем "exit " и выводим сообщение об ошибке
                break;
            } else if (strlen(shared_memory) > 0) {
                printf("%s", shared_memory); //Если память не пуста, выводим содержимое
                memset(shared_memory, 0, BUFFER_SIZE);  // Очистка памяти
            }
            usleep(100000);  //небольшая пауза для снижения нагрузки на процессор.
        }

        // Ожидание завершения дочернего процесса
        wait(NULL);
        munmap(shared_memory, BUFFER_SIZE); //освобождаем отображение памяти
        unlink(SHARED_FILE); //удаляем файл SHARED_FILE
        return 0;
    }
}
