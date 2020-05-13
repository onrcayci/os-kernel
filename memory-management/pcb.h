struct PCB {
    int PC;
    int PC_page;
    int PC_offset;
    int pages_max;
    int pageTable[10];
    char *fileName;
    struct PCB* next;
};

struct PCB* makePCB(int maxPages, char *filename);