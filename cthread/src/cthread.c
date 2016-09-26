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

int main(){
  TCB_t * mainThread = malloc(sizeof(TCB_t));
  mainThread->tid = 0;
  mainThread->ticket = newTicket();
  ucontext_t * context = malloc(sizeof(ucontext_t));
  int * isCommingBack = malloc(sizeof(int));
  *isCommingBack = 1;
  getcontext(&context);
  if(*isCommingBack == 1){
    *isCommingBack = 0;
    mainThread->context = *context;
    readyThread(mainThread);
  }
  free(isCommingBack);
}

int ccreate (void* (*start)(void*), void *arg){
  if(start == NULL) return ERROR;
  ucontext_t * threadContext = malloc(sizeof(ucontext_t)); //Aloca memória para um contexto
  createContext(threadContext, start); //Cria o contexto para a nova thread

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
  if(yield() == SUCCESS){ //Libera a execução e passa a thread para o estado apto
    if(*isCommingBack == 1){
      *isCommingBack = 0;
      sortAndExecuteThread(); //Executa uma nova thread
    }
  }
  free(isCommingBack);
  return SUCCESS;
}

int cjoin(int tid){
  waitForThread(tid);
  return ERROR;
}

int cwait(csem_t *sem){
  sem->count = sem->count - 1;
  if(sem->count < 0){ //Se o recurso estiver alocado
    int * isCommingBack = malloc(sizeof(int));
    *isCommingBack = 1;
    TCB_t * thread = blockThread(); //Bloqueia a thread
    AppendFila2(sem->fila, thread); //Coloca a thread na fila do semáforo
    getcontext(&(thread->context)); //salva o contexto
    if(*isCommingBack == 1){
      *isCommingBack = 0;
      sortAndExecuteThread(); //Executa uma nova thread
    }
    //Quando esta thread for executada novamente, isCommingBack será zero, e ela continuará sua execução normalmente
    free(isCommingBack);
  }
  return SUCCESS;
}

int csignal(csem_t *sem){
  sem->count = sem->count + 1; //Incrementa o contador do semaforo
  if(sem->count < 1){ //Se ainda tiver alguém esperando na fila, desbloqueia.
    FirstFila2(sem->fila);
    TCB_t * thread = (TCB_t*)GetAtIteratorFila2(sem->fila); //Pega uma thread da fila do semaforo
    unblockThread(thread); //Coloca a thread na fila de aptos
    DeleteAtIteratorFila2(sem->fila); //Deleta a thread da fila do semaforo
  }
  return SUCCESS;
}

int cidentify(char *name, int size){
  char fullNames[] = "Guilherme Tassinari(231060) & Caroline Knorr(229753)";
  char reducedNames[] = "GOT(231060) & CKC(229753)";
  if(size < sizeof(fullNames)/sizeof(char)){
    if(size < sizeof(reducedNames)/sizeof(char)){
      return ERROR; //Retorna erro se o tamanho passado for muito pequeno
    } else {
      name = reducedNames; //escreve nomes abreviados se o tamanho permitir
    }
  } else {
    name = fullNames; //Escreve os nomes completos se o tamanho permitir
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

int newTicket(){
  return rand()%256;
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
