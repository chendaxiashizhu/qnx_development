#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <time.h>
#include <sys/siginfo.h>

void* Timer_SendPulse(void * nouse);
int chid, coid, Count = 0;

int main(int argc, char *argv[]) {
	printf("Timer_Pulse process is ready, pid = %d\n", getpid());

	pthread_attr_t 		attr;
	timer_t 			timerid;
	struct sigevent 	event;
	struct itimerspec 	timer;

	if((chid = ChannelCreate(0)) == -1){
		printf("Create channel failure\n");
		exit(EXIT_FAILURE);
	}

	if((coid = ConnectAttach(0, 0, chid, 0, 0)) == -1){
		printf("Connect to self failure\n");
		exit(EXIT_FAILURE);
	}

	//�̳߳�ʼ��, Joinable��FIFO���ȡ����ȼ�, �߳�����
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	attr.__param.__sched_priority = 20;

	//��ʼ��Event, code = 1, value = 100
	SIGEV_THREAD_INIT(&event, Timer_SendPulse, NULL, &attr);//Ϊʲô�ܹ�ȥ�������������

//	pthread_create(NULL , &attr , Timer_SendPulse , NULL);

	if(timer_create(CLOCK_REALTIME, &event, &timerid) == -1){
		printf("Create timer failure\n");
		exit(EXIT_FAILURE);
	}

	//���ö�ʱ������
	timer.it_value.tv_sec = 1;//�ӵ�10�뿪ʼ
	timer.it_value.tv_nsec = 20;
	timer.it_interval.tv_sec = 2;//�����2s
	timer.it_interval.tv_nsec = 0;

	//������ʱ��
	timer_settime(timerid, 0, &timer, NULL);
//	while(1)
//	{
//		sleep(100);
//	}
	sleep(20);

	return EXIT_SUCCESS;
}

//Ϊʲô�������Ľ����
void* Timer_SendPulse(void * nouse){
	printf("timer wake up me, Count is %d\n", Count);
	Count += 1;
//	while(1)
//	{
//		sleep(2);
//	}
	return 0;
}