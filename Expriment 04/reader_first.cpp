#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>

using std::ifstream;

struct READER {
	int id;
	int delay;
	int duration;
};

struct WRITER {
	int id;
	int delay;
	int duration;
};


void *Read(void *param);
void *Write(void *param);



sem_t mutex;
sem_t _write;
int reader_count = 0;
READER readers[100];
WRITER writers[100]; 

int main(int argc, char *argv[]) {
	ifstream input("input.txt", std::ios::in);
	int r_id = 0, w_id = 0, delay, duration, id;
	char R_W;
	while (input >> id >> R_W >> delay >> duration) {
		if (R_W == 'R') {
			readers[r_id].id = r_id + 1;
			readers[r_id].delay = delay;
			readers[r_id].duration = duration;
			r_id++;
		}
		else if (R_W == 'W') {
			writers[w_id].id = w_id + 1;
			writers[w_id].delay = delay;
			writers[w_id].duration = duration;
			w_id++;
		}
	}
	
	pthread_t r[100], w[100];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	sem_init(&mutex, 0, 1);
	sem_init(&_write, 0, 1);
	
	for (int i = 0; i < r_id; i++) {
		int *_i = new int;
		*_i = i;
		pthread_create(&r[i], &attr, Read, (void *)_i);
	}
	for (int i = 0; i < w_id; i++) {
		int *_i = new int;
		*_i = i;
		pthread_create(&w[i], &attr, Write, (void *)_i);
	}
	for (int i = 0; i < r_id; i++)
		pthread_join(r[i], NULL);
	for (int i = 0; i < w_id; i++)
		pthread_join(w[i], NULL);
	return 0;
}

void *Read(void *param) {
	int *id = (int *)param;
	sleep(readers[*id].delay);
	if (reader_count == 0)
		sem_wait(&mutex);
	printf("Reader %d begins reading.\n", readers[*id].id);
	reader_count++;
	sleep(readers[*id].duration);
	printf("Reader %d finishes reading.\n", readers[*id].id);
	delete id;
	reader_count--;
	if (reader_count == 0) {
		printf("I'm the last reader, no reader is reading now.\n");
		sem_post(&mutex);
	}
}

void *Write(void *param) {
	int *id = (int *)param;
	sleep(writers[*id].delay);
	sem_wait(&_write);
	sem_wait(&mutex);
	printf("Writer %d begins writing.\n", writers[*id].id);
	sleep(writers[*id].duration);
	printf("Writer %d finishes writing.\n", writers[*id].id);
	sem_post(&mutex);
	sem_post(&_write);
	delete id;
}

