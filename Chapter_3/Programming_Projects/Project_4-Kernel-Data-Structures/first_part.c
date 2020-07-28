#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/sched/signal.h>  
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct color {
    int red;
    int blue;
    int green;

    struct list_head list;
};

LIST_HEAD(color_list);

void traverse_list(void) {
    printk(KERN_INFO "started traversing\n");
    struct color *ptr;
    list_for_each_entry(ptr, &color_list, list) {
        printk(KERN_INFO "blue %d red %d green %d\n", ptr->blue, ptr->red, ptr->green);
    }
    printk(KERN_INFO "finished traversing\n");
}

int simple_init(void) {
    struct color *node;
    int i;
    for (i = 0; i < 4; i++) {
        node = kmalloc(sizeof(*node), GFP_KERNEL);
        // default values
        node->red = i;
        node->blue = i;
        node->green = i;
        INIT_LIST_HEAD(&(node->list));
        list_add_tail(&(node->list), &color_list);
        printk(KERN_INFO "added %d node\n", i);
    }
    printk(KERN_INFO "done creating list\n");
    traverse_list();
    
    return 0;
}

void simple_exit(void) {
    // deleting list and freeing memory
    printk("deleting list\n");
    struct color *ptr, *next;
    list_for_each_entry_safe(ptr, next, &color_list, list) {
        list_del(&(ptr->list));
        kfree(ptr);
    }
    traverse_list();
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("module for listing all processes in kernel log");
MODULE_AUTHOR("m-czech");
