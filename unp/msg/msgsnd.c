/*********************************************************************
* Copyright (C) 2014 CFETS Financial Data Co.,LTD                    *
* All right reserved.                                                *
* Name: msgsnd.c
* Author: Canux canuxcheng@gmail.com                                 *
* Version: V1.0                                                      *
* Time: Tue 11 Nov 2014 08:20:12 PM CST
* Description:                                                       *
*********************************************************************/
#include "all.h"

int main(int argc, char **argv)
{
	int mqid;
	size_t len;
	long type;
	struct msgbuf *ptr;

	if (argc != 4)
		err_quit("Usage: msgsnd <pathname> <#bytes> <type>");
	len = atoi(argv[2]);
	type = atoi(argv[3]);

	mqid = Msgget(Ftok(argv[1], 0), MSG_W);

	ptr = (struct msgbuf *)Calloc(sizeof(long) + len, sizeof(char));
	ptr->mtype = type;

	Msgsnd(mqid, ptr, len, 0);

	exit(0);
}
