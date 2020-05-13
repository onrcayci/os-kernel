#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "shell.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"
#include "memorymanager.h"

struct PCB *head, *tail;

struct CPU *cpu;

int pcount = 0;                                             //number of pcb's in the ready queue

void boot();

int kernel();

void addToReady(struct PCB *ptr);

void clearReady();

int scheduler();

int main() {
    int errorCode = 0;
    boot();
    errorCode = kernel();
    return errorCode;
}

void boot() {
    int i;
    for(i=0; i<40; i++) {
        ram[i] = NULL;
    }
    struct stat stats;
    stat("./BackingStore", &stats);
    if(S_ISDIR(stats.st_mode)) {                            //folder is there so delete it and create a new one
        system("rm -r BackingStore");
        system("mkdir BackingStore");
    } else {
        system("mkdir BackingStore");
    }
}

int kernel() {
    int errorCode = 0;
    cpu = makeCPU();

    printf("Kernel 2.0 loaded!\n");
    printf("Welcome to the Onur Cayci's shell!\n");
	printf("Version 3.0 Updated March 2020\n");
    errorCode = shellUI();
    if(errorCode == -1) {
        exit(99);				                            //ignore all other errors
    }
}

void addToReady(struct PCB *ptr) {
    if(pcount == 0) {                                       //no PCBs are in the ready queue
        head = ptr;
        tail = ptr;
        pcount++;
    } else if (pcount == 1) {                               //one PCB is in the ready queue
        tail = ptr;
        head->next = tail;
        tail->next = head;
        pcount++;
    } else if (pcount == 2) {                               //two PCBs are in the ready queue
        tail->next = ptr;
        tail = tail->next;
        tail->next = head;
        pcount++;
    }
}

void clearReady() {
    if(pcount == 0) {
        return;
    }
    else if(pcount == 1) {
        struct PCB *tmp = head;
        tmp->next = NULL;
        head->next = NULL;
        tail->next = NULL;
        head = NULL;
        tail = NULL;
        free(tmp);
        pcount = 0;
    }
    else if(pcount == 2) {
        struct PCB *tmp1 = head;
        struct PCB *tmp2 = tail;
        tmp1->next = NULL;
        tmp2->next = NULL;
        head->next = NULL;
        tail->next = NULL;
        head = NULL;
        tail = NULL;
        free(tmp1);
        free(tmp2);
        pcount = 0;
    }
}

int scheduler() {
    int errorCode = 0;
    while(pcount != 0) {                                    //while there are still pcbs in the ready queue
        if(cpu->IP == -1) {                                 //cpu is available
            cpu->IP = (head->PC);                           //set IP to the line that will be interpreted
            errorCode = run(cpu->quanta);                   //check for interrupt signal to see if we need to switch to the next frame
            cpu->IP = -1;
            if(errorCode == 99) {                           //interrupt signal
                head->PC_page++;                            //determine the next page
                if(head->PC_page > head->pages_max) {       //program terminates
                    if(pcount > 1) {                        //if there are more than 1 pcbs
                        struct PCB *tmp = head;
                        head = head->next;
                        tmp->next = NULL;
                        int i;
                        for (i = 0; i < 10; i++) {
                            if(tmp->pageTable[i] != -1) {
                                int frame = tmp->pageTable[i];
                                clearFrame(frame);
                            }
                        }
                        free(tmp);
                        pcount--;
                    } else if(pcount == 1) {                //if there is only 1 pcb
                        struct PCB *tmp = head;
                        head = NULL;
                        tail = NULL;
                        tmp->next = NULL;
                        int i;
                        for (i = 0; i < 10; i++) {
                            if(tmp->pageTable[i] != -1) {
                                int frame = tmp->pageTable[i];
                                clearFrame(frame);
                            }
                        }
                        free(tmp);
                        pcount--;
                    }
                } else {                                            //program does not terminate
                    if(head->pageTable[head->PC_page - 1] != -1)  {     //if that page of the program is loaded to the RAM
                        head->PC = (head->pageTable[head->PC_page - 1] - 1) * 4;
                        head->PC_offset = 0;
                        if(pcount > 1) {                            //if there are more than 1 program, add the program to the back of the ready queue
                            struct PCB *tmp = head;                 
                            head = head->next;
                            tail->next = tmp;
                            tail = tmp;
                            tail->next = head;
                        }                                           //no extra condition needed for 1 pcb, since it will alwats be the active one
                    } else {                                        //if the page is not loaded to the RAM
                        char path[1000] = "BackingStore/";
                        strcat(path, head->fileName);
                        FILE *fptr = fopen(path, "r");              //open the file in the BackingStore
                        int frameNumber = findFrame();
                        if(frameNumber == -1) {                     //no empty frame in the RAM, find a victim frame
                            int victimFrame = findVictim(head);
                            int victimPage = 0;                     //page number of the victim frame to update pageTable
                            if(pcount == 2) {                       //if there are only 2 pcbs, victim frame will definitely belong to tail
                                for(int i = 0; i < 10; i++) {
                                    if(tail->pageTable[i] == victimFrame) {     //if we find the victim page, break the loop
                                        victimPage = i+1;
                                        break;
                                    }
                                }
                                loadPage(head->PC_page, fptr, victimFrame);                         //load the page to the RAM
                                fclose(fptr);
                                updatePageTable(head, head->PC_page, frameNumber, victimFrame);     //set page's frame to the victim frame
                                updatePageTable(tail, victimPage, frameNumber, -1);                 //set victim page's frame to -1 which indicates NULL
                            } else if(pcount == 3) {                                                //if there are 3 pcbs, two for loops are needed to determine the victim pcb
                                struct PCB *victim = head->next;
                                for(int i = 0; i < 10; i++) {
                                    if(victim->pageTable[i] == victimFrame) {                       //if victim page is found
                                        victimPage = i;
                                        break;
                                    }
                                }
                                if(victimPage == 0) {                                               //if victim page is not in that PCB
                                    victim = tail;
                                    for (int i = 0; i < 10; i++) {
                                        if(victim->pageTable[i] == victimFrame) {
                                            victimPage = i;
                                            break;
                                        }
                                    }
                                }
                                loadPage(head->PC_page, fptr, victimFrame);
                                fclose(fptr);
                                updatePageTable(head, head->PC_page, frameNumber, victimFrame);
                                updatePageTable(victim, victimPage, frameNumber, -1);
                            }
                        } else {                                    //RAM has space and an empty frame is found
                            loadPage(head->PC_page, fptr, frameNumber);
                            fclose(fptr);
                            updatePageTable(head, head->PC_page, frameNumber, frameNumber);
                        }
                        head->PC = (head->PC_page - 1) * 4;
                        head->PC_offset = 0;
                        if(pcount > 1) {                            //if there are more than 1 program, add the program to the back of the ready queue
                            struct PCB *tmp = head;                 
                            head = head->next;
                            tail->next = tmp;
                            tail = tmp;
                            tail->next = head;
                        }                                           //no extra condition needed for 1 pcb, since it will alwats be the active one
                    }
                }
            } else {                                        //no interrupt occurred, which means we are still in the same frame;
                if(pcount > 1) {                            //if there are more than 1 program, add the program to the back of the ready queue
                    struct PCB *tmp = head;                 
                    head = head->next;
                    tail->next = tmp;
                    tail = tmp;
                    tail->next = head;
                }                                           //no extra condition needed for 1 pcb, since it will alwats be the active one 
            }
        } else {                                        //cpu is not available
            errorCode = 9;
            return errorCode;
        }
    }
    return errorCode;
}