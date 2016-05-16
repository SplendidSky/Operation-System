#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

enum STATE {thinking, hungry, eating};

class dp {
	private:
	void test(int k) {
		if ((state[(k + 4) % 5] != eating) && (state[k] == hungry) && (state[(k + 1) % 5] != eating)) {
			state[k] = eating;
			sem_wait(&self[k]);
			sem_wait(&self[(k + 1) % 5]);
			//sem_post(&self[k]);
		}
	}
	STATE state[5];
	sem_t self[5];
	sem_t mutex;
	int self_count[5];
	

	public:
	dp() {
		for (int i = 0; i < 5; i++) {
			state[i] = thinking;
			sem_init(&self[i], 0, 1);
			sem_init(&mutex, 0, 1);
		}	
	}
	
	void pickup(int i) {
		state[i] = hungry;
		test(i);
		if (state[i] == eating)
			printf("Philosopher %d is eating.\n", i);
	}
	
	void putdown(int i) {
		if (state[i] == eating) {
			printf("Philosopher %d finishes eating.\n", i);
			state[i] = thinking;
		}
		sem_post(&self[i]);
		sem_post(&self[(i + 1) % 5]);
		test((i + 4) % 5);
		test((i + 1) % 5);
	}
	

};

dp DP;

void *dinning(void *param) {
	int *i = (int *)param;
	sleep(rand() % 3);
	DP.pickup(*i);
	sleep(rand() % 3);
	DP.putdown(*i);
}

int main() {
	pthread_t p[5];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	for (int i = 0; i < 5; i++) {
		int *_i = new int;
		*_i = i;
		pthread_create(&p[i], &attr, dinning, (void *)_i);
	}
	
	for (int i = 0; i < 5; i++)
		pthread_join(p[i], NULL);
	
	return 0;
}
