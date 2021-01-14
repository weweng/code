# A simple Makefile for linux kernel module

.PHONY: modules all

obj-m := procfs.o sysfs.o

procfs-objs := proc-fs.o
sysfs-objs := sys-fs.o

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: modules

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	@rm *.ko *.o *.mod *.mod.c *.order Module.symvers
