#!/bin/bash

export PATH=$PATH:/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin

# 彻底清理内核源码目录，为一次全新的编译做准备
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
# arm-linux-gnueabihf- 是工具链的前缀，make 会使用这个前缀来调用各种工具
# arm-linux-gnueabihf-gcc (C 编译器)
# arm-linux-gnueabihf-ld (链接器)
# distclean: 这是 Makefile 中定义的一个目标
# 它的功能是彻底清理源代码树，删除所有编译生成的文件、配置文件、对象文件以及之前编译产生的任何中间文件
# 它会让内核目录恢复到几乎刚从官网下载解压后的状态。通常在开始一次全新的编译，或者需要切换不同配置时使用，以确保没有之前的编译结果干扰

# 导入针对i.MX6ULL开发板（eMMC+512MB DDR版本）的预置内核配置，生成 .config 文件
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- mx6ull_14x14_ddr512_emmc_defconfig
# mx6ull_14x14_ddr512_emmc_defconfig: 这是本次命令的核心目标
# 在Linux内核源码的arch/arm/configs/目录下，存放着许多针对不同 ARM 开发板的默认配置文件
# mx6ull_14x14_ddr512_emmc_defconfig就是 NXP 官方为 i.MX6ULL 这款处理器提供的默认配置文件名
# 它包含了处理器类型、内存大小、外设（如eMMC）等所有必要的驱动和内核选项的预设置
# 当执行此命令时，make 会将该指定的配置文件复制到内核根目录，并重命名为 .config 文件
# 这个 .config 文件就是接下来编译内核所依据的直接配置清单

# 用 12 个线程并行编译以提升速度，并输出详细的编译信息以便于调试
make V=1 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j12
# V=1 会要求 make 显示完整的编译命令，包括编译器、所有参数和标志
# 这在调试编译错误时非常有用，可以清楚地看到每一步是如何执行的