KERNELDIR ?= /home/sucre/01_alpha/02_linux_system/02_kernel
CURRENT_PATH := $(shell pwd)
ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-

MODULE_NAME := led
obj-m := $(MODULE_NAME).o

BUILD_DIR ?= $(CURRENT_PATH)/build

ccflags-y := -I$(KERNELDIR)/../../03_driver/include -Wall

all: | $(BUILD_DIR)
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) \
		ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
	@mv -f *.ko *.o *.mod.* Module.symvers modules.order .*.cmd .tmp_versions $(BUILD_DIR)/ 2>/dev/null || true

debug: ccflags-y += -g -DDEBUG -O0
debug: all

release: ccflags-y += -O2 -DNDEBUG
release: all

$(BUILD_DIR):
	@mkdir -p $@

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
	@rm -rf $(BUILD_DIR)