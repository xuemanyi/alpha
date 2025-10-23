#include <linux/kernel.h>
#include <linux/types.h>    // ssize_t, loff_t
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/module.h>   // module_init, module_exit, MODULE_LICENSE
#include <linux/init.h>     // __init, __exit
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/fs.h>       // file_operations, register_chrdev
#include <linux/uaccess.h>  // copy_to_user, copy_from_user
#include <linux/string.h>   // memcpy
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/irq.h>
#include <linux/of_irq.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/fcntl.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
