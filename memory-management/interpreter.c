#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"
#include "ram.h"
#include "memorymanager.h"

int runFlag = 0;
int execFlag = 0;

void help(char *words[]);

void quit(char *words[]);

int set(char *words[], int wordCount);

int print(char *words[], int wordCount);

int run_script(char *words[], int wordCount);

int exec(char *words[], int wordCount);

int interpreter(char *words[], int wordCount) {							//assumes cmd switchs args
																		//assumes words[0] is cmd
	int errorCode = 0;

	if (strcmp(words[0], "help") == 0 && wordCount == 1) {
		help(words);
	} else if (strcmp(words[0], "quit") == 0 && wordCount == 1) {
		quit(words);
	} else if (strcmp(words[0], "set") == 0) {
		errorCode = set(words, wordCount);
	} else if (strcmp(words[0], "print") == 0) {
		errorCode = print(words, wordCount);
	} else if (strcmp(words[0], "run") == 0) {
		errorCode = run_script(words, wordCount);
	} else if (strcmp(words[0], "exec") == 0) { 
		errorCode = exec(words, wordCount);
	} else {
		errorCode = 1;
	}
	if(errorCode == 1) {                       //Unknown command error
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
	else if(errorCode == 10) {
		printf("Unsuccessful Launch, program execution aborted!\n");
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
	printf("exec p1 p2 p3\t\t\tExecutes concurrent programs\n\t\t\t\t$ exec prog.txt prog2.txt\n");
}

void quit(char *words[]) {
	if(runFlag == 1) {
		runFlag = 0;
		return;
	}
	if(execFlag > 0) {
		execFlag--;
		return;
	}
	printf("Bye!\n");
	exit(99);
}

int set(char *words[], int wordCount) {
	int errorCode = 0;
	if(wordCount == 1 || wordCount == 2) {
		errorCode = 5;
		return errorCode;
	} else {
		errorCode = store(words);
		return errorCode;
	}
}

int print(char *words[], int wordCount) {
	int errorCode = 0;
	if(wordCount == 1) {
		errorCode = 5;
		return errorCode;
	} else {
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
}

int run_script(char *words[], int wordCount) {
	int errorCode = 0;
	char line[1000];
	if(wordCount == 1) {
		errorCode = 3;
		return errorCode;
	} else {
		FILE *fptr;
		if(fptr = fopen(words[1], "r")) {
			runFlag = 1;
			while(fgets(line, 999, fptr) != NULL) {
				parseInput(line);
			}
		} else {
			errorCode = 4;
			return errorCode;
		}
		fclose(fptr);
		runFlag = 0;
		return errorCode;
	}
}

int exec(char *words[], int wordCount) {
	int errorCode = 0;
	int launch = 0;
	char *filename;
	int i;
	if(wordCount == 1) {
		errorCode = 5;
		return errorCode;
	} else if(wordCount > 4) {
		errorCode = 7;
		return errorCode;
	} else if(wordCount == 3 && strcmp(words[1], words[2]) == 0) {
		printf("Error: Script %s already loaded\n", words[1]);
		return 0;
	} else if(wordCount == 4 && (strcmp(words[1],words[2]) == 0 || strcmp(words[1], words[3]) == 0 || strcmp(words[2], words[3]) == 0)) {
		if(strcmp(words[1],words[2]) == 0 || strcmp(words[1], words[3]) == 0) {
			printf("Error: Script %s already loaded\n", words[1]);
		} else {
			printf("Error: Script %s already loaded\n", words[2]);
		}
		return 0;
	} else {
		for(i=1; i<wordCount; i++) {
			filename = words[i];
			fileName = filename;
			FILE *fptr = fopen(filename, "r");
			if(fptr == NULL) {							//file cannot be open, abort exec program
				errorCode = 8;
				clearRAM(0, 40);
				if(i > 1) {
					clearReady();
				}
				return errorCode;
			}
			launch = launcher(fptr);
			if(launch == 0) {							//unsuccessful launch, abort exec program
				errorCode = 10;
				clearRAM(0, 40);
				clearReady();
				return errorCode;
			}
			execFlag++;
		}
		errorCode = scheduler();
		system("rm BackingStore/*");					//delete the copied files
		execFlag = 0;
		return errorCode;
	}
}