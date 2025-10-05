#include "drv.h"

#define GPIO_LED_CNT  1
#define GPIO_LED_NAME "gpioled"
#define LED_OFF       0
#define LED_ON        1

struct gpioled_dev{
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    int major;
    int minor;
    struct device_node *nd;
    int led_gpio;
    struct mutex lock;
};

struct gpioled_dev gpioled;

static int led_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &gpioled;
    
    if (mutex_lock_interruptible(&gpioled.lock)) {
        return -ERESTARTSYS;
    }

#if 0
    mutex_lock(&gpioled.lock);
#endif

    return 0;
}

static ssize_t led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    return 0;
}

static ssize_t led_write(struct file *filp, const char  __user *buf, size_t cnt, loff_t *offt)
{
    int ret = 0;
    unsigned char databuf[1] = {0};
    unsigned char ledstat = 0;
    struct gpioled_dev *dev = filp->private_data;

    ret = copy_from_user(databuf, buf, cnt);
    if (ret < 0)
    {
        printk("kernel write failed\n");
        return -EFAULT;
    }

    ledstat = databuf[0];
    if (ledstat == LED_ON) {
        gpio_set_value(dev->led_gpio, 0);
    } else if (ledstat == LED_OFF) {
        gpio_set_value(dev->led_gpio, 1);
    }

    return 0;
}

static int led_release(struct inode *inode, struct file *filp)
{
    struct gpioled_dev *dev = filp->private_data;

    mutex_unlock(&dev->lock);

    return 0;
}

static struct file_operations gpioled_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .release = led_release,
};

static int __init led_init(void)
{
    int ret = 0;

    mutex_init(&gpioled.lock);

    gpioled.nd = of_find_node_by_path("/gpioled");
    if (gpioled.nd == NULL) {
        printk("gpioled node not find\n");
        return -EINVAL;
    } else {
        printk("gpioled node find\n");
    }

    gpioled.led_gpio = of_get_named_gpio(gpioled.nd, "led-gpio", 0);
    if (gpioled.led_gpio < 0)
    {
        printk("can't get led-gpio\n");
        return -EINVAL;
    }

    printk("led-gpio num = %d\n", gpioled.led_gpio);

    ret = gpio_direction_output(gpioled.led_gpio, 1);
    if (ret < 0) {
        printk("can't set gpio\n");
    }

    if (gpioled.major) {
        gpioled.devid = MKDEV(gpioled.major, 0);
        register_chrdev_region(gpioled.devid, GPIO_LED_CNT, GPIO_LED_NAME);
    } else {
        alloc_chrdev_region(&gpioled.devid, 0, GPIO_LED_CNT, GPIO_LED_NAME);
        gpioled.major = MAJOR(gpioled.devid);
        gpioled.minor = MINOR(gpioled.devid);
    }

    printk("gpioled major = %d, minor = %d\n", gpioled.major, gpioled.minor);

    gpioled.cdev.owner = THIS_MODULE;
    cdev_init(&gpioled.cdev, &gpioled_fops);
    cdev_add(&gpioled.cdev, gpioled.devid, GPIO_LED_CNT);
    gpioled.class = class_create(THIS_MODULE, GPIO_LED_NAME);
    if (IS_ERR(gpioled.class)) {
        return PTR_ERR(gpioled.class);
    }

    gpioled.device = device_create(gpioled.class, NULL, gpioled.devid, NULL, GPIO_LED_NAME);
    if (IS_ERR(gpioled.device)) {
        return PTR_ERR(gpioled.device);
    }
    
    return 0;
}

static void __exit led_exit(void)
{
    cdev_del(&gpioled.cdev);
    unregister_chrdev_region(gpioled.devid, GPIO_LED_CNT);
    device_destroy(gpioled.class, gpioled.devid);
    class_destroy(gpioled.class);
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xuemanyi");