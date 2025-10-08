#include "drv.h"

#define TIMER_CNT  1
#define TIMER_NAME "timer"
#define CLOSE_CMD  (_IO(0XEF, 0x1))
#define OPEN_CMD   (_IO(0XEF, 0x2))
#define SET_PERIOD_CMD (_IO(0XEF, 0x3))
#define LED_ON     1
#define LED_OFF    0

struct timerdev{
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    int major;
    int minor;
    struct device_node *nd;
    int led_gpio;
    int timerperiod;
    struct timer_list timer;
    spinlock_t lock;
};

struct timerdev timer_dev;

static int led_init(void)
{
    int ret = 0;

    timer_dev.nd = of_find_node_by_path("/gpioled");
    if (timer_dev.nd == NULL) {
        return -EINVAL;
    }

    timer_dev.led_gpio = of_get_named_gpio(timer_dev.nd, "led-gpio", 0);
    if (timer_dev.led_gpio < 0) {
        printk("can't get led\n");
        return -EINVAL;
    }

    gpio_request(timer_dev.led_gpio, "led");
    ret = gpio_direction_output(timer_dev.led_gpio, 1);
    if (ret < 0) {
        printk("can't set gpio\n");
    }

    return 0;
}

static int timer_open(struct inode *inode, struct file *filp)
{
    int ret = 0;

    filp->private_data = &timer_dev;
    timer_dev.timerperiod = 1000;
    ret = led_init();
    if (ret < 0) {
        return ret;
    }

    return 0;
}

static long timer_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct timerdev *dev = (struct timerdev *)filp->private_data;
    int timerperiod = 0;
    unsigned long flags = 0;

    switch (cmd) {
        case CLOSE_CMD:
            del_timer_sync(&dev->timer);
            break;
        case OPEN_CMD:
            spin_lock_irqsave(&dev->lock, flags);
            timerperiod = dev->timerperiod;
            spin_unlock_irqrestore(&dev->lock, flags);
            mod_timer(&dev->timer, jiffies + msecs_to_jiffies(timerperiod));
            break;
        case SET_PERIOD_CMD:
            spin_lock_irqsave(&dev->lock, flags);
            dev->timerperiod = arg;
            spin_unlock_irqrestore(&dev->lock, flags);
            mod_timer(&dev->timer, jiffies + msecs_to_jiffies(arg));
            break;
        default:
            break;
    }

    return 0;
}

static struct file_operations timers_fops = {
    .owner = THIS_MODULE,
    .open = timer_open,
    .unlocked_ioctl = timer_unlocked_ioctl,
};

void timer_function(unsigned long arg)
{
    struct timerdev *dev = (struct timerdev *)arg;
    static int sta = 1;
    unsigned long flags = 0;

    sta = !sta;

    gpio_set_value(dev->led_gpio, sta);

    spin_lock_irqsave(&dev->lock, flags);
    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(dev->timerperiod));
}

static int __init timer_init(void)
{
    spin_lock_init(&timer_dev.lock);

    if (timer_dev.major) {
        timer_dev.devid = MKDEV(timer_dev.major, 0);
        register_chrdev_region(timer_dev.devid, TIMER_CNT, TIMER_NAME);
    } else {
        alloc_chrdev_region(&timer_dev.devid, 0, TIMER_CNT, TIMER_NAME);
        timer_dev.major = MAJOR(timer_dev.devid);
        timer_dev.minor = MINOR(timer_dev.devid);
    }

    timer_dev.cdev.owner = THIS_MODULE;
    cdev_init(&timer_dev.cdev, &timers_fops);

    cdev_add(&timer_dev.cdev, timer_dev.devid, TIMER_CNT);
    timer_dev.class = class_create(THIS_MODULE, TIMER_NAME);
    if (IS_ERR(timer_dev.class)) {
        return PTR_ERR(timer_dev.class);
    }

    timer_dev.device = device_create(timer_dev.class, NULL, timer_dev.devid, NULL, 
                                        TIMER_NAME);
    if (IS_ERR(timer_dev.device)) {
        return PTR_ERR(timer_dev.device);
    }

    init_timer(&timer_dev.timer);
    timer_dev.timer.function = timer_function;
    timer_dev.timer.data = (unsigned long)&timer_dev;

    return 0;
}

static void __exit timer_exit(void)
{
    gpio_set_value(timer_dev.led_gpio, 1);
    del_timer_sync(&timer_dev.timer);
#if 0
    del_timer(&timer_dev.timer);
#endif
    cdev_del(&timer_dev.cdev);
    unregister_chrdev_region(timer_dev.devid, TIMER_CNT);

    device_destroy(timer_dev.class, timer_dev.devid);
    class_destroy(timer_dev.class);
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xuemanyi");