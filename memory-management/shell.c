#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

int parseInput(char input[]);

int shellUI() {

	char prompt[100] = {'$', '\0'};                     //prompts can be modified
	char userInput[1000];                               //user's input stored here
	int errorCode = 0;                                  //0 means no error, default

	while(1) {
		printf("%s ", prompt);
		fgets(userInput, 999, stdin);                   //limit input to array size

		errorCode = parseInput(userInput);
		if(errorCode == -1) {
		    exit(99);									//return errorCode to the kernel and shut down shellUI
	    }
	}
}

int parseInput(char input[]) {
	char tmp[200];
	char *words[100];
	int a,b;
	int w=0;												//integer that holds the word count

	b=0;
	for(a=0; a <= (strlen(input)); a++) {
		if(input[a] == ' ' || input[a] == '\0' || input[a] == '\n' || input[a] == '\r') {
			if(b != 0) {
				tmp[b] = '\0';
				words[w] = strdup(tmp);
				w++;
				b=0;
			}
		} else {
			tmp[b] = input[a];
			b++;
		}
	}
	return interpreter(words, w);
}

