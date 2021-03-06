/* client.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iomsg.h>

#include "my_hdr.h"

int main( int argc, char **argv)
{
	int chid, coid, srv_coid, rcvid;
	struct my_msg msg;
	struct _pulse pulse;

	/* we need a channel to receive the pulse notification on */
	chid = ChannelCreate( 0 );

	/* and we need a connection to that channel for the pulse to be
     delivered on */
	coid = ConnectAttach( 0, 0, chid, _NTO_SIDE_CHANNEL, 0 );

	/* fill in the event structure for a pulse */
	SIGEV_PULSE_INIT( &msg.event, coid, SIGEV_PULSE_PRIO_INHERIT,
			MY_PULSE_CODE, 0 );
//	msg.type = MSG_GIVE_PULSE;
	msg.type = MSG_GAVE;
	msg.temp = 28;
	/* find the server */
	if ( (srv_coid = name_open( MY_SERV, 0 )) == -1)
	{
		printf("failed to find server, errno %d\n", errno );
		exit(1);
	}

	/* give the pulse event we initialized above to the server for
     later delivery */
	MsgSend( srv_coid, &msg, sizeof(msg), NULL, 0 );
    printf("client::end\n");

	//为什么发的是event，收到的是pulse呢？
	/* wait for the pulse from the server *///相当于是一个确认？
	rcvid = MsgReceivePulse( chid, &pulse, sizeof( pulse ), NULL );
	printf("got pulse with code %d, waiting for %d\n", pulse.code,
			MY_PULSE_CODE );
	printf("got pulse int is %d\n",pulse.value.sival_int);
//	printf("fu end\n");

	return 0;
}
