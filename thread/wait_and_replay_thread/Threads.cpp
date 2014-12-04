#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "remote.h"
#include "Threads.h"
#include "Engineer.h"

pthread_mutex_t epg_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t Playing_cond = PTHREAD_COND_INITIALIZER;
int ui_gIsPauseGetEpg = 0;
void *thread_GetEpg(void *arg)
{
	int i = 1;
    while(1) {		
		pthread_mutex_lock(&epg_mutex);
		if (ui_gIsPauseGetEpg == 1){
			printf("Thread EPG >> PAUSE AT %d\n", i);
			pthread_cond_wait(&Playing_cond, &epg_mutex);
			ui_gIsPauseGetEpg = 0;
		}
		
		printf("Thread EPG >> Process 1/7: 0.2 sec\n");
		usleep(200000);
		printf("Thread EPG >> Process 2/7: 1 sec\n");
		sleep(1);
		printf("Thread EPG >> Process 3/7: 0.5 sec\n");
		usleep(500000);
		printf("Thread EPG >> Process 4/7: 1 sec\n");
		sleep(1);
		printf("Thread EPG >> Process 5/7: 0.5 sec\n");
		usleep(500000);
		printf("Thread EPG >> Process 6/7: 1 sec\n");
		sleep(1);
		printf("Thread EPG >> Process 7/7: 0.5 sec\n");
		usleep(500000);
		
        pthread_mutex_unlock(&epg_mutex);
    }
}

pthread_t pGetEpgID;
void StartGetEpg()
{
    pthread_create(&pGetEpgID, NULL, thread_GetEpg, NULL);
	RegisterKeyEpg();
	
	//pthread_join(pGetEpg, NULL);
}

void RegisterKeyEpg()
{
	SParam param = {THREAD_GET_EPG, &ui_gIsPauseGetEpg, &Playing_cond, 0, 2, {Key_UP, Key_DOWN}}; 
	printf("Register EPG key: for thread: pGetEpgID, with key(Key_UP, Key_DOWN)\n");
	RegisterKey(param);
}

void UnregisterKeyEpg()
{
	UnregisterKey(THREAD_GET_EPG);
}

void ReRunGetEpg()
{
	SetRerunThread(THREAD_GET_EPG);
}


