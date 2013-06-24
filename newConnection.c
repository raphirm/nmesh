/* Mesh Switch Implementation for Concurrent Programming in C
 * newConnection.c
 * Copyright (C) rm 2013 <raphael@marques.com>
	 * 

 */
 
 //Again the very important includes...
#include <stdlib.h>      // exit
#include <stdio.h>
#include <string.h>      // strlen
#include <assert.h>
#include <unistd.h>      // getopt
#include "nodelist.h"
#include "main.h"
#include "conn_server.h"
#include "conn_io.h"     // send_all
#include "thread.h"
#include <pthread.h>
#include <stdbool.h>
#include "buffer.h"

//Because the startmethod of the thread needs to be in the same file, don't know why
//it's a bit wierd...
void  *startthread(void *argument)
{
	startsthread (argument);
}

// Create a thread for each new connection. The thread reads and write to the connection.
void openNewNode(struct route *routes,llist_t *nodes, struct packetList *pkgs, int connection_fd, short node_role, int tcp_port)
{
			//Generate all resources which are important for the thread
			pthread_t thread;
			struct bufmsg *buffer;
			buffer = malloc(sizeof(struct bufmsg));
			buf_init(buffer);
			bool alive = true;
			llist_insert_data(connection_fd, &thread, buffer, nodes, alive);
			//Make a nice Struct to give pass all information.
			struct threadinfos *ti;
			ti = malloc(sizeof(struct threadinfos));
			ti->nodes = nodes;
			ti->packages = pkgs;
			ti->routes = routes;
			ti->node_role = node_role;
			ti->port = tcp_port;
			ti->me = llist_find_data (connection_fd, nodes);
			// Create and detach thread. He will kill himself if the connection finishs.
			pthread_create(&thread, NULL, (void*)&startthread ,(void *) ti);
			pthread_detach(thread);


}
