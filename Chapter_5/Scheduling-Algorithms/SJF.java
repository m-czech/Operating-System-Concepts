import java.util.*;

public class SJF implements Algorithm {
    List<Task> queueOne;
    List<Task> queueTwo = new ArrayList<Task>();
    final int timeSlice = 10;
    int currentTime;

    SJF(List<Task> queue) {
        this.queueOne = queue;
    }

    public void schedule() {
        try {
            sort();
        }
        catch (NoSuchElementException e) {
            System.out.println("nothing to schedule !");
            return;
        }

        while (true) {
            while (!queueOne.isEmpty()) {
                Task task = pickNextTask();

                if(!task.isWaitingTimeSet()) {
                    task.setWaitingTime(currentTime);
                }

                CPU.run(task, timeSlice);
                // symulating CPU utilization
                currentTime += timeSlice;

                if(!task.isResponseTimeSet()) {
                    task.setResponseTime(currentTime);
                }

                queueOne.remove(task);
                task.setBurst(task.getBurst() - timeSlice);
                if (task.getBurst() <= 0) {
                    task.setTurnaroundTime(currentTime);
                    task.printFinishedProcessInformation();
                }
                else {
                    queueTwo.add(queueTwo.size(), task);
                }
            }
            if (queueTwo.isEmpty()) {
                break;
            }
            // references swap
            List<Task> temp = queueOne;
            queueOne = queueTwo;
            queueTwo = temp;
        }

    }

    public void sort() throws NoSuchElementException {
        List<Task> sortedQueue = new ArrayList<Task>();

        while(!queueOne.isEmpty()) {
            Task highestPriorityTask = null;
            Iterator<Task> it = queueOne.iterator();
            try {
                highestPriorityTask = it.next();
            }
            catch (NoSuchElementException e) {
                throw new NoSuchElementException();
            }

            while (it.hasNext()) {
                Task t = it.next();
                if (t.getPriority() > highestPriorityTask.getPriority()) {
                    highestPriorityTask = t;
                }
            }

            it = queueOne.iterator();

            while (it.hasNext()) {
                Task t = it.next();
                if (highestPriorityTask.getPriority() == t.getPriority()) {
                    sortedQueue.add(t);
                    it.remove();
                }
            }
        }
        queueOne = sortedQueue;
    }

    public Task pickNextTask() {
        Task shortestJobTask = queueOne.get(0);
        for (Task t : queueOne) {
            if (shortestJobTask.getBurst() > t.getBurst()) {
                shortestJobTask = t;
            }
        }
        return shortestJobTask;
    }

}
