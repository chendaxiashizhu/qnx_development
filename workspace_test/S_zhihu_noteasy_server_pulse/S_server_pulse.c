/* server.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include "my_hdr.h"

int main( int argc, char **argv)
{
	int result = spawnl(P_NOWAIT, "/tmp/S_zhihu_noteasy_client_pulse", "S_zhihu_noteasy_client_pulse", NULL);
	if (result == -1)
	{
		perror("Error from spawnl");
		exit(1);
	}

	int rcvid;
	struct my_msg msg;
	name_attach_t *attach;

	/* attach the name the client will use to find us */
	/* our channel will be in the attach structure */
	if ( (attach = name_attach( NULL, MY_SERV, 0 )) == NULL)
	{
		printf("server:failed to attach name, errno %d\n", errno );
		exit(1);
	}

	/* wait for the message from the client */
	rcvid = MsgReceive( attach->chid, &msg, sizeof( msg ), NULL );
	sleep(2);
	MsgReply(rcvid, 0, NULL, 0);


	//test
	printf("server::temp is %d\n",msg.temp);

	// msg.type 是可以定义的
	if ( msg.type == MSG_GIVE_PULSE )
	{
		/* wait until it is time to notify the client */
		sleep(4);
		/* deliver notification to client that client requested */
		MsgDeliverEvent( rcvid, &msg.event );
		printf("server:delivered event\n");
	} else if(msg.type == MSG_GAVE){
		sleep(3);
		msg.event.sigev_value.sival_int = 125;
		//这个是异步
		MsgDeliverEvent( rcvid, &msg.event );//不给client那边就会receive block
		printf("server::my own\n");
	}
	else
	{
		printf("server: unexpected message \n");
	}
	return 0;
}
