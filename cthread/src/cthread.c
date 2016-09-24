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
  int * isCommingBack = malloc(sizeof(int));
  *isCommingBack = 1;
  if(yield() == SUCCESS){
    if(*isCommingBack == 1){
      *isCommingBack = 0;
      sortAndExecuteThread();
    }
  }
  free(isCommingBack);
  return SUCCESS;
}

int cjoin(int tid){
  //TODO: trocar context para thread da TID passada
  return ERROR;
}

int cwait(csem_t *sem){
  sem->count = sem->count - 1;
  if(sem->count < 0){
    int * isCommingBack = malloc(sizeof(int));
    *isCommingBack = 1;
    TCB_t * thread = blockThread();
    AppendFila2(sem->fila, thread);
    getcontext(&(thread->context));
      if(*isCommingBack == 1){
        *isCommingBack = 0;
        sortAndExecuteThread();
      }
    free(isCommingBack);
  }
  return SUCCESS;
}

int csignal(csem_t *sem){
  sem->count = sem->count + 1;
  if(sem->count < 1){
    FirstFila2(sem->fila);
    TCB_t * thread = (TCB_t*)GetAtIteratorFila2(sem->fila);
    DeleteAtIteratorFila2(sem->fila);
    readyThread(thread);
  }
  return SUCCESS;
}

int cidentify(char *name, int size){
  char fullNames[] = "Guilherme Tassinari & Caroline Knorr";
  char reducedNames[] = "GOT & CKC";
  if(size < sizeof(fullNames)/sizeof(char)){
    if(size < sizeof(reducedNames)/sizeof(char)){
      return ERROR;
    } else {
      name = reducedNames;
    }
  } else {
    name = fullNames;
  }
  return SUCCESS;
}

int csem_init (csem_t *sem, int count){
  if(sem == NULL){
    sem = malloc(sizeof(csem_t*));
  }
  sem->count = count;
  sem->fila = malloc(sizeof(PFILA2));
  CreateFila2(sem->fila);
  return SUCCESS;
}

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
