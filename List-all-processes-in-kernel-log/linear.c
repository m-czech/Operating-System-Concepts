#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/sched/signal.h>

struct task_Struct *task;

int simple_init(void) {
    printk("BEG");
    struct task_struct *task;
    for_each_process(task) {
        printk("command %s pid %d state %d", task->comm, task->pid, task->state);
    }
    
    return 0;
}

void simple_exit(void) {
    printk("END");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("module for listing all processes in kernel log");
MODULE_AUTHOR("m-czech");