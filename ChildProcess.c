#include <stdio.h> //подключение стандартной библиотеки для работы с вводом/выводом
#include <stdlib.h> //подключение стандартной библиотеки для работы с функциями общего назначения (exit, atof и т.д.)
#include <unistd.h> //предоставляет функции POSIX API, такие как close и sleep
#include <string.h>
#include <fcntl.h> // библиотека для управления файловыми дескрипторами
#include <sys/mman.h> //библиотека для управления отображением файлов в память (mmap, munmap)

#define SHARED_FILE "/tmp/mmapfile" //определяет путь к файлу, который будет использоваться для отображения в память.
#define BUFFER_SIZE 256 //устанавливает размер буфера для обмена данными (256 байт)

int main() {
    int fd = open(SHARED_FILE, O_RDWR); //Открытие файла по пути SHARED_FILE с флагом O_RDWR (чтение/запись). Возвращается файловый дескриптор fd
    if (fd == -1) {
        perror("Ошибка при открытии отображаемого файла");
        exit(EXIT_FAILURE);
    }

    char *shared_memory = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Отображаем файл в память с помощью mmap; NULL: система выбирает адрес памяти; BUFFER_SIZE: размер отображаемого региона; PROT_READ | PROT_WRITE: права доступа — чтение и запись; MAP_SHARED: изменения в памяти будут видны другим процессам; fd: файловый дескриптор; 0: смещение от начала файла 
    if (shared_memory == MAP_FAILED) {
        perror("Ошибка отображения файла в память");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);

    char buffer[BUFFER_SIZE]; //Объявление буфера для ввода строки от пользователя

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) { //Считываем строку из стандартного ввода (stdin) до конца файла (EOF)
        buffer[strcspn(buffer, "\n")] = 0; //Удаляем символ новой строки (\n), заменяя его на \0 (конец строки).

        float numbers[10]; //Объявляем массив для хранения чисел (до 10) и переменную count для подсчёта чисел. Разбиваем строку на токены, разделённые пробелами
        int count = 0;
        char* token = strtok(buffer, " ");
        while (token != NULL && count < 10) {
            numbers[count++] = atof(token); //Преобразуем токены в числа с плавающей точкой (atof) и записываем их в массив numbers
            token = strtok(NULL, " ");
        }

        if (count < 2) {
            snprintf(shared_memory, BUFFER_SIZE, "Неверный формат команды\n"); //Проверяем, введено ли хотя бы 2 числа. Если нет, записываем сообщение об ошибке в общую память и продолжаем цикл
            sleep(1);
            continue;
        }

        float result = numbers[0]; //Инициализируем результат первым числом.
        for (int i = 1; i < count; ++i) { //Последовательно делим его на остальные числа.
            if (numbers[i] == 0) { //Если обнаружено деление на ноль, записываем сообщение об ошибке, освобождаем память и завершаем программу
                snprintf(shared_memory, BUFFER_SIZE, "exit Ошибка: деление на ноль.\n");
                munmap(shared_memory, BUFFER_SIZE);
                exit(EXIT_FAILURE);
            }
            result /= numbers[i];
        }

        snprintf(shared_memory, BUFFER_SIZE, "Результат: %f\n", result);
        sleep(1);  // Пауза для передачи данных родителю
    }

    // Сообщение о завершении обработки
    strncpy(shared_memory, "done", 4); //После выхода из цикла записываем сообщение о завершении (done)
    //освобождаем отображение памяти с помощью munmap и завершаем программу
    munmap(shared_memory, BUFFER_SIZE);
    return 0;
}
