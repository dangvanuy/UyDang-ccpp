#ifndef __REMOTE_H__
#define __REMOTE_H__

#ifdef __cplusplus
extern "C"
{
#endif

void *smp_remote_event_thread(void *n);
int driver_open_device(void);
void run_remote(void);
void stop_remote(void);
void set_handle(void *handle);
void set_engineer_handle(void *handle);
#define Key_POWER 0xb54acb04
#define Key_ENTER 0xfd02cb04
#define Key_RIGHT 0xfe01cb04
#define Key_LEFT  0xbc43cb04
#define Key_UP    0xf906cb04
#define Key_DOWN  0xf50acb04
#define Key_ZERO  0xe41bcb04
#define Key_ONE   0xbe41cb04
#define Key_TOW   0xfc03cb04

#ifdef __cplusplus
}
#endif


#endif //__REMOTE_H__

