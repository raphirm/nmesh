/* Mesh Switch Implementation for Concurrent Programming in C
 * pkglisthandler.h
 * Copyright (C) rm 2013 <raphael@marques.com>
	 * 

 */
 

#ifndef __PKGLISTHANDLER_H__
#define __PKGLISTHANDLER_H__
#define MESSAGE_SIZE 1024
#include <pthread.h>
#include <stdbool.h>

struct pkgListItem {
	unsigned short pid;
	int sourceSocket;
	long int msgtime;
};
struct packetList{
	struct pkgListItem itm[MESSAGE_SIZE];
	pthread_mutex_t mutex;
};

/*
int calcIdx(struct paket *message);
void msg_init(struct packetList *pkgs);
int *msg_getSocket(struct packetList *pkgs, unsigned short pid);
bool msg_check(int connection_fd, struct paket *message);
void msg_add(int connection_fd, struct paket *message);
*/
#endif
