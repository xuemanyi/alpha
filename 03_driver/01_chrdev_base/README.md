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
## 2.源代码中的问题
### 2.1 未指定const导致的常量性丢失-app
1. 警告信息
    ```
    arm-linux-gnueabihf-gcc -I../include chrdev_base_app.c -o app
    chrdev_base_app.c: 在函数‘main’中:
    chrdev_base_app.c:18:14: 警告： assignment discards ‘const’ qualifier from pointer target type
    filename = argv[1];
    ```
    
    - 这个警告是因为类型不匹配导致的常量性丢失：`main`函数的`argv`参数被声明为`const char*[]`类型，表示命令行参数字符串  
    不应被修改；而`filename`变量是`char*`类型，表示可通过它修改字符串内容。将`argv[1]`赋值给`filename`相当于丢弃了  
    `const`限定符，编译器警告这种操作可能存在风险——如果原始字符串存储在只读内存中，后续通过`filename`修改内容会导致程序崩溃
    
    - 合法的赋值方向：  
    `char*       → const char*    ✓ 安全（添加const）`  
    `const char* → char*          ✗ 危险（丢弃const）`

    - 程序启动时的典型布局：  
        
        | 内存区域     | 内容                 |
        |-------------|---------------------|
        | 代码段      | 程序指令（只读）     |
        | 只读数据段  | 字符串常量（只读）   |
        | 数据段      | 全局变量（可写）     |
        | 栈段        | 局部变量（可写）     |
        | 堆段        | 动态分配（可写）     | 

2. 源代码
    ```
    int main(int argc, char const *argv[])  
    // argv 被声明为 const char *argv[]，即 argv[1] 是 const char*
    {
        char *filename = NULL;  // filename 是 char* (非常量)
        // ...
        filename = argv[1];     // 这里警告：将 const char* 赋值给 char*        ↑
        // argv[1] 是 const char*，但 filename 是 char*
    }
    ```
3. 解决方法
    - 移除main定义中不必要的const
    - 如果需要保持`const`，使用类型转换，给`filename`添加类型转换
    - 直接使用`argv[1]`，不额外声明变量

