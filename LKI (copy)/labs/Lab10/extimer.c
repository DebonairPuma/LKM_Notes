/* focalpoint LKI */
/* Lab10: Timing Management Lab */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/jiffies.h>

#define DRIVER_AUTHOR "focalpoint"
#define DRIVER_DESC   "Lab10"

MODULE_LICENSE("GPL");           // Get rid of taint message by declaring code as GPL.

/*  Or with defines, like this: */
MODULE_AUTHOR(DRIVER_AUTHOR);    // Who wrote this module?
MODULE_DESCRIPTION(DRIVER_DESC); // What does this module do?

/* Global timer */
static struct timer_list my_timer;

int init(void);
void cleanup(void);

void my_timer_callback(unsigned long data)
{
	
	unsigned long jiff2;
	jiff2 = jiffies_64;
	
	printk("extimer: my_timer_callback called: %i\n", jiff2);
}

int init(void)
{
	unsigned long delay;
	unsigned long j2;
	printk(KERN_INFO "init_module() called\n");
	
	printk("extimer: delaying startup for 10 seconds\n");
	
	//jiffies = jiffies_64;
	delay = jiffies + 10*HZ;
	j2 = jiffies_64;
	/* delay startup for 10 seconds */
	printk(KERN_INFO "Initial Jimmies: %i\n", jiffies);	
	printk(KERN_INFO "HZ Value is: %i", HZ);
	schedule_timeout(10000000*HZ);

	jiffies = jiffies_64;
	printk(KERN_INFO "Rustled Jimmies: %i\n", jiffies);

	//schedule_timeout(delay);


	printk("extimer: installing timer\n");

	/* install timer my_timer */
	init_timer(&my_timer);
	delay = 1000;
	my_timer.expires = jiffies + delay; 
	my_timer.function = my_timer_callback;

	printk("extimer: setting timer to fire callback in 20 seconds\n");
	add_timer(&my_timer);
	/* add a timer to fire my_timer_callback in 20 seconds */

	return 0;
}

void cleanup(void)
{
	/* remove timer */

	printk(KERN_ALERT "Unloading extimer ...\n");
	del_timer(&my_timer);

}

module_init(init);
module_exit(cleanup);

