#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/context.h"
#include "../include/cdata.h"

#define MAX_TICKET 255

int tid = 0;
int newTicket();
int newTid();
void teste();

int main(){
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
}

int ccreate (void* (*start)(void*), void *arg){
    if(start == NULL) return ERROR;
    ucontext_t threadContext;
    getcontext(&threadContext);
    makecontext(&threadContext, (void*)start, 0);
    setStack(&threadContext);

    int threadTid = newTid();
    TCB_t * newThread = malloc(sizeof(TCB_t));
    if (newThread == NULL) return ERROR;

    newThread->ticket = newTicket();
    newThread->state = PROCST_APTO;
    newThread->tid = threadTid;

    printf("TID: %d | TCKT : %d\n", newThread->tid, newThread->ticket);

    return threadTid;
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
  srand(time(NULL));
  return (rand() % (MAX_TICKET+1));
}

void teste(){
  printf("uma função de teste");
}
