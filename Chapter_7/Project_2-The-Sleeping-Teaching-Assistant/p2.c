//
// Created by mat on 8/11/20.
//
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

#define STUDENTS_AMOUNT 10
#define CHAIRS_AMOUNT 5

typedef struct student {
    struct student* next;
    sem_t* sem;
    int id;
} student;

typedef struct teacher {
    sem_t* sem;
    int visits;
} teacher;

typedef struct queue {
    student* head;
    int students_waiting;
}queue;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
queue q;
sem_t teacher_sem;
pthread_t students[STUDENTS_AMOUNT];
teacher t = {&teacher_sem, 0};

int enqueue(student* s) {
    if (q.students_waiting >= CHAIRS_AMOUNT) {
        return 1;
    }

    student* new_student = malloc(sizeof(struct student));
    new_student->sem = s->sem;
    new_student->id = s->id;
    new_student->next = NULL;

    if (q.students_waiting == 0) {
        q.head = new_student;
        q.students_waiting = 1;
        return 0;
    }

    student* prev_student = NULL;
    student* next_student = q.head;
    while (next_student != NULL) {
        prev_student = next_student;
        next_student = next_student->next;
    }
    prev_student->next = new_student;

    q.students_waiting += 1;
    return 0;
}

student dequeue() {
    if (q.students_waiting <= 0) {
        student student;
        student.id = -1;
        return student;
    }
    student dequeued_student = *q.head;
    student* student_to_free = q.head;

    q.head = q.head->next;
    q.students_waiting -= 1;

    free(student_to_free);
    return dequeued_student;
}

void* teacher_worker() {
    while(1) {
        pthread_mutex_lock(&queue_mutex);
        if (q.students_waiting == 0) {
            pthread_mutex_unlock(&queue_mutex);
            sem_wait(&teacher_sem);
            sleep(1);
        }
        else {
            student s = dequeue();
            sem_post(s.sem);
            sleep(1);
            printf("student with id %d visited teacher\n", s.id);
            t.visits += 1;

        }
        if (t.visits == STUDENTS_AMOUNT) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        pthread_mutex_unlock(&queue_mutex);
    }

}

void* student_worker(void* p) {
    student* s = (student*)p;

    while(1) {
        pthread_mutex_lock(&queue_mutex);
        if (enqueue(s) == 0) {
            pthread_mutex_unlock(&queue_mutex);
            sem_post(t.sem);
            sem_wait(s->sem);
            break;
        }
        pthread_mutex_unlock(&queue_mutex);
        sleep(1);
    }
    free(p);
}

int main() {
    sem_init(&teacher_sem, 0, 0);
    pthread_t teacher;
    pthread_create(&teacher, NULL, teacher_worker, (void*)&t);


    sem_init(&teacher_sem, 0, 0);
    for (int i = 0; i < STUDENTS_AMOUNT; ++i) {
        student* new_student = malloc(sizeof(student));
        sem_t* new_sem = malloc(sizeof(sem_t));
        sem_init(new_sem, 0, 0);
        new_student->sem = new_sem;
        new_student->id = i;
        pthread_create(&students[i], NULL, student_worker, (void*)new_student);
    }

    for (int i = 0; i < STUDENTS_AMOUNT; ++i) {
        pthread_join(students[i], NULL);
    }
    return 0;
}
