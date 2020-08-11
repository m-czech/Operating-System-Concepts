/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define NUMBER_OF_THREADS 5

// this represents work that has to be 
// completed by a thread in the pool
typedef struct task{
    void (*function)(void *p);
    void *data;
    struct task* next;
}
task;

typedef struct queue{
    task* head;
    int counter;
}
queue;

queue q;
pthread_t tids[NUMBER_OF_THREADS];
pthread_attr_t attributes[NUMBER_OF_THREADS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task* t)
{
    pthread_mutex_lock(&mutex);
    if (q.counter == 0) {
        q.head = t;
        q.counter += 1;
    }
    else {
        task* next_task = q.head;
        task* prev_task = NULL;
        while (next_task != NULL) {
            prev_task = next_task;
            next_task = next_task->next;
        }
        if (prev_task == NULL) {
            pthread_mutex_unlock(&mutex);
            return 1;
        }
        prev_task->next = t;
        q.counter += 1;
    }

    pthread_mutex_unlock(&mutex);

    return 0;
}

// remove a task from the queue
task dequeue() 
{
    pthread_mutex_lock(&mutex);
    if (q.counter == 0) {
        task task = {NULL, NULL, NULL};
        pthread_mutex_unlock(&mutex);
        printf("dequeued\n");
        return task;
    }
    task task_to_return = *q.head;
    task* task_to_free = q.head;
    q.head = q.head->next;
    q.counter -= 1;

    free(task_to_free);
    pthread_mutex_unlock(&mutex);
    printf("dequeued\n");
    return task_to_return;
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

// the worker thread in the thread pool
_Noreturn void *worker(void *param)
{
    while (1) {
        task t = dequeue();
        if (t.data == NULL) {
            sem_wait(&semaphore);
            continue;
        }
        execute(t.function, t.data);
        free(t.data);
        sem_post(&semaphore);
    }
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p) {
    task* task = malloc(sizeof(struct task));
    task->function = somefunction;
    task->data = p;
    task->next = NULL;
    if (enqueue(task) == 0) {
        printf("enqueued\n");
        return 0;
    }
    return 1;
}

// initialize the thread pool
void pool_init(void)
{
    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        pthread_attr_init(&attributes[i]);
        pthread_create(&tids[i], &attributes[i], worker, (void *)(&q));
    }

    sem_init(&semaphore, 0, 0);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        pthread_cancel(tids[i]);
        pthread_join(tids[i], NULL);
    }
    sem_destroy(&semaphore);
    printf("ended\n");
}
