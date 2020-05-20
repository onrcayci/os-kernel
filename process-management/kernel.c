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

void clearReady();

int scheduler();

int main() {
    int errorCode = 0;
    cpu = makeCPU();

    printf("Kernel 1.0 loaded!\n");
    printf("Welcome to the Onur Cayci's shell!\n");
	printf("Version 2.0 Updated February 2020\n");
    errorCode = shellUI();
    if(errorCode == -1) {
		exit(99);				                    //ignore all other errors
	}
}

int myInit(char *filename) {
    int errorCode = 0;
    FILE *fptr;
    int start, end;
    int *sptr, *eptr;
    sptr = &start;
    eptr = &end;
    fptr = fopen(filename, "r");
    if(fptr != NULL) {
        addToRAM(fptr, sptr, eptr);
        struct PCB *ptr = makePCB(start, end);
        addToReady(ptr);
    } else {
        clearReady();
        clearRAM(0, 999);
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
                        clearRAM(tmp->start, tmp->end);
                        free(tmp);
                        pcount--;
                    } else if(pcount == 1) {
                        struct PCB *tmp = head;
                        head = NULL;
                        tail = NULL;
                        tmp->next = NULL;
                        int i;
                        clearRAM(tmp->start, tmp->end);
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