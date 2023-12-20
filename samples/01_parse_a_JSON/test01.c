// SPDX-License-Identifier: GPL-2.0
// Authors: Emanuele Santini <emanuele.santini.88@gmail.com>

/* Hoto use it:
 * insmod test.ko
 * read dmesg command output
 * rmmod test.ko
 */

#include <linux/module.h>
#include "../../kjson.h"

int __init test_init(void)
{
    // We have two objects to read into our first JSON. kjson_object_t is a <key, value> object into kjson_container
    struct kjson_object_t *obj1, *obj2;

    // This is the JSON data structure
    struct kjson_container *my_json;

    // Our first JSON (a number and a string).
    char *json_str = 
	    "{ \"TestNumber\": 1, \"TestName\": \"LinuxKernelUnderstandJSON\" }";

    // Parse the json string
    my_json = kjson_parse(json_str);
    if(!my_json)
    {
		printk(KERN_INFO "parse error\n");
		return -1;
    }

    // Read the elements using the key
    obj1 = kjson_lookup_object(my_json, "TestNumber");
    obj2 = kjson_lookup_object(my_json, "TestName");

    // prints the integer and string value 
    printk(KERN_INFO "TestNumber: %lld, TestName: %s\n", kjson_as_integer(obj1), kjson_as_string(obj2));

    // Adding some others object to the json
    kjson_push_string_array(my_json, "RandomString", "String1", "String2", "ALotOfStrings");

    // Printig the new json text
    struct kjstring_t *dmp = kjson_dump(my_json);
    printk("New json: %s\n", kjstring_str(dmp));
    
    // Dealloc the json container
    kjson_delete_container(my_json);

    return 0;
}

void __exit test_exit(void)
{
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");

