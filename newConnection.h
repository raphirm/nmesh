/* Mesh Switch Implementation for Concurrent Programming in C
 * newConnection.c
 * Copyright (C) rm 2013 <raphael@marques.com>
	 * 

 */
#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <pthread.h>
void openNewNode(struct route *routes,llist_t *nodes, struct packetList *pkgs, int connection_fd, short node_role, int port);
void  *startthread(void *argument);
#endif
