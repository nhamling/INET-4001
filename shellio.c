#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAXCOM 1000
#define MAXLIST 100

#define clear() printf("\033[H\033[J")

void main() {
	printf("working");
}

//Function that provides a display at the start of the shell.
void startPrompt() {
	clear();
	printf("\n\n**Welcome to HamShell**\n");
	sleep(1);
	clear();
}



//Function that takes user input
int takeIn(char* str) {
	char* buffer;

	buffer = readline("\n--> ");
	if (strlen(buffer) != 0) {
		add_history(buffer);
		strcpy(str, buffer);
		return 0;
	}
	else {
		return 1;
	}
}

//Function that executes system command
void execArg(char** parsed){
	//fork child
	int i = fork();
	
	if (i == -1) {
		printf("\nFailed to fork child");
		return;
	}
	else if (i == 0) {
		if (execvp(parsed[0], parsed) < 0) {
			printf("\nFailed to execute command");
		}
		exit(0);
	}
	//wait to for child to end
	else {
		wait(NULL);
		return;
	}
}

//prints current directory
void curDir() {
	char cd[1024];
	getcwd(cd, sizeof(cd));
	printf("\nDirectory: %s", cd);
}

//Shell Help
void openHelp() {
	puts("\n**Help Menu**"
	"\nBuilt in Commands:"
	"\n-->cd (Change Directory)"
	"\n-->exit"
	"\n-->help"
	"\n-->hello world"
	"\n**All other Linux commands are available**"
	);
}

//function that executes built in commands
int bICmd(char** parsed) {

	int cmdCount = 4;
	int switchArg = 0;
	char* cmdList[cmdCount];
	
	cmdList[0] = "exit";
	cmdList[1] = "cd";
	cmdList[2] = "help";
	cmdList[3] = "hello world";

	for (int i = 0; i < cmdCount; i++) {
		if (strcmp(parsed[0], cmdList[i]) == 0) {
			switchArg = i + 1;
			break;
		}
	}	
	switch (switchArg) {
	case 1:
		printf("\nSee you next time!\n");
		exit(0);
	case 2:
		chdir(parsed[1]);
		return 1;
	case 3:
		openHelp();
		return 1;
	case 4:
		printf("Hello World, this is Hamshell speaking");
		return 1;
	default:
		break;
	}
	return 0;
}

//Parses command words
void parseCmd(char* str, char** parsed) {
	for(int i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, " ");
		if(parsed[i] == NULL)
			break;
		if(strlen(parsed[i]) == 0)
			i--;
	}
}

//finds pipe
int parsepipe(char* str, char** pipedstr) {
	for(int i = 0; i < 2; i++) {
		pipedstr[i] = strsep(&str, "|");
		if(pipedstr[i] == NULL)
			break;
	}
	//pipe not found
	if(pipedstr[1] == NULL)
		return 0;
	else
		return 1;
}

//Function to process string
int processStr(char* str, char** parsed, char** ppipe) {
	char* strpiped[2];
	int piped = 0;
	piped = parsepipe(str, strpiped);

	if (piped) {
		parseCmd(strpiped[0], parsed);
		parseCmd(strpiped[1], ppipe);
	}
	else {
		parseCmd(str, parsed);
	}
	if (bICmd(parsed))
		return 0;
	else
		return 1 + piped;
}








