# If you define TEST_ONLY env you'll build only the test, using kjson as dependency. Be sure to build kjson library first with the provided makefile and inser the module.
# If you not define TEST_ONLY, the library is statically insert in the test module.

ifdef TEST_ONLY
test-objs := test01.o
obj-m := test.o
else
test-objs := ../../kjson_main.o ../../kjson_parser.o test01.o
obj-m := test.o
endif

KDIR = /lib/modules/$(shell uname -r)/build

all:
ifdef TEST_ONLY
	$(MAKE) -C $(KDIR) M=$(PWD) modules KBUILD_EXTRA_SYMBOLS=`pwd`/../../Module.symvers
else
	$(MAKE) -C $(KDIR) M=$(PWD) modules
endif
clean:
	rm -v *.o *.ko

