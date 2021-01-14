/*
 * proc-fs to demostrate how to use proc fs read/write
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>    /* printk() */
#include <linux/proc_fs.h>  /* API for proc entry */
#include <linux/seq_file.h>
#include <asm/uaccess.h>
 
#define WEWENG_PROC_DIR "weweng"
#define WEWENG_PROC_ENTRY "weweng/syscall_stats"
 
static struct proc_dir_entry *weweng_proc = NULL;

#define NR_SYSCALLS 10
 
static unsigned long __syscall_counter[NR_SYSCALLS] = {1,2,3,5,7,20,7,9,8,0};

static int syscall_stat_proc_show(struct seq_file *m, void *v)
{
   int i = 0;

   seq_printf(m, "Refer /usr/include/asm/unistd*.h for definition.\n");
   seq_printf(m, "Syscall: \t Counter \n");
   for (i=0; i<NR_SYSCALLS; i++)
       seq_printf(m, "%d:\t\t %lu \n", i, __syscall_counter[i]);

   return 0;
}

static int
procfs_init (void)
{
	struct proc_dir_entry *pde;
  
	printk (KERN_INFO "procfs loaded sucessfuly.\n");
	weweng_proc = proc_mkdir(WEWENG_PROC_DIR, NULL);
	if (!weweng_proc)
		return -ENOMEM;
	
	pde = proc_create_single(WEWENG_PROC_ENTRY, 0, NULL, syscall_stat_proc_show);
	if (!pde) {
		remove_proc_entry(WEWENG_PROC_DIR, NULL);
		return -ENOMEM;
	}
 
	return 0;
}
 
static void
procfs_cleanup (void)
{
	printk (KERN_INFO "procfs unloaded succefully.\n");
	remove_proc_entry(WEWENG_PROC_DIR, NULL);
}
 
module_init (procfs_init);
module_exit (procfs_cleanup);
MODULE_LICENSE("Dual BSD/GPL");
