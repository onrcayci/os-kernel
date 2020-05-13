#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "ram.h"
#include "pcb.h"
#include "kernel.h"
#include "shell.h"

struct CPU* makeCPU() {
    struct CPU *cpu;
    cpu = (struct CPU*)malloc(sizeof(struct CPU));
    cpu->IP = -1;
    cpu->quanta = 2;
    return cpu;
}

int run(int quanta) {
    int errorCode = 0;
    int temp = head->end;
    int q = quanta;
    while(q != 0) {
        if(cpu->IP == temp+1) {
            return errorCode;
        }
        int j;
        for(j=0; j<1000; j++) {
            cpu->IR[j] = ram[cpu->IP][j];
        }
        cpu->IP++;
        errorCode = parseInput(cpu->IR);
        if(errorCode != 0) {
            return errorCode;
        }
        q--;
    }
    return errorCode;
}