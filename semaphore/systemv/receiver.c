#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    int semid = semget(key, 1, 0666 | IPC_CREAT);

    char *shm = (char*)shmat(shmid, NULL, 0);

    struct sembuf sb = {0, -1, 0};

    while (1) {
        sb.sem_op = -1;
        semop(semid, &sb, 1);

        time_t now = time(NULL);
        pid_t pid = getpid();
        printf("Receiver (PID: %d, Time: %s): %s\n", pid, ctime(&now), shm);

        sb.sem_op = 1;
        semop(semid, &sb, 1);
    }

    shmdt(shm);
    return 0;
}
