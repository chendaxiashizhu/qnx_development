struct my_msg
{
   short type;//为什么要加一个类型？
   struct sigevent event;
   int temp;
};

#define MY_PULSE_CODE _PULSE_CODE_MINAVAIL+5
#define MSG_GIVE_PULSE _IO_MAX+4
#define MSG_GAVE 3
#define MY_SERV "my_server_name"
