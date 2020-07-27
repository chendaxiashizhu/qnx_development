/*
 * pulse_client.c
 *
 * This program, along with pulse_server.c, demonstrate pulses between a
 * server and a client using MsgDeliverEvent().
 *
 * The client finds the server using name_open(), passing it the name the
 * server has registered with name_attach().
 *
 *  To test it, first run pulse_server and then run the client as follows:
 *    pulse_client
 */


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/siginfo.h>//是不是都放在这个里面的？
#include <sys/neutrino.h>
#include <sys/dispatch.h>

#include "pulse_server.h"
#include <sched.h>

#define PROGNAME "pulse_client: "

#define MY_PULSE_CODE (_PULSE_CODE_MINAVAIL + 2)//这个没有区别？

union recv_msg {
	struct _pulse pulse;
	short type;
} recv_buf;

int main(int argc, char *argv[]) {
	int server_coid, self_coid, chid, rcvid;
	struct reg_msg msg;//注册信息

	setvbuf (stdout, NULL, _IOLBF, 0);

	/* look for server 寻找服务器 */
	server_coid = name_open(RECV_NAME, 0);
	while (server_coid == -1) {
		sleep(1);
		server_coid = name_open(RECV_NAME, 0);
	}

	chid = ChannelCreate(0);
	if (-1 == chid) {
		perror(PROGNAME "ChannelCreate");
		exit(EXIT_FAILURE);
	}
	self_coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);
	if (-1 == self_coid) {
		perror(PROGNAME "ConnectAttach");
		exit(EXIT_FAILURE);
	}

	msg.type = REG_MSG;

	/* Initialize the sigevent structure (msg.ev) in the message
	 * to be sent to the server.
	 */
	//msg.ev 这个是消息传递的核心
	SIGEV_PULSE_INIT(&msg.ev, self_coid, getprio(0), MY_PULSE_CODE, 0);

	if (MsgSend(server_coid, &msg, sizeof( msg), NULL, 0 ))
	{
		perror(PROGNAME "MsgSend");//应该类似于输出错误的吧
		exit(EXIT_FAILURE);
	}

	while (1) {
		rcvid = MsgReceive(chid, &recv_buf, sizeof(recv_buf), NULL);

		if (-1 == rcvid) {
			perror(PROGNAME "MsgReceive");
			continue;
		}
		if (0 == rcvid) {//rcvid==0
			if (MY_PULSE_CODE == recv_buf.pulse.code) {
				printf(PROGNAME "got my pulse, value is %d\n", recv_buf.pulse.value.sival_int);
			} else {
				printf(PROGNAME "got unexpected pulse with code %d, value %d\n",
						recv_buf.pulse.code, recv_buf.pulse.value.sival_int);
			}
			continue;
		}
		//printf("RCVID = %d\n" , rcvid);//运行不到这一步
		printf(PROGNAME "got unexpected message, type: %d\n", recv_buf.type);
		MsgError(rcvid, ENOSYS);
	}
}
