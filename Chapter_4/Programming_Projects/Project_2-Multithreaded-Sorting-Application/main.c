#include <stdio.h>
#include <pthread.h>

#define MAX_AMOUNT 100

int numbers_to_sort[MAX_AMOUNT];
int resulting_arr[MAX_AMOUNT];

struct Indexes{
    int start_idx;
    int middle_idx;
    int last_idx;
};

void *sort(const void *args) {
    struct Indexes *index = (struct Indexes *) args;
    for (int i = index->last_idx; i > index->start_idx; i--) {
        for (int j = index->start_idx; j < i; j++) {
            if (numbers_to_sort[j] > numbers_to_sort[i]) {
                int temp = numbers_to_sort[j];
                numbers_to_sort[j] = numbers_to_sort[i];
                numbers_to_sort[i] = temp;
            }
        }
    }
}

void *merge(const void *args) {
    struct Indexes *index = (struct Indexes *)args;

    int a = 0;
    int b = index->middle_idx;
    int k = 0;

    while (a < b && b <= index->last_idx) {
        if (numbers_to_sort[a] < numbers_to_sort[b]) {
            resulting_arr[k] = numbers_to_sort[a++];
        }
        else {
            resulting_arr[k] = numbers_to_sort[b++];
        }
        k++;
    }

    while (a < b) {
        resulting_arr[k++] = numbers_to_sort[a++];
    }

    while (b <= index->last_idx) {
        resulting_arr[k++] = numbers_to_sort[b++];
    }
}

void display_array(int arr[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int numbersAmount;
    scanf("%d", &numbersAmount);
    for (int i = 0; i < numbersAmount; i++) {
        scanf("%d", numbers_to_sort + i);
    }

    int start_idx = 0;
    int middle_idx = (numbersAmount - 1) / 2;
    struct Indexes arr_indexes = {start_idx, middle_idx, numbersAmount - 1};

    // thread to sort first half of the array
    pthread_t first_half_t;
    pthread_attr_t first_half_attr;
    pthread_attr_init(&first_half_attr);
    pthread_create(&first_half_t, &first_half_attr, sort, &arr_indexes);

    // thread to sort second half of the array
    pthread_t second_half_t;
    pthread_attr_t second_half_attr;
    pthread_attr_init(&second_half_attr);
    pthread_create(&second_half_t, &second_half_attr, sort, &arr_indexes);

    pthread_join(first_half_t, NULL);
    pthread_join(second_half_t, NULL);

    // thread to merge both sorted halves
    pthread_t merge_t;
    pthread_attr_t merge_attr;
    pthread_attr_init(&merge_attr);
    pthread_create(&merge_t, &merge_attr, merge, &arr_indexes);
    pthread_join(merge_t, NULL);

    // input array
    display_array(numbers_to_sort, numbersAmount);
    // resulting array
    display_array(resulting_arr, numbersAmount);
    return 0;
}
