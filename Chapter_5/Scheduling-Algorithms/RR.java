import java.util.*;

public class RR implements Algorithm {
    List<Task> queue;
    Iterator<Task> it;
    final int timeSlice = 10;
    int currentTime;

    RR(List<Task> queue) {
        this.queue = queue;
        it = queue.iterator();
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

            task.setBurst(task.getBurst() - timeSlice);
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
