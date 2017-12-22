#include <stdio.h>
#include <unistd.h>
#include <linux/sched.h>
#include <sys/resource.h>

/*
 * Scheduling policies defined in sched.h
#define SCHED_NORMAL	0
#define SCHED_FIFO		1
#define SCHED_RR		2
#define SCHED_BATCH		3
 */

struct sched_param {
	int sched_priority;
};

struct sched_param s_param;
pid_t thisPid;
void setscheduler(void)
{

	/* call sched_getparam() to initialize s_param struct */
	thisPid = getpid();
	
	sched_getparam(thisPid, &s_param);
	s_param.sched_priority = 1;

	sched_setscheduler(0, SCHED_FIFO, &s_param);
	setpriority(PRIO_PROCESS,0,1);
	

	/* set sched priority to 1 and schedule policy to FIFO */


	printf("Scheduler set to SCHED_FIFO with priority %i...\n", s_param.sched_priority);

	/* check for errors */
	printf("!!!Scheduler set to SCHED_FIFO with priority %i FAILED!!!\n", s_param.sched_priority);
}

int main()
{
	setscheduler();

	while(1)
	{
		printf("PID %i sleeping..\n", getpid());
		switch(sched_getscheduler(getpid()))
		{
			case SCHED_NORMAL: printf("sched_getscheduler() = SCHED_NORMAL\n");
								break;
			case SCHED_FIFO: printf("sched_getscheduler() = SCHED_FIFO\n");
								break;
			case SCHED_RR: printf("sched_getscheduler() = SCHED_RR\n");
								break;
			case SCHED_BATCH: printf("sched_getscheduler() = SCHED_BATCH\n");
								break;
			default:
				break;
		}

		printf("Process priority %i...\n", getpriority(PRIO_PROCESS, 0));
		sleep(5);
	}
}
