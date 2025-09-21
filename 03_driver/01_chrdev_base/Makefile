# CROSS_COMPILE=/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf-
KERNELDIR := /home/sucre/01_alpha/02_linux_system/02_kernel
CURRENT_PATH := $(shell pwd)

ARCH=arm 
CROSS_COMPILE=arm-linux-gnueabihf-

obj-m := chrdev_base.o
# chrdev_base.o 会被编译为最终的 chrdev_base.ko（内核模块）

# 添加编译选项禁用PIC
# ccflags-y := -fno-pic

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
# $(MAKE) -C $(KERNELDIR) 表示切换到内核源码目录执行 make
# M=$(CURRENT_PATH)       指定当前模块的源码路径，内核构建系统会进入这个目录编译模块
# modules                 指定目标，表示只编译模块（不编译整个内核）

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean