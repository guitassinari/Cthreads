int sortAndExecuteThread();
void finishExecution();
TCB_t * blockThread();
int readyThread(TCB_t * thread);
int executeThread(TCB_t * thread);
