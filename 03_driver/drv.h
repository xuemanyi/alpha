#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/module.h>   // module_init, module_exit, MODULE_LICENSE
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // file_operations, register_chrdev
#include <linux/uaccess.h>  // copy_to_user, copy_from_user
#include <linux/string.h>   // memcpy
#include <linux/types.h>    // ssize_t, loff_t