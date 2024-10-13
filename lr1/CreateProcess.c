#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int pipe_in[2];  // pipe для передачи данных дочернему процессу (stdin)
    int pipe_out[2]; // pipe для получения данных от дочернего процесса (stdout)
    
    // Создаем пайпы
    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) { // Дочерний процесс
        close(pipe_in[1]);  // Закрываем конец записи
        close(pipe_out[0]); // Закрываем конец чтения

        // Перенаправляем стандартный ввод на pipe_in
        dup2(pipe_in[0], STDIN_FILENO);
        // Перенаправляем стандартный вывод на pipe_out
        dup2(pipe_out[1], STDOUT_FILENO);

        // Закрываем неиспользуемые концы пайпов
        close(pipe_in[0]);
        close(pipe_out[1]);

        // Выполняем дочерний процесс
        execlp("./ChildProcess_exe", "ChildProcess_exe", NULL);
        perror("execlp"); // Если execlp не сработает, выводим ошибку
        exit(EXIT_FAILURE);
    } else { // Родительский процесс
        close(pipe_in[0]);  // Закрываем конец чтения
        close(pipe_out[1]); // Закрываем конец записи

        // Получаем имя файла от пользователя
        char filename[256];
        printf("Введите имя файла: ");
        scanf("%s", filename);

        // Добавляем абсолютный путь
        char filepath[512];
        sprintf(filepath, "../%s.txt", filename);

        // Отладочная информация
        // printf("Пытаемся открыть файл: %s\n", filepath);

        // Открываем файл
        int file_fd = open(filepath, O_RDONLY);
        if (file_fd == -1) {
            perror("Ошибка открытия файла");
            exit(EXIT_FAILURE);
        }

        // Чтение данных из файла и передача их в дочерний процесс через pipe
        char buffer[256];
        ssize_t nbytes;
        while ((nbytes = read(file_fd, buffer, sizeof(buffer))) > 0) {
            write(pipe_in[1], buffer, nbytes);
        }

        // Закрываем файл и запись в pipe
        close(file_fd);
        close(pipe_in[1]);

        // Чтение результата из дочернего процесса
        while ((nbytes = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[nbytes] = '\0';
            printf("%s", buffer);
        }

        close(pipe_out[0]); // Закрываем конец чтения
        wait(NULL);         // Ожидание завершения дочернего процесса
        return 0;
    }
}
