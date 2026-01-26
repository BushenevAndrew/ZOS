#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    int semid = semget(key, 1, 0666 | IPC_CREAT);

    char *shm = (char*)shmat(shmid, NULL, 0);

    // Инициализация семафора
    semctl(semid, 0, SETVAL, 0);

    struct sembuf sb = {0, -1, 0}; // Операция P (ожидание)

    while (1) {
        time_t now = time(NULL);
        pid_t pid = getpid();
        snprintf(shm, SHM_SIZE, "Time: %sPID: %d", ctime(&now), pid);

        // Разрешаем чтение
        sb.sem_op = 1; // Операция V (освобождение)
        semop(semid, &sb, 1);

        sleep(3); // Отправка раз в 3 секунды
    }

    // Деинициализация
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    return 0;
}
