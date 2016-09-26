int newTicket();
void createContext(ucontext_t * context, void* (*start)(void*));
void createThread(TCB_t * thread, ucontext_t * context);
int newTid();
