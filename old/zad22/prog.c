#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "kz_device"
#define SUCCESS 0
#define BUF_LEN 80
#define TARGET_STRING "IMIE NAZWISKO"
#define RESPONSE_STRING "NUMER INDEXU"

static int major; 
static char msg[BUF_LEN];
static char *msg_ptr;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int __init kz_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", major);
        return major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);

    return SUCCESS;
}

static void __exit kz_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Goodbye, world!\n");
}

static int device_open(struct inode *inode, struct file *file)
{
    msg_ptr = msg;
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset)
{
    int bytes_read = 0;

    if (*msg_ptr == 0)
        return 0;

    while (length && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    int i;

    for (i = 0; i < len && i < BUF_LEN; i++)
        get_user(msg[i], buff + i);

    msg[i] = '\0';

    if (strcmp(msg, TARGET_STRING) == 0) {
        strncpy(msg, RESPONSE_STRING, BUF_LEN);
    } else {
        msg[0] = '\0';
    }

    return i;
}

module_init(kz_init);
module_exit(kz_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IMIE I NAZWISKO");
MODULE_DESCRIPTION("A simple character device driver");
