#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void exit_handler(void) {
    printf("Обработчик atexit: процесс завершается.\n");
}

void sigint_handler(int sig) {
    printf("Получен сигнал SIGINT (%d)\n", sig);
    exit(0);
}

void sigterm_handler(int sig) {
    printf("Получен сигнал SIGTERM (%d)\n", sig);
    exit(0);
}

int main() {
    pid_t pid;
    int status;

    atexit(exit_handler);

    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigterm_handler);

    pid = fork();

    if (pid < 0) {
        perror("Ошибка вызова fork");
        exit(1);
    } else if (pid == 0) {
        printf("Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
        sleep(5);
    } else {
        printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Дочерний процесс завершился с кодом %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
