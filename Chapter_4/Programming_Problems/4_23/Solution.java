import java.util.Scanner;


public class Solution {
    
    static class Primes implements Runnable {
        Integer bound;
        Primes(Integer bound) {
            this.bound = bound;
        }
        public void run() {
            for (int i = 2; i <= bound; i++) {
                boolean isRemainder = true;
                for (int j = 2; j <= Math.sqrt(i); j++) {
                    if ((i % j) == 0) {
                        isRemainder = false;
                    }
                }
                if (isRemainder) {
                    System.out.print(i + " ");
                }
                isRemainder = true;
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        Integer bound = scan.nextInt();
        Thread primes_worker = new Thread(new Primes(bound));
        primes_worker.start();
    }
}
