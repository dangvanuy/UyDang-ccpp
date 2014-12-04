/* Includes */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <unistd.h>     //for pipe

int samplepipe[2] = {-1, -1};

void * KeyPress_Proc ( void* n )
{
	printf("KeyPress_Proc\n");
	char key;
	while (1){
		key = getchar();	
		if (key >= 97 && key <= 122){
			write(samplepipe[1], &key, 1);
			printf("Write key %c into pipe\n", key);		
		}
		if (key == 'q'){
			break;
		}
	}
	printf("KeyPress_Proc exit\n");
	return NULL;
}

void * ListenerKeyPress_Proc(void* n)
{
	printf("ListenerKeyPress_Proc\n");
	int err;
	fd_set rfds;
	struct timeval timeout;
	timeout.tv_sec  = 0;
	timeout.tv_usec = 0;
	
	char buf;
	while(1){
		FD_ZERO( &rfds );
		FD_SET(samplepipe[0], &rfds );
		err = select(samplepipe[0] + 1, &rfds, NULL, NULL, &timeout);
		if (err){
			read(samplepipe[0], &buf, 1);
			printf("You have just pressed key <%c>\n", buf);
			if (buf == 'q')
				break;
		}		
	}
	printf("ListenerKeyPress_Proc exit\n");
	return NULL;
}


int main()
{
    pthread_t tkeyprss, tlistener;

	if (pipe(samplepipe) < 0){
		printf("Create pipe is error\n");
		return 0;
	}
	
    pthread_create (&tkeyprss, NULL, KeyPress_Proc, NULL);
    pthread_create (&tlistener, NULL, ListenerKeyPress_Proc, NULL);

    pthread_join(tkeyprss, NULL);
     
	close(samplepipe[1]);
	close(samplepipe[0]);
	printf("Program exit\n");
	return 0;
} 


