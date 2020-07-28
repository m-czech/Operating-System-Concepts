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

void traverse_list(struct list_head list_to_traverse) {
    struct color *ptr;
    list_for_each_entry(ptr, &list_to_traverse, list) {
        printk("blue %d red %d green %d", ptr->blue, ptr->red, ptr->green);
    }
}

LIST_HEAD(color_list);

int simple_init(void) {
    struct color *node;
    int i;
    for (i = 0; i < 4; i++) {
        node = kmalloc(sizeof(*node), GFP_KERNEL);
        node->red = i + 10;
        node->blue = i + 20;
        node->green = i + 30;
        INIT_LIST_HEAD(&node->list);
        list_add_tail(&node->list, &color_list);
    }

    traverse_list(color_list);
    
    return 0;
}

void simple_exit(void) {
    // deleting list and freeing memory
    struct color *ptr, *next;
    list_for_each_entry_safe(ptr, next, &color_list, list) {
        list_del(&ptr->list);
        kfree(ptr);
    }
    traverse_list(color_list);
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("module for listing all processes in kernel log");
MODULE_AUTHOR("m-czech");