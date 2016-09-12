#include <ucontext.h>
#define STACK_SIZE 32000

void createContext(void* (*start)(void*), ucontext_t * ucp, ucontext_t * link);
