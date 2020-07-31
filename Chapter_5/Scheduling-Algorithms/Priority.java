import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;

public class Priority implements Algorithm {
    List<Task> queue;
    List<Task> aux = new ArrayList<Task>();
    Iterator<Task> it;
    final int timeSlice = 10;
    int currentTime;

    Priority (List<Task> queue) {
        this.queue = queue;
        it = queue.iterator();
    }

    public void sort() {
        while (!queue.isEmpty()) {
            Task maxPriority = null;
            try {
                it = queue.iterator();
                maxPriority = it.next();
            }
            catch (NoSuchElementException e) {
                throw new NoSuchElementException();
            }

            while (it.hasNext()) {
                Task candidate = it.next();
                if (maxPriority.getPriority() < candidate.getPriority()) {
                    maxPriority = candidate;
                }
            }

            it = queue.iterator();

            while (it.hasNext()) {
                Task candidate = it.next();
                if (maxPriority.getPriority() == candidate.getPriority()) {
                    aux.add(candidate);
                    it.remove();
                }
            }
        }
        queue = aux;
    }

    public void schedule() {
        try {
            sort();
        }
        catch (NoSuchElementException e) {
            System.out.println("no tasks to schedule !");
        }

        while (!queue.isEmpty()) {
            Task task = pickNextTask();

            task.setWaitingTime(currentTime);

            CPU.run(task, timeSlice);
            currentTime += timeSlice;

            task.setResponseTime(currentTime);

            task.setBurst(0);
            task.setTurnaroundTime(currentTime);
            task.printFinishedProcessInformation();
            it.remove();
        }
    }

    public Task pickNextTask() {
        if (!queue.isEmpty()) {
            if (!it.hasNext()) {
                it = queue.iterator();
            }
            return it.next();
        }
        return null;
    }
}
