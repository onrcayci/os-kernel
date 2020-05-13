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
    cpu->offset = 0;
    cpu->quanta = 2;
    return cpu;
}

int run(int quanta) {
    int errorCode = 0;
    int q = quanta;
    cpu->offset = head->PC_offset;
    int index = (cpu->IP) + (cpu->offset);              //get the index of the command in the RAM
    while(q != 0) {                                     //while there is still quanta left
        if(cpu->offset >= 4 || ram[index] == NULL) {    //if offset is 4 or if the lines in the frame ended, interrupt the program
            errorCode = 99;
            return errorCode;
        } else {
            int j;
            for(j=0; j<1000; j++) {                     //copy the command from ram to the IR
                cpu->IR[j] = ram[index][j];
            }
            cpu->offset++;                              //increase offset by 1 since we move to the next line
            index++;
            head->PC_offset = cpu->offset;              //set pcb's offset to cpu's offset after each increment
            errorCode = parseInput(cpu->IR);
            if(errorCode != 0) {
                return errorCode;
            }
            q--;
        }
    }
    return errorCode;
}