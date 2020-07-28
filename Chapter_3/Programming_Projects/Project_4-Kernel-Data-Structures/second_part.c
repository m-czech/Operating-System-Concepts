#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/sched/signal.h>  
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>

int start = 15;
module_param(start, int, 0);

struct collatz {
    int num;

    struct list_head list;
};

LIST_HEAD(collatz_list);

void traverse_list(void) {
    printk(KERN_INFO "started traversing\n");
    struct collatz *ptr;
    list_for_each_entry(ptr, &collatz_list, list) {
        printk(KERN_INFO "%d\n", ptr->num);
    }
    printk(KERN_INFO "finished traversing\n");
}

void append_to_list(int num) {
    struct collatz *node;
    node = kmalloc(sizeof(*node), GFP_KERNEL);
    node->num = num;
    INIT_LIST_HEAD(&(node->list));
    list_add_tail(&(node->list), &collatz_list);
}


int get_collatz_nums(int num) {
    append_to_list(num);
    if (num == 1) {
    	return 1;
    }
    if ((num % 2) == 0) {
    	return get_collatz_nums(num / 2);
    }
    else {
        return get_collatz_nums(3 * num + 1);
    }
}

int simple_init(void) {
    get_collatz_nums(start);
    traverse_list();
    
    return 0;
}

void simple_exit(void) {
    // deleting list and freeing memory
    printk("deleting list\n");
    struct collatz *ptr, *next;
    list_for_each_entry_safe(ptr, next, &collatz_list, list) {
        list_del(&(ptr->list));
        kfree(ptr);
    }
    // checking if list is truly empty
    traverse_list();
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("module for presenting next collatz numbers in linked list");
MODULE_AUTHOR("m-czech");
