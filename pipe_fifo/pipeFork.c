#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) { // Родительский процесс
        close(fd[0]); // Закрываем чтение

        time_t t = time(NULL);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Parent time: %ld, PID: %d", t, getpid());
        
        write(fd[1], buffer, strlen(buffer) + 1);
        close(fd[1]);

        wait(NULL); // Ждем завершения дочернего процесса
    } 
    else { // Дочерний процесс
        close(fd[1]); // Закрываем запись

        sleep(5); // Задержка 5 секунд

        char buffer[256];
        read(fd[0], buffer, sizeof(buffer));
        close(fd[0]);

        time_t t = time(NULL);
        printf("Child time: %ld, PID: %d\n", t, getpid());
        printf("Received: %s\n", buffer);
    }

    return 0;
}