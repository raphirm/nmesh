#include <stdlib.h>      // exit
#include <stdio.h>
#include <string.h>      // strlen
#include <assert.h>
#include <unistd.h>      // getopt
#include "nodelist.h"
#include "main.h"
#include "conn_server.h"
#include "conn_io.h"     // send_all
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdbool.h>
#include "buffer.h"
#include <arpa/inet.h>
#include <netinet/in.h>
//thread who is reachable for all others, to send data. It spawns a listener thread to recv data and parses it.

void *listenT(void *argument)
{
	struct threadinfos *ti;
	ti = (struct threadinfos *) argument;
	short node_role = ti->node_role;
	llist_node_t *me = ti->me;
	int socket = me->index;
	char *ca = malloc(2);
	struct bufmsg *buffer = me->datap;
	while(recv(socket, ca, 2, MSG_PEEK)>0){
		struct paket message; /* we expect some line of text shorter than 132 chars */
		while(fgets(&message, 132, read_line(socket)) != NULL ) 
		{
			if(message.paketType == 'N'){

				char ip[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &(message.content[0]), ip, INET_ADDRSTRLEN);
				unsigned short prt = (unsigned short) (message.content[4]<<8)|message.content[5];
				//node_add(noderoot, ip, prt);
				int connection_fd = open_connection_to (ip, prt);
				openNewNode (ti->routes, ti->nodes, ti->packages, connection_fd, ti->node_role);
			}
			
			else if(message.paketType == 'C'){
				
				if(msg_check(&message, ti->packages)==0){
					msg_add(socket, &message, ti->packages);
					printf("Neues Packet empfangen mit ID %i, route vorhanden?\n", ntohs(message.paketID));
					if((message.target == 1 && node_role == ZIEL) || (message.target == 0 && node_role == QUELLE)){
						printf("Am Ziel angekommen! Sende antwort für ID %i und String %s\n", ntohs(message.paketID), message.content);
						message.paketType = 'O';
						buf_push(message, buffer);
					}
					else if((message.target == 1 && ti->routes->zielt != NULL) ||(message.target ==0 && ti->routes->quellet != NULL)){
						printf("Route schon vorhanden\n");
						if(message.target ==1){
							buf_push(message, ti->routes->zielt->datap);	
						}else{
							buf_push(message, ti->routes->quellet->datap);	
						}
					}else{
						 pthread_mutex_lock(&(ti->nodes->mutex));
						llist_node_t *curr = ti->nodes->first;
						while(curr != NULL){
							if(curr->index != socket){
								buf_push(message, curr->datap);
							}
							curr = curr->nextp;
							
						}
						pthread_mutex_unlock(&(ti->nodes->mutex));
						printf("keine Route vorhanden, broadcaste in das Netzwerk!\n");
						
					}

				}else{
					printf("Packet mit id %i schon gesehen, verwerfen!\n",  ntohs(message.paketID));
	

				}

			}else if(message.paketType == 'O'){
				int *sket = msg_getSocket(ti->packages, &message);
				llist_node_t *target = llist_find_data (*sket, ti->nodes);
				if(message.target == 0) {

					ti->routes->quellet = ti->me;
				}else{
					ti->routes->zielt = ti->me;
				}
				printf("Leite Antwort weiter mit  id %i und inhalt %s\n", ntohs(message.paketID), message.content);
				buf_push(message, target->datap);
			}

		}
	}

	pthread_mutex_lock(&buffer->mutex);
	ti->me->alive=false;
	pthread_cond_signal(&buffer->buff_empty);
	pthread_mutex_unlock(&buffer->mutex);
	pthread_exit(NULL);
	close(socket);
}


//thread waits for work (looks to a buffer) and sends the packet over his socket. The buffer is directly reachable over the list "nodes".
void  startsthread(void *argument)
{

	struct threadinfos *ti;
	ti = (struct threadinfos *) argument;
	llist_node_t *me = ti->me;
	int socket = me->index;
	struct bufmsg *buffer = me->datap;
	pthread_t thread;
	struct route *routes = ti->routes;
	llist_t *nodes = ti->nodes;
	struct pkgListItem *packages =	ti->packages;
	int rt= pthread_create(&thread, NULL, (void*)&listenT ,(void *) ti);
	pthread_mutex_lock(&buffer->mutex);
	while(1){

		if(buffer->rd == buffer->wr && me->alive){
			pthread_cond_wait(&buffer->buff_empty, &buffer->mutex);
		}
		if(!me->alive)
		{
			free(buffer);
			me->index = socket;
			llist_remove_data (socket, nodes);
			pthread_mutex_unlock(&buffer->mutex);
			pthread_exit(NULL);
		}
		//mutex lock
		//buffer consumer/worker
		//mutex unlock
		struct paket *message;
		message = buf_pull(buffer);
		send_all(socket, message, sizeof(*message));

	}

	pthread_mutex_unlock(&buffer->mutex);
}

