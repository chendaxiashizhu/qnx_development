#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

void* Client_function(void* notuse);

int main(int argc, char *argv[]) {
	printf("Sem client process is ready, pid = %d\n", getpid());//为什么要分开没说清楚

	pthread_attr_t attr;
	int result, Thread_ID;

//	result = spawnl(P_NOWAIT, "/tmp/Z__Shm_Reader", "Z__Client_NameOpen", NULL);
//	if (result == -1)
//	{
//		perror("Error from spawnl");
//		exit(1);
//	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	attr.__param.__sched_priority = 22;
	pthread_create(&Thread_ID, &attr, Client_function, NULL);

	pthread_join(Thread_ID, NULL);

	return EXIT_SUCCESS;
}

void* Client_function(void* notuse)
{
	sem_t *sem;
	sem = sem_open("global_Sem", O_EXCL);
	while(1)
	{
		sem_wait(sem);
		printf("get a sem\n");
	}
}
