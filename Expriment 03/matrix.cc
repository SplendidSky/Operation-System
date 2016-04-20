#include <pthread.h>
#include <stdio.h>

int M, N, K;
int **A;
int **B;
int **C;

struct d {
	int i;
	int j;
};

void* calculate(void *dd);

int main() {
	pthread_t *tid = new pthread_t[M * N];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	printf("enter M, N, K\n");
	scanf("%d %d %d", &M, &N, &K);
	A = new int*[M];
	B = new int*[K];
	C = new int*[M];
	printf("enter entries of A\n");
	for (int i = 0; i < M; i++) {
		A[i] = new int[K];
		for (int j = 0; j < K; j++) {
			scanf("%d", &A[i][j]);
		}
	}
	
	printf("enter entries of B\n");
	for (int i = 0; i < K; i++) {
		B[i] = new int[N];
		for (int j = 0; j < N; j++) {
			scanf("%d", &B[i][j]);
		}
	}
	
	for (int i = 0;i < M; i++) {
		C[i] = new int[N];
		for (int j = 0; j < N; j++) {
			d *data = new d;
			data->i = i;
			data->j = j;
			pthread_create(&tid[i * N + j], &attr, calculate, data);
			pthread_join(tid[i * N + j], NULL);
			delete data;
		}
	}
	
	printf("the result is:\n");
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", C[i][j]);
		}
		printf ("\n");
	}
}

void* calculate(void *dd) {
	///*
	printf("A\n");
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < K; j++) {
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}
	printf("B\n");
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", B[i][j]);
		}
		printf("\n");
	}
	//*/
	d data = * ((d*)(dd));
	C[data.i][data.j] = 0;
	printf("data.i: %d, data.j: %d \n", data.i, data.j);
	for (int k = 0; k < K; k++) {
		printf("A[%d][%d]: %d, B[%d][%d]: %d \n", data.i, k, A[data.i][k], k, data.j, B[k][data.j]);
		C[data.i][data.j] += A[data.i][k] * B[k][data.j];
	}
	pthread_exit(0);
}
