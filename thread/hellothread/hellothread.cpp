#include <stdio.h>
#include <string.h> //for memset
#include <stdlib.h> //for malloc
#include <unistd.h> //for usleep
#include <pthread.h> //for thread
typedef struct Stest
{
	int per;
	int value;
}var;

class B
{
public:
	var *varB;
	pthread_t var_thread;
	int thread_status;
	B();
	~B();
	void my_thread();
	int fun_use_thread();
	
};

void *thread(void *prt)
{
	B *my_class = (B*)prt;	
	my_class->my_thread();
	return NULL;
}

B::B()
{
	printf("===> constructor B <===\n");	
	varB = (var*)malloc(sizeof(var));
	varB->per 	  = 0;
	varB->value   = 0;
	thread_status = 0;	
}

void B::my_thread()
{
	printf("===> begin thread <===\n");
	while(varB->per <= 4)
	{
		varB->per = varB->per + 1;
		printf("varB->per = %d and waiting ....\n",varB->per);
		usleep(1000000);
	}	
	printf("===>  End thread  <===\n");
}

int B::fun_use_thread()
{
	printf("===> fun_use_thread <===\n");
	return pthread_create(&var_thread, NULL, thread, this);	
}

int main()
{
	B *b = new B();
	b->fun_use_thread();
	printf("===> Main <===\n");
	pthread_join(b->var_thread);
	return 0;
}
