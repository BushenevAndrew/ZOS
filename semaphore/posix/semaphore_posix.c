#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 20

char shared_buffer[BUFFER_SIZE];
sem_t semaphore;
int counter = 0;

void* writer_thread(void* arg) {
    while (1) {
        sem_wait(&semaphore);
        snprintf(shared_buffer, BUFFER_SIZE, "%09d", counter++);
        sem_post(&semaphore);
        sleep(1);
    }
    return NULL;
}

void* reader_thread(void* arg) {
    long tid = (long)arg;
    while (1) {
        sem_wait(&semaphore);
        printf("Reader %ld: %s\n", tid, shared_buffer);
        sem_post(&semaphore);
        usleep(500000);
    }
    return NULL;
}

int main() {
    pthread_t writer, reader;

    if (sem_init(&semaphore, 0, 1) != 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&writer, NULL, writer_thread, NULL) != 0) {
        perror("pthread_create writer");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&reader, NULL, reader_thread, (void*)1) != 0) {
        perror("pthread_create reader");
        exit(EXIT_FAILURE);
    }

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    sem_destroy(&semaphore);
    return 0;
}
