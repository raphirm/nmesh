/* Mesh Switch Implementation for Concurrent Programming in C
 * main.c
 * Copyright (C) rm 2013 <raphael@marques.com>
	 * 

 */
//Include everything important
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
#include "newConnection.h"
#include "pkglisthandler.h"




//Global Variable, defines the role of Node. Possible is HOP, ZIEL, QUELLE
short node_role = HOP;

int   tcp_port  = NO_TCP_PORT;
void report_error( char* message ) {
	fprintf( stderr, "ERROR: %s\n", message );
}

//Parse options (-z for ZIEL, -q for QUELLE and port number)
void parse_options( int argc, char *argv[])
{
	char optchar;

	while( ( optchar = getopt( argc, argv, "-hqz" ) ) != -1 ) {

		switch( optchar ) {
			case 'z':
				node_role = ZIEL;
				break;;
			case 'q':
				node_role = QUELLE;
				break;;
				case 1: // optchar '-' will assign non-option to 1
					tcp_port = atoi(optarg);
				break;

			case 'h':
			case '?':
			default:
				if (tcp_port == NO_TCP_PORT ) {
					printf("Usage: server port [-z|-q]\n");
					exit( 0 );
				}
		}
	}

	if (tcp_port == NO_TCP_PORT ) {
		report_error("no port provided");
		exit( -1);
	}
}

//Implement the timeout
void *timeoutcleanup(void *argument){
	
	struct packetList *pkgs;
	pkgs = (struct packetList *) argument;
	while(1){
		//this is the actual time
		long acttime;
		time(&acttime);
		//everything older than this, is too old
		long timeouttime = acttime - TIMEOUT;
	//	printf("%ld is actual, timeout is %ld\n", acttime, timeouttime);
		int i;
		//look into all elements of the list
		for(i = 0; i < sizeof(pkgs->itm) / sizeof(struct pkgListItem); i++)
        {		
               if((pkgs->itm[i].msgtime < timeouttime) && (pkgs->itm[i].msgtime > 0)){
				//   too old! delete!
				   pkgs->itm[i].pid = -1;
				   pkgs->itm[i].msgtime = -1;	
			   }
        }
		usleep(100000*TIMEOUT); // look 10 times of timeout time if we have to clean up something
		
	}
	fflush(stdout);
	
}

//Start the program
int main(int argc, char *argv[])
{
	//Initialize all three important Lists: routes (Route to Ziel and Quelle), nodes (List of all Peers), pkgs (Package index)
	struct route *routes;
	routes = malloc(sizeof(struct route));
	routes->zielt = NULL;
	routes->quellet = NULL;
	llist_t *nodes;
	nodes = malloc(sizeof(llist_t));
	llist_init(nodes);
	struct packetList *pkgs;
	pkgs = msg_init(&pkgs);
	//Initialize the Serversocket (sock_fd) and Client Socket (connection_fd) which spawns when something connects.
	int sock_fd, connection_fd;
	extern int optind;                // from unistd.h:getopt
	pthread_t timecleanup;
	//This little cleanup witch ensures that the same id is again accepted after TIMEOUT (which is default 2 seconds)
	pthread_create(&timecleanup, NULL, (void*)&timeoutcleanup ,(void *) pkgs);
	pthread_detach(timecleanup);
	parse_options( argc, argv );
	while (1)
	{	
		//Listen to Port (Busy Wait on Socket)
		if( (sock_fd = listen_on_port(tcp_port) ) < 0 ) {
			report_error("failed to listen on the port");
			return sock_fd;
		}
		//Open Connection Back to client
		if( (connection_fd = connect_with_client( sock_fd )) != 0) {
			//Setup everything for new connection (That means, spawn all threads that handle the connection accordingly)
			openNewNode(routes, nodes, pkgs, connection_fd, node_role, tcp_port);

		
			
		}
		else {
			//OOps no connection :(
			report_error("failed to get a client connection");
		}
		//Free the block on the Listener Port.
		disconnect_client( sock_fd);
	}
	return 0; 
}


