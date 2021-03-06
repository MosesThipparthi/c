/*********************************************************************
* Copyright (C) 2014 CFETS Financial Data Co.,LTD                    *
* All right reserved.                                                *
* Name: mqnotifysig2.c
* Author: Canux canuxcheng@gmail.com                                 *
* Version: V1.0                                                      *
* Time: Mon 10 Nov 2014 10:32:17 AM CST
* Description:                                                       *
*********************************************************************/
#include "all.h"

volatile sig_atomic_t mqflag; // set nonzero by signal handler

static void sig_usr1(int signo)
{
	mqflag = 1;
	return;
}

int main(int argc, char **argv)
{
	mqd_t mqd;
	void *buff;
	ssize_t n;
	sigset_t zeromask, newmask, oldmask;
	struct mq_attr attr;
	struct sigevent sigev;

	if (argc != 2)
		err_quit("Usage: nfs2 <name>");

	mqd = Mq_open(argv[1], O_RDONLY);
	Mq_getattr(mqd, &attr);
	buff = (void *)Malloc(attr.mq_msgsize);
    
	Sigemptyset(&zeromask);
	Sigemptyset(&newmask);
	Sigemptyset(&oldmask);
	Sigaddset(&newmask, SIGUSR1);

	Signal(SIGUSR1, sig_usr1);
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	Mq_notify(mqd, &sigev);

	for ( ; ; )
	{
		Sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		while (mqflag == 0)
			sigsuspend(&zeromask);
		mqflag = 0;

		Mq_notify(mqd, &sigev);
		n = Mq_receive(mqd, buff, attr.mq_msgsize, NULL);
		printf("read %ld bytes\n", (long)n);
		Sigprocmask(SIG_UNBLOCK, &newmask, NULL);
	}
	exit(0);
}
