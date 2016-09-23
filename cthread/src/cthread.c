#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/scheduler.h"

#define STACK_SIZE 1024*32

int tid = 0;
int newTid();
int newTicket();
void teste();
void teste2();

int main(){
  printf("\ninicio\n");
  ccreate((void*)teste, 0);
  ccreate((void*)teste, 0);
  ccreate((void*)teste2, 0);
  ccreate((void*)teste, 0);
  ccreate((void*)teste2, 0);
  ccreate((void*)teste, 0);
  ccreate((void*)teste, 0);
  ccreate((void*)teste, 0);
  ccreate((void*)teste, 0);
  ccreate((void*)teste2, 0);
  ccreate((void*)teste, 0);



  sortAndExecuteThread();
  return 0;
}

//TODO: testar adição na fila;
int ccreate (void* (*start)(void*), void *arg){
  if(start == NULL) return ERROR;
  ucontext_t * threadContext = malloc(sizeof(ucontext_t));
  createContext(threadContext, start);

  TCB_t * newThread = malloc(sizeof(TCB_t));
  createThread(newThread, threadContext);
  readyThread(newThread);
  return newThread->tid;
}

void createThread(TCB_t * thread, ucontext_t * context){
  thread->tid = newTid();
  thread->ticket = newTicket();
  thread->context = *context;
}

int cyield(void){
  int isCommingBack = 1;
  if(yield() == SUCCESS){
    if(isCommingBack == 1){
      isCommingBack = 0;
      sortAndExecuteThread();
    }
  }
  return 1;
}

// int cjoin(int tid){
//   //TODO: trocar context para thread da TID passada
//   return ERROR;
// }
//
// int csem_init(csem_t *sem, int count){
//   sem->count = count;
//   CreateFila2(sem->fila);
//   return 0;
// }
//
// int cwait(csem_t *sem){
//   //se a thread ta ocupada, bloquear a thread e colocar outra em execução
//   if(sem->count < 1){
//     TCB_t * runningThread = GetAtIteratorFila2(running);
//     DeleteAtIteratorFila2(running);
//     AppendFila2(blocked, runningThread);
//     AppendFila2(sem->fila, runningThread);
//     //TODO: trocar execução de thread
//   }
//
//   sem->count = sem->count - 1;
//   return ERROR;
// }
//
// int csignal(csem_t *sem){
//   //incrementa contador do semaforo
//   sem->count = sem->count + 1;
//
//   //se existe alguem na fila de espera, acorda para a fila de aptos
//   if(sem->count < 1){
//     FirstFila2(sem->fila);
//     TCB_t * readyThread = GetAtIteratorFila2(sem->fila);
//     DeleteAtIteratorFila2(sem->fila);
//     //TODO: retirar thread da lista de bloqueados
//     AppendFila2(ready, readyThread);
//   }
//
//   return ERROR;
// }
//
// int cidentify(char *name, int size){
//   return 0;
// }

int newTid(){
  tid++;
  return tid;
}

//TODO: testar criação de ticket randomico entre 0 e 255
int newTicket(){
  return rand()%256;
}

void teste(){
  printf("\numa funcao de teste\n");
}

void teste2(){
  printf("\numa funcao de teste 2 ");
  cyield();
  printf("Pos yield \n");
}

void createContext(ucontext_t * context, void* (*start)(void*)){
  ucontext_t * returnContext = malloc(sizeof(ucontext_t));
  getcontext(returnContext);
  returnContext->uc_stack.ss_sp = malloc(STACK_SIZE);
  returnContext->uc_stack.ss_size = STACK_SIZE;
  returnContext->uc_stack.ss_flags = 0;
  context->uc_link = 0;
  makecontext(returnContext, (void*)&finishExecution, 0);

  getcontext(context);
  context->uc_stack.ss_sp = malloc(STACK_SIZE);
  context->uc_stack.ss_size = STACK_SIZE;
  context->uc_stack.ss_flags = 0;
  context->uc_link = returnContext;
  makecontext(context, (void*)start, 0);
}
