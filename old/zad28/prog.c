#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/timer.h>

#define DEVICE_NAME "sterownik"
#define BUF_LEN 80

static int major;
static int period = 1;
static struct timer_list my_timer;
static char msg[BUF_LEN];
static int device_open = 0;

static void timer_callback(struct timer_list *t) {
    printk(KERN_INFO "Timer callback called! Period: %d seconds\n", period);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(period * 1000));
}

static int device_open(struct inode *inode, struct file *file) {
    if (device_open)
        return -EBUSY;
    device_open++;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    device_open--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset) {
    if (copy_from_user(msg, buffer, length))
        return -EFAULT;
    msg[length] = '\0';

    if (msg[0] == '1') {
        period = 1;
    } else if (msg[0] == '4') {
        period = 4;
    } else if (msg[0] == '8') {
        period = 8;
    } else {
        printk(KERN_INFO "Invalid input, use 1, 4, or 8.\n");
        return -EINVAL;
    }

    mod_timer(&my_timer, jiffies + msecs_to_jiffies(period * 1000));
    printk(KERN_INFO "Period set to %d seconds\n", period);
    return length;
}

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .write = device_write,
};

static int __init timer_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", major);
        return major;
    }

    timer_setup(&my_timer, timer_callback, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(period * 1000));

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    return 0;
}

static void __exit timer_exit(void) {
    del_timer(&my_timer);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Goodbye, world!\n");
}

module_init(timer_init);
module_exit(timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple timer char driver");
