#include <stdio.h>
#include <pthread.h>



//各线程中信号同步的一种方式,条件同步.

int data_ready = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;


void * consumer(void *notused){
	printf("In consumer ...");
	while(1){
		pthread_mutex_lock(&mutex);
		while(!data_ready){
			pthread_cond_wait(&condvar,&mutex);
		}
		printf("consumer:got data from profucter\n");
		data_ready = 0;
		pthread_cond_signal(&condvar);
        pthread_mutex_unlock(&mutex);
	}

}

void *producer(void * notused){
	printf("in the producer...\n");
	while(1){
		sleep(2);
		printf("producer:got data from h/w\n");
		pthread_mutex_lock(&mutex);
		while(data_ready){
			pthread_cond_wait(&condvar,&mutex);
		}
		data_ready = 1;
		pthread_cond_signal(&condvar);
        pthread_mutex_unlock(&mutex);
	}
}


int main(){
	pthread_create(NULL,NULL,consumer,NULL);
	pthread_create(NULL,NULL,producer,NULL);
	sleep(20);


}
