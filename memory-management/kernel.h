extern struct CPU *cpu;
extern struct PCB *head;
extern struct PCB *tail;
extern int pcount;

int myInit(char *filename);
int addToReady(struct PCB *ptr);
void clearReady();
int scheduler();