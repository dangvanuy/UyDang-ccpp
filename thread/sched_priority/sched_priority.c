#include <stdio.h>
#include <pthread.h>
#include <sched.h> //struct sched_param 

void Get_Sched_Priority_MinMax(int *min, int *max)
{
	*min = sched_get_priority_min(SCHED_RR);
	*max = sched_get_priority_max (SCHED_RR);
	printf("Get_Sched_Priority_MinMax min = %d, max = %d\n", *min, *max);
}

static void *Sample_Priority(void *p)
{
	("Thread Sample_Priority is running\n");
	printf("I am a new thread #%08lx\n", (long)pthread_self());
	return NULL;
}

pthread_t create_thread_with_priority(char *name, int greePriority)
{
	pthread_t tid;
	pthread_attr_t attr;
	int rc = pthread_attr_init(&attr);
	struct sched_param param = {0,};
	int priority_min = sched_get_priority_min(SCHED_RR);
	int priority_max = sched_get_priority_max(SCHED_RR);
	param.sched_priority = (priority_max - priority_min)/2 + greePriority;
	printf("create thread %s with real time priority %d (min=%d, max=%d)\n", (char*)name, param.sched_priority, priority_min, priority_max);

	rc = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	rc = pthread_attr_setschedparam(&attr, &param);
	rc = pthread_create(&tid, &attr, Sample_Priority, NULL);
	pthread_attr_destroy(&attr);
	return tid;
}

int main()
{
	int sched_min, sched_max;
	Get_Sched_Priority_MinMax(&sched_min, &sched_max);
	
	pthread_t tid = create_thread_with_priority((char*)"Sample_Priority", 0);
	pthread_join(tid, NULL);
	return 0;
}


//gcc -o sched_priority sched_priority.c -lpthread

