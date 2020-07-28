#include <stdio.h>

#define MIN_PID 100
#define MAX_PID 200

char pid_table[MAX_PID - MIN_PID];

// redundant func
int allocate_map(void) {
    for (int i = 0; i < MAX_PID - MIN_PID; i++) {
        pid_table[i] = 0;
    }
    return 1;
}

int allocate_pid(void) {
    for (int i = 0; i < MAX_PID - MIN_PID; i++) {
        if (pid_table[i] == 0) {
            pid_table[i] = 1;
            return i + MIN_PID;
        }
    }
    return -1;
}

void release_pid(int pid) {
    if (escopid > MAX_PID) {
        return;
    }
    pid_table[pid - MIN_PID] = 0;
}

void printPidTable(void) {
    for (int i = 0; i < MAX_PID - MIN_PID; i++) {
        printf("%d %d \n", i + MIN_PID, pid_table[i]);
    }
}

int main() {
    allocate_map();
    for (int i = 0; i < 120; i++) {
        allocate_pid();
    }
    printPidTable();
    printf("\n\n");
    for (int i = 0; i < 120; i++) {
        release_pid(i + MIN_PID);
    }
    printPidTable();

    return 0;
}

