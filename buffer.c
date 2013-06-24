#include <stdlib.h>      // exit
#include <stdio.h>
#include <string.h>      // strlen
#include <assert.h>
#include <unistd.h>      // getopt
#include "nodelist.h"
#include "main.h"
#include "conn_server.h"
#include "conn_io.h"     // send_all
#include "buffer.h"
#include "thread.h"
#include <pthread.h>
#include <stdbool.h>
void buf_init(struct bufmsg *buffer){
	pthread_mutex_init(&(buffer->mutex), NULL);
	pthread_cond_init(&(buffer->buff_empty), NULL);
	buffer->rd = 0;
	buffer->wr = 0;
	int i;
	for(i = 0; i < sizeof(buffer->pkg) / sizeof(struct paket); i++)
	{
		buffer->pkg[i].paketID = -1;
	}
	}

struct paket *buf_pull(struct bufmsg *buffer){
	int rd = buffer->rd;
	buffer->rd = (buffer->rd + 1) % MAX_BUF_SIZE;
	return &buffer->pkg[rd];
	


}

void buf_push(struct paket packet, struct bufmsg *buffer){
	pthread_mutex_lock(&buffer->mutex);
	buffer->pkg[buffer->wr] = packet;
	buffer->wr = (buffer->wr + 1) % MAX_BUF_SIZE;
	pthread_cond_signal(&buffer->buff_empty);
	pthread_mutex_unlock(&buffer->mutex);

}

