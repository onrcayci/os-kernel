#include <stdio.h>
#include <string.h>

char *ram[40];

int addToRAM(FILE *p, int *start, int *end) {
    int errorCode = 0;
    char line[1000];
    int s, e;
    int i;
    for(i=0; i<40; i++) {
        if(ram[i] == NULL) {
            s = i;
            break;
        }
    }
    if(i == 40) {
        printf("insufficient RAM, cannot load rest of the script");
        errorCode = 9;
        return errorCode;
    }
    *start = s;
    while(fgets(line, 999, p) != NULL) {
        ram[s] = strdup(line);
        s++;
    }
    e = s-1;
    *end = e;
    return errorCode;
}

void clearRAM(int start, int end) {
    int i;
    for (i=start; i <= end; i++) {
        ram[i] = NULL;
    }
}

void clearFrame(int frame) {
    int index = (frame - 1) * 4;        //find the index the frame is pointing to
    int start = index;                  //set up start integer as the start of the frame
    int end = start + 3;                //set up integer as the end of the frame
    clearRAM(start, end);
}