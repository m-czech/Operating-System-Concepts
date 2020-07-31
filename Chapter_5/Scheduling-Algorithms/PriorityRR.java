import java.util.*;

public class PriorityRR implements Algorithm {
    List<Task> queue;
    List<Task> aux = new ArrayList<Task>();
    Iterator<Task> it;
    final int timeSlice = 10;
    int currentTime;

    PriorityRR (List<Task> queue) {
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
        while (!queue.isEmpty()) {
            Task task = pickNextTask();

            if (!task.isWaitingTimeSet()) {
                task.setWaitingTime(currentTime);
            }

            CPU.run(task, timeSlice);
            currentTime += timeSlice;

            if (!task.isResponseTimeSet()) {
                task.setResponseTime(currentTime);
            }

            task.setBurst(task.setBurst(task.getBurst() - timeSlice));
            if (task.getBurst() <= 0) {
                task.setTurnaroundTime(currentTime);
                task.printFinishedProcessInformation();
                it.remove();
            }
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
