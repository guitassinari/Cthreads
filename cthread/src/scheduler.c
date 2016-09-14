#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <time.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"

ucontext_t MAIN_CONTEXT;
FILA2 ready, running, blocked;
CreateFila2(ready);
CreateFila2(running);
CreateFila2(blocked);

TCB_t * sortThread();

int sortAndExecuteThread(){
  TCB_t * nextThread = sortThread();
  executeThread(nextThread);
  return SUCCESS;
}

TCB_t * sortThread(){
  TCB_t * sortedThread = NULL; //Thread sorteada para ser executada
  TCB_t * itThread = NULL;  //Thread de iteração
  while(sortedThread == NULL){
    int sortedTicket = newTicket(); //Sorteia um novo ticket
    FirstFila2(&ready); //Posiciona-se no inicio da fila de aptos
    do {  //Varre a fila até o fim procurando pelo ticket sorteado
      itThread = (TCB_t*)GetAtIteratorFila2(&ready);
      if(itThread == NULL) break;
      if((itThread->ticket == sortedTicket) && (sortedThread == NULL || (sortedThread->tid < itThread->tid))){
        /*
          Se encontrar o ticket e:
           não houver uma thread ja sorteada,
           ou o tid da atual for maior que a ultima sorteada,
          a thread atual será a nova thread sorteada
        */
        sortedThread = itThread;
      }
    }while(NextFila2(&ready) == SUCCESS);
  }

  return sortedThread;
}

int stopExecution(){
  TCB_t * thread;
  if(FirstFila2(&running) != SUCCESS) return ERROR;
  thread = GetAtIteratorFila2(&running);
  DeleteAtIteratorFila2(&running);
  thread->state = PROCST_APTO;
  getcontext(&(thread->context));
  AppendFila2(&ready, thread);
  return SUCCESS;
}

int executeThread(TCB_t * thread){
  RemoveThreadFila2(ready, thread->tid);
  thread->state = PROCST_EXEC;
  AppendFila2(&running, thread);
  ucontext_t context = thread->context;
  setcontext(&context);
  return SUCCESS;
}

//adiciona a thread na fila de aptos para execução
int readyThread(TCB_t * thread){
  thread->state = PROCST_APTO;
  return AppendFila2(&ready, thread);
}

int blockThread(TCB_t * thread){
  thread->state = PROCST_BLOQ;
  return AppendFila2(&blocked, thread);
}

int RemoveThreadFila2(FILA2 fila, int tid){
  TCB_t * itThread = NULL;
  FirstFila2(&fila); //Posiciona-se no inicio da fila de aptos
  do {  //Varre a fila até o fim procurando pelo tid
    itThread = (TCB_t*)GetAtIteratorFila2(&fila);
    if(itThread == NULL) return ERROR;
    if(itThread->tid == tid){ //Se achar o tid, deleta da fila e retorna a função
      DeleteAtIteratorFila2(&fila);
      break;
    }
  }while(NextFila2(&ready) == SUCCESS);
  return SUCCESS;
}
