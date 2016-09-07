#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"
#include <stdlib.h>

#define MAX_TICKET 255

int tid = 0;

int ccreate (void* (*start)(void*), void *arg){
    int threadTid = newTid();
    TCB_t * newThread = malloc(sizeof(TCB_t));
    newThread->ticket = newTicket();
    newThread->state = PROCST_APTO;
    newThread->tid = threadTid;

    return threadTid
}

int cyield(void);
int cjoin(int tid);
int csem_init(csem_t *sem, int count);
int cwait(csem_t *sem);
int csignal(csem_t *sem);

int newTid(){
  int actualTid = tid;
  tid++;
  return actualTid;
}

int newTicket(){
  return (rand() % 10) + (rand() % 1);
}

int teste(){
  printf("uma função de teste");
}
