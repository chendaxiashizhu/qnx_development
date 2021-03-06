
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

/*信号灯的使用方法很巧妙，直接点说就是在过桥时，桥上游两个信号灯，表示只能是两个连队通过
问题是线程的运行逻辑还是没有特别清楚
为什么这里的函数里，存在for循环，但是实际上并没有完成for循环？
这个示例貌似并没有解释清楚
*/

int onBridge=0;
int saveNum = 3;
sem_t sem;

void* semFunction1(void* arg) {
	int i=0;
	for (i=0;i<20;i++)
	{
		printf("waiting a sem \n");
		sem_wait(&sem);
		printf("one soldier walking through the bridge\n");
		onBridge++;
		sleep(1);
		printf("one soldier left the bridge\n");
		onBridge--;
		printf("posting a sem \n");
		sem_post(&sem);
	}
	return 0;
}
int main(int argc, char *argv[]) {

	printf("Starting Semaphore Test\n");
	sem_init( &sem, 0, saveNum);
	pthread_create( NULL, NULL, &semFunction1, NULL );
	pthread_create( NULL, NULL, &semFunction1, NULL );
	pthread_create( NULL, NULL, &semFunction1, NULL );
	int i=0;
	while(1)
	{
		i++;
		if (onBridge>saveNum)
		{
			printf("%d people on the bridge,the bridge is falling down.\n",onBridge);
		}
		else
		{
			printf("%d people on the bridge,it is save.\n",onBridge);
		}
		delay(200);
		if(i==10)
			break;
	}
	printf("fuck you\n");
	return 0;
	return EXIT_SUCCESS;
}
