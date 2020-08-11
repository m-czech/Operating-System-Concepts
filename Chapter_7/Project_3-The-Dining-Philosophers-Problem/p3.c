//
// Created by mat on 8/11/20.
//
#include <pthread.h>
#include <stdio.h>
#define PHILOSOPHERS_AMOUNT 5

enum State {HUNGRY = 0, THINKING = 1, EATING = 2};

typedef struct philosopher {
    enum State state;
    int id;
    pthread_mutex_t mutex;
    pthread_cond_t condition;

} philosopher;

philosopher philosophers[PHILOSOPHERS_AMOUNT];

void test_if_able_to_eat(int i) {
    if ((philosophers[(i + 4) % 5].state != EATING) && (philosophers[i].state == HUNGRY)
        && (philosophers[(i + 1) % 5].state != EATING)) {
        philosophers[i].state = EATING;
        pthread_cond_signal(&philosophers[i].condition);
    }
}

int pickup_chopsticks(int i) {
    philosophers[i].state = HUNGRY;
    test_if_able_to_eat(i);

    pthread_mutex_lock(&philosophers[i].mutex);
    while (philosophers[i].state != EATING) {
        pthread_cond_wait(&philosophers[i].condition, &philosophers[i].mutex);
    }
    pthread_mutex_unlock(&philosophers[i].mutex);
    printf("philosopher %d picked up chopstick\n", philosophers[i].id);
}

int release_chopsticks(int i) {
    philosophers[i].state = THINKING;
    printf("philosopher %d released chopstick\n", philosophers[i].id);
    test_if_able_to_eat((i + 1) % 5);
    test_if_able_to_eat((i + 4) % 5);
}

void* worker(void* params) {
    philosopher* p = (philosopher*)params;
    pickup_chopsticks(p->id);
    release_chopsticks(p->id);

    return 0;
}

void initialize_philosophers() {
    for (int i = 0; i < PHILOSOPHERS_AMOUNT; ++i) {
        pthread_mutex_init(&philosophers[i].mutex, NULL);
        pthread_cond_init(&philosophers[i].condition, NULL);
        philosophers[i].id = i;
    }
}

int main() {
    initialize_philosophers();

    pthread_t philosophers_tids[PHILOSOPHERS_AMOUNT];
    // create
    for (int i = 0; i < PHILOSOPHERS_AMOUNT; ++i) {
        pthread_create(&philosophers_tids[i], NULL, worker, &philosophers[i]);
    }
    // wait
    for (int i = 0; i < PHILOSOPHERS_AMOUNT; ++i) {
        pthread_join(philosophers_tids[i], NULL);
    }

    return 0;
}
