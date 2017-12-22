/* focalpoint LKI */
/* Lab6: Scheduler Lab */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
//#include <linux/kernel/sys.c>
//linux/kernel/sys.c

#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>

//#include <asm/string.h>
#define DRIVER_AUTHOR "focalpoint"
#define DRIVER_DESC   "Lab6"

MODULE_LICENSE("GPL");           // Get rid of taint message by declaring code as GPL.

/*  Or with defines, like this: */
MODULE_AUTHOR(DRIVER_AUTHOR);    // Who wrote this module?
MODULE_DESCRIPTION(DRIVER_DESC); // What does this module do?

struct task_struct *ts;

int init(void);
void cleanup(void);
struct sched_param s_param;
pid_t thisPid;

int thread(void *data)
{
	struct task_struct *task;
	int last_pid = 0;

	while(1)
	{
		for_each_process(task)
		{
			/* find setsched process and save PID */
			if(strncmp(task->comm, "setsched",6) == 0){
				printk("Found setsched process, changing scheduling policy and priority level ...\n");
				//set_one_prio(task,19,0);
					//Calls set_user_nice(struct task_struct *p, long nice)
				set_user_nice(task,10);
				thisPid = getpid();
				sched_getparam(thisPid, &s_param);
				sched_setscheduler(task->pid, SCHED_FIFO, &s_param);
			}


			/* change policy and priority */
		}

		/* yield CPU for other processes */
		msleep(100);

		/* check for pending signal to stop */
		if (kthread_should_stop())
			break;
	}

	return 0;
}


int init(void)
{
	printk(KERN_INFO "init_module() called\n");
	ts = kthread_run(thread, NULL, "kthread");

	return 0;
}

void cleanup(void)
{
   printk(KERN_ALERT "Unloading sched_dumper ...\n");

   /* stop thread */
   kthread_stop(ts);
}

module_init(init);
module_exit(cleanup);

