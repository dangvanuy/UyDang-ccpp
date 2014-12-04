#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>



pthread_cond_t Playing_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t Handle_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qu_mutex = PTHREAD_MUTEX_INITIALIZER;


int ui_gFlagPlaying = 0;

void *thread_playing(void *arg)
{
	int i = 1;
    while(1) {		
		pthread_mutex_lock(&qu_mutex);
		if (ui_gFlagPlaying == 1){
			printf("Thread Playing >> PAUSE AT %d\n", i);
			pthread_cond_wait(&Playing_cond, &qu_mutex);
			ui_gFlagPlaying = 0;
		}
        pthread_mutex_unlock(&qu_mutex);
		
		printf("Thread Playing >> PLAYING: %d\n", i++);
		
		usleep(500000);
    }
}

void *thread_handle(void *arg)
{
	int i = -1;
    while(1) {
		i++; 
		if (i == 0){
			ui_gFlagPlaying = 1;
			//pthread_cond_wait(&Playing_cond, &qu_mutex);
			printf("Thread Handle >> Begin\n");
		} else if ( i < 10){
			printf("Thread Handle >> Doing(%d)\n", i);
		}else if (i == 10){
			printf("Thread Handle >> Done\n");		
			pthread_cond_signal(&Playing_cond);
		} else if (i < 20){
			//printf("Thread Handle >> Do nothing\n");			
		} else {
			i = -1;
		}
		usleep(500000);
    }
}


int main()
{
	pthread_t pthPlaying;
    pthread_create(&pthPlaying, NULL, thread_playing, NULL);	
	
	pthread_t pthHandle;
    pthread_create(&pthHandle, NULL, thread_handle, NULL);
	
	
	pthread_join(pthPlaying, NULL);
	pthread_join(pthHandle, NULL);
  /*
    while(1) {

        //produce(&num);
		num += 1;
        pthread_mutex_lock(&qu_mutex);
        qu.push(num);
        pthread_cond_signal(&qu_empty_cond);
		int m_size = qu.size();
        if (m_size >= 10)
             do {
				printf("Thread Buffer  >> Wait thread, Full MEM: %d/10\n", m_size);
                pthread_cond_wait(&qu_full_cond, &qu_mutex);
             } while (qu.size() >= 5);
        pthread_mutex_unlock(&qu_mutex);
		printf("Thread Buffer  >> PUSH(%d)\n", m_size);
		//usleep(500000);
		sleep(3);
    }
	*/
}
//g++ -o tow_thread_cross_bright tow_thread_cross_bright.cpp -lpthread





/*
bool m_pause; // initialise to false in constructor!
boost::mutex m_pause_mutex;
boost::condition_variable m_pause_changed;

void block_while_paused()
{
    boost::unique_lock<boost::mutex> lock(m_pause_mutex);
    while(m_pause)
    {
        m_pause_changed.wait(lock);
    }
}

void set_paused(bool new_value)
{
    {
        boost::unique_lock<boost::mutex> lock(m_pause_mutex);
        m_pause = new_value;
    }

    m_pause_changed.notify_all();
}
*/
