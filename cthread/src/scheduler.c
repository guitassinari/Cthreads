#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <time.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/utils.h"
#include "../include/scheduler.h"

typedef struct s_join {
  int thread;
  int threadToWait;
} joint;

ucontext_t MAIN_CONTEXT;
PFILA2 ready, running, blocked, finished, joints;

int initMainThread(){
  initFila(&running);
  TCB_t * mainThread = malloc(sizeof(TCB_t));
  mainThread->tid = 0;
  mainThread->ticket = Random2()%256;
  getcontext(&mainThread->context);
  AppendFila2(running, mainThread);
  return SUCCESS;
}

int sortAndExecuteThread(){
  TCB_t * nextThread = sortThread(); //Sorteia uma nova thread para ser executada
  if(nextThread != NULL){
    RemoveThreadFila2(ready, nextThread->tid); //Remove a thread da fila de aptos
    executeThread(nextThread); //Executa a thread
  }
  return SUCCESS;
}

TCB_t * sortThread(){
  TCB_t * sorted = NULL, * itThread = NULL;  //Thread de iteração
  int found = 0;
  while(found == 0){
    int sortedTicket = Random2()%256; //Sorteia um novo ticket
    if(FirstFila2(ready) != SUCCESS) return NULL; //Posiciona-se no inicio da fila de aptos
    do {  //Varre a fila até o fim procurando pelo ticket sorteado
      itThread = (TCB_t*)GetAtIteratorFila2(ready);
      if(itThread == NULL) break;
      if((itThread->ticket == sortedTicket) && (sorted == NULL || sorted->tid < itThread->tid)){
        /*
          Se encontrar o ticket e:
           não houver uma thread ja sorteada,
           ou o tid da atual for maior que a ultima sorteada,
          a thread atual será a nova thread sorteada
        */
        sorted = itThread;
      }
    }while(NextFila2(ready) == SUCCESS);
    if(sorted != NULL) found = 1;
  }
  return sorted;
}

int waitForThread(int tid){
  initFila(&joints);
  joint * join;
  FirstFila2(joints);
  do {  //Varre a fila de espera procurando por alguma thread esteja esperando a thread solicitada
    join = (joint*)GetAtIteratorFila2(joints);
    if(join == NULL) break;
    if(join->threadToWait == tid){ //Se ja houver alguem na espera, retorna erro, pois não podem haver duas threads esperando a mesma thread
      return ERROR;
      break;
    }
  }while(NextFila2(joints) == SUCCESS);
  //Caso não hajam problemas, bloqueia a thread atual e a coloca na fila de espera da thread solicitada
  TCB_t * currentThread = blockThread();
  join = malloc(sizeof(joint));
  join->thread = currentThread->tid;
  join->threadToWait = tid;
  AppendFila2(joints, join); //Registro na fila de espera
  int * isCommingBack = malloc(sizeof(int));
  *isCommingBack = 1;
  getcontext(&(currentThread->context)); //A execução continuará daqui quando a thread for liberada
  if(*isCommingBack == 1){
    *isCommingBack = 0;
    sortAndExecuteThread(); //Sorteia a proxima thread a ser executada
  }
  free(isCommingBack);
  return SUCCESS;
}

void finishExecution(){
  initFila(&finished);
  initFila(&joints);
  FirstFila2(running);
  TCB_t * thread = (TCB_t*)GetAtIteratorFila2(running); //Pega a thread da fila de execução
  joint * join;
  FirstFila2(joints); //Posiciona-se no inicio da fila
  do {  //Varre a fila até o fim procurando pelo tid
    join = (joint*)GetAtIteratorFila2(joints);
    if(join == NULL) break;
    if(join->threadToWait == thread->tid){ //Se achar o tid, já tem alguem esperando. Retornar erro
      SearchThreadByTidFila2(blocked, join->thread);
      TCB_t * blockedThread = (TCB_t*)GetAtIteratorFila2(blocked);
      DeleteAtIteratorFila2(blocked);
      readyThread(blockedThread);
    }
  }while(NextFila2(joints) == SUCCESS);
  DeleteAtIteratorFila2(running); //Remove a thread da fila de execução
  AppendFila2(finished, thread); //Coloca a thread na fila de terminados
  sortAndExecuteThread(); //Executa uma nova thread.
}

int yield(){
  TCB_t * thread;
  if(FirstFila2(running) != SUCCESS) return ERROR;
  thread = GetAtIteratorFila2(running); //pega a thread da fila de execução
  DeleteAtIteratorFila2(running); //Remove a thread da fila de execução
  readyThread(thread); //Coloca a thread na fila de aptos
  getcontext(&(thread->context)); //Atualiza o contexto da thread
  return SUCCESS;
}

int executeThread(TCB_t * thread){
  initFila(&running);
  thread->state = PROCST_EXEC;
  AppendFila2(running, thread); //Coloca a thread na fila de execução
  setcontext(&(thread->context)); //Executa o contexto da thread
  return SUCCESS;
}

//adiciona a thread na fila de aptos para execução
int readyThread(TCB_t * thread){
  initFila(&ready);
  thread->state = PROCST_APTO;
  AppendFila2(ready, thread);
  return SUCCESS;
}

TCB_t * blockThread(){
  if(blocked == NULL){ //inicializa a fila de bloqueados, se necessário
    blocked = malloc(sizeof(PFILA2));
    CreateFila2(blocked);
  }
  FirstFila2(running);
  TCB_t * thread = (TCB_t*)GetAtIteratorFila2(running); //Pega o primeiro(e único) thread da fila de em execução
  RemoveThreadFila2(running, thread->tid); //Retira a thread da fila de execuçãp
  thread->state = PROCST_BLOQ;
  AppendFila2(blocked, thread); //Coloca a thread na fila de bloqueados
  return thread; //Retorna a thread que foi bloqueada
}

int RemoveThreadFila2(PFILA2 fila, int tid){
  if(SearchThreadByTidFila2(fila, tid) == ERROR) return ERROR;
  if(DeleteAtIteratorFila2(fila) == ERROR) return ERROR;
  return SUCCESS;
}

int unblockThread(TCB_t * thread){
  RemoveThreadFila2(blocked, thread->tid); //Remove a thread da fila de bloqueados
  readyThread(thread); //Adiciona a thread à fila de aptos
  return SUCCESS;
}

int SearchThreadByTidFila2(PFILA2 fila, int tid){
  TCB_t * itThread = NULL;
  if(FirstFila2(fila) != SUCCESS) return ERROR; //Posiciona-se no inicio da fila
  do {  //Varre a fila até o fim procurando pelo tid
    itThread = (TCB_t*)GetAtIteratorFila2(fila);
    if(itThread == NULL) break;
    if(itThread->tid == tid){ //Se achar o tid, termina a busca
      return SUCCESS;
      break;
    }
  }while(NextFila2(fila) == SUCCESS);
  return ERROR;
}

int initFila(PFILA2 * fila){
  if(*fila == NULL){ //inicializa a fila de aptos, se necessário
    *fila = malloc(sizeof(PFILA2));
    CreateFila2(*fila);
  }
  return SUCCESS;
}
