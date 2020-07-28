#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int matrix[9][9];
int results[27];
pthread_t ids[27];
int nextFreeIndex = 0;

typedef struct {
    int row;
    int column;
    int thread_id
} parameters;

void setZeros(int arr[], int length) {
    for (int i = 0; i < length; i++) {
        arr[i] = 0;
    }
}

int check_if_every_number_exists(int nums[]) {
    for (int i = 0; i < 9; i++) {
        if (nums[i] == 0) {
            return 0;
        }
    }
    return 1;
}

void check_subgrid(parameters *grid) {
    int nums[9];
    setZeros(nums, 9);

    for(int row = grid->row; row < grid->row + 3; row++) {
        for (int column = grid->column; column < grid->column + 3; column++) {
            int element = matrix[row][column];
            nums[element - 1] = 1;
        }
    }

    if(check_if_every_number_exists(nums)) {
        results[grid->thread_id] = 1;
    }
}

void *run_subgrid(void *args) {
    check_subgrid((parameters *)args);
    free(args);
}

void check_grids() {
    for (int row = 0; row <= 7; row += 3) {
        for (int column = 0; column <= 7; column += 3) {
            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);

            parameters *params = (parameters *)malloc(sizeof(parameters));
            params->row = row;
            params->column = column;
            params->thread_id = nextFreeIndex;
            pthread_create(&tid, &attr, run_subgrid, params);

            ids[nextFreeIndex++] = tid;
        }
    }
}

void *run_check_row(void *args) {
    int row = ((parameters *)args)->row;
    int nums[9];
    setZeros(nums, 9);

    for (int column = 0; column < 9; column++) {
        int element = matrix[row][column];
        nums[element - 1] = 1;
    }

    if(check_if_every_number_exists(nums)) {
        results[((parameters *)args)->thread_id] = 1;
    }

    free(args);
}

void *run_check_column(void *args) {
    int column = ((parameters *)args)->column;
    int nums[9];
    setZeros(nums, 9);

    for (int row = 0; row < 9; row++) {
        nums[matrix[row][column] - 1] = 1;
    }

    if(check_if_every_number_exists(nums)) {
        results[((parameters *)args)->thread_id] = 1;
    }

    free(args);
}

void check_rows_columns() {
    for (int i = 0; i < 9; i++) {
        pthread_t row_tid;
        pthread_attr_t row_attr;
        pthread_attr_init(&row_attr);
        parameters *row_params = (parameters *)malloc(sizeof(parameters));
        row_params->row = i;
        row_params->column = -1;
        row_params->thread_id = nextFreeIndex;
        pthread_create(&row_tid, &row_attr, run_check_row, row_params);
        ids[nextFreeIndex++] = row_tid;

        pthread_t col_tid;
        pthread_attr_t col_attr;
        pthread_attr_init(&col_attr);
        parameters *col_params = malloc(sizeof(parameters));
        col_params->row = -1;
        col_params->column = i;
        col_params->thread_id = nextFreeIndex;
        pthread_create(&col_tid, &col_attr, run_check_column, col_params);
        ids[nextFreeIndex++] = col_tid;

    }
}

void load_sudoku_matrix() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int input;
            scanf("%d", &input);
            if (input < 1 || input > 9) {
                printf("wrong input\n");
                exit(1);
            }
            matrix[i][j] = input;
        }
    }
}

int main() {
    load_sudoku_matrix();
    check_grids();
    check_rows_columns();

    for (int i = 0; i < nextFreeIndex; i++) {
        pthread_join(ids[i], NULL);
    }
    for (int j = 0; j < nextFreeIndex; j++) {
        if (results[j] == 0) {
            printf("not correct\n");
            return 1;
        }
    }
    printf("correct");
    return 0;
}
