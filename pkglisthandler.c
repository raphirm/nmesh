#include "pkglisthandler.h"

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





/* berechnet den Index im Array */
struct packetList *msg_init(struct packetList *pkgs){
	pkgs = malloc(sizeof(struct packetList));
	pthread_mutex_init(&(pkgs->mutex), NULL);
	int i;
	for(i = 0; i < sizeof(pkgs->itm) / sizeof(struct pkgListItem); i++)
        {
                pkgs->itm[i].pid = -1;
                pkgs->itm[i].msgtime = -1;
        }
	return pkgs;
}

int calcIdx(struct paket *message){
	int idx= message->paketID % PACKAGE_LIST_MAX_ITEMS;
	return idx;
}
int msg_getSocket(struct packetList *pkgs, struct paket *message){
	int idx = calcIdx(message);
	int sock;
	if(pkgs->itm[idx].pid == message->paketID){
		sock = pkgs->itm[idx].sourceSocket;
	}
	else{
		sock = -1;
	}
	return sock;
}

	/* Prüfe ob Paket bereits in Liste existiert 
 * true: Paket existiert bereits 
 * */

int msg_check(struct paket *message, struct packetList *pkgs)
{	 
	int idx= calcIdx( message);
	return ( pkgs->itm[idx].pid == message->paketID); 
}

void msg_add (int connection_fd, struct paket *message, struct packetList *pkgs, long int time){
	int idx= calcIdx( message);
    /* Mutex auf pkgList */
	pkgs->itm[idx].sourceSocket= connection_fd;
	pkgs->itm[idx].pid= message->paketID;
	pkgs->itm[idx].msgtime = time;
}


void msg_del(struct paket *message, struct packetList *pkgs){
	 int idx= calcIdx( message);
	 pkgs->itm[idx].pid == -1; 
		
}


