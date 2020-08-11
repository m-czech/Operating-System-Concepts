import java.util.*;
public class Client
{
    public static void main(String[] args) {
        ThreadPool pool = new ThreadPool();

        pool.add(new Task(1,2));
        pool.add(new Task(2,3));
        pool.add(new Task(3,4));
        pool.add(new Task(4,5));
        pool.add(new Task(5,6));
        pool.add(new Task(6,7));

        try {
            Thread.sleep(5000);
        }
        catch (InterruptedException ie) {
            System.out.println("sleep");
        }

        pool.shutdown();
    }
}
