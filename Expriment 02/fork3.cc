#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main() {
	int i, a = 0;
	pid_t pid;
	if (pid=fork()) a = 1;
	for (i = 0; i < 2; i++) {
		printf("X");
	}
	
	if (pid == 0) printf("%d\n", a);
	return 0;
}
