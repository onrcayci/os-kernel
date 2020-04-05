#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shellmemory.h"
#include "shell.h"

int flag;											//flag for quit method

void help(char *words[]);							//help command method

void quit(char *words[]);							//quit command method

int set(char *words[]);								//set command method

int print(char *words[]);							//print command method

int run(char *words[]);								//run command method

int interpreter(char *words[]) {					//assumes cmd switchs args
													//assumes words[0] is cmd
	int errorCode = 0;

	if (strcmp(words[0], "help") == 0) {
		help(words);
	} else if (strcmp(words[0], "quit") == 0) {
		quit(words);
	} else if (strcmp(words[0], "set") == 0) {
		errorCode = set(words);
	} else if (strcmp(words[0], "print") == 0) {
		errorCode = print(words);
	} else if (strcmp(words[0], "run") == 0) {
		errorCode = run(words);
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
	}
	return errorCode;
};
void help(char *words[]) {
	printf("These shell commands are defined internally. Type 'help' to see this list.\n\n");
	printf("COMMAND\t\t\t\tDESCRIPTION\n\n");
	printf("help\t\t\t\tdisplays all commands\n");
	printf("quit\t\t\t\texits / terminates the shell with 'Bye!'\n");
	printf("set VAR STRING\t\t\tassigns a value to shell memory\n");
	printf("print VAR\t\t\tdisplays the STRING assigned to VAR\n");
	printf("run SCRIPT.TXT\t\t\texecutes the file SCRIPT.TXT\n");
}

void quit(char *words[]) {
	printf("Bye!\n");
	exit(0);
}

int set(char *words[]) {
	int errorCode = 0;
	errorCode = store(words);
	return errorCode;
}

int print(char *words[]) {
	int errorCode = 0;
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

int run(char *words[]) {
	int errorCode = 0;
	char line[1000];
	if(words[1] == NULL) {
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
	}
	return errorCode;
}

