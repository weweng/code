#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");

static struct timer_list my_timer;
static int timer_in_ms = 17;

void my_timer_callback(struct timer_list *t ) {
	/* typically embedded t into a private structure, and we can retrieve private data structure */
	// drivers/staging/rtl8188eu/core/rtw_led.c
	// e.g. struct LED_871x *pLed = from_timer(pLed, t, BlinkTimer); 
	/* do your timer stuff here */
	printk(KERN_INFO "my_timer-callback is hit \n");
	dump_stack();

	// renew timer if needed
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(timer_in_ms));
}

/*
from the following stack trace, we can see that timer handler is invoked by softIRQ

[566705.836494]  dump_stack+0x6b/0x88
[566705.836498]  call_timer_fn+0x2d/0x130
[566705.836501]  __run_timers.part.0+0x1b5/0x230
[566705.836503]  ? __hrtimer_run_queues+0x128/0x280
[566705.836506]  ? kvm_clock_get_cycles+0xd/0x10
[566705.836507]  ? ktime_get+0x38/0xa0
[566705.836510]  ? native_apic_msr_write+0x28/0x30
[566705.836512]  run_timer_softirq+0x26/0x50  <<<<<<<<<<<<<<<<<<<
[566705.836542]  __do_softirq+0xe9/0x2da
[566705.836546]  asm_call_on_stack+0x12/0x20
[566705.836548]  </IRQ>
[566705.836551]  do_softirq_own_stack+0x39/0x50
[566705.836554]  irq_exit_rcu+0xc2/0x100
[566705.836556]  sysvec_apic_timer_interrupt+0x36/0x80
[566705.836559]  asm_sysvec_apic_timer_interrupt+0x12/0x20
 */

int init_module(void) {
	/* setup your timer to call my_timer_callback */
	timer_setup(&my_timer, my_timer_callback, 0);
	/* setup timer interval to desired time interval */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(timer_in_ms));
	return 0;
}

void cleanup_module(void) {
	/* remove kernel timer when unloading module */
	del_timer(&my_timer);
	return;
}
