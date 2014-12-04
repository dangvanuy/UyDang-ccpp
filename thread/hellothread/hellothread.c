#include <stdio.h>
#include <string.h> //for memset
#include <stdlib.h> //for malloc
#include <unistd.h> //for usleep
#include <pthread.h> //for thread

#include <termio.h>

int Run = 1;
void *thread(void *)
{
	while(Run){
		printf("\nThread is running");
		//sleep(1);
		usleep(5000000);
	}
	printf("\nThread is exit");
	return NULL;
}

pthread_t var_thread;
int fun_use_thread()
{
	return pthread_create(&var_thread, NULL, thread, NULL);	
}

int getch( void )
{
	char ch;
	int fd = fileno(stdin);	
	struct termio old_tty, new_tty;
	ioctl(fd, TCGETA, &old_tty);
	new_tty = old_tty;
	new_tty.c_lflag &= ~(ICANON | ECHO | ISIG);
	ioctl(fd, TCSETA, &new_tty);
	fread(&ch, 1, sizeof(ch), stdin);	
	ioctl(fd, TCSETA, &old_tty);
	printf("get key(press 1 = quit): %d\n",ch); 
	return ch;
}

int main()
{
	fun_use_thread();
	int exit;
	while(Run){		
		exit = getch();
		if (exit == 49)
			Run = 0;		
		//sleep(1);
		//usleep(500000);
	}
}


