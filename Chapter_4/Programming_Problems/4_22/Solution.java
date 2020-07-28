public class Solution {
    static int average = 5;
    static int minimum = 5;
    static int maximum = 5;

    static class Average implements Runnable {
        Integer[] data;
        Average(Integer[] data) {
            this.data = data;
        }
        
        public void run() {
            int sum = 0;
            for (Integer x : data) {
                sum += x;
            }
            average = sum / data.length;
        }
    }

    static class Minimum implements Runnable {
        Integer[] data;
        Minimum(Integer[] data) {
            this.data = data;
        }
        
        public void run() {
            if (data.length < 2) {
                minimum = data[0];
            }
            int min = data[0];
            for (int i = 1; i < data.length; i++) {
                if (data[i] < min) {
                    min = data[i];
                }
            }
            minimum = min;
        }
    }

    // Maximum is done similarly

    public static void main(String[] args) {
        Integer data[] = new Integer[args.length];
        for (int i = 0; i < args.length; i++) {
            data[i] = Integer.parseInt(args[i]);
        }

        Thread average_worker = new Thread(new Average(data));
        Thread minimum_worker = new Thread(new Minimum(data));
        average_worker.start();
        minimum_worker.start();
        
        System.out.println(average);
        System.out.println(minimum);

    }
}