#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define SHM_SIZE 1024
#define SHM_KEY 1234

int main() {
    int shmid;
    char *shmaddr;

    // Подключение к разделяемой памяти
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        time_t now = time(NULL);
        printf("Мое время: %sМой PID: %d\n", ctime(&now), getpid());
        printf("Принятые данные: %s\n", shmaddr);
        sleep(1); // Пауза для имитации работы
    }

    return 0;
}
