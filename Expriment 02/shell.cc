#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string>
#include <string.h>

using std::string;

#define MAX_LINE 80
#define BUFFER_SIZE 10
string buffer[BUFFER_SIZE];
int command_NO = 1;  //indicate the count of commands
char inputBuffer[MAX_LINE];

void setup(char inputBuffer[], char *args[], int *background);
void handle_SIGINT(int);

int main() {
	int background;
	char *args[MAX_LINE / 2 + 1];
	pid_t pid;
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	sigaction(SIGINT, &handler, NULL);
	
	while (1) {
		//set if it is allowed to run background
		background = 0;		
		
		while (inputBuffer[0] == '\0') {
			printf("COMMAND-> ");
			fgets(inputBuffer, MAX_LINE, stdin);
		}
		
		setup(inputBuffer, args, &background);
		if ((pid = fork()) == -1) 
			printf("Fork Error\n");
		if (pid == 0) {
			execvp(args[0], args);
			exit(0);
		}
		if (background == 0) wait(0);
		inputBuffer[0] = '\0';
	}
}

void setup(char inputBuffer[], char* args[], int *background) {
	int length, start = -1, ct = 0;
	
	length = strlen(inputBuffer);
	//save the command to buffer
	buffer[command_NO % BUFFER_SIZE] = string(inputBuffer);
	command_NO++;

	if (length == 0) exit(0);
	if (length < 0) {
		perror("error reading the command");
		exit(-1);
	}
	
	//contruct args
	for (int i = 0; i < length; i++) {
		switch(inputBuffer[i]) {
			case ' ':
			case '\t':
				if (start != -1)
					args[ct++] = &inputBuffer[start];
				inputBuffer[i] = '\0';
				start = -1;
				break;
				
			case '\n':
				if(start != -1)
					args[ct++] = &inputBuffer[start];
				inputBuffer[i] = '\0';
				args[ct] = NULL;
				break;
			
			case '&':
				*background = 1;
				inputBuffer[i] = '\0';
				break;
			
			default:
				if(start == -1)
					start = i;
		}
	}
	args[ct] = NULL;
	fflush(NULL);
}

void handle_SIGINT(int signum) {
	write(STDOUT_FILENO, "\nCaught Control C\n", 19);
	
	//print the command_NO and its corresponding command
	for (int i = (command_NO > BUFFER_SIZE) ? command_NO - BUFFER_SIZE : 1; i <= command_NO - 1; i++) {
		char str[50];
		sprintf(str, "%d: ", i);
		write(STDOUT_FILENO, str, strlen(str));
		write(STDOUT_FILENO, buffer[i % BUFFER_SIZE].c_str(), buffer[i % BUFFER_SIZE].length());
	}
	
	//check the command in history mode, if it is correct, update inputBuffer so that it can be pharsed and run later
	char historyBuffer[MAX_LINE];
	int length;
	fflush(NULL);
	length = read(STDIN_FILENO, historyBuffer, MAX_LINE);
	
	//if the command is not in a corret format, return
	if (historyBuffer[0] != 'r' || historyBuffer[2] > 'z' || historyBuffer[2] < 'a' || historyBuffer[3] != '\n') {
		write(STDOUT_FILENO, "wrong command\n", 15);
		inputBuffer[0] = '\0';
		return;
	}
	else {
		int background;
		pid_t pid1;
		char *args[MAX_LINE / 2 + 1];
		for (int i = 0; i <= BUFFER_SIZE; i++) {
			if (historyBuffer[2] == buffer[i][0]) {
				for (int j = 0; j < buffer[i].length(); j++)
					inputBuffer[j] = buffer[i][j];
				break;
			}
		}
	}
	fflush(NULL);
}

