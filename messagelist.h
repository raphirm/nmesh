#ifndef __MESSAGELIST_H__
#define __MESSAGELIST_H__

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct llist_node {
  int index;
  pthread_t *thread;
  struct bufmsg *datap;
  struct llist_node *nextp;
  bool alive;
} llist_node_t;

typedef struct llist { 
  llist_node_t *first;
  pthread_mutex_t mutex;
} llist_t;

int llist_init (llist_t *llistp);
int llist_insert_data (int index, pthread_t *thread, struct bufmsg *datap, llist_t *llistp, bool alive);
int llist_remove_data(int index, llist_t *llistp);
llist_node_t *llist_find_data(int index,  llist_t *llistp);
int llist_show(llist_t *llistp);

#endif
