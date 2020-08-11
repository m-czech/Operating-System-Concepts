import java.util.ArrayList;
import java.util.List;

/**
 * A simple thread pool API.
 * 
 * Tasks that wish to get run by the thread pool must implement the
 * java.lang.Runnable interface.
 */
public class ThreadPool
{
	final int size;

	class Wrapper implements Runnable {
		void tryDequeueTask() throws InterruptedException {
			synchronized (queueMonitor) {
				while (queue.size() <= 0) {
					queueMonitor.wait();
				}
				Runnable task = queue.remove(0);
				task.run();
				queueMonitor.notify();
			}
		}

		public void run() {
			try {
				while (!Thread.currentThread().isInterrupted()) {
					tryDequeueTask();
				}
			}
			catch (InterruptedException ie) {
				// exception from tryDequeueTask()
			}
		}
	}


	List<Runnable> queue = new ArrayList<>();
	Thread[] pool;
	final Object queueMonitor = new Object();

	ThreadPool() {
		size = 10;
		pool = new Thread[size];

		for (int i = 0; i < size; ++i) {
			pool[i] = new Thread(new Wrapper());
			pool[i].start();
		}
	}

	public ThreadPool(int size) {
		this.size = size;
		pool = new Thread[size];

		for (int i = 0; i < size; ++i) {
			pool[i] = new Thread(new Wrapper());
			pool[i].start();
		}
    }

	/**
	 * shut down the pool.
	 */
	public void shutdown() {
		for (int i = 0; i < size; ++i) {
			pool[i].interrupt();
		}
	}
	
	/**
	 * Add work to the queue.
	 */
	public void add(Runnable task) {
		synchronized (queueMonitor) {
			queue.add(task);
			queueMonitor.notify();
		}
	}
}
