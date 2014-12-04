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
#include "Threads.h"
#include "Engineer.h"
#include "remote.h"
#include "directfb.h"
/*
* will control Threads
*/
#define MAX_THREAD_ITEM 3

SParam a_sparam[MAX_THREAD_ITEM];
int inum_thread = 0;

void StartHandleEngineer()
{
	set_engineer_handle((void*)HandleMainEnginer);
}

void StopHandleEngineer()
{
	set_engineer_handle(NULL);
}

void HandleMainEnginer(unsigned long key)
{
	FindThreadAndPause(key);
	switch (key) {
		case Key_ENTER:
			printf("ENG >> ENTER\n");
			break;
		case Key_LEFT:
			printf("ENG >> Left \n");
			break;
		case Key_RIGHT:
			printf("ENG >> Right \n");
			break;
		case Key_UP:
			printf("ENG >> Up\n");
			break;
		case Key_DOWN:
			printf("ENG >> Down\n");
			break;
	}

}

void RegisterKey(SParam sparam)
{
	if (inum_thread < MAX_THREAD_ITEM){
		for (int i = 0; i < inum_thread; i ++){
			if (a_sparam[i].idThread == sparam.idThread){
				printf("This thread is already registry key\n");
				return;
			}
		}
		a_sparam[inum_thread] = sparam;
		inum_thread += 1;
	} else {
		printf("Can not register Key, out of mem\n");
	}
}

void UnregisterKey(eThreadId threadId)
{
	if (inum_thread == 0 || threadId < 0 || threadId > inum_thread){
		printf("Error: Can not UnregisterKey for thread %d\n", threadId);
		return;
	}
	if (inum_thread == 1){
		memset(&a_sparam[0], 0, sizeof(a_sparam[0]));
		inum_thread = 0;
		printf("UnregisterKey >> Remove param: 0, inum_thread = %d\n", inum_thread);
	} else {
		int isFound = 0;
		int index = 0;
		for (;index < inum_thread; index ++){
			if (a_sparam[index].idThread == threadId){
				isFound = 1;
				break;
			}
		}
		if (isFound){
			if (index == (inum_thread - 1)){
				memset(&a_sparam[index], 0, sizeof(a_sparam[0]));
				inum_thread -= 1;
				printf("UnregisterKey >> Remove param: %d(last item), inum_thread = %d\n", index, inum_thread);
			} else	if (inum_thread > 1){
				a_sparam[index] = a_sparam[inum_thread - 1];
				memset(&a_sparam[inum_thread - 1], 0, sizeof(a_sparam[0]));
				inum_thread -= 1;
				printf("UnregisterKey >> Remove param: %d, inum_thread = %d\n", index, inum_thread);
			}
		}
	}
}

void FindThreadAndPause(unsigned long key)
{
	for (int i = 0; i < inum_thread; i++){
		if (a_sparam[i].isPaused == 1){
			printf("Thread %d is Paused, do not check\n", a_sparam[i].idThread);
			continue;
		}
		for (int k = 0; k < a_sparam[i].lenthkey; k++){
			if (a_sparam[i].listKey[k] == key){
				printf("Control >> Press key in list key was registed -> will pause thread\n");
				*a_sparam[i].pointerVar = 1;
				a_sparam[i].isPaused = 1;
				break;
			}
		}		
	}
}

void SetRerunThread(eThreadId threadId)
{
	for (int i = 0; i < inum_thread; i++){
		if (a_sparam[i].idThread == threadId){
			if (a_sparam[i].pCond != NULL){
				printf("SetRerunThread for thread: %d\n", i);
				a_sparam[i].isPaused = 0;
				pthread_cond_signal((pthread_cond_t*)a_sparam[i].pCond);
				break;
			}
		}
	}
}