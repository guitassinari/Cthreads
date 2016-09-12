#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <time.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/context.h"

PFILA2 ready, running, blocked, semaphores;

int executeNewThread(){
  if(ready == NULL) return ERROR;
  TCB_t *itThread, *nextThread = NULL;
  int nextTicket;
  while(nextThread == NULL){ //Enquanto a fila de threads sorteadas estiver vazia
    FirstFila2(ready);
    if(ready->it == NULL) return ERROR; //Se o primeiro item da fila de aptos for nulo (fila vazia), retorna erro
    nextTicket = newTicket(); //Sorteia novo ticket
    while(ready->it != NULL){ //Enquanto não chega no fim da fila de aptos
      itThread = (TCB_t*)GetAtIteratorFila2(ready);  //Pega uma TCB da fila de aptos
      if(itThread->ticket == nextTicket){
        /*
          Quando achar uma thread com o ticket correspondente, testar se ja foi encontrada alguma.
          Se já foi encontrada, comparar Tids para escolher o maior.
          Se não foi encontrada, salva a encontrada e continua a pesquisa
        */
        if(nextThread == NULL || (nextThread != NULL && itThread->tid > nextThread->tid)) nextThread = itThread;
      }
      NextFila2(ready);
    }
  }

  //Encontra novamente e retira a TCB da fila de aptos
  FirstFila2(ready);
  while(ready->it != NULL){
    itThread = (TCB_t*)GetAtIteratorFila2(ready);
    if(itThread->tid > nextThread->tid){
      DeleteAtIteratorFila2(ready);
      break;
    }
    NextFila2(ready);
  }

  AppendFila2(running ,nextThread);  //Coloca a thread na fila de execução
  setcontext(&(nextThread->context));  //Executa a thread
  return 0;

}

int blockThread(){
  if(running == NULL) return ERROR; //Se a fila de execução estiver fazia, estoura erro
  //Retira a thread em execução da fila RUNNING e coloca na fila READY
  FirstFila2(running);
  TCB_t * wasRunningThread;
  wasRunningThread = (TCB_t*) GetAtIteratorFila2(running); //Pega TCB em execução da fila
  //TODO: UC_LINK
  createContext(NULL, wasRunningThread, NULL);
  DeleteAtIteratorFila2(running);
  AppendFila2(blocked, (void*)wasRunningThread);
  return 0;

}

int stopExecution(){
  if(running == NULL) return ERROR; //Se a fila de execução estiver fazia, estoura erro
  //Retira a thread em execução da fila RUNNING e coloca na fila READY
  FirstFila2(running);
  TCB_t * wasRunningThread;
  wasRunningThread = (TCB_t*) GetAtIteratorFila2(running); //Pega TCB em execução da fila
  //TODO: UC_LINK
  createContext(NULL, wasRunningThread, NULL);
  DeleteAtIteratorFila2(running);
  AppendFila2(ready, (void*)wasRunningThread);

  return 0;
}

int readyThread(TCB_t * thread){
  //TODO: adicionar contexto de escalonador em uc_link
  if(ready == NULL) CreateFila2(ready);
  AppendFila2(ready, thread);

  return SUCCESS;
}
