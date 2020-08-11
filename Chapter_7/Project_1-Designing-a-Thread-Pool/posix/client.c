/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"
#include <stdlib.h>

#define WORKS_TO_BE_DONE 10

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for (int i = 0; i < WORKS_TO_BE_DONE; ++i) {
        struct data* work = malloc(sizeof(struct data));
        work->a = i;
        work->b = i + 1;
        pool_submit(&add, work);
    }

    // may be helpful
    sleep(3);

    pool_shutdown();

    return 0;
}
