struct my_msg
{
   short type;
   struct sigevent event;
   int temp;
};

#define MY_PULSE_CODE _PULSE_CODE_MINAVAIL+5
#define MSG_GIVE_PULSE _IO_MAX+4
#define MSG_GAVE 3		//�Լ������typeҲ��û�������
#define MY_SERV "my_server_name"