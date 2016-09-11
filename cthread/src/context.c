#include "../include/context.h"
#include <ucontext.h>
#include <stdlib.h>

#define STACK_SIZE 32000

void createContext(void* (*start)(void*), ucontext_t * ucp, ucontext_t * link){
  getcontext(&ucp);
  ucp->uc_stack.ss_flags = 0;
  ucp->uc_stack.ss_sp = malloc(STACK_SIZE);
  ucp->uc_stack.ss_size = STACK_SIZE;
  if(link != NULL) ucp->uc_link = link;
  if(start != NULL) makecontext(&ucp, (void*)start, 0);
}
