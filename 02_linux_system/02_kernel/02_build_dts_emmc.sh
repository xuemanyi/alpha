#!/bin/sh
export PATH=$PATH:/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs