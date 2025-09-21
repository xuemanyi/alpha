#include "../drv.h"

#define CHRDEVBASE_MAJOR 200
#define CHRDEVBASE_NAME  "chrdevbase"

static char read_buf[128];
static char write_buf[128];
static char kernel_data[] = {"kernel data"};

static int chrdevbase_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "open chrdevbase\n");
    return 0;
}

static ssize_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, loff_t *ppos)
{
    int ret;

    memcpy(read_buf, kernel_data, sizeof(kernel_data));

    ret = copy_to_user(buf, read_buf, cnt);
    if (ret == 0) {
        printk(KERN_INFO "send data to user succeeded\n");
        return cnt;
    } else {
        printk(KERN_ERR "send data to user failed, %d bytes could not be copied\n", ret);
        return -EFAULT;
    }
}

static ssize_t chrdevbase_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *ppos)
{
    int ret;

    ret = copy_from_user(write_buf, buf, cnt);
    if (ret == 0) {
        printk(KERN_INFO "write data from user succeeded\n");
        return cnt;
    } else {
        printk(KERN_ERR "write data from user failed, %d bytes could not be copied\n", ret);
        return -EFAULT;
    }
}

static int chrdevbase_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "release chrdevbase\n");
    return 0;
}

static struct file_operations chrdevbase_fops = {
    .owner = THIS_MODULE,
    .open = chrdevbase_open,
    .read = chrdevbase_read,
    .write = chrdevbase_write,
    .release = chrdevbase_release,
};

static int __init chrdevbase_init(void)
{
    int ret;

    ret = register_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME, &chrdevbase_fops);
    if (ret < 0) {
        printk(KERN_ERR "chrdevbase driver register failed\n");
        return ret;
    }

    printk(KERN_INFO "chrdevbase_init()\n");
    return 0;
}

static void __exit chrdevbase_exit(void)
{
    unregister_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME);
    printk(KERN_INFO "chrdevbase_exit()\n");
}

module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sucre");
