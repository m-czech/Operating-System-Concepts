import java.util.List;

public class FCFS implements Algorithm {
    List<Task> queue;
    int posInQueue;

    FCFS(List<Task> queue) {
        this.queue = queue;
    }


    public void schedule() {
        if (queue.isEmpty()) {
            System.out.println("no tasks to schedule !");
            return;
        }

        Task task = pickNextTask();
        while (!(task == null)) {
            CPU.run(task, task.getBurst());
            task.setBurst(0);
            if (task.getBurst() <= 0) {
                task.printFinishedProcessInformation();
            }
            task = pickNextTask();
        }
    }

    public Task pickNextTask() {
        try {
            Task task = queue.get(posInQueue++);
            return task;
        }
        catch (IndexOutOfBoundsException ignored) {
            return null;
        }
    }
}
