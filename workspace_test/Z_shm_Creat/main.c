///*
// * main.c
// *
// *  Created on: 2020-7-22
// *      Author: ronaldo
// */
//
//
//
//
//#include<fcntl.h>
//
//
//#include "shmem.h"
//
////char *progname = "shmemcreator";//����֮��İ�
//int main(){
//	char *text = "��ð�������\n";
//	int fd;
//	shmem_t *ptr;
//
//	pthread_mutexattr_t myattr;//mutex����Ҫ����
//	fd = shm_open("/temp", O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
//	if(fd == -1){
//		printf("error");
//	}
//	ftruncate(fd, sizeof(shmem_t));
//	ptr = mmap(0, sizeof(shmem_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
//
//
//}