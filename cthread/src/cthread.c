#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/context.h"
#include "../include/cdata.h"
#include "../include/scheduler.h"

int tid = 0;
PFILA2 ready, running, blocked, semaphores;
int newTicket();
int newTid();
void teste();

int main(){
  //TODO: criar thread main
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);

  return 0;
}

//TODO: testar adição na fila;
int ccreate (void* (*start)(void*), void *arg){
    if(start == NULL) return ERROR;
    TCB_t * newThread = malloc(sizeof(TCB_t));
    newThread->tid = newTid();
    newThread->ticket = newTicket();
    newThread->state = PROCST_APTO;

    //TODO: UC_LINK
    ucontext_t threadContext;
    createContext(start, &threadContext, NULL);
    newThread->context = threadContext;
    readyThread(&newThread);
    return newThread->tid;
}

int cyield(void){
    stopExecution();
    executeNewThread();
    //Insere a nova thread na fila RUNNING
    //TODO: Pegar a próxima thread do escalonador;
    TCB_t * nowRunningThread;
    InsertAfterIteratorFila2(running, (void*)nowRunningThread);

    return 1;
}

int cjoin(int tid){
  //TODO: trocar context para thread da TID passada
  return ERROR;
}

int csem_init(csem_t *sem, int count){
  sem->count = count;
  CreateFila2(sem->fila);
  return 0;
}

int cwait(csem_t *sem){
  //se a thread ta ocupada, bloquear a thread e colocar outra em execução
  if(sem->count < 1){
    TCB_t * runningThread = GetAtIteratorFila2(running);
    DeleteAtIteratorFila2(running);
    AppendFila2(blocked, runningThread);
    AppendFila2(sem->fila, runningThread);
    //TODO: trocar execução de thread
  }

  sem->count = sem->count - 1;
  return ERROR;
}

int csignal(csem_t *sem){
  //incrementa contador do semaforo
  sem->count = sem->count + 1;

  //se existe alguem na fila de espera, acorda para a fila de aptos
  if(sem->count < 1){
    FirstFila2(sem->fila);
    TCB_t * readyThread = GetAtIteratorFila2(sem->fila);
    DeleteAtIteratorFila2(sem->fila);
    //TODO: retirar thread da lista de bloqueados
    AppendFila2(ready, readyThread);
  }

  return ERROR;
}

int cidentify(char *name, int size){
  return 0;
}

int newTid(){
  tid++;
  return tid;
}

//TODO: testar criação de ticket randomico entre 0 e 255
int newTicket(){
  return Random2();
}

void teste(){
  printf("uma função de teste");
}

//procura um tid dentro de uma fila
int findTidFila2(int tid, PFILA2 pfila2){
  if(pfila2 == NULL) return ERROR;
  FirstFila2(pfila2);
  TCB_t * thread;
  do{
      thread = GetAtIteratorFila2(pfila2);
      if(NextFila2(pfila2) != 0) return ERROR;
  } while(thread->tid != tid || thread != NULL);

  return 0;
}
