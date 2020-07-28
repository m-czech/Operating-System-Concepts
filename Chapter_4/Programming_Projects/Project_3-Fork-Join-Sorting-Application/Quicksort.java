import java.util.Random;
import java.util.concurrent.*;

class SortingTask extends RecursiveAction {
    int[] arr;
    int start;
    int stop;
    final int TRESHOLD = 100;

    SortingTask(int[] arr, int start, int stop) {
        this.arr = arr;
        this.start = start;
        this.stop = stop;
    }

    void insertionSort() {
        for (int i = start + 1; i <= stop; i++) {
            int key = arr[i];
            int j = i - 1;

            while (j >= start && key < arr[j]) {
                arr[j + 1] = arr[j];
                j -= 1;
            }

            arr[j + 1] = key;
        }
    }

    public void compute() {
        if ((stop - start) < TRESHOLD) {
            insertionSort();
        }
        else {
            Partition partition = new Partition(arr, start, stop, TRESHOLD);
            partition.fork();
            Integer pivot = partition.join();

            SortingTask leftHalf = new SortingTask(arr, start, pivot - 1);
            SortingTask rightHalf = new SortingTask(arr, pivot + 1, stop);

            invokeAll(leftHalf, rightHalf);
        }
    }
}

class Partition extends RecursiveTask<Integer> {
    int[] arr;
    int start;
    int stop;
    final int TRESHOLD;

    Partition(int[] arr, int start, int stop, final int TRESHOLD) {
        this.arr = arr;
        this.start = start;
        this.stop = stop;
        this.TRESHOLD = TRESHOLD;
    }

    void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    public Integer compute() {
        int pivot = arr[stop];
        int i = start - 1;
        for (int j = start; j < stop; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr, i, j);
            }
        }
        swap(arr, i + 1, stop);
        return  i + 1;
    }
}

public class Quicksort {
    public static void mergeSort(int[] arr) {
        int start = 0;
        int stop = arr.length - 1;

        ForkJoinPool pool = new ForkJoinPool();
        SortingTask task = new SortingTask(arr, start, stop);
        pool.invoke(task);
    }

    public static void main(String[] args) throws InterruptedException {
        final int size = 888;

        Random rand = new Random();
        int[] arr = new int[size];
        for (int i = 0; i < size; i++) {
            arr[i] = rand.nextInt();
        }

        mergeSort(arr);


        for (int i = 0; i < size; i++) {
            System.out.println(arr[i]);
        }
    }
}
