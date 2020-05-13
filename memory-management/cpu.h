struct CPU {
    int IP;
    int offset;
    char IR[1000];
    int quanta;
};

struct CPU* makeCPU();

int run(int quanta);