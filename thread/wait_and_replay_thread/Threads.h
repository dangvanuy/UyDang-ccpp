#ifndef __THREAD_H__
#define __THREAD_H__

typedef enum
{
	THREAD_GET_EPG = 0,
	THREAD_NUM
}eThreadId;

void *thread_GetEpg(void *arg);
void StartGetEpg();

void RegisterKeyEpg();
void UnregisterKeyEpg();
void ReRunGetEpg();

#endif //__THREAD_H__