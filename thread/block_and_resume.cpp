#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>

template <class T>
class CQueue
{
	int msize;
	T list[100];	
public:
	CQueue()
	{
		msize = 0; 
	}
	T pop(){
		T tmp = 0;
		if ( msize < 0 ){
			return tmp;
		}
		tmp = list[msize];
		msize --;
		//printf("POP  >> (value, size) = (%d, %d)\n", tmp, msize);
		return tmp;
	}
	
	void push(T value)
	{
		if (msize < 100){			
			msize +=1;
			list[msize] = value;
			//printf("PUSH >> (value, size) = (%d, %d)\n", value, msize);
		}
	}
	
	int size(){
		return msize;
	}
};

pthread_cond_t qu_empty_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t qu_full_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qu_mutex = PTHREAD_MUTEX_INITIALIZER;


CQueue<int> qu;
void *num_consumer(void *arg)
{
    while(1) {
        int num;

        pthread_mutex_lock(&qu_mutex);
		
		int msize1 = qu.size();
        while (msize1 < 1){
			printf("Thread Playing >> Wait for buffering(%d)\n", msize1);
			pthread_cond_wait(&qu_empty_cond, &qu_mutex);
		}
        num = qu.pop();
		int msize = qu.size();
        if (msize < 5){
			printf("Thread Playing >> Resum thread buffer: %d\n", msize);
            pthread_cond_signal(&qu_full_cond);
		}
        pthread_mutex_unlock(&qu_mutex);
		printf("Thread Playing >> POP: %d\n", msize);
		usleep(200000);
		//sleep(1);
    }
}

int main()
{
	pthread_t tid;
    pthread_create(&tid, NULL, num_consumer, NULL);
   int num = 0;
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
}
//g++ -o block_and_resume block_and_resume.cpp -lpthread
