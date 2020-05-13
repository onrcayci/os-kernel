#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcb.h"

struct PCB* makePCB(int start, int end) {
    struct PCB* pcb;
    pcb = (struct PCB*)malloc(sizeof(struct PCB));
    pcb->start = start;
    pcb->end = end;
    pcb->PC = start;
    pcb->next = NULL;
    return pcb;
}

