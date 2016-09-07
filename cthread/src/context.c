#include "../include/context.h"
#include <ucontext.h>
#include <stdlib.h>

void setStack(ucontext_t * ucp){
  ucp->uc_stack.ss_sp = malloc(STACK_SIZE);
  ucp->uc_stack.ss_size = STACK_SIZE;
}
