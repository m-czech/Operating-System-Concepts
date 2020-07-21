#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

long res = 0;

ssize_t proc_write(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
int rv = 0;
char *k_mem;

k_mem = kmalloc(count, GFP_KERNEL);

copy_from_user(k_mem, usr_buf, count);

if (kstrtol(k_mem, 10, &res)) {
printk(KERN_INFO "%saved pid");
}

kfree(k_mem);
return count;
}

static struct file_operations proc_ops = {
        .owner = THIS_MODULE;
        .write = proc_write;
};

int simple_init(void) {
    printk(KERN_INFO "Loading Module\n");
    return 0;
}

void simple_exit(void) {
    printk(KERN_INFO "Removin Module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("module for writing and reading from /proc filesystem");
MODULE_AUTHOR("m-czech");
