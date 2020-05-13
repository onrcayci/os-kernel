struct PCB {
    int PC;
    int start;
    int end;
    struct PCB* next;
};

struct PCB* makePCB(int start, int end);