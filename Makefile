# Makefile for building the USB Logger Kernel Module

# Include all the necessary object files for the kernel module
obj-m := test.o  # Add both .o files

# Path to the kernel source (kernel headers)
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# Build rule
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean up the build files
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
