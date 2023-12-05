// SPDX-License-Identifier: GPL-2.0
// Authors: Emanuele Santini <emanuele.santini.88@gmail.com>

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/fdtable.h>
#include <linux/inet.h>
#include <net/sock.h>

#include "../../kjson.h"

// We are receiving this JSON from somewhere (a process or file for example)
char *a_json = "{ \"process_pid\": 1, \"request\": [\"process_path\", \"file_open\", \"process_socket\"] }";

// Insert the excutable process path of task into the my_json structure
static void set_process_path(struct kjson_container *my_json, struct task_struct *task);
// Insert the files open by the task into the my_json structure
static void set_file_opens(struct kjson_container *my_json, struct task_struct *task);
// Insert the task command line string into the my_json structure
static void set_socket_data(struct kjson_container *my_json, struct task_struct *task);

int __init test_init(void)
{
    // This will contain a_json text parsed
    struct kjson_container *my_json;    
    
    // kjson_object_t rappresent a single <key, value> on a kjson_container
    struct kjson_object_t *obj;
    
    // Setup the JSON string to parse
    struct kjstring_t json_str;
    kjstring_new_string_buffer(&json_str, a_json, strlen(a_json));
    
    // Parse the json string
    my_json = kjson_parse(&json_str);
    if(!my_json)
    {
		pr_info("parse error\n");
		goto EXIT;
    }
    
    // Read the elements using the key
    obj = kjson_lookup_object(my_json, "process_pid");
    pid_t ppid = kjson_as_integer(obj);
    
    obj = kjson_lookup_object(my_json, "request");
    // Get the array len
    size_t array_len = kjson_array_length(obj); 
    // Get the pointer to the string array
    char **options = kjson_as_string_array(obj);
    
    // ------ Execute our program ------

    struct task_struct *task = get_pid_task(find_get_pid(ppid), PIDTYPE_TGID);
    if(!task)
    {
		pr_info("task %ld doesn't exist\n", (long int)ppid);
		goto EXIT;
    }
    
    int i = 0;
    while(i < array_len)
    {
    	if(strcmp(options[i], "process_path") == 0)
    		set_process_path(my_json, task);
    	else if(strcmp(options[i], "file_open") == 0)
    		set_file_opens(my_json, task);
    	else if(strcmp(options[i], "process_socket") == 0)
    		set_socket_data(my_json, task);
    		
    	i++;
    }
    
    // Get the json dump and print it
    struct kjstring_t *dmp = kjson_dump(my_json);
    if(!dmp)
    {
		pr_info("dump error\n");
		goto EXIT;
    }
    
    pr_info("%s\n", kjstring_str(dmp));

EXIT:
    //Remember to dealloc the json container before to exit
	kjson_delete_container(my_json);
    return 0;
}

void set_process_path(struct kjson_container *my_json, struct task_struct *task)
{
    // Here we will do some operation to get the executable file to know it's path

    struct file *exe_file = NULL;
    struct mm_struct *mm;
    char exe_path_str[256], *res;

    if(unlikely(!task || !task->mm))
        return;
        
    memset(exe_path_str, 0x0, 256);

    task_lock(task);

    // The executable file object is stored inside the mm_struct of the process
    mm = task->mm;
    rcu_read_lock();
    exe_file = rcu_dereference(mm->exe_file);
    if(exe_file && !get_file_rcu(exe_file))
        exe_file = NULL;
    rcu_read_unlock();

    task_unlock(task);
    
    if(IS_ERR(res = d_path(&exe_file->f_path, exe_path_str, 256)))
        return;

    // Now, I can write the executable path to the my_json
    kjson_push_string(my_json, "ExecutablePath", res);
}

void set_file_opens(struct kjson_container *my_json, struct task_struct *task)
{
    // Here we will do some operation to get the file opened by the process
    
	struct files_struct *fss;
	struct file *filp;
	char *paths[32], *buffer[32];
	int i = 0;
	
	memset(paths, 0x0, 32);
	
	task_lock(task);
	if(!(fss = task->files))
		goto EXIT;
		
	while(fss->fd_array[i] != NULL && i < 32)
	{
		filp = fss->fd_array[i];
		if(!(buffer[i] = (char*)get_zeroed_page(GFP_ATOMIC)))
			goto EXIT;
		paths[i] = d_path(&filp->f_path, buffer[i], 4096);
		i++;
	}
	
	// Now, I can write the process files open
	kjson_push_object(my_json, "ProcessFilesOpen", KOBJECT_TYPE_STRING_ARRAY, paths, i);
		
EXIT:	
	task_unlock(task);
	while(i > 0)
		free_page((unsigned long)buffer[--i]);
}

void set_socket_data(struct kjson_container *my_json, struct task_struct *task)
{
    // Here we will do some operation to get the socket opened by the process
    
	struct files_struct *fss;
	struct file *filp;
	struct inode *inodp;
	struct socket *sk;
	struct sock *sockp;
	char addr[INET_ADDRSTRLEN];
	int i = 0, j = 0;
	
	/* Pointer to an array of annidated JSON
	 * We are storing each socket into a single JSON container
	 */
	struct kjson_container* json_socks[32];
	
	task_lock(task);
	if(!(fss = task->files))
		goto EXIT;
		
	while(fss->fd_array[i] != NULL && i < 32)
	{
		filp = fss->fd_array[i];
		if(((inodp = file_inode(filp)) != NULL) && S_ISSOCK(inodp->i_mode))
		{
			if((sk = (struct socket*)filp->private_data) != NULL)
			{
				sockp = sk->sk;
				if((json_socks[j] = kjson_new_container()) != NULL)
				{
					kjson_push_integer(json_socks[j], "Protocol", (int64_t)sockp->sk_protocol);
					
					snprintf(addr, 16, "%pI4", &sockp->sk_daddr);
					kjson_push_string(json_socks[j], "DestinationIP", addr);
					
					snprintf(addr, 16, "%pI4", &sockp->sk_rcv_saddr);
					kjson_push_string(json_socks[j], "SourceIP", addr);
					
					kjson_push_integer(json_socks[j], "DestinationPort", sockp->sk_dport);
					kjson_push_integer(json_socks[j], "SourceIP", sockp->sk_num);
					
					j++;
				}
			}
		}
		i++;
	}
	
	// Now, I can write the process files open
	kjson_push_object(my_json, "ProcessSockOpen", KOBJECT_TYPE_OBJECT_ARRAY, json_socks, j);
		
EXIT:	
	task_unlock(task);
}

void __exit test_exit(void)
{
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");

