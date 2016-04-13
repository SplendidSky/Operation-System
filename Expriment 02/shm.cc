#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

using std::ostringstream;
using std::string;

#define PERM S_IRUSR|S_IWUSR
#define MAX_SEQUENCE 10

typedef struct {
	long fib_sequence[MAX_SEQUENCE];
	int sequence_size;
} shared_data;

int main(int argc, char **argv)
{
	// check whether the parameters are correct
	if (argc != 2) {
		printf("Invalid parameters!\n");
		exit(0);
	}
	int sequence = atoi(argv[1]);
	if (sequence > 10 || sequence < 0) {
		printf("Invalid size of sequences!\n");
		exit(0);
	}
	
	// create share memory
	// if it fails, exit with returning 1
	key_t shmid;
	if ((shmid = shmget(IPC_PRIVATE, 84, PERM)) == -1) {
		fprintf(stderr, "Create share memory eroor:%s\n\a", strerror(errno));
		exit(1);
	}
	
	// create a child process
	char* data;
	pid_t pid = fork();
	// in parent process
	if (pid > 0) {
		// share memory is read-only to parent process
		data = (char*)shmat(shmid, NULL, 1);
		wait(NULL);
		char fib[100];
		// copy data from child process to fib
		strncpy(fib, data, 84);
		printf("%s\n", fib);
		// release the share memory
		shmctl(shmid, IPC_RMID, NULL);
	}
	// in child process
	else {
		data = (char*)shmat(shmid, NULL, 0);
		string fib_seq;
		shared_data fib;
		fib.sequence_size = sequence;
		// calculate the fibonacci sequence
		for (int i = 0; i < sequence; i++) {
			if (i == 0 || i == 1)
				fib.fib_sequence[i] = 1;
			else
				fib.fib_sequence[i] = fib.fib_sequence[i - 1] + fib.fib_sequence[i - 2];
		}
		
		// use ostringstream to convert long type to string
		ostringstream os;
		for (int i = 0; i < sequence; i++) {
			os << fib.fib_sequence[i] << " ";
		}
		fib_seq = os.str();
		
		// write the fibnacci sequence to data
		sprintf(data, (const char*)fib_seq.c_str());
	}
	return 0;
}
