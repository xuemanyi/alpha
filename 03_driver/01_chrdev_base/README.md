# 字符驱动
## 1.Makefile
1. 检查头文件路径  
    `CHECK_HEADER := $(shell if [ -f "$(INCLUDE_DIR)/drv.h" ]; then echo "exists"; else echo "missing"; fi)`  
    `@echo "Header file status: $(CHECK_HEADER)"`
2. 在编译命令中添加自定义的头文件路径  
    `@$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) EXTRA_CFLAGS="$(EXTRA_CFLAGS)" modules`
3. __直接在`Makefile`文件中添加`EXTRA_CFLAGS="$(EXTRA_CFLAGS)`没在make命令中添加，执行`make`命令回报错找不到头文件__
    - 命令行 EXTRA_CFLAGS → 生效，因为作为环境变量传入了内核顶层 Makefile
    - 模块 Makefile 里 EXTRA_CFLAGS → 可能被忽略，新内核不推荐这么写
    - 正确写法：在模块 Makefile 里用 ccflags-y := -I$(INCLUDE_DIR)  
    
    Kbuild 的执行流程：当在模块目录下写 obj-m := xxx.o，然后调用：`make -C $(KERNELDIR) M=$(PWD) modules`  
    编译流程分两步：
    1. 外层内核`Makefile`（位于`$(KERNELDIR)`）首先被执行
    2. 然后它会`cd`到模块目录（`M=$(PWD)`），去读取模块的`Makefile`
    在这个过程中，`Kbuild`会传递一些环境变量（比如`EXTRA_CFLAGS、ccflags-y`等），并把它们拼接到编译命令
    
    为什么命令行能生效？
    命令行写`EXTRA_CFLAGS=...`时，它作为 环境变量 传给了顶层内核`Makefile`
    `Kbuild`内部会把命令行传入的`EXTRA_CFLAGS`透传下去，所以编译时能成功
4. `Makfile`中的变量  
    1. `obj-m`是内核模块`Makefile`中用来指定哪些模块需要被构建为`loadable module (.ko)`的变量
    2. `ccflags-y`是`Linux`内核`Makefile`的特殊变量，用来给模块添加额外的编译选项，通常用于：
        - 添加模块需要的头文件路径
        - 定义宏
        - 调整警告等级
        - 开启调试信息等  
        只影响 当前模块 的编译，不影响其他模块，和`CFLAGS/EXTRA_CFLAGS`的不同：
        - `CFLAGS/EXTRA_CFLAGS`是在外层`Makefile`传给内核构建系统的
        - `ccflags-y`是在模块的`Makefile`内部声明，更推荐、可靠
    3. `|`:在`Makefile`中，`|`符号表示`order-only prerequisites`（仅顺序依赖）  
        `all: | $(BUILD_DIR)`中`|`的作用：
        - 确保`$(BUILD_DIR)`在`all`目标之前执行
        - 但如果`$(BUILD_DIR)`的时间戳比`all`新，不会触发`all`重新构建
        有没有`|`的区别
        - 有`|`：
            1. 第一次运行：先创建`build`目录，再执行`all`
            2. 后续运行：如果`build`目录已存在，不会因为目录时间戳更新而重新执行`all`，当`all`目标的真正依赖（即源码文件）发生变化时，内核构建系统会检测到并重新编译模块
        - 没有`|`
            1. 每次运行：都会检查`build`目录的时间戳
            2. 如果`build`目录的时间戳比`all`新，会重新执行`all`（即使源码没变化）
