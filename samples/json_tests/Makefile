test-objs := ../../kjson_main.o ../../kjson_parser.o json_test.o
obj-m := test.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -v *.o *.ko

