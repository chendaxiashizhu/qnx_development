#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <spawn.h>

//这里探索时间超时的定义
//产生no message的时候，说明收取时间超时了

//自己定义
#define MAX_TEXT_LEN    30
#define TEXT_MSG_TYPE 0
typedef struct
{
	uint16_t type;
	char    text[MAX_TEXT_LEN];
	int baba;
} message_t;
typedef struct
{
	int     crc;
} reply_t;
int 			chid;
//自己定义结束

void* Server_function(void* notused);
void * clientWork(void *i);

int main(int argc, char *argv[]){
	printf("Server is ready, pid = %d\n", getpid());

	pthread_attr_t attr;

//	pthread_attr_init(&attr);
//	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
//	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
//	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
//	attr.__param.__sched_priority = 20;
//	pthread_create(NULL, &attr, &Server_function, NULL);
	pthread_create(NULL, NULL, &Server_function, NULL);
	pthread_create(NULL, NULL, &clientWork, NULL);

	while(1){
		sleep(10);
	}

	return EXIT_SUCCESS;
}

void* Server_function(void* notused){
	int 			rcvid;
	char 			message[512];
	message_t   msg;
	uint64_t 		timeout;
	struct sigevent event;
//	chid = ChannelCreate(_NTO_CHF_UNBLOCK);
	chid = ChannelCreate(0);

	//设置等待超时
	//超时的定义与我想象的是一致的
	SIGEV_UNBLOCK_INIT(&event);
	timeout = 3LL * 1000000000LL;

	while(1){
		TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_RECEIVE, &event, &timeout, NULL);

		rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);

		if(rcvid < 0){
			printf("no message\n");
		}
		else if (rcvid == 0){
			printf("Got a pluse, rcvid is %X\n", rcvid);
		}
		else{
			printf("Got a message, rcvid is %X\n", rcvid);
			printf("int is %d\n",msg.baba);
			strcpy(message, "This is the reply");
			MsgReply(rcvid, 100, message, sizeof(message));
		}
	}
}


void * clientWork(void *i) {
	message_t   msg;
	char 			message[512];
	int         coid, nbytes, ret;
	coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);//一个channel中可以有多个连接attach

	while (1) {
		strcpy(msg.text , "fuck");
		nbytes = 5;//这没用
		msg.baba = 1000;
		msg.type = TEXT_MSG_TYPE;
		msg.text[nbytes] = '\0'; /* null terminate it */

//		printf(" (worker) sending text '%s'\n", msg.text);
		if ('b' == msg.text[0]) {
			msg.type++;
		}
		/*
		 * class: Send a message to the MsgReceive() in main().
		 * A connection has already been created for you above.
		 * The message is already in 'msg'.  Use the variable,
		 * 'reply', for the reply buffer.
		 */
		ret = MsgSend(coid, &msg, sizeof(msg), &message, sizeof(message));
		printf("hello\n");
		sleep(5);
	}
	return(NULL);
}






