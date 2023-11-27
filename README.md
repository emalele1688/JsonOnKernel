# JsonOnKernel

This is a project to support JSON standard text on the Linux Kernel.

## The KJSON library

KJSON (JsonOnKernel) library as an internal parser to create the JSON data structure from a JSON standard text.

You can link the library direclty on you kernel module code, or use the prebuilt module with the makefile provvided.

## JSON object supported

* Integer
* String
* Annidate JSON
* Integer array
* String array
* Annidate JSON array

## Example

Create a Makefile like this:

```
kjson-test-objs := kjson_main.o kjson_parser.o json_test.o
obj-m := kjson-test.o

COMPILE_DIR = /tmp/modules_compile
KDIR = /lib/modules/$(shell uname -r)/build

all:
	@rm -rf ${COMPILE_DIR}
	@ln -r -s ./ ${COMPILE_DIR}
	$(MAKE) -C $(KDIR) M=$(COMPILE_DIR) modules
	@rm -f ${COMPILE_DIR}

clean:
	rm -v *.o *.ko
	@rm -f ${COMPILE_DIR}
```

Create a C source file called `json_test.c`

```
#include <linux/module.h>

#include "kjson_parser.h"
#include "kjson.h"

int __init test_init(void)
{
    struct kjstring_t test;
    struct kjstring_t *parsed;
    struct kjson_object_t *obj1 *obj2;
    struct kjson_container *my_json;

    char *json_str = "{ \"TestNumber\": 1, \"TestName\": \"LinuxKernelUnderstandJSON\" }";
    // Create the stream buffer
    kjstring_new_string_buffer(&test, json_str, strlen(json_str));

    // Parse the json string
    my_json = kjson_parse(&test);
	if(!my_json)
	{
		printk("parse error\n");
		return 0;
    }

    obj1 = kjson_lookup_object(my_json, "TestNumber");
    obj2 = kjson_lookup_object(my_json, "TestName");

    printk("TestNumber: %d, TestName: %s\n", kjson_as_integer(obj1), kjson_as_integer(obj2));

    return 0;
}
```

Execute `make` and load the module `insmod kjson-test.ko`. Read the log with `dmes`
