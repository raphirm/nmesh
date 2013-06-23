/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *
 ********************************************************
 * llist_threads.h --
 *
 * Include file for linked list with threads support
 */
#include "messagelist.h"
#include <stdio.h>
#include <stdlib.h>

int llist_init (llist_t *llistp) 
{	
  int rtn;

  llistp->first = NULL;
  if ((rtn = pthread_mutex_init(&(llistp->mutex), NULL)) !=0)
    fprintf(stderr, "pthread_mutex_init error %d",rtn), exit(1);
  return 0;
}

int llist_insert_data (int index, pthread_t *thread, struct bufmsg *datap, llist_t *llistp, bool alive) 
{
  llist_node_t *cur, *prev, *new;
  int found = 0;

  pthread_mutex_lock(&(llistp->mutex));

  for (cur=prev=llistp->first; cur != NULL; prev=cur, cur=cur->nextp) {
   /* if (cur->index == index) {
      free(cur->datap);
	  cur->thread = thread;
	  cur->alive = alive;
      cur->datap = datap;
      found= 1;
      break;
    }*/
  }
  if (!found) {
    new = (llist_node_t *)malloc(sizeof(llist_node_t));
    new->index = index;
	new->thread = thread;
	new->alive = alive;
    new->datap = datap;
    new->nextp = cur;
    if (cur==llistp->first)
      llistp->first = new;
    else
      prev->nextp = new;
  }

  pthread_mutex_unlock(&(llistp->mutex));

  return 0;
}
int llist_remove_data(int index, llist_t *llistp) 
{
 
  llist_node_t *cur, *prev;

  /* Initialize to "not found" */

  pthread_mutex_lock(&(llistp->mutex));
  cur = llistp->first;
  if(llistp->first->index == index){
	  llistp->first = llistp->first->nextp;
	  free(cur);
  }else{
  for (cur=prev=llistp->first; cur != NULL; prev=cur, cur=cur->nextp) {
    if (cur->index == index) {
      prev->nextp = cur->nextp;
      free(cur);
      break;
    }
  }
  }

  pthread_mutex_unlock(&(llistp->mutex));

  return 0;
}
llist_node_t  *llist_find_data(int index, llist_t *llistp ) 
{
  llist_node_t *cur, *prev;

  /* Initialize to "not found" */

  pthread_mutex_lock(&(llistp->mutex));

  /* Look through index for our entry */
  for (cur=prev=llistp->first; cur != NULL; prev=cur, cur=cur->nextp) {
    if (cur->index == index) {
		  pthread_mutex_unlock(&(llistp->mutex));
       return cur;
      break;
    }
  }
	
  pthread_mutex_unlock(&(llistp->mutex));
  return NULL;
 
}
int llist_show(llist_t *llistp)
{
  llist_node_t *cur;

  pthread_mutex_lock(&(llistp->mutex));

  printf (" Linked list contains : \n");
  for (cur=llistp->first; cur != NULL; cur=cur->nextp) {
    printf ("Index: %d\tData: %s \n", cur->index, cur->datap);    
  }

  pthread_mutex_unlock(&(llistp->mutex));

  return 0;
}
