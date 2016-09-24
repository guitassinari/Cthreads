#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <time.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"

ucontext_t MAIN_CONTEXT;
PFILA2 ready, running, blocked, finished;
CreateFila2(running);
CreateFila2(blocked);
CreateFila2(finished);

TCB_t * sortThread();

int sortAndExecuteThread(){
  TCB_t * nextThread = sortThread(nextThread);
  if(nextThread != NULL){
    RemoveThreadFila2(ready, nextThread->tid);
    executeThread(nextThread);
  }
  return SUCCESS;
}

TCB_t * sortThread(){
  TCB_t * sorted = NULL, * itThread = NULL;  //Thread de iteração
  int found = 0;
  while(!found){
    int sortedTicket = newTicket(); //Sorteia um novo ticket
    if(FirstFila2(ready) != SUCCESS) return NULL; //Posiciona-se no inicio da fila de aptos
    do {  //Varre a fila até o fim procurando pelo ticket sorteado
      itThread = (TCB_t*)GetAtIteratorFila2(ready);
      if(itThread == NULL) break;
      if((itThread->ticket == sortedTicket) && (sorted == NULL || sorted->tid < itThread->tid)){
        found = 1;
        /*
          Se encontrar o ticket e:
           não houver uma thread ja sorteada,
           ou o tid da atual for maior que a ultima sorteada,
          a thread atual será a nova thread sorteada
        */
        sorted = itThread;
      }
    }while(NextFila2(ready) == SUCCESS);
  }
  return sorted;
}

void finishExecution(){
  if(finished == NULL){
    finished = malloc(sizeof(PFILA2));
    CreateFila2(finished);
  }
  TCB_t * thread;
  if(FirstFila2(running) != SUCCESS) return ERROR;
  thread = (TCB_t*)GetAtIteratorFila2(running);
  DeleteAtIteratorFila2(running);
  AppendFila2(finished, thread);
  sortAndExecuteThread();
}

int yield(){
  TCB_t * thread;
  if(FirstFila2(running) != SUCCESS) return ERROR;
  thread = GetAtIteratorFila2(running);
  DeleteAtIteratorFila2(running);
  readyThread(thread);
  getcontext(&(thread->context));
  return SUCCESS;
}

int executeThread(TCB_t * thread){
  if(running == NULL){
    running = malloc(sizeof(PFILA2));
    CreateFila2(running);
  }
  thread->state = PROCST_EXEC;
  AppendFila2(running, thread);
  setcontext(&(thread->context));
}

//adiciona a thread na fila de aptos para execução
int readyThread(TCB_t * thread){
  if(ready == NULL){
    ready = malloc(sizeof(PFILA2));
    CreateFila2(ready);
  }
  thread->state = PROCST_APTO;
  AppendFila2(ready, thread);
}

TCB_t * blockThread(){
  if(blocked == NULL){
    blocked = malloc(sizeof(PFILA2));
    CreateFila2(blocked);
  }
  FirstFila2(running);
  TCB_t * thread = (TCB_t*)GetAtIteratorFila2(running);
  RemoveThreadFila2(running, thread->tid);
  thread->state = PROCST_BLOQ;
  AppendFila2(blocked, thread);
  return thread;
}

int RemoveThreadFila2(PFILA2 fila, int tid){
  SearchThreadByTidFila2(fila, tid);
  DeleteAtIteratorFila2(fila);
  return SUCCESS;
}

int SearchThreadByTidFila2(PFILA2 fila, int tid){
  TCB_t * itThread = NULL;
  if(FirstFila2(fila) != SUCCESS) return ERROR; //Posiciona-se no inicio da fila de aptos
  do {  //Varre a fila até o fim procurando pelo tid
    itThread = (TCB_t*)GetAtIteratorFila2(fila);
    if(itThread == NULL) break;
    if(itThread->tid == tid){ //Se achar o tid, deleta da fila e retorna a função
      return SUCCESS;
      break;
    }
  }while(NextFila2(fila) == SUCCESS);
  return ERROR;
}

void printReady(){
  TCB_t * itThread = NULL;  //Thread de iteração
  if(FirstFila2(ready)) return NULL; //Posiciona-se no inicio da fila de aptos
  printf("Aptos : %d - ", &ready);
  do {  //Varre a fila até o fim procurando pelo ticket sorteado
    itThread = (TCB_t*)GetAtIteratorFila2(ready);
    if(itThread != NULL){
      printf(" %d -", itThread->tid);
    }
  }while(NextFila2(ready) == SUCCESS);
  printf("\n");
}
