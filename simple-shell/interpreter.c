#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shellmemory.h"
#include "shell.h"

int flag;										//flag for quit method

int help(char *words[], int wordCount);			//help command method

int quit(char *words[], int wordCount);			//quit command method

int set(char *words[], int wordCount);				//set command method

int print(char *words[], int wordCount);			//print command method

int run(char *words[], int wordCount);				//run command method

int interpreter(char *words[], int wordCount) {		//assumes cmd switchs args
													//assumes words[0] is cmd
	int errorCode = 0;

	if (strcmp(words[0], "help") == 0) {
		errorCode = help(words, wordCount);
	} else if (strcmp(words[0], "quit") == 0) {
		errorCode = quit(words, wordCount);
	} else if (strcmp(words[0], "set") == 0) {
		errorCode = set(words, wordCount);
	} else if (strcmp(words[0], "print") == 0) {
		errorCode = print(words, wordCount);
	} else if (strcmp(words[0], "run") == 0) {
		errorCode = run(words, wordCount);
	} else {
		errorCode = 1;
	} 
	if(errorCode == 1) {                       		//Unknown command error
		printf("Unknown command\n");
	} else if(errorCode == 2) {						//variable does not exist error
		printf("Variable does not exist\n");
	} else if(errorCode == 3) {						//no file name error
		printf("No file name is given\n");
	} else if(errorCode == 4) {						//script not found error
		printf("Script not found\n");
	} else if(errorCode == 5) {						//missing argument error
		printf("Missing argument\n");
	} else if(errorCode == 6) {						//shell memory is full error
		printf("Shell memory is full\n");
	} else if(errorCode == 7) {
		printf("Too many arguments!\n");
	}
	return errorCode;
};
int help(char *words[], int wordCount) {
	int errorCode = 0;
	if(wordCount == 1) {
		printf("These shell commands are defined internally. Type 'help' to see this list.\n\n");
		printf("COMMAND\t\t\t\tDESCRIPTION\n\n");
		printf("help\t\t\t\tdisplays all commands\n");
		printf("quit\t\t\t\texits / terminates the shell with 'Bye!'\n");
		printf("set VAR STRING\t\t\tassigns a value to shell memory\n");
		printf("print VAR\t\t\tdisplays the STRING assigned to VAR\n");
		printf("run SCRIPT.TXT\t\t\texecutes the file SCRIPT.TXT\n\n");
	} else {
		errorCode = 7;
		return errorCode;
	}
	return errorCode;
}

int quit(char *words[], int wordCount) {
	int errorCode = 0;
	if(wordCount == 1) {
		if(flag == 1) {
			flag = 0;
			return errorCode;
		}
		printf("Bye!\n");
		exit(0);
	} else {
		errorCode = 7;
		return errorCode;
	}
	return errorCode;
}

int set(char *words[], int wordCount) {
	int errorCode = 0;
	if(wordCount < 3) {
		errorCode = 5;
		return errorCode;
	}
	errorCode = store(words);
	return errorCode;
}

int print(char *words[], int wordCount) {
	int errorCode = 0;
	if(wordCount == 1) {
		errorCode = 5;
		return errorCode;
	}
	char *variable = strdup(words[1]);
	char *tmp = get(variable);
	char *string = strdup(tmp);
	if(strcmp(string, "Variable does not exist") == 0) {
		errorCode = 2;
		return errorCode;
	}
	printf("%s\n", string);
	return errorCode;
}

int run(char *words[], int wordCount) {
	int errorCode = 0;
	flag = 1;
	char line[1000];
	if(wordCount == 1) {
		errorCode = 3;
		return errorCode;
	} else {
		FILE *fptr;
		fptr = fopen(words[1], "r");
		if(fptr != NULL) {
			flag = 1;
			while(fgets(line, 999, fptr) != NULL) {
                parseInput(line);
            }
		} else {
			errorCode = 4;
			return errorCode;
		}
		fclose(fptr);
		flag = 0;
	}
	return errorCode;
}

