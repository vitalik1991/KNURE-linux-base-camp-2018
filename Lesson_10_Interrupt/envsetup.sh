#! /bin/sh

export CROSS_COMPILE=/home/user/build/cache/toolchains/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-


export BUILD_KERNEL=/home/user/build/cache/sources/linux-mainline/linux-4.14.y
#export BUILD_ROOTFS=${TRAINING_ROOT}/rootfs

#export KERNEL_IMG=${BUILD_KERNEL}/arch/x86/boot/bzImage
#export ROOTFS_IMG=${TRAINING_ROOT}/rootfs.img

echo -e "\t CROSS_COMPILE \t = ${CROSS_COMPILE}"
echo -e "\t TRAINING_ROOT \t = ${TRAINING_ROOT}"
echo -e "\t BUILD_KERNEL \t = ${BUILD_KERNEL}"

