#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "char_device"
#define BUF_LEN 1024

static int major;
static char msg[BUF_LEN];
static int device_open = 0;

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
    int i;
    if (length > BUF_LEN - 1)
        length = BUF_LEN - 1;
    if (copy_from_user(msg, buffer, length))
        return -EFAULT;
    msg[length] = '\0';

    for (i = 0; i < length; i++) {
        if (msg[i] >= 'a' && msg[i] <= 'z') {
            msg[i] = msg[i] - 'a' + 'A';
        }
    }

    printk(KERN_INFO "Received message: %s\n", msg);
    return length;
}

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .write = device_write,
};

static int __init char_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", major);
        return major;
    }
    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    return 0;
}

static void __exit char_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Goodbye, world!\n");
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
