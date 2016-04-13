#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

bool wait_mark;

void stop_waiting(int signum) {
	wait_mark = false;
}

void waiting() {
	while (wait_mark != false);
}

int main() {
	pid_t pid1 = fork();
	if (pid1 != 0) {
		pid_t pid2 = fork();
		if (pid2 != 0) {
			wait_mark = true;
			signal(SIGINT, stop_waiting);
			waiting();
			kill(pid1, SIGUSR1);
			kill(pid2, SIGUSR2);
			wait(0);
			wait(0);
			printf("parent process is killed!\n");
			exit(0);
		}
		else {
			wait_mark = true;
			signal(SIGINT, SIG_IGN);
			signal(SIGUSR2, stop_waiting);
			waiting();
			printf("child process 1 is killed by parent!\n");
			exit(0);
		}
	}
	else {
		wait_mark = true;
			signal(SIGINT, SIG_IGN);
			signal(SIGUSR1, stop_waiting);
			waiting();
			printf("child process 2 is killed by parent!\n");
			exit(0);
	}
	return 0;
}


