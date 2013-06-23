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
#include "messagelist.h"
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


int main(int argc, char *argv[])
{
	struct route *routes;
	routes = malloc(sizeof(struct route));
	routes->zielt = NULL;
	routes->quellet = NULL;
	llist_t *nodes;
	nodes = malloc(sizeof(llist_t));
	llist_init(nodes);
	
	struct packetList *pkgs;
	pkgs = msg_init(&pkgs);
	
	int sock_fd, connection_fd;
	pthread_t pthread;
	extern int optind;                // from unistd.h:getopt

	parse_options( argc, argv );
	while (1)
	{
		if( (sock_fd = listen_on_port(tcp_port) ) < 0 ) {
			report_error("failed to listen on the port");
			return sock_fd;
		}

		if( (connection_fd = connect_with_client( sock_fd )) != 0) {
			openNewNode(routes, nodes, pkgs, connection_fd, node_role);

		
			//start_thread
			//parse(connection_fd, noderoot, pkgroot, routes, node_role);
		}
		else {
			report_error("failed to get a client connection");
		}
		disconnect_client( sock_fd);
	}
	return 0; 
}


