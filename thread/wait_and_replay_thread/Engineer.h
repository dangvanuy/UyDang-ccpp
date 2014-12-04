#ifndef __ENGINEER_H__
#define __ENGINEER_H__

#define MAX_KEY 10

typedef struct sparam
{
	eThreadId 		idThread; //thread ID
	int 			*pointerVar; //point to var control thread
	void			*pCond; //point to pthread_cond_t
	int 			isPaused; //0: thread is running, 1: thread is Pausing
	int 			lenthkey; //lengh of list key listKey
	unsigned long 	listKey[MAX_KEY];
}SParam;

void *get_remote_thread(void *n);
void StartHandleEngineer();
void HandleMainEnginer(unsigned long key);
void StopHandleEngineer();

void RegisterKey(SParam m_sparam);
void UnregisterKey(eThreadId threadId);
void FindThreadAndPause(unsigned long key);
void SetRerunThread(eThreadId threadId);

#endif //__ENGINEER_H__