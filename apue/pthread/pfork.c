/*********************************************************************
* Copyright (C) 2014 CFETS Financial Data Co.,LTD                    *
* All right reserved.                                                *
* Name: pfork.c
* Author: Canux canuxcheng@gmail.com                                 *
* Version: V1.0                                                      *
* Time: Tue 13 Jan 2015 12:43:49 PM CST
* Description:                                                       *
*********************************************************************/
#include "all.h"

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void prepare(void)
{
	printf("preparing locks...\n");
	pthread_mutex_lock(&lock1);
	pthread_mutex_lock(&lock2);
}

void parent(void)
{
	printf("parent unlocking locks...\n");
	pthread_mutex_unlock(&lock1);
	pthread_mutex_unlock(&lock2);
}

void child(void)
{
	printf("child unlocking locks...\n");
	pthread_mutex_unlock(&lock1);
	pthread_mutex_unlock(&lock2);
}

void *thr_fn(void *arg)
{
	printf("thread started...\n");
	pause();
	return(0);
}

int main(int argc, char **argv)
{
	int err;
	pid_t pid;
	pthread_t tid;

	if ((err = pthread_atfork(prepare, parent, child)) != 0)
		err_exit(err, "cat not install fork handlers");
	if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0)
		err_exit(err, "cat not create thread");
	sleep(2);
	printf("parent about to fork...\n");
	if ((pid = fork()) < 0)
		err_quit("fork error");
	else if (pid == 0)
		printf("child returned from fork\n");
	else
		printf("parent returned from fork\n");
	exit(0);
}
