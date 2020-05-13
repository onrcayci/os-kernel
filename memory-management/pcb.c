#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcb.h"

struct PCB* makePCB(int maxPages, char *filename) {
    struct PCB* pcb;
    pcb = (struct PCB*)malloc(sizeof(struct PCB));      //create the PCB and malloc in the memory
    pcb->PC = 0;                                        //initialize PC as 0 since it is looking at the code at index 0 of the page
    pcb->PC_page = 1;                                   //initialize PC_page as 1 since that is how my page count goes, it starts from 1
    pcb->PC_offset = 0;                                 //initialize PC_offset as there is no offset so far to save
    pcb->pages_max = maxPages;                          //initialize pages_max to the pageCount calculated in the launcher helper method
    pcb->fileName = filename;                           //keep track of the fileName if we need to upload more pages to the RAM
    pcb->next = NULL;                                   //initialized as NULL but will be set inside the scheduler for the Ready Queue
    for(int i = 0; i < 10; i++) {
        pcb->pageTable[i] = -1;                         //-1 is assumed as NULL, no corresponding frame for that page number
    }
    return pcb;
}

