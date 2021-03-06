
/*
 *  sendrec.c
 *
 *  This process demonstrates Neutrino messaging by creating a
 *  client thread and a server thread all in the same process.
 *
 *  Why would you do this?  Consider the case where you have a
 *  process that has to spend most of its time blocked on MsgReceive()
 *  ready to handle time critical messages.   However, it may also
 *  want to get data from a serial port.  The standard way to get
 *  data from a serial port is to do a read().  So how can it block
 *  on MsgReceive() and do read()s as well?  It could create a
 *  worker thread who will block on read() for it.  When serial
 *  data arrives, the worker thread would send it to the receiving
 *  thread.
 *
 *  To test it, run it as follows:
 *    sendrec
 *    sendrec [name_of_device_to_read_from]
 *
 *  For example:
 *    sendrec /dev/con1
 *
 */
#include <confname.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

void *worker(void *);
int calc_crc(char *text);

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

int     chid;           /* the channel id that the server will receive on */
char    *device;        /* the device that the worker thread will read from */
char    *progname = "sendrec";

int
main(int argc, char **argv) {
	message_t   msg;
	reply_t     reply;
	int         rcvid;

	setvbuf (stdout, NULL, _IOLBF, 0);

	switch(argc) {
		case 1:
			device = NULL;
			break;
		case 2:
			device = argv[1];
			break;
		default:
			printf("use: %s [name_of_device_to_read_from]\n"
					"example: %s /dev/ttyp2\n", progname, progname);
			exit(EXIT_FAILURE);
			break;
	}
	printf("fuck you\n");
	/*
	 * we create the channel before creating the worker thread since
	 * one of the first things the worker thread will do is attach to it
	 */

	chid = ChannelCreate(0);
	if (chid == -1) {
		printf("%s: (server) couldn't create a channel: %s\n",
				progname, strerror(errno));
		exit(EXIT_FAILURE);
	}

	pthread_create(NULL, NULL, worker, NULL);

	while (1) {
		/*
		 * class: Fill this loop with the code for receiving
		 * a message and replying to it.  Use the channel that was created
		 * above.  Put the message in 'msg'.
		 */
		rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
//		rid = MsgRead(rcvid, &msg,sizeof(msg),NULL);
		if (TEXT_MSG_TYPE ==  msg.type) {
			printf("%s: (server) msg.text = '%s'\n", progname, msg.text);
			printf("baba is %d\n",msg.baba);//这里的baba int也收到了。
//			sleep(4);
			/* class: Don't forget to reply.  First, fill in the reply message.
			 * Do this by calling calc_crc(msg.text) and putting what it
			 * returns into reply.crc.  Second, reply by using the variable
			 * 'reply' for the reply buffer.
			 */
			reply.crc = calc_crc(msg.text);
			reply.crc = 10;
			sleep(1);//这里sleep会导致client send block
//			MsgReply(rcvid, 0, &reply, sizeof(reply));
			MsgReply(rcvid, 0, &reply, sizeof(reply));


		} else {
			printf("%s: (server) got unknown msg type %d\n", progname, msg.type);
			MsgError(rcvid, ENOSYS);
		}
		printf("end\n");
		sleep(2);
	}
	return 0;
}

void *
worker(void *i) {
	message_t   msg;
	reply_t     reply;
	int         coid, fd, nbytes, ret;
	coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);//一个channel中可以有多个连接attach

	if (device) {
		fd = open(device, O_RDONLY);
		if (fd == -1) {
			printf("%s: (worker) open(%s) failed: %s\n",
					progname, device, strerror(errno));
			exit(EXIT_FAILURE);   /* exit() terminates the process */
		}
	} else {
		fd = 0;
	}

	while (1) {
//		printf("%s: (worker) reading from device %s\n", progname,
//				(device ? device : "stdin"));
//		int nbytes = read(fd, msg.text, MAX_TEXT_LEN - 1);
////		if ((nbytes = read(fd, msg.text, MAX_TEXT_LEN - 1)) == -1) {
////			printf("%s: (worker) read() failed: %s\n", progname, strerror(errno));
////			close(fd);
////			exit(EXIT_FAILURE);   /* exit() terminates the process */
////		}
////		printf("%d\n",nbytes);
////		printf("hello\n");

		strcpy(msg.text , "fuck");nbytes = 5;//这没用
		msg.baba = 1000;
		msg.type = TEXT_MSG_TYPE;
		msg.text[nbytes] = '\0'; /* null terminate it */

		printf("%s: (worker) sending text '%s'\n", progname, msg.text);
		if ('b' == msg.text[0]) {
			msg.type++;
		}
		/*
		 * class: Send a message to the MsgReceive() in main().
		 * A connection has already been created for you above.
		 * The message is already in 'msg'.  Use the variable,
		 * 'reply', for the reply buffer.
		 */
		ret = MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));
		printf("hello\n");
		if (ret != -1) {
			printf("%s: (worker) got reply, crc = %d\n", progname, reply.crc);
		} else {
			printf("MsgSend failed, errno is %d, '%s'\n", errno, strerror(errno));
		}
	}
	return(NULL);
}



int
calc_crc(char *text) {
	char *c;
	int crc = 0;

	for (c = text; *c != NULL; c++) {
		crc += *c;
	}
	return crc;
}
