#include <linux/module.h>
#include "../../kjson.h"

int __init test_init(void)
{
	struct kjstring_t *parsed;
	struct kjson_container *ctn;
	struct kjson_object_t *obj;

	// Passed ------------------------------------------------------------------------------------------------------------------------------------------------------
	char *test_ok_01 = "{ \"value\": 2, \"string\": \"aString\", \"value1\": 20344, \"string1\": \"The String\", \"IsNull\": null, \"isTrue\": true, \"isFalse\": false }";

	if((ctn = kjson_parse(test_ok_01)) == NULL)
		pr_info("WARNING: test_ok_01 not passed\n");
	else
	{
		if(!IS_ERR((parsed = kjson_dump(ctn))))
			pr_info("test_ok_01: %s\n", kjstring_str(parsed));
			
		kjson_delete_container(ctn);
	}

	// Passed
	char *test_ok_02 = "  {   \"value\": 2, \"string\": \"aString\", \"value1\": 20344, \"string1\": \"The String\", \"array\": [1, 2, 3], \"arrayNull\": []  } ";
	
	if((ctn = kjson_parse(test_ok_02)) == NULL)
		pr_info("WARNING: test_ok_02 not passed\n");
	else
	{
		if(!IS_ERR((parsed = kjson_dump(ctn))))
			pr_info("test_ok_02: %s\n", kjstring_str(parsed));
			
		obj = kjson_lookup_object(ctn, "array");
		pr_info("test_ok_02 contains an array of len: %lu\n", kjson_array_length(obj));
		pr_info("test_ok_02 second array parameter: %lld\n", (kjson_as_integer_array(obj))[1]);
		
		kjson_delete_container(ctn);		
	}	

	char *test_ok_03 = "{\"locale\": \"en-US.UTF-8\", \"Nullname\": \"\", \"domain_name\": \"\", \"version\": \"10.33.22\", \"anotherString\": \"linuxkernel\", \"linuxkernel\": [ 1, 3, 5, 5040, 22 ] }";
	
	if((ctn = kjson_parse(test_ok_03)) == NULL)
		pr_info("WARNING: test_ok_03 not passed\n");
	else
	{
		if(!IS_ERR((parsed = kjson_dump(ctn))))
			pr_info("test_ok_03: %s\n", kjstring_str(parsed));
			
		obj = kjson_lookup_object(ctn, "anotherString");
		pr_info("test_ok_03 anotherString value: %s\n", kjson_as_string(obj));
		
		kjson_delete_container(ctn);		
	}

	char *test_ok_04 = "{\"locale\": \"en-US.UTF-8\", \"Nullname\": \"\", \"domain_name\": \"\", \"version\": \"10.33.22\", \"anotherString\": \"linuxkernel\", \"arrayN\": [ 1, 3, 5, 5040, 22 ], \"arrayS\": [\"ciao\", \"1\", \"a test\"] }";

	if((ctn = kjson_parse(test_ok_04)) == NULL)
		pr_info("WARNING: test_ok_04 not passed\n");
	else
	{
		if(!IS_ERR((parsed = kjson_dump(ctn))))
			pr_info("test_ok_04: %s\n", kjstring_str(parsed));
			
		obj = kjson_lookup_object(ctn, "arrayS");
		pr_info("test_ok_04 arrayS len: %lu\n", kjson_array_length(obj));
		pr_info("test_ok_04 second array parameter: %s\n", (kjson_as_string_array(obj))[2]);
		
		kjson_delete_container(ctn);		
	}

	char *test_ok_05 = "{ \"light\": {\"lineEnabled\": 4}, \"lightParams\": {\"intensity\": \"125\", \"voltage\": 5}, \"room\": 1, \"RandomJsonNull\": {}, \"emptyArray\": [] }";

	if((ctn = kjson_parse(test_ok_05)) == NULL)
		pr_info("WARNING: test_ok_05 not passed\n");
	else
	{
		if(!IS_ERR((parsed = kjson_dump(ctn))))
			pr_info("test_ok_05: %s\n", kjstring_str(parsed));
			
		obj = kjson_lookup_object(ctn, "lightParams");
		// lightParams is an annidated json
		struct kjson_container *annidated = kjson_as_container(obj);
		obj = kjson_lookup_object(annidated, "voltage");
		pr_info("test_ok_05 voltage is: %lld\n", kjson_as_integer(obj));
		
		kjson_delete_container(ctn);		
	}

	char *test_ok_06 = "{ \"light\": { \"lineEnabled\": [ 4, 7, 8 ], \"lightParams\": {\"intensityString\": \"125\", \"voltage\": 5} } }";
	
	if((ctn = kjson_parse(test_ok_06)) == NULL)
		pr_info("WARNING: test_ok_06 not passed\n");
	else
	{
		if(!IS_ERR((parsed = kjson_dump(ctn))))
			pr_info("test_ok_06: %s\n", kjstring_str(parsed));
			
		obj = kjson_lookup_object(ctn, "light");
		struct kjson_container *annidated = kjson_as_container(obj);
		// lightParams is an annidate and annidated json
		obj = kjson_lookup_object(annidated, "lightParams");
		struct kjson_container *annidated_annidated = kjson_as_container(obj);
		obj = kjson_lookup_object(annidated_annidated, "intensityString");		
		pr_info("test_ok_06 intensityString is: %s\n", kjson_as_string(obj));
		
		kjson_delete_container(ctn);		
	}	

	char *test_ko_07 = "{\"JsonArr\": [{\"aInteger\": 1, \"aString\": \"string\" }, {\"aJson\": {\"aStringArr\": [\"one\", \"two\"]}}]}";
	
	if((ctn = kjson_parse(test_ko_07)) == NULL)
		pr_info("WARNING: test_ko_07 not passed\n");
	else
	{
		if(!IS_ERR((parsed = kjson_dump(ctn))))
			pr_info("test_ko_07: %s\n", kjstring_str(parsed));
		
		obj = kjson_lookup_object(ctn, "JsonArr");
		pr_info("test_ko_07 JsonArr len: %lu\n", kjson_array_length(obj));
		
		struct kjson_container **array_annidated = kjson_as_container_array(obj);
		
		obj = kjson_lookup_object(array_annidated[0], "aInteger");
		pr_info("test_ko_07 aInteger: %lld\n", kjson_as_integer(obj));
		obj = kjson_lookup_object(array_annidated[0], "aString");
		pr_info("test_ko_07 aString: %s\n", kjson_as_string(obj));
		
		// aJson is the second element -> an annidated json
		obj = kjson_lookup_object(array_annidated[1], "aJson");
		struct kjson_container *second_elem = kjson_as_container(obj);
		// aStringArr is a string array into aJson
		obj = kjson_lookup_object(second_elem, "aStringArr");
		pr_info("test_ko_07 aStringArr: %s,%s\n", (kjson_as_string_array(obj))[0], (kjson_as_string_array(obj))[1]);
				
		kjson_delete_container(ctn);		
	}
	
	// This not pass now: aJsonArr size is 1, but find_ctn_array_size compute 2
	char *test_ko_08 = "{\"aJsonArr\": [{\"arrJson\": {\"codes\": []}, \"aString\": \"1.0\"}], \"aString\": \"Linux\"}";
	
	if((ctn = kjson_parse(test_ko_08)) == NULL)
		pr_info("WARNING: test_ko_08 not passed\n");
	else
	{
		if(!IS_ERR((parsed = kjson_dump(ctn))))
			pr_info("test_ko_08: %s\n", kjstring_str(parsed));
		
		kjson_delete_container(ctn);		
	}

	// Cosi funziona: json annidati dentro array di json, ma lo json annidato e' l'ultimo elemento dell'array
	// char *str = "{\"accepted_1\": {\"codes\": []}, \"not_accepted\": [{\"questo_ok\": 1}, {\"questo_no\": {\"ciao\": []}}]}";
	// Cosi non funziona, perche' find_ctn_array_size calcola 1 la dimensione dell'array di json, quando questa e' 2
	// char *str = "{\"accepted_1\": {\"codes\": []}, \"not_accepted\": [{\"questo_no\": {\"ciao\": []}}, {\"questo_ok\": 1}]}";
	// Cosi non funziona, la dimensione di plugins e' 1, ma find_ctn_array_size la calcola 2
	//char *str = "{\"plugins\": [{\"alerts\": {\"codes\": []}, \"plugin_version\": \"1.0.58\"}], \"product_name\": \"Linux Endpoint\"}";

	// Funziona
	//char *str = "{\"accepted_1\": {\"codes\": []}, \"accepted_2\": [{\"enabled\": 1}, {\"codes\": []}]}";

    return 0;
}

void __exit test_exit(void)
{
}


module_init(test_init);
module_exit(test_exit);


MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");
