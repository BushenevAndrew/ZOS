// condvar_example.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFFER_SIZE 20
#define NUM_READERS 10

char shared_buffer[BUFFER_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int counter = 0;
bool is_writing = false;

void* writer_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (is_writing) {
            pthread_cond_wait(&cond, &mutex);
        }
        is_writing = true;
        snprintf(shared_buffer, BUFFER_SIZE, "%09d", counter++);
        is_writing = false;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    return NULL;
}

void* reader_thread(void* arg) {
    long tid = (long)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (is_writing) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Reader %ld: %s\n", tid, shared_buffer);
        pthread_mutex_unlock(&mutex);
        usleep(150000);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writer;

    // Создаем пишущий поток
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

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
