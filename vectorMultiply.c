
#include <stdio.h>
#include <unistd.h>

#include "my_pthread_t.h"

#define DEFAULT_THREAD_NUM 2

#define VECTOR_SIZE 1000

my_pthread_mutex_t   mutex;

int thread_num;

int* counter;
my_pthread_t *thread;

int r[VECTOR_SIZE];
int s[VECTOR_SIZE];
int res = 0;

/* A CPU-bound task to do vector multiplication */
void vector_multiply(void* arg) {
	int i = 0;
	int n = *((int*) arg);
	
	for (i = n; i < VECTOR_SIZE; i += thread_num) {
		my_pthread_mutex_lock(&mutex);
		res += r[i] * s[i];
		my_pthread_mutex_unlock(&mutex);		
	}
}

void verify() {
	int i = 0;
	res = 0;
	for (i = 0; i < VECTOR_SIZE; i += 1) {
		res += r[i] * s[i];	
	}
	printf("verified res is : %d\n", res);
}

int main(int argc, char **argv) {
	int i = 0;

	if (argc == 1) {
		thread_num = DEFAULT_THREAD_NUM;
	} else {
		if (argv[1] < 1) {
			printf("enter a valid thread number\n");
			return 0;
		} else
			thread_num = atoi(argv[1]);
	}

	// initialize counter
	counter = (int*)malloc(thread_num*sizeof(int));
	for (i = 0; i < thread_num; ++i)
		counter[i] = i;

	// initialize pthread_t
	thread = (my_pthread_t*)malloc(thread_num*sizeof(my_pthread_t));

	// initialize data array
	for (i = 0; i < VECTOR_SIZE; ++i) {
		r[i] = i;
		s[i] = i;
	}

	my_pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < thread_num; ++i)
		my_pthread_create(&thread[i], NULL, &vector_multiply, &counter[i]);

	for (i = 0; i < thread_num; ++i)
		my_pthread_join(thread[i], NULL);

	printf("res is: %d\n", res);

	my_pthread_mutex_destroy(&mutex);

	// feel free to verify your answer here:
	 verify();

	// Free memory on Heap
	free(thread);
	free(counter);

	return 0;
}
