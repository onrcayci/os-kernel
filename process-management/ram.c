#include <stdio.h>
#include <string.h>

char *ram[1000];

void addToRAM(FILE *p, int *start, int *end) {
    char line[1000];
    int s, e;
    int i;
    for(i=0; i<1000; i++) {
        if(ram[i] == NULL) {
            s = i;
            break;
        }
    }
    *start = s;
    while(fgets(line, 999, p) != NULL) {
        ram[s] = strdup(line);
        s++;
    }
    e = s-1;
    *end = e;
} 