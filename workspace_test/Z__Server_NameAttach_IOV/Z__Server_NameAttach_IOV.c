#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <process.h>



#define ATTACH_POINT "Server_AttachPoint_IOV"
//这里的逻辑应该是用名字取取代进程的编号,顺利找到需要通信的两个进程


void Server_function(void);
name_attach_t* attach;

int main(int argc, char *argv[]) {
	printf("Server process is ready, pid = %d\n", getpid());

	pthread_attr_t attr;
	int result, Thread_ID;

	 if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL)
	 {
		 printf("name_attach failure");
		 return EXIT_FAILURE;
	 }

	 printf("The chid = %d\n", attach->chid);

	 result = spawnl(P_NOWAIT, "/tmp/Z__Client_NameOpen", "Z__Client_NameOpen", NULL);

	 if (result == -1)
	 {
		perror("Error from spawnl");
		exit(1);
	 }

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	attr.__param.__sched_priority = 21;
	pthread_create(&Thread_ID, &attr, &Server_function, NULL);

	pthread_join(Thread_ID, NULL);
	return EXIT_SUCCESS;
}

void Server_function(void)
{
	int rcvid;
	char ServerRecmsg[300];
	 while(1)
	 {
		 rcvid = MsgReceive(attach->chid, ServerRecmsg, sizeof(ServerRecmsg), NULL);

		 printf("Server get a message: %s\n", ServerRecmsg);

		 strcpy(ServerRecmsg, "This is the reply");

		 MsgReply(rcvid, 100, ServerRecmsg, sizeof(ServerRecmsg));
	 }

}
