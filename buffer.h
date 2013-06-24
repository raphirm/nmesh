/* Mesh Switch Implementation for Concurrent Programming in C
 * main.c
 * Copyright (C) rm 2013 <raphael@marques.com>
	 * 

 */

#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <pthread.h>
//This is the struct for the buffer for every sender thread.
struct bufmsg
{
	struct paket pkg[MAX_BUF_SIZE];
	int rd;
	int wr;
	pthread_mutex_t mutex;
	pthread_cond_t buff_empty;
};


void buf_init(struct bufmsg *buffer);
void buf_push(struct paket packet, struct bufmsg *buffer);
struct paket *buf_pull(struct bufmsg *buffer);
void buf_list(struct bufmsg *buffer);
#endif 
