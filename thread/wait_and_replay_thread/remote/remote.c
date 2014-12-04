#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <pthread.h> //for thread
#include <fcntl.h>
#include "remote.h"

//#define Test 1

#ifndef Test
#include "directfb.h"
#endif

#define SMP_REMOTE_DEV "/dev/ir"
unsigned flags = O_RDONLY;
int fno;
fd_set rfds;
struct timeval tv;
int ret;
void (*pFUNC_Global_Handle)(unsigned long) = NULL;
void (*pFUNC_Engineer_Handle)(unsigned long) = NULL;

void *smp_remote_event_thread(void *n)
{
    union {
    unsigned long l;
    unsigned char c[sizeof(unsigned long)];
    }u;
    int cnt = 0;
	while(1){
		if (flags & O_NONBLOCK) {
            if (read(fno, &u.l, sizeof(unsigned long)) > 0)
                printf("A. Iter(%d): 0x%08lx\n", cnt, u.l);
            else
                printf("A. Iter(%d): nothing >0x%08lx<\n", cnt, u.l);
        } else {
            fd_set rfds;
            struct timeval tv;
            int ret;

            FD_ZERO(&rfds);
            FD_SET(fno, &rfds);
            tv.tv_sec = 5;
            tv.tv_usec = 0;

            ret = select(fno + 1, &rfds, NULL, NULL, &tv);
            if (ret) {
                if (read(fno, &u.l, sizeof(unsigned long)) > 0){
					printf("==============================================\n");
					if (pFUNC_Global_Handle != NULL){
						pFUNC_Global_Handle(u.l); //for gui
					} else {
						printf("pFUNC_Global_Handle IS NULL\n");
					}
					if (pFUNC_Engineer_Handle != NULL){
						pFUNC_Engineer_Handle(u.l); //for gui
					} else {
						printf("pFUNC_Engineer_Handle IS NULL\n");
					}
                }else{
                    printf("B. Iter(%d): nothing >0x%08lx<\n", cnt, u.l);
				}
            } else {
				//printf("B. Iter(%d): timeout\n", cnt);
            }
        }
        cnt += 1;
        if (u.l == Key_POWER){ //key power
            break;
        }
	}
	printf("\nGui is stoped to receive Key!\n");
	return NULL;
}

pthread_t var_thread_remote;
int driver_open_device() {

	int status = pthread_create(&var_thread_remote, NULL, smp_remote_event_thread, NULL);
	
	return status;
}

void run_remote()
{
	if ((fno = open(SMP_REMOTE_DEV, flags)) < 0) {
		printf("error opening %s\n", SMP_REMOTE_DEV);
		printf("You have run cmd: \"mknod %s c 253 0\" yet?\n", SMP_REMOTE_DEV);
		return;
	}
	
    FD_ZERO(&rfds);
    FD_SET(fno, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 500000; /* 0.5 second */

    ret = select(fno + 1, &rfds, NULL, NULL, &tv);
    driver_open_device();    
}

void stop_remote()
{
	pthread_join(var_thread_remote, NULL);
	close(fno);
	printf("Close remote\n");
}
void set_handle(void *handle)
{
	pFUNC_Global_Handle = handle;
}
void set_engineer_handle(void *handle)
{
	pFUNC_Engineer_Handle = handle;
}

#if 0
void main_handle(unsigned long key)
{
	switch (key) {
		case Key_ENTER:
			printf("Enter >> \n");
			break;
		case Key_LEFT:
			printf("Left >> \n");
			break;
		case Key_RIGHT:
			printf("Right >> \n");
			break;
		case Key_UP:
			printf("Up >> \n");
			break;
		case Key_DOWN:
			printf("Down >> \n");
			break;
	}
}
int main()
{
	set_handle(main_handle);
	run_remote();
	return 0;
}
#endif



