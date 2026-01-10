// rwlock_example.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 20
#define NUM_READERS 10

char shared_buffer[BUFFER_SIZE];
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int counter = 0;

void* writer_thread(void* arg) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        snprintf(shared_buffer, BUFFER_SIZE, "%09d", counter++);
        pthread_rwlock_unlock(&rwlock);
        usleep(100000);
    }
    return NULL;
}

void* reader_thread(void* arg) {
    long tid = (long)arg;
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("Reader %ld: %s\n", tid, shared_buffer);
        pthread_rwlock_unlock(&rwlock);
        usleep(150000);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writer;

    if (pthread_create(&writer, NULL, writer_thread, NULL) != 0) {
        perror("Failed to create writer thread");
        exit(EXIT_FAILURE);
    }

    for (long i = 0; i < NUM_READERS; ++i) {
        if (pthread_create(&readers[i], NULL, reader_thread, (void*)i) != 0) {
            perror("Failed to create reader thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_join(writer, NULL);
    for (int i = 0; i < NUM_READERS; ++i) {
        pthread_join(readers[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
