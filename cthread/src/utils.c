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

int newTid(){
  tid++;
  return tid;
}

int newTicket(){
  return Random2()%256;
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

void createThread(TCB_t * thread, ucontext_t * context){
  thread->tid = newTid();
  thread->ticket = newTicket();
  thread->context = *context;
}
