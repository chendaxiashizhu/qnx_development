struct my_msg
{
   short type;
   struct sigevent event;
   int temp;
};

#define MY_PULSE_CODE _PULSE_CODE_MINAVAIL+5
#define MSG_GIVE_PULSE _IO_MAX+4
#define MSG_GAVE 3		//自己定义的type也是没有问题的
#define MY_SERV "my_server_name"
