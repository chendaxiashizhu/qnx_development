#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>
#include <string.h>
#include <process.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

#define Mq_Attach_POINT "Mq_AttachPoint_Entry_my"

void* Write_function(void* notuse);
void* mq_Read_function(void* notuse);


int main(int argc, char *argv[]) {
	printf("Mq Write process is ready, pid = %d\n", getpid());

	pthread_attr_t attr;
	int result, Thread_ID;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	attr.__param.__sched_priority = 21;
	pthread_create(&Thread_ID, &attr, Write_function, NULL);

	pthread_join(Thread_ID, NULL);
	return EXIT_SUCCESS;

}

void* Write_function(void* notuse)
{
	int fd, num = 0, ffd = 0;
	char send_data[100]= "this is send data-";
	char num_str[10];

//	mq_unlink(Mq_Attach_POINT);//����ɶ��
	fd = mq_open(Mq_Attach_POINT, O_RDWR | O_CREAT, S_IWRITE,  NULL);
	if(fd == -1)
	{
		printf("mq open failure\n");
		return EXIT_FAILURE;
	}

	while(1)
	{
		itoa(num, num_str, 10);
		strcat(send_data, num_str);
		ffd = mq_send(fd, send_data, strlen(send_data) + 1, 20);
		printf("write process send the string	 is %s\n", send_data);
		num += 1;
		sleep(3);
	}

}







