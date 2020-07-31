/**
 * Task to be scheduled by the scheduling alogrithm.
 *
 * Each task is represented by
 *
 *  String name - a task name, not necessarily unique
 *
 *  int tid - unique task identifier
 *
 *  int priority - the relative priority of a task where a higher number indicates
 *  higher relative priority.
 *
 *  int burst - the CPU burst of this this task
 */

import java.util.concurrent.atomic.AtomicInteger;

public class Task
{
    // the representation of each task
    private String name;
    private int tid;
    private int priority;
    private int burst;
    private int turnaroundTime;
    private int waitingTime;
    private int responseTime;
    private boolean isWaitingTimeSet;
    private boolean isResponseTimeSet;

    /**
     * We use an atomic integer to assign each task a unique task id.
     */
    private static AtomicInteger tidAllocator = new AtomicInteger();

    public Task(String name, int priority, int burst) {
        this.name = name;
        this.priority = priority;
        this.burst = burst;

        this.tid = tidAllocator.getAndIncrement();
    }

    /**
     * Appropriate getters
     */
    public String getName() {
        return name;
    }

    public int getTid() {
        return tid;
    }

    public int getPriority() {
        return priority;
    }

    public int getBurst() {
        return burst;
    }

    public int getTurnaroundTime() {
        return turnaroundTime;
    }

    public int getWaitingTime() {
        return waitingTime;
    }

    public int getResponseTime() {
        return responseTime;
    }

    public boolean isWaitingTimeSet() {
        return isWaitingTimeSet;
    }

    public boolean isResponseTimeSet() {
        return isResponseTimeSet;
    }

    public void printFinishedProcessInformation() {
        System.out.println("Task " + name + " finished\n" + "Waiting time " + waitingTime + "\n" + "Response time: " +
                responseTime + "\n" + "Turnaround time: " + turnaroundTime + "\n");
    }

    /**
     * Appropriate setters
     */
    public int setPriority(int priority) {
        this.priority = priority;

        return priority;
    }
    
    public int setBurst(int burst) {
        this.burst = burst;

        return burst;
    }

    public int setTurnaroundTime(int turnaroundTime) {
        this.turnaroundTime = turnaroundTime;

        return turnaroundTime;
    }

    public int setWaitingTime(int waitingTime) {
        this.waitingTime = waitingTime;
        isWaitingTimeSet = true;
        return waitingTime;
    }

    public int setResponseTime(int responseTime) {
        this.responseTime = responseTime;
        isResponseTimeSet = true;

        return responseTime;
    }

    /**
     * We override equals() so we can use a
     * Task object in Java collection classes.
     */
    public boolean equals(Object other) {
        if (other == this)
            return true;

        if (!(other instanceof Task))
            return false;

        /**
         * Otherwise we are dealing with another Task.
         * two tasks are equal if they have the same tid.
         */
        Task rhs = (Task)other;
        return (this.tid == rhs.tid) ? true : false;
    }

    public String toString() {
        return
            "Name: " + name + "\n" + 
            "Tid: " + tid + "\n" + 
            "Priority: " + priority + "\n" + 
            "Burst: " + burst + "\n";
    }
}
