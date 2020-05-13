#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//String matrix, var and value seperated per row
char *memory[1000][2];

int store(char *words[]) {
	int errorCode = 0;
	int i;
	for(i=0; i<1000; i++) {
		if(memory[i][0] == NULL) {
			memory[i][0] = strdup(words[1]);			//second word is VAR, store it as the first string
			memory[i][1] = strdup(words[2]);			//third word is STRING, store it as the second string
			return errorCode;
		}
		else if(strcmp(memory[i][0], words[1]) == 0) {
			memory[i][1] = strdup(words[2]);
			return errorCode;
		}
	}
	errorCode = 6;
	return errorCode;
}

char *get(char *word) {
	int i;
	char *result;
	for(i=0; i<1000; i++) {
		if(memory[i][0] != NULL && strcmp(memory[i][0], word) == 0) {
			result = strdup(memory[i][1]);
			return result;
		}
	}
	result = strdup("Variable does not exist");
	return result;
}
