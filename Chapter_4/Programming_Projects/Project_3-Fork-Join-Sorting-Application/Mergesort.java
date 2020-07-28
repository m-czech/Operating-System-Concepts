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

    public void compute() {
        if (start < stop) {
            int middle = start + (stop - start) / 2;

            SortingTask leftHalf = new SortingTask(arr, start, middle);
            SortingTask rightHalf = new SortingTask(arr, middle + 1, stop);

            invokeAll(leftHalf, rightHalf);
            MergeTask merge = new MergeTask(arr, start, stop, middle);
            merge.fork();
            merge.join();

        }
    }
}

class MergeTask extends RecursiveAction {
    int[] arr;
    int start;
    int stop;
    final int TRESHOLD;

    MergeTask(int[] arr, int start, int stop, final int TRESHOLD) {
        this.arr = arr;
        this.start = start;
        this.stop = stop;
        this.TRESHOLD = TRESHOLD;
    }

    public void compute() {
        int elementsAmount = stop - start + 1;
        if (elementsAmount < TRESHOLD) {
            insertionSort();
        }
        else {
            mergeSort();
        }
    }

    void mergeSort() {
        int middle = start + (stop - start) / 2;
        int elementsInLeftHalf = middle - start + 1;
        int elementsInRightHalf = stop - middle;

        int[] leftHalf = new int[elementsInLeftHalf];
        int[] rightHalf = new int[elementsInRightHalf];

        for (int i = 0; i < elementsInLeftHalf; i++) {
            leftHalf[i] = arr[start + i];
        }

        for (int i = 0; i < elementsInRightHalf ; i++) {
            rightHalf[i] = arr[middle + 1 + i];
        }

        int a = 0;
        int b = 0;
        int k = start;
        while (a < elementsInLeftHalf && b < elementsInRightHalf) {
            if (leftHalf[a] <= rightHalf[b]) {
                arr[k] = leftHalf[a++];
            }
            else {
                arr[k] = rightHalf[b++];
            }
            k++;
        }

        while (a < elementsInLeftHalf) {
            arr[k++] = leftHalf[a++];
        }

        while (b < elementsInRightHalf) {
            arr[k++] = rightHalf[b++];
        }
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
}


public class Mergesort {
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
