import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

enum State {THINKING, HUNGRY, EATING};

class Philosophers implements DiningServer{

    class Philosopher implements Runnable{
        Condition condition;
        int id;
        State state;

        Philosopher(int id, ReentrantLock lock) {
            condition = lock.newCondition();
            this.id = id;
        }

        public void run() {
            takeForks(id);
            returnForks(id);
        }
    }

    static final int PHILOSOPHERS_AMOUNT = 5;
    Philosopher[] philosophers = new Philosopher[PHILOSOPHERS_AMOUNT];
    ReentrantLock lock = new ReentrantLock();

    Philosophers() {
        initializePhilosophers();
    }


    public void takeForks(int philosopherNumber){
        philosophers[philosopherNumber].state = State.HUNGRY;
        testIfAbleToEat(philosopherNumber);
        lock.lock();
        if (philosophers[philosopherNumber].state != State.EATING) {
            try {
                philosophers[philosopherNumber].condition.await();
            }
            // very bad
            catch (InterruptedException ie) { System.out.println(ie.toString()); }
        }
        lock.unlock();
        System.out.println("Philosopher " + philosopherNumber + " picked up forks");

    }
    public void returnForks(int philosopherNumber) {
        philosophers[philosopherNumber].state = State.THINKING;
        System.out.println("Philosopher " + philosopherNumber + " released forks");
        testIfAbleToEat((philosopherNumber + 1) % 5);
        testIfAbleToEat((philosopherNumber + 4) % 5);
    }

    private void testIfAbleToEat(int philosopherNumber) {
        lock.lock();
        if ((philosophers[(philosopherNumber + 4) % 5].state != State.EATING)
                && (philosophers[philosopherNumber].state == State.HUNGRY)
                && ((philosophers[(philosopherNumber + 1) % 5].state != State.EATING))) {
            philosophers[philosopherNumber].state = State.EATING;
            philosophers[philosopherNumber].condition.signal();
        }
        lock.unlock();
    }

    private void initializePhilosophers() {
        for (int i = 0; i < PHILOSOPHERS_AMOUNT; ++i) {
            philosophers[i] = new Philosopher(i, lock);
        }
    }

    public static void main(String[] args) {
        Philosophers philosophersWrapper = new Philosophers();
        Thread[] thread = new Thread[5];

        // create
        for (int i = 0; i < PHILOSOPHERS_AMOUNT; ++i) {
            thread[i] = new Thread(philosophersWrapper.philosophers[i]);
            thread[i].start();
        }

        // wait
        for (int i = 0; i < PHILOSOPHERS_AMOUNT; ++i) {
            try {
                thread[i].join();
            }
            catch (InterruptedException ie) { System.out.println(ie.toString()); }
        }

    }
}


