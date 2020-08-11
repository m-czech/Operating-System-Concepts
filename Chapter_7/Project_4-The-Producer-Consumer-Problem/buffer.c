//
// Created by mat on 8/11/20.
//
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include "buffer.h"
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty;
sem_t full;

buffer_item buff[BUFFER_SIZE];
int pos = 0;

int insert_item(int num) {
    if (sem_wait(&empty) == 1) { return -1; }
    if (pthread_mutex_lock(&mutex) != 0) { return -1; }

    buff[pos] = num;
    printf("inserted %d into buffer\n", buff[pos]);
    pos = (pos + 1) % BUFFER_SIZE;

    if (pthread_mutex_unlock(&mutex) != 0) { return -1; }
    if (sem_post(&full) == 1) { return -1; }
    return 0;
}

int remove_item() {
    if (sem_wait(&full) == 1) { return -1;};
    if (pthread_mutex_lock(&mutex) != 0) { return -1; };

    if (pos == 0) {
        pos = BUFFER_SIZE - 1;
    }
    else {
        pos = (pos - 1) % BUFFER_SIZE;
    }
    printf("removed %d from buffer\n", buff[pos]);
    if (pthread_mutex_unlock(&mutex) != 0) { return -1; };
    if (sem_post(&empty) == 1) { return -1; };

    return 0;
}

void* producer_worker(void* params) {
    while (1) {
        sleep(THREADS_SLEEP_TIME);
        int num = rand() % 100;
        if (insert_item(num) == -1) {
            fprintf(stderr, "error inserting into buffer\n");
        }
        else {
            /*printf("inserted into buffer\n");*/
        }
    }
}

void* consumer_worker(void* params) {
    while (1) {
        sleep(THREADS_SLEEP_TIME);
        if (remove_item() == -1) {
            fprintf(stderr, "error removing item from buffer\n");
        }
        else {
            /*printf("element has been taken out from buffer\n");*/
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("bad amount of arguments entered\n");
        exit(1);
    }

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    srand(time(0));

    long working_time = strtol(argv[1], NULL, 10);
    long producents_amount = strtol(argv[2], NULL, 10);
    long consumers_amount = strtol(argv[3], NULL, 10);

    // create producents threads
    pthread_t producents[producents_amount];
    for (int i = 0; i < producents_amount; i++) {
        pthread_create(&producents[i], NULL, producer_worker, NULL);
    }

    // create consumer threads
    pthread_t consumers[consumers_amount];
    for (int i = 0; i < consumers_amount; i++) {
        pthread_create(&consumers[i], NULL, consumer_worker, NULL);
    }

    // wait until time expires
    sleep(working_time);

    for (int i = 0; i < producents_amount; i++) {
        pthread_cancel(producents[i]);
    }

    for (int i = 0; i < consumers_amount; i++) {
        pthread_cancel(consumers[i]);
    }

    printf("finished\n");
    return 0;
}

