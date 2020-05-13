struct CPU {
    int IP;
    char IR[1000];
    int quanta;
};

struct CPU* makeCPU();

int run(int quanta);