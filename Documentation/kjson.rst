=================
The KJSON Library
=================

12/12/2023

Copyright (C) 2023 Emanuele Santini <emanuele.santini.88@gmail.com>

General
=======

The KJSON Library (JsonOnKernel project) implements a JSON parser for the Linux 
Kernel. The purpose of the project is to create a fast way to send and to 
receive serialized data between kernel and user space, using a JSON format.

Why JSON? Like YAML, JSON is a human-readable text to store and trasmit data 
objects. Each object consist in a pair of Key-Value with an unique key. Many 
user applications use JSON to interchange data over the network or beetwen 
local processes. You could serialize the data in a JSON format on your kernel 
device to make more easy in user space the communication with it. 

Case of uses:

        - A complex sensors system that exposed to a user differents data: 
          temperature, pressure, humidity and so on. You could serialize this
          data and expose it in a human-readable format directly in user space.
        - A driver to read informations about the system (statics of a process
          or memory, system temperature and voltage, and so on).

Howto Import the Library on your Project
========================================

We have two sources file:
 
- kjson_main.c -> Implements the interface to the library and data structure to
  rappresent the JSON data in the memory.
	
- kjson_parser.c -> Implements the JSON text parser, to create the JSON data 
  structure starting from a JSON text.
	
And one utility library:
	
- kjstring_t -> Internally, KJSON library uses this utility lib to iterate the
  JSON text string.
	
The main interface to the library is the kjson.h header file.

You can link the code into your project; But if you prefer a separate module for
the library, you can build the project using the provided Makefile, then import
the Module.symvers on your project Makefile, and include the kjson.h header 
where you need. Example 01 on the samples folder will help you with this. 

In the case you will use kjson as separate module, remember to insert first the
kjson.ko module before yours:

	.. code:: shell

            $ modprobe kjson.ko
	        $ modprobe your_module.ko

Howto Use the Library
=====================

Every JSON is rappresented by the ``struct kjson_container``. You can start to 
build a new JSON with:

.. code:: c

        struct kjson_container *new_json = kjson_new_container();
	
Or you can parse an existing JSON text with:

.. code:: c
        
        struct kjson_container* a_json = kjson_parse(json_str);

kjson_parse returns NULL in case of error.        
The json parser is able to register an error message in case of parsing error.
If kjson_parse returns NULL, you can read the error message pointed by (the 
char* type) kjstring_parser_error_msg.

json_str should be a non NULL const char* pointer. For example, the following 
json text string:

.. code:: c
        
        char* json_str = "{ 
        \"TestNumber\": 1, 
        \"TestName\": \"LinuxKernelUnderstandJSON\" }";

To create the json text from the data structure you can use the:

.. code:: c
        
        struct kjstring_t* str_t = kjson_dump(a_json)
	
Access the string buffer of the kjstring_t is simple:

.. code:: c
        
        char* str = kjstring_str(str_t)

Adding object to a JSON
-----------------------

Unsigend Integer 64 bit:

.. code:: c
        
        kjson_push_integer(a_json, "a_key", 2)
	
or 

.. code:: c
        
        int val = 2;
        kjson_push_object(a_json, a_key", KOBJECT_TYPE_INTEGER, &val, 0);

String:

.. code:: c
	
        kjson_push_string(a_json, "a_key", "a string")``
	
or

.. code:: c
	
        char* str = "blabla";
	kjson_push_object(a_json, "a_key", KOBJECT_TYPE_STRING,
                str, strlen(T))
	
Integer array:

.. code:: c
	
        kjson_push_integer_array(a_json, "a_key", 1, 23, 3, 7)
	
or

.. code:: c
	
        int64_t* arr = [ 3, 4, 6 ];
	kjson_push_object(a_json, "a_key", KOBJECT_TYPE_INTEGER_ARRAY, arr, 
                sizeof(arr) / sizeof(int64_t))
	
String array:

.. code:: c
	
        kjson_push_string_array(a_json, "a_key", "string 1", "string 2",
                "string n")
	
or
	
.. code:: c
	
        char** arr = [ "hello", "world" ];
	kjson_push_object(a_json, "a_key", KOBJECT_TYPE_STRING_ARRAY, arr, 
		sizeof(arr) / sizeof(char*));
	
All the integer are int64_t.
String objects are dinamically allocated.
All the integer and string objects are deeply copied into the kjson structure.

Null object:
        kjson_push_null(json_a, key)

Boolean:
        kjson_push_true(json_a, key)
        kjson_push_false(json_a, key);

Nested json: First declare and write all annidate json's you need, for example:

.. code:: c
	
        struct kjson_container* json_a = kjson_new_container()
	struct kjson_container* json_b = kjson_new_container()
	kjson_push_string(json_a, "a_key", "a string")
	kjson_push_integer_array(json_b, "a_key", 1, 23, 3, 7)
	
Then, you can nested with:

.. code:: c
	
        kjson_push_container(a_json, "a_key", json_a)
	
or

.. code:: c
	
        kjson_push_object(a_json, "key", KOBJECT_TYPE_OBJECT, json_a, 0)
	
or an array of nested json's:

.. code:: c
	
        kjson_push_container_array(a_json, "a_key", json_a, json_b)
	
or

.. code:: c
	
        struct json_container** arr = [ json_a, json_b ];
	kjson_push_object(a_json, "key", KOBJECT_TYPE_OBJECT, arr, 0)
	
You must not to deallocate json_a and json_b. The nested json are not deeply
copied. When you will destroy the parent json (a_json in the example), the 
childs (json_a and json_b) will be deallocated:

.. code:: c
	
        kjson_delete_container(a_json)
	
Call kjson_delete_container when you want to destroy your kjson and all nested
(if there are) from the memory.

The kjson_object_t rappresent a pair <Key,Value> into the kjson data structure.
The field type rappresent the type of the value.

To find an object from it's key you can use:

.. code:: c
	
        struct kjson_object_t* obj = kjson_lookup_object(a_json, "the_key")
	
Dependig of the type of the object, you can use:

.. code:: c
	
        int a = kjson_as_integer(obj)
	
	char* str = kjson_as_string(obj)
	
	struct kjson_container* c = kjson_as_container(obj)
	
	int* a = kjson_as_integer_array(obj)
	
	char** str = kjson_as_string_array(obj)	
	
	struct kjson_container** c = kjson_as_container_array(obj)

        int bool = kjson_as_bool(obj)

        int isNull = kjson_object_is_null(obj)
	
To know the lenght of an array object you can use:

.. code:: c
	
        size_t array_len = kjson_array_length(obj)
	
**Be careful. Each of the access functions above must to be used with the 
correct object type. Calling, for example, kjson_array_length on an object that
is not an array or kjson_as_integer on an string object, will lead to an 
undefined state.**

The internals
=============

The Key Value dictionary is implemented using the internal linux kernel hash 
table, using jhash for the key hashing.

The macro KJSON_BITS_SIZE defines the power of 2 of the size of the hash bucket
array. Read the Kernel Hash Table documentation if you are interested on any 
improvement.

The macro KJSON_KEY_SIZE is the maximun size of the key. You can change this
value as you prefer.

The macro KJSON_MEMORY_DUMP_SIZE defines the maximun size of JSON text you can
obtain from the dump operation. If you have a very large json, you could
increase this value.
	
	
	
	





