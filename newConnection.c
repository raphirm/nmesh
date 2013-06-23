#include <stdlib.h>      // exit
#include <stdio.h>
#include <string.h>      // strlen
#include <assert.h>
#include <unistd.h>      // getopt
#include "messagelist.h"
#include "main.h"
#include "conn_server.h"
#include "conn_io.h"     // send_all

#include "thread.h"
#include <pthread.h>
#include <stdbool.h>
#include "buffer.h"

void  *startthread(void *argument)
{
	startsthread (argument);
}
void openNewNode(struct route *routes,llist_t *nodes, struct packetList *pkgs, int connection_fd, short node_role)
{
			pthread_t thread;
			struct bufmsg *buffer;
			buffer = malloc(sizeof(struct bufmsg));
			buf_init(buffer);
			bool alive = true;
			llist_insert_data(connection_fd, &thread, buffer, nodes, alive);
			struct threadinfos *ti;
			ti = malloc(sizeof(struct threadinfos));
			ti->nodes = nodes;
			ti->packages = pkgs;
			ti->routes = routes;
			ti->node_role = node_role;
			ti->me = llist_find_data (connection_fd, nodes);
			pthread_create(&thread, NULL, (void*)&startthread ,(void *) ti);
			pthread_detach(thread);


}