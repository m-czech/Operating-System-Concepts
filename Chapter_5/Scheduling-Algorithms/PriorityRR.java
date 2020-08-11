import java.util.*;

public class PriorityRR implements Algorithm {
    List<Task> queue;
    ArrayList<ArrayList<Task>> arr = new ArrayList<ArrayList<Task>>();
    Iterator<ArrayList<Task>> arr_it = arr.iterator();
    Iterator<Task> it;
    final int timeSlice = 10;
    int currentTime;

    PriorityRR (List<Task> queue) {
        this.queue = queue;
        it = queue.iterator();
    }

    public void splitToSeparateQueues() {
        arr_it = arr.iterator();
        while (!queue.isEmpty()) {
            Task maxPriority = null;
            try {
                it = queue.iterator();
                maxPriority = it.next();
            }
            catch (NoSuchElementException e) {
                throw new NoSuchElementException();
            }

            it = queue.iterator();
            ArrayList<Task> arr = arr_it.next();

            while (it.hasNext()) {
                Task candidate = it.next();
                if (maxPriority.getPriority() == candidate.getPriority()) {
                    arr.add(candidate);
                    it.remove();
                }
            }
        }
    }

    public void schedule() {
        try {
            splitToSeparateQueues();
        }
        catch (NoSuchElementException e) {
            System.out.println("no tasks to schedule !");
        }

        while (!arr.isEmpty()) {
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
        if(arr.isEmpty()) {
           return null;
        }

        ArrayList<Task>  single_queue = arr_it.next();
        if (single_queue.size() == 1) {
            return single_queue.get(0);
        }
        it = single_queue.iterator();


    }
}
