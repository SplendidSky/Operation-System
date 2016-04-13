#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main() {
	int pid1 = fork();
	if (pid1 != 0) {
		cout << "a\n";
		int pid2 = fork();
		if (pid2 == 0)
			cout << "c\n";
	}
	else
		cout << "b\n";
	return 0;
}
