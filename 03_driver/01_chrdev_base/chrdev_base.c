#include "drv.h"

#define CHRDEVBASE_MAJOR 200
#define CHRDEVBASE_NAME  "chrdevbase"

static char read_buf[128];
static char write_buf[128];
static char kernel_data[] = {"kernel data"};

static int chrdevbase_open(struct inode *inode, struct file *filp)
{
    // printk("open chrdebase \r\n");
    printk(KERN_INFO "open chrdevbase\n"); // kern info level should set

    return 0;
}

static ssize_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int ret = 0;

    memcpy(read_buf, kernel_data, sizeof(kernel_data));
    
    ret = copy_to_user(buf, read_buf, cnt);
    if (ret == 0) {
        printk("send data to user succeeded\n");
        return cnt;
    }
    else {
        printk("send data to user failed, %d bytes could not be copied\n", ret);
        return -EFAULT;
    }

    // return 0;
    return cnt; // read and write shoule return cnt, not 0
}

static ssize_t chrdevbase_write(struct file *filp, const char __user *buf, 
    size_t cnt, loff_t offt)
{
    int ret = 0;

    ret = copy_from_user(write_buf, buf, cnt);
    if (ret == 0) {
        printk("recv data to user succeeded\n");
        return cnt;
    }
    else {
        printk("recv data to user failed, %d bytes could not be copied\n", ret);
        return -EFAULT;
    }

    return 0;
    return cnt;
}

static int chrdevbase_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static struct file_operations chrdevbase_fops = {
    .owner = THIS_MODULE,
    .open = chrdevbase_open,
    .read = chrdevbase_read,
    .write = chrdevbase_write,
    .release = chrdevbase_release,
};

static int __init  chrdevbase_init(void)
{
    int ret = 0;

    ret = register_chrdev(CHRDEVBASE_MAJIR, CHRDEVBASE_NAME, &chrdevbase_fops);
    if (ret < 0)
    {
        printk("chrdevbase driver register failed\n");
    }

    printk("chrdevbase_init()\n");

    return 0;
}

static void __exit chrdevbase_exit(void)
{
    unregister_chrdev(CHRDEVBASE_MAJIR, CHRDEVBASE_NAME);

    printk("chrdevbase_exit()\n");
}

module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sucre");
