/*
 * sys-fs to demostrate how to use sys fs read/write
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sysfs.h>   /* API for sysfs entry */
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include<linux/sysfs.h> 
#include<linux/kobject.h> 
MODULE_LICENSE("Dual BSD/GPL");

volatile int www_value_int = 7;
struct kobject *www_kobj_ref;

#define SYSFS_NAME "www_sysfs"

/*
** This fuction will be called when we read the sysfs file
*/
static ssize_t www_sysfs_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf)
{
        printk(KERN_INFO "Sysfs - Read!!!\n");
        return sprintf(buf, "%d", www_value_int);
}
/*
** This fuction will be called when we write the sysfsfs file
*/
static ssize_t www_sysfs_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count)
{
        printk(KERN_INFO "Sysfs - Write!!!\n");
        sscanf(buf,"%d",&www_value_int);
	// if the value is string, simply copy into as string
	
        return count;
}

struct kobj_attribute www_obj_int_attr = __ATTR(www_value_int, 0660, www_sysfs_show, www_sysfs_store);

static int
my_sysfs_init (void)
{
	/*Creating a directory in /sys/kernel/ so we will have /sys/kernel/${SYSFS_NAME} */
        www_kobj_ref = kobject_create_and_add(SYSFS_NAME, kernel_kobj);
 
        /*Creating sysfs file for www_value_int*/
        if(sysfs_create_file(www_kobj_ref, &www_obj_int_attr.attr)){
                printk(KERN_ALERT "Cannot create sysfs file......\n");
                goto r_sysfs;
	}
	
        printk(KERN_INFO "%s Driver Insert...Done!!!\n", SYSFS_NAME);
        return 0;
 
r_sysfs:
        kobject_put(www_kobj_ref); 
        sysfs_remove_file(kernel_kobj, &www_obj_int_attr.attr);
	return 0;
}
 
static void
my_sysfs_cleanup (void)
{
	printk (KERN_INFO "%s unloaded succefully.\n", SYSFS_NAME);
        kobject_put(www_kobj_ref); 
        sysfs_remove_file(kernel_kobj, &www_obj_int_attr.attr);
}
 
module_init (my_sysfs_init);
module_exit (my_sysfs_cleanup);
