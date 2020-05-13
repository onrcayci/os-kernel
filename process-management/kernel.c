#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"


struct PCB *head, *tail;

struct CPU *cpu;

int pcount = 0;                                             //number of pcb's in the ready queue

int myInit(char *filename);

void addToReady(struct PCB *ptr);

int scheduler();

int main() {
    int errorCode = 0;
    cpu = makeCPU();

    printf("Kernel 1.0 loaded!\n");
    printf("Welcome to the Onur Cayci's shell!\n");
	printf("Version 2.0 Updated February 2020\n");
    while(1) {
        errorCode = shellUI();
        if(errorCode == -1) {
		    exit(99);				                    //ignore all other errors
	    }
        else if(errorCode == 1) {                       //Unknown command error
            printf("Unknown command\n");
        }
        else if(errorCode == 2) {
            printf("Variable does not exist\n");
        }
        else if(errorCode == 3) {
            printf("No file name is given\n");
        }
        else if(errorCode == 4) {
            printf("Script not found\n");
        }
        else if(errorCode == 5) {
            printf("Missing argument\n");
        }
        else if(errorCode == 6) {
            printf("Shell memory is full\n");
        }
        else if(errorCode == 7) {
            printf("Too many programs are tried to be executed!\n");
        }
        else if(errorCode == 8) {
            printf("Unable to load script!\n");
        }
        else if(errorCode == 9) {
            printf("CPU is not available!\n");
        }
    }
}

int myInit(char *filename) {
    int errorCode = 0;
    FILE *fptr;
    int start, end;
    int *sptr, *eptr;
    sptr = &start;
    eptr = &end;
    if(fptr = fopen(filename, "r")) {
        addToRAM(fptr, sptr, eptr);
        struct PCB *ptr = makePCB(start, end);
        addToReady(ptr);
    } else {
        errorCode = 8;
        return errorCode;
    }
    fclose(fptr);
    return errorCode; 
}

void addToReady(struct PCB *ptr) {
    if(pcount == 0) {                           //no PCBs are in the ready queue
        head = ptr;
        tail = ptr;
        pcount++;
    } else if (pcount == 1) {                   //one PCB is in the ready queue
        tail = ptr;
        head->next = tail;
        tail->next = head;
        pcount++;
    } else if (pcount == 2) {                   //two PCBs are in the ready queue
        tail->next = ptr;
        tail = tail->next;
        tail->next = head;
        pcount++;
    }
}

int scheduler() {
    int errorCode = 0;
    int temp = head->end;
    while(pcount != 0) {
        if(cpu->IP == -1) {                     //cpu is available
            cpu->IP = head->PC;
            errorCode = run(cpu->quanta);
            head->PC = (head->PC) + 2;
            cpu->IP = -1;
            if(errorCode != 0) {
                return errorCode;
            } else {
                if(head->PC >= (temp + 1)) {    //the program has ended, take it out from ready list and delete the pcb
                    if(pcount > 1) {
                        struct PCB *tmp = head;
                        head = head->next;
                        tmp->next = NULL;
                        int i;
                        for(i=tmp->start; i<=(tmp->end); i++) {
                            ram[i] = NULL;
                        }
                        free(tmp);
                        pcount--;
                    } else if(pcount == 1) {
                        struct PCB *tmp = head;
                        head = NULL;
                        tail = NULL;
                        tmp->next = NULL;
                        int i;
                        for(i=tmp->start; i<=(tmp->end); i++) {
                            ram[i] = NULL;
                        }
                        free(tmp);
                        pcount--;
                    }
                } else {                        //program still needs to run, add it to the back of the ready queue
                    if(pcount > 1) {            //if there are more than 1 program, add the program to the back of the ready queue
                        struct PCB *tmp = head;
                        head = head->next;
                        tail->next = tmp;
                        tail = tmp;
                        tail->next = head;
                    }
                }
            }
        } else {
            errorCode = 9;
            return errorCode;
        }
    }
    return errorCode;
}