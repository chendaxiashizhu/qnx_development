/*  M U T E X   U S A G E  */

/* Project Name: "example_mutex" */

/* What is a mutex?
 * A mutex (short for MUTual EXclusion) is a means of
 * synchronisation for accessing any kind of resource that
 * requires exclusive access. This can be a set of hardware
 * registers, an area of memory shared between threads
 * (or processes) or some variables.
 * 11-uuio
 * When do you need exclusive access?
 * Exclusive access means that only one part of a program
 * may access the resource. If you know that two programs
 * or threads accessing the resource at the same time could
 * lead to a "mess", then it's time for a mutex.
 *
 * Usage of mutexes
 * This example shows the usage of a mutex between threads.
 * A mutex can also be used between processes, see the
 * "example_mutex_process" project for this.
 *
 * The mutex must be "aquired" by locking it. Every time
 * you want to write to or read from the critical resource,
 * lock the mutex. It's like a key to a bathroom: A person
 * enters it, locks the door, uses any resources in the
 * bathroom, unlocks the door and leaves. During the time the
 * door is locked, another person who wants to enter the
 * bathroom will have to wait in front of the door and
 * can only enter when the door is unlocked.
 *
 * Careful: The mutex doesn't "know" what you are using it
 * for, it doesn't protect anything automatically. You, the
 * programmer, must lock/unlock it before/after access to what
 * you want to protect. If you forget, it's like you break
 * through the wall of the bathroom, ignoring the locked door.
 * ;-)

 * More information:
 * Read the paragraph "Mutual exclusion locks" in the System
 * Architecture Guide, "Synchronization services" chapter.
 * You will find it in the Help Viewer.
 */
//实际上是一种数据保护的行为，以免两个线程在同一时刻访问同一段内存
//需要注意的是,在一个线程锁住一段数据时,此时另外一个线程若是要访问这段数据必须要进行等待.
//此时后者相当于阻塞状态(block)


//V2: 这里尝试优先级高的线程会不会直接抢占到CPU的mutex操作，发现系统还是在这里等了半天，并不太合理
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <process.h>

void bushchanger_nomutex();
void kerrychanger_nomutex();
void bushchanger_mutex();
void kerrychanger_mutex();
void my_mutex();

/* We use two strings that belong together as an example
 * for a resource to be protected.
 * We first execute some routines that modify them without
 * making sure they have exclusive access (=without a mutex)
 * and print the result some times.
 * Then we use better routines which do use a mutex to gain
 * excluse access to the resource.
*/

char *firstname;
char *lastname;
static int x1=0,x2=0;

/* The mutex is of the type "pthread_mutex_t". */
pthread_mutex_t presidentmutex;//只针对一个？而且为什么要用h文件里对于参量的定义？

int main(int argc, char *argv[]) {
//	printf("pidin id is %d\n",getpid());
	int i;
	pthread_t t1,t2;

	firstname = malloc(32);//就这样分配字符串就可以了
	lastname = malloc(32);

	printf("The president mixer\n");
	printf("Demonstrating the usage of mutexes\n");

/* We're now starting two threads which alter the firstname
 * and lastname strings without synchronizing each other, i.e.
 * without using a mutex. The main thread will print out
 * the two strings together. */

//	printf("\nStarting threads WITHOUT mutex usage!!\n");
//	//这两个线程是如何向下的，还是问题
//	pthread_create(&t1, NULL,(void*) &bushchanger_nomutex, NULL);
//	pthread_create(&t2, NULL,(void*) &kerrychanger_nomutex, NULL);
//
///* Because of no mutex being used in our two threads,
// * "firstname" and "lastname" will sometimes not fit together.
// * We see it now when the main threads prints them: */
//
//	//就运行这么久
//	for (i=0; i<40; i++) {
//		delay(50);		//是delay不是sleep
//		printf("%s %s for president!\n",firstname,lastname);
//	}
//
////  这里必须要把线程都结束才可以吗？
////	这里结束了为什么就没反应了？
//	pthread_detach(t1); // don't care about return
//	pthread_detach(t2); // values of threads 1 & 2
//	pthread_abort(t1);	// kill thread 1
//	pthread_abort(t2);	// kill thread 2
//	pthread_join(t1, NULL);
//	pthread_join(t2, NULL);
//	printf("\nNow invoking functions WITH mutex usage!!\n");

/* A mutex has to be initialized once before first usage. Once
 * initialized, you can use it over and over. */
	pthread_mutex_init(&presidentmutex, NULL);//这里说可以重复使用

/* Now we're creating two new threads, this time with
 * different functions. They also alter the strings, but
 * they make sure that the other thread doesn't get into the
 * way by locking a mutex before they change, and unlocking
 * only after they have changed both of them.*/
	pthread_attr_t attr1,attr2 ,attr3;
	printf("pidin id is %d\n",getpid());
	//调整优先级之后发现并没有作用，这是为什么？
	pthread_attr_init(&attr1);
	pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr1, SCHED_FIFO);
	attr1.__param.__sched_priority = 30;


	pthread_attr_init(&attr2);
	pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
	attr2.__param.__sched_priority = 16;

	// 第三个线程
	pthread_t t3;
	//调整优先级之后发现并没有作用，这是为什么？
	pthread_attr_init(&attr3);
	pthread_attr_setdetachstate(&attr3, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr3, SCHED_FIFO);
	attr3.__param.__sched_priority = 28;



	//开启线程
	pthread_create(&t2, &attr2, (void*) &kerrychanger_mutex, NULL);
	sleep(1);
	pthread_create(&t1, &attr1,(void*) &bushchanger_mutex, NULL);

//	pthread_create(&t3, &attr3,(void*) &my_mutex, NULL);

/* We are printing the "firstname" and "lastname" strings
 * again now. We also must lock the mutex before accessing, to
 * make sure we do not read the strings in the middle of being
 * changed. */

	for (i=0; i<40; i++) {
		delay(500);
		pthread_mutex_lock(&presidentmutex);//存在一个这样的参数是为了统一同步的指令？
//		printf("%s %s for president!\n",firstname,lastname);
//		printf("presidentmutex is %d\n",presidentmutex.__count);
		pthread_mutex_unlock(&presidentmutex);
	}

	return EXIT_SUCCESS;
}

/* The delays in the following routine are only there for
 * demonstration purposes - to make the problem much more
 * visible. If you remove the delays, it may happen you won't
 * SEE the problem, yet it is still there because it's
 * a race condition. */

void bushchanger_nomutex() {
	while (1) {
		strcpy(firstname, "George");
		delay(55);//为什么是55？这样不会太大了吗？延长10
		strcpy(lastname, "Bush");
		delay(55);
		x1++;
//		printf("x1  = %d\n",x1);
	}
}

void kerrychanger_nomutex() {
	while (1) {
		strcpy(firstname, "John");
		delay(105);
		strcpy(lastname, "Kerry");
		delay(105);
		x2++;
//		printf("x2  = %d\n",x2);
	}
}

/* Now the new, improved change routines:
 * We are locking the mutex before we modify a part of
 * our critical resource. We unlock it after we finished with
 * the second part. This is because in this example, the
 * two strings belong together. In a different case, it could
 * also be imagined that each one represents a single resource
 * and would be locked seperately.
 * The general rule is: lock the mutex as short as possible!
 * The delays here are only in for demonstration purposes!!
 */

void bushchanger_mutex() {
//	while (1) {
		// lock the mutex, ensure we have exclusive access.
		// our critical resources to protect are "firstname"
		// and "lastname".
		pthread_mutex_lock(&presidentmutex);
		// Now no one else can access the resources, assuming
		// everyone who wants to also tries to lock the mutex
		// first.
		strcpy(firstname, "George");//给字符串赋值
		sleep(1);
		strcpy(lastname, "Bush");
		printf("bushchanger_mutex\n");
		pthread_mutex_unlock(&presidentmutex);
//	}
}

void kerrychanger_mutex() {
//	while (1) {
		// We lock the mutex to get exclusive access. If the
		// other thread has already locked it, this function
		// will not return unless the mutex becomes unlocked!
		pthread_mutex_lock(&presidentmutex);
		strcpy(firstname, "John");
//		delay(5000); // delay to make the problem more visible
		sleep(5);
		strcpy(lastname, "Kerry");
		printf("kerrychanger_mutex\n");
		pthread_mutex_unlock(&presidentmutex);
//		delay(1000);
//	}
}

void my_mutex() {
//	while (1) {
		// We lock the mutex to get exclusive access. If the
		// other thread has already locked it, this function
		// will not return unless the mutex becomes unlocked!
		pthread_mutex_lock(&presidentmutex);
		strcpy(firstname, "fuck");
		sleep(1);
		strcpy(lastname, "you");
		printf("my_mutex\n");
		pthread_mutex_unlock(&presidentmutex);
		sleep(1);
//	}
}



