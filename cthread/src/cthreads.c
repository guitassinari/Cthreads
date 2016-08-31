#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"

int ccreate (void* (*start)(void*), void *arg);
int cyield(void);
int cjoin(int tid);
int csem_init(csem_t *sem, int count);
int cwait(csem_t *sem);
int csignal(csem_t *sem);
