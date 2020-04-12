#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

int parseInput(char input[]);

int main(int argc, char *argv[]) {

        char prompt[100] = {'$', '\0'};                         //prompt is a simple $ sign, no directory navigation
        char userInput[1000];                                   //user's input stored here
        int errorCode = 0;                                      //0 means no error, default
	int i;

        printf("Welcome to the Onur Cayci's shell!\n");
        printf("Version 1.0 Created January 2020\n");

        while(1) {                                              //infinite loop until the quit command
			printf("%s ", prompt);
			fgets(userInput, 999, stdin);                   	//limit input to array size to 999 characters since last character is \0

			errorCode = parseInput(userInput);					//ignore any other errors and stop the program
			if(errorCode == -1) {
				exit(99);
			}
        }
}

int parseInput(char input[]) {
	char tmp[200];												//char array used to seperately store words of the command
	char *words[100];											//string array to store each word of the command
	int a,b;													//integers to follow the index
	int w=0;													//word count

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

