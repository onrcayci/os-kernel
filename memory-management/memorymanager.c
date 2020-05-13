#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "ram.h"
#include "pcb.h"
#include "kernel.h"

char *fileName;

int launcher(FILE *p);

int countTotalPages(FILE *f);

void loadPage(int PageNumber, FILE *f, int FrameNumber);

int findFrame();

int findVictim(struct PCB *p);

int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);

int launcher(FILE *p) {
    int launch = 0;

    char line[1000];
    char copy[1000];
    char path[1000] = "BackingStore/";

    struct PCB *pcb;

    strcat(path, fileName);
    FILE *wptr = fopen(path, "w");
    if(wptr != NULL) {
        while(fgets(line, 999, p) != NULL) {
            strcpy(copy, line);
            fprintf(wptr, "%s", copy);
        }
    }
    fclose(p);
    fclose(wptr);
    wptr = fopen(path, "r");
    if(wptr != NULL) {                                          //if the file can be opened, try to load the program to the RAM
        int pageCount = countTotalPages(wptr);
        pcb = makePCB(pageCount, fileName);
        rewind(wptr);
        if(pageCount == 1) {                                    //if the program has only one page, upload it without a loop
            int frameNumber = findFrame();                      //check if there is an empty frame in the RAM
            loadPage(1, wptr, frameNumber);
            pcb->PC = (frameNumber-1)*4;
            pcb->pageTable[0] = frameNumber;
        } else {                                                //if the program has two or more pages, upload the first two pages
            int frameNumber = findFrame();
            for(int i = 0; i < 2; i++) {
                if(frameNumber < 10) {
                    int page = i+1;
                    loadPage(page, wptr, frameNumber);
                    rewind(wptr);
                    pcb->pageTable[i] = frameNumber;
                    frameNumber++;
                } else {                                    //frame number is outside of the RAM, therefore do not upload the page
                    break;
                }
            }
            pcb->PC = (pcb->pageTable[0] - 1)*4;
        }
    } else {
        return launch;
    }
    fclose(wptr);
    addToReady(pcb);
    launch = 1;
    return launch;
}

int countTotalPages(FILE *f) {
    int pageCount = 0;
    int lineCount = 0;
    char line[1000];
    while(fgets(line, 999, f) != NULL) {
        lineCount++;
    }
    if(lineCount <= 4) {
        pageCount = 1;
        return pageCount;
    } else if(lineCount > 4) {
        if(lineCount % 4 == 0) {
            pageCount = (lineCount / 4);
        } else {
            pageCount = (lineCount / 4) + 1;
        }
        return pageCount;
    }
    return pageCount;
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
    char line[1000];
    int lineIndex = (pageNumber - 1) * 4;
    int frameIndex = (frameNumber - 1) * 4;
    int i;

    int k;
    if(lineIndex != 0) {
        for(k = 0; k < lineIndex; k++) {                //for loop to get to the desired page from the beginning of the file
            fgets(line, 999, f);
        }
    }
    for (i = 0; i < 4; i++) {
        if(fgets(line, 999, f) !=NULL) {
            ram[frameIndex] = strdup(line);
            frameIndex++; 
        }
    }
}

int findFrame() {
    int frameNumber = 1;
    int i;
    for(i=0; i<40; i = i+4) {
        if(ram[i] == NULL) {
            return frameNumber;
        }
        frameNumber++;
    }
    frameNumber = -1;
    return frameNumber;
}

int findVictim(struct PCB *p) {
    srand(time(NULL));
    int frameNumber = rand() % 10;
    int i;
    for(i = 0; i < 10; i++) {
        if(frameNumber == p->pageTable[i]) {
            frameNumber++;
            if(frameNumber > 10) {                      //if frame number goes beyond 10, bring it back to 1
                frameNumber = 1;
            }
        }
    }
    return frameNumber;
}

int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame) {
    if(frameNumber == -1) {
        p->pageTable[pageNumber - 1] = victimFrame;
    } else {
        p->pageTable[pageNumber - 1] = frameNumber;
    }
}