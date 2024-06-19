#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/device.h>

#define DEVICE_NAME "simple_driver"

#define IOCTL_SET_FIRSTNAME _IOW(0, 0, char *)
#define IOCTL_SET_LASTNAME _IOW(0, 1, char *)
#define IOCTL_SET_INDEXNUMBER _IOW(0, 2, int)

#define IOCTL_GET_FIRSTNAME _IOR(0, 3, char *)
#define IOCTL_GET_LASTNAME _IOR(0, 4, char *)
#define IOCTL_GET_INDEXNUMBER _IOR(0, 5, int)

static char firstname[100];
static char lastname[100];
static int index_number;

static int major_num;
static struct class* simple_driver_class = NULL;
static struct device* simple_driver_device = NULL;

static long simple_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch(cmd) {
        case IOCTL_SET_FIRSTNAME:
            copy_from_user(firstname, (char *)arg, sizeof(firstname));
            break;
        case IOCTL_SET_LASTNAME:
            copy_from_user(lastname, (char *)arg, sizeof(lastname));
            break;
        case IOCTL_SET_INDEXNUMBER:
            copy_from_user(&index_number, (int *)arg, sizeof(index_number));
            break;
        case IOCTL_GET_FIRSTNAME:
            copy_to_user((char *)arg, firstname, sizeof(firstname));
            break;
        case IOCTL_GET_LASTNAME:
            copy_to_user((char *)arg, lastname, sizeof(lastname));
            break;
        case IOCTL_GET_INDEXNUMBER:
            copy_to_user((int *)arg, &index_number, sizeof(index_number));
            break;
    }
    return 0;
}

static int simple_open(struct inode *inode, struct file *file) {
    return 0;
}

static int simple_release(struct inode *inode, struct file *file) {
    return 0;
}

static struct file_operations fops = {
    .unlocked_ioctl = simple_ioctl,
    .open = simple_open,
    .release = simple_release
};

static int __init simple_driver_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    simple_driver_class = class_create(THIS_MODULE, DEVICE_NAME);
    simple_driver_device = device_create(simple_driver_class, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
    return 0;
}

static void __exit simple_driver_exit(void) {
    device_destroy(simple_driver_class, MKDEV(major_num, 0));
    class_unregister(simple_driver_class);
    class_destroy(simple_driver_class);
    unregister_chrdev(major_num, DEVICE_NAME);
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Twoje Imię i Nazwisko");
MODULE_DESCRIPTION("Prosty sterownik znakowy z dynamicznym przydziałem numeru głównego i ioctl");
