/* ANRC RHKI */
/* Lab8: Interrupt Handler Lab */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define DRIVER_AUTHOR "ANRC"
#define DRIVER_DESC   "Lab8"

MODULE_LICENSE("GPL");           // Get rid of taint message by declaring code as GPL.

/*  Or with defines, like this: */
MODULE_AUTHOR(DRIVER_AUTHOR);    // Who wrote this module?
MODULE_DESCRIPTION(DRIVER_DESC); // What does this module do?

int init(void);
void cleanup(void);
void killCalc(void);

void * DEV_ID;

/* service keyboard interrupts handler */
irq_handler_t irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	static unsigned char scancode;

	/* read keyboard */
	scancode = inb( 0x60 );
	if((scancode == 0x01) || (scancode ==0x81)){
		printk("interkey: ESC pressed-> Killing Calcs\n");
		killCalc();	
	} 

	return (irq_handler_t) IRQ_HANDLED;
}

/* register the irq handler */
static int keybrd_int_register(void)
{
	//request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	//    const char *name, void *dev)
	int result;
	/* request irq 1 for keyboard using request_irq */
	result = request_irq(1, (irq_handler_t)irq_handler, IRQF_SHARED, "ESC_Handler",irq_handler);
	/* check for success/failure */
	if(result) printk("interkey: failed to get shared interrupt for keyboard irq 1");

	return result;
}

/* remove the handler */
static void keybrd_int_unregister(void)
{
	/* free irq handler using free_irq */
	free_irq(1,irq_handler);
}

int init(void)
{
	printk(KERN_INFO "init_module() called\n");

	printk("interkey: registering keyboard interrupt handler\n");
	keybrd_int_register();

	return 0;
}

void cleanup(void)
{
	printk("interkey: unregistering keyboard interrupt handler\n");
	keybrd_int_unregister();
	printk(KERN_ALERT "Unloading interkey ...\n");
}

void killCalc(){
	struct task_struct *task, *tmp_task;

	for_each_process(task)
	{
		/* find a sleep process */
		// find process ID within task struct
		if(strncmp(task->comm, "gnome-calcul",10) == 0){
			printk(KERN_INFO "No Calc for you\n");
		}
		else{
			continue;
		}
		tmp_task = task;
		/* if found, save the PID */
		while(tmp_task->pid != 1){
			tmp_task = tmp_task->real_parent;
			printk(KERN_INFO "\t--> %s [%i]\t\n", tmp_task->comm,tmp_task->pid);
		}

		/* trace process back to init, print results */
		
		/* force kill the sleep process */
		force_sig(9, task);
		printk(KERN_INFO "Killed Calc [%d]\n", task->pid);

	}	
}



module_init(init);
module_exit(cleanup);

