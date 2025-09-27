KERNELDIR := /home/sucre/01_alpha/02_linux_system/02_kernel
CURRENT_PATH := $(shell pwd)
BUILD_DIR := $(CURRENT_PATH)/build

ARCH=arm 
CROSS_COMPILE=arm-linux-gnueabihf-

obj-m := chrdev_base.o

build: kernel_modules

kernel_modules:
	@mkdir -p $(BUILD_DIR)
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	@echo "Copying build results to $(BUILD_DIR)"
	@cp *.ko *.mod.* *.o Module.symvers modules.order $(BUILD_DIR)/ 2>/dev/null || true
	@rm -f *.ko *.mod.* *.o Module.symvers modules.order 2>/dev/null || true
# $(MAKE) -C $(KERNELDIR) 表示切换到内核源码目录执行 make
# M=$(CURRENT_PATH)       指定当前模块的源码路径，内核构建系统会进入这个目录编译模块
# modules                 指定目标，表示只编译模块（不编译整个内核）

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
	rm -rf $(BUILD_DIR)
