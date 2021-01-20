/*
 * main.c -- the bare scull char module
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
 
#include <linux/kernel.h>       /* printk() */
#include <linux/slab.h>         /* kmalloc() */
#include <linux/fs.h>           /* everything... */
#include <linux/errno.h>        /* error codes */
#include <linux/types.h>        /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#include <asm/uaccess.h>        /* copy_*_user */
 
#include "scull.h"              /* local definitions */
 
/*
 * Parameters which can be set at load time.
 */
int scull_major =   SCULL_MAJOR;
int scull_minor =   0;
int scull_nr_devs = SCULL_NR_DEVS;      /* number of bare scull devices */
int scull_quantum = SCULL_QUANTUM;
int scull_qset =    SCULL_QSET;
 
module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_nr_devs, int, S_IRUGO);
module_param(scull_quantum, int, S_IRUGO);
module_param(scull_qset, int, S_IRUGO);
 
MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");

struct scull_dev *scull_devices;        /* allocated in scull_init_module */
 
/*
 * Empty out the scull device; must be called with the device
 * semaphore held.
 */
int scull_trim(struct scull_dev *dev) {
	struct scull_qset *next, *dptr;
	int qset = dev->qset;   /* "dev" is not-null */
	int i;
   
	for (dptr = dev->head; dptr; dptr = next) { /* all the list items */
		if (dptr->data) {
			for (i = 0; i < qset; i++)
				kfree(dptr->data[i]);
			kfree(dptr->data);
			dptr->data = NULL;
		}
		next = dptr->next;
		kfree(dptr);
	}
	dev->size = 0;
	dev->quantum = scull_quantum;
	dev->qset = scull_qset;
	dev->head = NULL;
	return 0;
}

#ifdef SCULL_DEBUG /* use proc only if debugging */
/*
 * The proc filesystem: function to read and entry
 */
int scull_read_procmem(struct seq_file *m, void *v) {
	int i, j, len = 0;
	int limit = SCULL_NR_DEVS;

	for (i = 0; i < scull_nr_devs && len <= limit; i++) {
		struct scull_dev *d = &scull_devices[i];
		struct scull_qset *qs = d->head;
		if (down_interruptible(&d->sem))
			return -ERESTARTSYS;
		seq_printf(m, "\nDevice %i: qset %i, q %i, sz %li\n",
			   i, d->qset, d->quantum, d->size);
		for (; qs && len <= limit; qs = qs->next) { /* scan the list */
			seq_printf(m, "  item at %p, qset at %p\n",
				   qs, qs->data);
			if (qs->data && !qs->next) /* dump only the last item */
				for (j = 0; j < d->qset; j++) {
					if (qs->data[j])
						seq_printf(m, "    % 4i: %8p\n", j, qs->data[j]); 
				}
		}
		up(&scull_devices[i].sem);
	}
	
	return 0;
}

/*
 * For now, the seq_file implementation will exist in parallel.  The
 * older read_procmem function should maybe go away, though.
 */

/*
 * Here are our sequence iteration methods.  Our "position" is
 * simply the device number.
 */
static void *scull_seq_start(struct seq_file *s, loff_t *pos) {
   if (*pos >= scull_nr_devs) return NULL;   /* No more to read */
   return scull_devices + *pos;
}

static void *scull_seq_next(struct seq_file *s, void *v, loff_t *pos) {
   (*pos)++;
   if (*pos >= scull_nr_devs)
      return NULL;
   return scull_devices + *pos;
}

static void scull_seq_stop(struct seq_file *s, void *v) {
   /* Actually, there's nothing to do here */
}

static int scull_seq_show(struct seq_file *s, void *v) {
   struct scull_dev *dev = (struct scull_dev *) v;
   struct scull_qset *d;
   int i;
   
   if (down_interruptible(&dev->sem)) return -ERESTARTSYS;
   seq_printf(s, "\nDevice %i: qset %i, q %i, sz %li\n",
	      (int) (dev - scull_devices), dev->qset,
	      dev->quantum, dev->size);
   for (d = dev->head; d; d = d->next) { /* scan the list */
      seq_printf(s, "  item at %p, qset at %p\n", d, d->data);
      if (d->data && !d->next) /* dump only the last item */
	 for (i = 0; i < dev->qset; i++) {
	    if (d->data[i])
	       seq_printf(s, "    % 4i: %8p\n",
			  i, d->data[i]);
	 }
   }
   up(&dev->sem);
   return 0;
}
         
/*
 * Tie the sequence operators up.
 */
static struct seq_operations scull_seq_ops = {
   .start = scull_seq_start,
   .next  = scull_seq_next,
   .stop  = scull_seq_stop,
   .show  = scull_seq_show
};

/*
 * Now to implement the /proc file we need only make an open
 * method which sets up the sequence operators.
 */
static int scull_proc_open(struct inode *inode, struct file *file) {
   return seq_open(file, &scull_seq_ops);
}

/*
 * Create a set of file operations for our proc file.
 */
static struct proc_ops scull_proc_ops = {
   .proc_open    = scull_proc_open,
   .proc_read    = seq_read,
   .proc_lseek  = seq_lseek,
   .proc_release = seq_release
};

/*
 * Actually create (and remove) the /proc file(s).
 */
static void scull_create_proc(void) {
	struct proc_dir_entry *pde;
	pde = proc_create_single("scullmem", 0, NULL, scull_read_procmem);
	if (!proc_create("scullseq", 0644, NULL, &scull_proc_ops))
		goto err;
	return ;
 
err:
	remove_proc_entry("scullmem", NULL);
	return;
}

static void scull_remove_proc(void) {
	/* no problem if it was not registered */
	remove_proc_entry("scullmem", NULL);
	remove_proc_entry("scullseq", NULL);
}

#endif /* SCULL_DEBUG */

/*
 * Open and close
 */
 
int scull_open(struct inode *inode, struct file *filp) {  /* linux/fs.h struct file */
	struct scull_dev *dev; /* retrieve device information */

	/*  scull_setup_cdev has registered scull_dev dev->cdev, */
	/*  which is passed back by inode->i_cdev, nice! */
	dev = container_of(inode->i_cdev, struct scull_dev, cdev);

	/* filp is created for each device file open in user space */
	/* which is mapped to each its respective fd in user space */
	printk(KERN_NOTICE "scull_open with filp=%p ", filp);
	filp->private_data = dev;

	/* now trim to 0 the length of the device if open was write-only */
	if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;
		scull_trim(dev); /* ignore errors */
		up(&dev->sem);
	}
	return 0;          /* success */
}

int scull_release(struct inode *inode, struct file *filp) {
	return 0;
}

/*
 * Follow the list
 */
struct scull_qset *scull_follow(struct scull_dev *dev, int n) {
	struct scull_qset *qs = dev->head;
 
	/* Allocate first qset explicitly if need be */
	if (!qs) {
		qs = dev->head = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
		if (qs == NULL)
			return NULL;  /* Never mind */
		memset(qs, 0, sizeof(struct scull_qset));
	}
   
	/* Then follow the list */
	while (n--) {
		if (!qs->next) {
			qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
			if (qs->next == NULL)
				return NULL;  /* Never mind */
			memset(qs->next, 0, sizeof(struct scull_qset));
		}
		qs = qs->next;
		continue;
	}
	return qs;
}

/*
 * Data management: read and write
 */
ssize_t scull_read(struct file *filp, char __user *buf, size_t count,
		   loff_t *f_pos) {
	struct scull_dev *dev = filp->private_data; 
	struct scull_qset *dptr;        /* the first listitem */
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset; /* how many bytes in the listitem */
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;
   
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	if (*f_pos >= dev->size) goto out;
	if (*f_pos + count > dev->size)
		count = dev->size - *f_pos;
   
	/* find listitem, qset index, and offset in the quantum */
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	s_pos = rest / quantum; q_pos = rest % quantum;
   
	/* follow the list up to the right position (defined elsewhere) */
	dptr = scull_follow(dev, item);
   
	if (dptr == NULL || !dptr->data || ! dptr->data[s_pos])
		goto out; /* don't fill holes */
   
	/* read only up to the end of this quantum */
	if (count > quantum - q_pos)
		count = quantum - q_pos;
   
	if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count)) {
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;
	retval = count;
   
 out:
	up(&dev->sem);
	return retval;
}

ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,
		    loff_t *f_pos)
{
	struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr;
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset;
	int item, s_pos, q_pos, rest;
	ssize_t retval = -ENOMEM; /* value used in "goto out" statements */
   
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
   
	/* find listitem, qset index and offset in the quantum */
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	s_pos = rest / quantum; q_pos = rest % quantum;
   
	/* follow the list up to the right position */
	dptr = scull_follow(dev, item);
	if (dptr == NULL)
		goto out;
	if (!dptr->data) {
		dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
		if (!dptr->data)
			goto out;
		memset(dptr->data, 0, qset * sizeof(char *));
	}
	if (!dptr->data[s_pos]) {
		dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
		if (!dptr->data[s_pos])
			goto out;
	}
	/* write only up to the end of this quantum: */
	if (count > quantum - q_pos)
		count = quantum - q_pos;
   
	if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count)) {
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;
	retval = count;
   
	/* update the size */
	if (dev->size < *f_pos)
		dev->size = *f_pos;
   
 out:
	up(&dev->sem);
	return retval;
}

static char scull_tasklet_data[] = "scull tasklet data";
struct tasklet_struct scull_tasklet;

void scull_tasklet_handler(unsigned long data ) {
	printk(KERN_INFO " scull_taskelt_handler: %s\n", (char *)data);
	dump_stack();
	return;
}
/*
case 1: executed by ksoftirqd

[561937.626079]  dump_stack+0x6b/0x88
[561937.626086]  scull_tasklet_handler+0x19/0x1a [scull]
[561937.626091]  tasklet_action_common.constprop.0+0x110/0x130
[561937.626096]  __do_softirq+0xe9/0x2da
[561937.626100]  run_ksoftirqd+0x26/0x40
[561937.626103]  smpboot_thread_fn+0xc5/0x160
[561937.626107]  ? smpboot_register_percpu_thread+0xf0/0xf0
[561937.626109]  kthread+0x11b/0x140
[561937.626111]  ? __kthread_bind_mask+0x60/0x60
[561937.626114]  ret_from_fork+0x22/0x30


case 2: by APIC timer
[561871.987724]  dump_stack+0x6b/0x88
[561871.987730]  scull_tasklet_handler+0x19/0x1a [scull]
[561871.987733]  tasklet_action_common.constprop.0+0x110/0x130
[561871.987736]  __do_softirq+0xe9/0x2da
[561871.987739]  asm_call_on_stack+0x12/0x20
[561871.987740]  </IRQ>
[561871.987742]  do_softirq_own_stack+0x39/0x50
[561871.987744]  irq_exit_rcu+0xc2/0x100
[561871.987745]  sysvec_apic_timer_interrupt+0x36/0x80
[561871.987748]  asm_sysvec_apic_timer_interrupt+0x12/0x20

 */	

static char scull_work_data[] = "scull work data";
static struct work_struct scull_work;

static void scull_work_handler(struct work_struct *work) {
	/* typically we will embedded work inside a private data structure, which */
	/* holds device specific info, and processing received info there */
	printk(KERN_INFO " scull_work_handler: %s\n", (char *)scull_work_data);
	dump_stack();
	return;	
}

/*
the work is executed by kworker daemon thread:

565003.861718]  dump_stack+0x6b/0x88
[565003.861720]  scull_work_handler+0x1d/0x1e [scull]
[565003.861723]  process_one_work+0x1b4/0x370
[565003.861725]  worker_thread+0x53/0x3e0
[565003.861726]  ? process_one_work+0x370/0x370
[565003.861729]  kthread+0x11b/0x140
[565003.861731]  ? __kthread_bind_mask+0x60/0x60
[565003.861734]  ret_from_fork+0x22/0x30

 */
/*
 * The ioctl() implementation
 */

long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
   
	int tmp = 0;
	int retval = 0;
   
	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;
      
	switch(cmd) {
      	case SCULL_IOCRESET:
		scull_quantum = SCULL_QUANTUM;
		scull_qset = SCULL_QSET;
		break;
      
	case SCULL_IOCSQUANTUM: /* Set: arg points to the value */
		/* kernel/capability.c: determine if the current user is allowed to do */
		/* see uapi/linux/capability.h for various capability */
		if (!capable (CAP_SYS_ADMIN))
			return -EPERM;
		retval = __get_user(scull_quantum, (int __user *)arg);
		break;
      
	case SCULL_IOCTQUANTUM: /* Tell: arg is the value */
		if (!capable (CAP_SYS_ADMIN))
			return -EPERM;
		scull_quantum = arg;
		break;
      
	case SCULL_IOCGQUANTUM: /* Get: arg is pointer to result */
		retval = __put_user(scull_quantum, (int __user *)arg);
		break;
      
	case SCULL_IOCQQUANTUM: /* Query: return it (it's positive) */
		return scull_quantum;
      
	case SCULL_IOCXQUANTUM: /* eXchange: use arg as pointer */
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		tmp = scull_quantum;
		retval = __get_user(scull_quantum, (int __user *)arg);
		if (retval == 0)
			retval = __put_user(tmp, (int __user *)arg);
		break;
      
	case SCULL_IOCHQUANTUM: /* sHift: like Tell + Query */
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		tmp = scull_quantum;
		scull_quantum = arg;
		return tmp;
      
	case SCULL_IOCSQSET:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		retval = __get_user(scull_qset, (int __user *)arg);
		break;
      
	case SCULL_IOCTQSET:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		scull_qset = arg;
		break;
      
	case SCULL_IOCGQSET:
		retval = __put_user(scull_qset, (int __user *)arg);
		break;
      
	case SCULL_IOCQQSET:
		return scull_qset;
      
	case SCULL_IOCXQSET:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		tmp = scull_qset;
		retval = __get_user(scull_qset, (int __user *)arg);
		if (retval == 0)
			retval = put_user(tmp, (int __user *)arg);
		break;
      
	case SCULL_IOCHQSET:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		tmp = scull_qset;
		scull_qset = arg;
		return tmp;
      
	/*
	 * The following two change the buffer size for scullpipe.
	 * The scullpipe device uses this same ioctl method, just to
	 * write less code. Actually, it's the same driver, isn't it?
	 */
	case SCULL_P_IOCTSIZE:
		scull_p_buffer = arg;
		break;
      
	case SCULL_P_IOCQSIZE:
		return scull_p_buffer;
      
	case SCULL_IOCTASKLET:		
		tasklet_init(&scull_tasklet, scull_tasklet_handler, (unsigned long)scull_tasklet_data);
		tasklet_schedule(&scull_tasklet);
		break;
      
	case SCULL_IOCWORKQUEUE:
		INIT_WORK(&scull_work, scull_work_handler);
		schedule_work(&scull_work);
		break;
      
	default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	return retval;
}

/*
 * The "extended" operations -- only seek
 */

loff_t scull_llseek(struct file *filp, loff_t off, int whence) {
	struct scull_dev *dev = filp->private_data;
	loff_t newpos;
   
	switch(whence) {
	case 0: /* SEEK_SET */
		newpos = off;
		break;
      
	case 1: /* SEEK_CUR */
		newpos = filp->f_pos + off;
		break;
      
	case 2: /* SEEK_END */
		newpos = dev->size + off;
		break;
      
	default: /* can't happen */
		return -EINVAL;
	}
	if (newpos < 0) return -EINVAL;
	filp->f_pos = newpos;
	return newpos;
}

struct file_operations scull_fops = {
	.owner =    THIS_MODULE,
	.llseek =   scull_llseek,
	.read =     scull_read,
	.write =    scull_write,
	.unlocked_ioctl = scull_ioctl,
	.open =     scull_open,
	.release =  scull_release,
};

/************************************************************************/
/*
 * Finally, the module stuff
 */

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void scull_cleanup_module(void) {
	int i;
	dev_t devno = MKDEV(scull_major, scull_minor);
   
	/* Get rid of our char dev entries */
	if (scull_devices) {
		for (i = 0; i < scull_nr_devs; i++) {
			scull_trim(scull_devices + i);
			cdev_del(&scull_devices[i].cdev);
		}
		kfree(scull_devices);
	}
   
#ifdef SCULL_DEBUG /* use proc only if debugging */
	scull_remove_proc();
#endif
   
	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, scull_nr_devs);
   
	/* and call the cleanup functions for friend devices */
	scull_p_cleanup();
	scull_access_cleanup();
   
}

/*
 * Set up the char_dev structure for this device.
 */
static void scull_setup_cdev(struct scull_dev *dev, int index) {
	int err, devno = MKDEV(scull_major, scull_minor + index);

	/* adding scull_dev dev->cdev with file_operations, cdev address will be referenced */
	/* later in scull_open to find scull_dev object */
	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add (&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

int scull_init_module(void) {
	int result, i;
	dev_t dev = 0;
   
	/*
	 * Get a range of minor numbers to work with, asking for a dynamic
	 * major unless directed otherwise at load time.
	 */
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_nr_devs, "scull");  /* fs.h */
	} else {
		/* deault mode, allocate dynamically, it can be found from /proc/device */
		/* cat /proc/devices | grep scull | awk '{print $1}' */
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs,
					     "scull");
		scull_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		return result;
	}
   
	/* 
	 * allocate the devices -- we can't have them static, as the number
	 * can be specified at load time
	 */
	scull_devices = kmalloc(scull_nr_devs*sizeof(struct scull_dev), GFP_KERNEL); /* slab.h */
	if (!scull_devices) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}
	memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev)); /* string.h */
   
	/* Initialize each device. */
	for (i = 0; i < scull_nr_devs; i++) {
		scull_devices[i].quantum = scull_quantum;
		scull_devices[i].qset = scull_qset;
		sema_init(&scull_devices[i].sem, 1);
		scull_setup_cdev(&scull_devices[i], i);
	}
   
	/* At this point call the init function for any friend device */
	dev = MKDEV(scull_major, scull_minor + scull_nr_devs);
	dev += scull_p_init(dev);
	dev += scull_access_init(dev);
   
#ifdef SCULL_DEBUG /* only when debugging */
	scull_create_proc();
#endif
   
	return 0; /* succeed */
   
 fail:
	scull_cleanup_module();
	return result;
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);