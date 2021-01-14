/*
 * This file serves as a template of character device driver Example 
 * Author: Wenwei Weng
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wenwei Weng");

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static long device_ioctl(struct file*file,unsigned int num, unsigned long param);
static int device_mmap(struct file *file,struct vm_area_struct* vma);

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.unlocked_ioctl = device_ioctl,
	.release = device_release,
	.mmap = device_mmap
};

#define SUCCESS 0
#define DEVICE_NAME "wwwchardev" /* Dev name as it appears in /dev/ */

/*
* Global variables are declared as static, so are global within the file.
*/
/* Major number assigned to our device driver, allocate dynamically*/
static int Major; 

static struct class *dev_class;

/* Put internal implementation required variables here */


/*
 * This function is called when the module is loaded
 */
static int __init init_www_chrdrv_module(void)
{
	printk(KERN_INFO "%s init module is called \n", DEVICE_NAME);

#if 0
	// more cumbersome way
	dev_t dev;
	struct cdev my_cdev;
	allo_chrdev_region(&dev, 0, DEVICE_NAME); // allocate MAJOR number
	cdev_init(&my_cdev, &fops);
	cdev_add(&my_cdev, dev,1);
#else
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}
#endif

	printk(KERN_INFO "%s is being created with major number=%d \n", DEVICE_NAME, Major);

#if 0
	/* manually to create device file if it is chosen to do so */
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");
#else
	/* Create device name under /dev/ automatically */
	/* Creating struct class */
        if((dev_class = class_create(THIS_MODULE, DEVICE_NAME)) == NULL){
		printk(KERN_ERR "Cannot create the struct class\n");
		unregister_chrdev(Major, DEVICE_NAME);
		return -ENOMEM;
        }
 
        /* Creating device file thru API */
        if((device_create(dev_class, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME)) == NULL){
		printk(KERN_ERR "Cannot create the Device 1\n");
		class_destroy(dev_class);
		return -ENOMEM;
        }
#endif
	return SUCCESS;
}

/*
* This function is called when the module is unloaded
*/
static void __exit cleanup_www_chrdrv_module(void)
{
	printk(KERN_INFO "%s cleanup is called \n", DEVICE_NAME);
	device_destroy(dev_class, MKDEV(Major, 0));
	class_destroy(dev_class);
	unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * Methods
 */
/*
* Called when a process tries to open the device file, like
* "cat /dev/${DEVICE_NAME}"
*/
static int
device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;
	printk(KERN_INFO "%s open is called \n", DEVICE_NAME);

	/* do something special for this driver */
	counter++;

	/**************************************************************/

	/* mark this module is being used, prevent module/this driver to be removed */
	try_module_get(THIS_MODULE);
	
	return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s release is called \n", DEVICE_NAME);
	
 	/* do something special for this driver */
	
	/**************************************************************/

	/*
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module.
	 */
	module_put(THIS_MODULE);
	return 0;
}

/*
* Called when a process, which already opened the dev file, attempts to
* read from it.
*/
static ssize_t
device_read(struct file *filp, /* see include/linux/fs.h */
	    char *buffer, /* buffer to fill with data */
	    size_t length, /* length of the buffer */
	    loff_t * offset)
{
	int bytes_read = 0;
	printk(KERN_INFO "%s read is called \n", DEVICE_NAME);

	/* do something special for this driver */

	/**************************************************************/

        /*
	 * Most read functions return the number of bytes put into the buffer
	 */
	return bytes_read;
	
	
}


/*
* Called when a process writes to dev file: echo "hi" > /dev/hello
*/
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	/* do something special for this driver if needed */

	/**************************************************************/
	printk(KERN_INFO "%s write is being called  \n", DEVICE_NAME);

	/* return EINVALID to say not supported yet */
	return -EINVAL;
}

static long device_ioctl(struct file*file, unsigned int num, unsigned long param)
{
	printk(KERN_INFO "%s device_ioctl is called, num=%d .\n", DEVICE_NAME, num);
    
	switch(num)
	{
        case 10:
		/* do something */
                break;
        case 20:
 		/* do something */
               break;
        default:
                break;
	}
	return 0;
}
 

static int device_mmap(struct file *file,struct vm_area_struct* vma)
{
	int size;
	size=vma->vm_end - vma->vm_start;
	printk(KERN_INFO "%s device_mmap is called.\n", DEVICE_NAME);

	return 0;
}


module_init(init_www_chrdrv_module);
module_exit(cleanup_www_chrdrv_module);
