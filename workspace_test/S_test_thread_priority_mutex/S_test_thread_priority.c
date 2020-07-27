//
//
//#include <pthread.h>
//#include<stdlib.h>
//
//#include <sys/siginfo.h>
//#include <unistd.h>
//
//void func1();
//void func2();
//int main(int argc, char *argv[]) {
//	printf("Client process is ready, pid = %d\n", getpid());
//
//	pthread_attr_t attr1,attr2,attr3;
//	int Thread_ID1,Thread_ID2,Thread_ID3;
//
//	pthread_attr_init(&attr1);
//	pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
//	pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
//	pthread_attr_setschedpolicy(&attr1, SCHED_FIFO);
//	attr1.__param.__sched_priority = 3;
//	pthread_create(&Thread_ID1, &attr1, func1, NULL);
//
//
//	pthread_attr_init(&attr2);
//	pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);
//	pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
//	pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
//	attr2.__param.__sched_priority = 14;
//	pthread_create(&Thread_ID2, &attr2, func2, NULL);
//
//
//	sleep(10);
//	pthread_join(Thread_ID1, NULL);
//	pthread_join(Thread_ID2, NULL);
//	return EXIT_SUCCESS;
//
//}
//
//void func1(){
//	while(1){
//		sleep(1);
//		printf("func 1\n");
//	}
//
//}
//
//void func2(){
//	while(1){
//		sleep(1);
//		printf("func 2\n");
//	}
//
//}
