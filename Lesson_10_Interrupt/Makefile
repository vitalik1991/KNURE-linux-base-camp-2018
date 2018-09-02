CONFIG_MODULE_SIG=n

obj-m := Led.o

KDIR ?= /home/user/build/cache/sources/linux-mainline/linux-4.14.y

PWD := $(shell pwd)

default:
		  $(MAKE) ARCH=arm  -C $(KDIR) M=$(PWD) modules

clean:
		  $(MAKE) ARCH=arm  -C $(KDIR) SUBDIRS=$(PWD) clean

config:
		$(MAKE) ARCH=arm -C $(KDIR) menuconfig

dtb:
		$(MAKE) ARCH=arm -C $(KDIR) dtbs

.PHONY: clean default config dtb
