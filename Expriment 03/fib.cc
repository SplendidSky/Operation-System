#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* fib(void* p);
int fib_seq[47];

int main(int argc, char**argv) {
	if (argc != 2) {
		printf("Invalid parameters!\n");
		exit(0);
	}
	int n = atoi(argv[1]);
	if (n < 0 || n > 47) {
		printf("Invalid parameters!\n");
		exit(0);
	}
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, fib, &n);
	pthread_join(tid, NULL);
	for (int i = 0; i < n; i++)
		printf("%d ",fib_seq[i]);
	printf("\n"); 
}

void* fib(void* p) {
	int *pp = (int *)p;
	int n = *pp;
	int f = 0, s = 1;
	fib_seq[0] = 0;
	fib_seq[1] = 1;
	for (int i = 2; i < n; i++) {
		fib_seq[i] = fib_seq[i - 1] + fib_seq[i - 2];
	}
}
