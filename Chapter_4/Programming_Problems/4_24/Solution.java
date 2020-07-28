import java.util.Scanner;
import java.util.concurrent.ThreadLocalRandom;

public class Solution {
    static Integer pointsInsideCircle = 0;

    static class PiCalculation implements Runnable {
        Integer pointsToCreate = 0;
        PiCalculation(int pointsToCreate) {
            this.pointsToCreate = pointsToCreate;
        }

        boolean isInsideCircle(Integer x, Integer y) {
            if ((Math.pow(x, 2) + Math.pow(y, 2)) <= 1) {
                return true;
            }
            return false;
        }

        public void run() {
            int a = 0;
            for (int i = 0; i < pointsToCreate; i++) {
                Integer x = ThreadLocalRandom.current().nextInt(-1, 2);
                Integer y = ThreadLocalRandom.current().nextInt(-1, 2);
                System.out.println(a++ + " " + x + " " + y);
                if (isInsideCircle(x, y)) {
                    pointsInsideCircle += 1;
                }
            }
        }
    }
    
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        Integer pointsToCreate = scan.nextInt();
        Thread piWorker = new Thread(new PiCalculation(pointsToCreate));
        piWorker.start();
        try {
            piWorker.join();
        }
        catch (Exception ignored) {}
        float remainder = (float)pointsInsideCircle / pointsToCreate;
        float pi = 4 * remainder;
        System.out.println(pi);

    }
}