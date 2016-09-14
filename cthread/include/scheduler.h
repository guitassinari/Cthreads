int sortAndExecuteThread();
// int blockThread();
// int stopExecution();
int readyThread(TCB_t * thread);
int executeThread(TCB_t * thread);
int execute(ucontext_t * context);
