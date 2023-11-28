kjson-objs := kjson_main.o kjson_parser.o
obj-m := kjson.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -v *.o *.ko

