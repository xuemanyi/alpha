#include "drv.h"

#define NEW_CHR_LED_CNT  1
#define NEW_CHR_LED_NAME "newchrled"
#define LED_OFF          0
#define LED_ON           1

#define CCM_CCGR1_BASE         (0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE (0X020E02F4)
#define GPIO1_DR_BASE          (0X0209C000)
#define GPIO1_GDIR_BASE        (0X0209C004)

void __iomem *IMX6U_CCM_CCGR1;
void __iomem *SW_MUX_GPIO1_IO03;
void __iomem *SW_PAD_GPIO1_IO03;
void __iomem *GPIO1_DR;
void __iomem *GPIO1_GDIR;

struct newchrled_dev{
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    int major;
    int minor;
};

struct newchrled_dev newchrled;

void led_switch(u8 sta)
{
    u32 val = 0;

    if (sta == LED_ON) {
        val = readl(GPIO1_DR);
        val &= ~(1 << 3);
        writel(val, GPIO1_DR);
    } else if (sta == LED_OFF) {
        val = readl(GPIO1_DR);
        val |= (1 << 3);
        writel(val, GPIO1_DR);
    }
}

static int led_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &newchrled;

    return 0;
}

static int led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    return 0;
}

static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int ret = 0;
    unsigned char databuf[1];
    unsigned char ledstat = 0;

    ret = copy_from_user(databuf, buf, cnt);
    if (ret < 0)
    {
        printk("kernel write failed\n");
        return -EFAULT;
    }

    ledstat = databuf[0];

    if (ledstat == LED_ON) {
        led_switch(LED_ON);
    } else if (ledstat == LED_OFF) {
        led_switch(LED_OFF);
    }

    return 0;
}

static int led_release(struct inode *inode,  struct file *filp)
{
    return 0;
}

static struct file_operations new_chrled_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .release = led_release,
};

static int __init led_init(void)
{
    u32 val = 0;

    IMX6U_CCM_CCGR1 = ioremap(CCM_CCGR1_BASE, 4);
    SW_MUX_GPIO1_IO03 = ioremap(SW_MUX_GPIO1_IO03_BASE, 4);
    SW_PAD_GPIO1_IO03 = ioremap(SW_PAD_GPIO1_IO03_BASE, 4);
    GPIO1_DR = ioremap(GPIO1_DR_BASE, 4);
    GPIO1_GDIR = ioremap(GPIO1_GDIR_BASE, 4);

    val = readl(IMX6U_CCM_CCGR1);
    val &= ~(3 << 26);
    val |= (3 << 26);
    writel(val, IMX6U_CCM_CCGR1);
    writel(5, SW_MUX_GPIO1_IO03);
    writel(0x10B0, SW_PAD_GPIO1_IO03);
    
    val = readl(GPIO1_GDIR);
    val &= ~(1 << 3);
    val |= (1 << 3);
    writel(val, GPIO1_GDIR);

    val = readl(GPIO1_DR);
    val |= (1 << 3);
    writel(val, GPIO1_DR);

    if (newchrled.major) {
        newchrled.devid = MKDEV(newchrled.major, 0);
        register_chrdev_region(newchrled.devid, NEW_CHR_LED_CNT, NEW_CHR_LED_NAME);
    } else {
        alloc_chrdev_region(&newchrled.devid, 0, NEW_CHR_LED_CNT, NEW_CHR_LED_NAME);
        newchrled.major = MAJOR(newchrled.devid);
        newchrled.minor = MINOR(newchrled.devid);
    }

    printk("major = %d, minor = %d\n", newchrled.major, newchrled.minor);

    newchrled.cdev.owner = THIS_MODULE;
    cdev_init(&newchrled.cdev, &new_chrled_fops);

    cdev_add(&newchrled.cdev, newchrled.devid, NEW_CHR_LED_CNT);

    newchrled.class = class_create(THIS_MODULE, NEW_CHR_LED_NAME);
    if (IS_ERR(newchrled.class)) {
        return PTR_ERR(newchrled.class);
    }

    newchrled.device = device_create(newchrled.class, NULL, newchrled.devid, NULL, NEW_CHR_LED_NAME);

    if (IS_ERR(newchrled.device)) {
        return PTR_ERR(newchrled.device);
    }

    return 0;
}

static void __exit led_exit(void)
{
    iounmap(IMX6U_CCM_CCGR1);
    iounmap(SW_MUX_GPIO1_IO03);
    iounmap(SW_PAD_GPIO1_IO03);
    iounmap(GPIO1_DR);
    iounmap(GPIO1_GDIR);

    cdev_del(&newchrled.cdev);
    unregister_chrdev_region(newchrled.devid, NEW_CHR_LED_CNT);
    device_destroy(newchrled.class, newchrled.devid);
    class_destroy(newchrled.class);
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xuemanyi");