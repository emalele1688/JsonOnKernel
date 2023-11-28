// SPDX-License-Identifier: GPL-2.0
// Authors: Emanuele Santini <emanuele.santini.88@gmail.com>

#include <linux/module.h>
#include "../../kjson.h"

int __init test_init(void)
{
    /* 
     * kjstring is an internal object to rappresent a string type.
     * It contains utility function to manage a string type with iterator and insertion functions. 
     * test is the JSON string to parse.
     */
    struct kjstring_t test;

    // We have two objects to read into our first JSON.
    struct kjson_object_t *obj1, *obj2;

    // This is the JSON data structure
    struct kjson_container *my_json;

    // Our first JSON (a number and a string).
    char *json_str = 
	    "{ \"TestNumber\": 1, \"TestName\": \"LinuxKernelUnderstandJSON\" }";

    /* Create the stream buffer from the json_str. (kjson uses kjstring for string type).
     * kjstring_new_string_buffer creates a kjstring object using a preallocated area (json_str).
     */
    kjstring_new_string_buffer(&test, json_str, strlen(json_str));

    // Parse the json string
    my_json = kjson_parse(&test);
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

    return 0;
}

void __exit test_exit(void)
{
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");

