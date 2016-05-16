#include "buffer.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t mutex;
sem_t empty;
sem_t full;
int size = 0;
buffer_item buffer[BUFFER_SIZE];

int insert_item(buffer_item item) {
	if (size >= BUFFER_SIZE)
		return -1;
	else {
		size = size + 1 == BUFFER_SIZE ? 0 : size + 1;
		buffer[size] = item;
		return 0;
	}
}

int remove_item(buffer_item *item) {
	if (size < 0)
		return 0;
	*item = buffer[size];
	size = size - 1 == -1 ? 0 : size - 1;
	return 0;
}


void *producer(void *param) {
	//printf("producer\n");
	buffer_item r;
	while (true) {
		sleep(rand() % 3);
		sem_wait(&empty);
		sem_post(&full);
		sem_wait(&mutex);
		r = rand();
		printf("producer produced %d\n", r);
		if (insert_item(r))
			printf("report error condidtion\n");
		sem_post(&mutex);
	}
}

void *consumer(void *param) {
	//printf("consumer\n");
	buffer_item r;
	while (true) {
		sleep(rand() % 3);
		sem_wait(&full);
		sem_post(&empty);
		sem_wait(&mutex);
		if (remove_item(&r))
			printf("report error condidtion\n");
		else
			printf("consumer consumed %d\n", r);
		sem_post(&mutex);
	}
}


int main(int argc, char *argv[]) {
	//pthread_mutex_init(&mutex, NULL);
	if (argc != 4) {
		printf("Invaid parameters\n");
		return 0;
	}
	pthread_t p[500], c[500];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 5);
	for (int i = 0; i < atoi(argv[2]); i++)
		pthread_create(&p[i], &attr, producer, NULL);
	for (int i = 0; i < atoi(argv[3]); i++)
		pthread_create(&c[i], &attr, consumer, NULL);
	sleep(atoi(argv[1]));
	//for (int i = 0; i < size; i++)
		//printf("%d: ", buffer[i]);
	return 0;
}

