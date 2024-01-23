#include <linux/module.h>

#include "../../kjson.h"
#include "../../kjstring.h"


int __init test_init(void)
{
	struct kjson_container *ctn;
	struct kjstring_t *dump;

	long int a_int = 3;
	char *a_string = "Linux";
	
	if((ctn = kjson_new_container()) == NULL)
	{
		pr_err("kjson_new_container error\n");
		return 1;
	}
	
    kjson_push_integer(ctn, "an_integer", 3);
    kjson_push_integer(ctn, "another_integer", a_int);
    kjson_push_string(ctn, "Logo", "Tux");
    kjson_push_string(ctn, "SO", a_string);    
    kjson_push_integer_array(ctn, "AnArrayInt", 3, 5, a_int);
    kjson_push_string_array(ctn, "AnArrayString", a_string, "ubuntu", "archlinux", "centos", "debian");
    kjson_push_null(ctn, "ThisIsNull");
    kjson_push_true(ctn, "ThisIsTrue");
    kjson_push_false(ctn, "ThisIsFalse");

   	// Create annidate json array
    struct kjson_container *ctn1 = kjson_new_container();
    kjson_push_string_array(ctn1, "sarray2", "abc", "cde", "efgh");
   	// Push annidate json    
    kjson_push_container(ctn, "cont1", ctn1);

	// Create annidate json array
    struct kjson_container *ctn2 = kjson_new_container();
    kjson_push_integer(ctn2, "val", 2);
    kjson_push_integer(ctn2, "pro1", -2);
    kjson_push_string(ctn2, "por3", "boo");
    struct kjson_container *ctn3 = kjson_new_container();
    kjson_push_integer(ctn3, "val", 22);
    kjson_push_integer(ctn3, "pro1", -21);
    kjson_push_string(ctn3, "por33", "boa");
    struct kjson_container *ctn4 = kjson_new_container();
    kjson_push_string_array(ctn4, "sarray112", "no", "sense");
	// push annidate json array
    kjson_push_container_array(ctn, "ctn_array", ctn2, ctn3, ctn4);
    
    // Test insert with kjson_push_object
    kjson_push_object(ctn, "hello", KOBJECT_TYPE_INTEGER, &a_int, 0);
    kjson_push_object(ctn, "hello2", KOBJECT_TYPE_STRING, a_string, strlen(a_string));
    kjson_push_object(ctn, "array1", KOBJECT_TYPE_INTEGER_ARRAY, ((uint64_t[]){3, 4, 5}), 3);
    kjson_push_object(ctn, "sarray1", KOBJECT_TYPE_STRING_ARRAY, ((char*[]){ "hello", "world" }), 2);
    
	// Collison test
    if(!kjson_push_object(ctn, "hello", KOBJECT_TYPE_STRING, "test", 4))
        printk("collision passed\n");    

    dump = kjson_dump(ctn);
    if(dump)
        pr_info("%s\n", kjstring_str(dump));
	else
		pr_info("Dump error\n");

    kjson_delete_container(ctn);
    kjstring_free(dump);

    return 0;
}

void __exit test_exit(void)
{
}


module_init(test_init);
module_exit(test_exit);
MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");

