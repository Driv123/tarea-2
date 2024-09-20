#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 16

int array[ARRAY_SIZE];
int sum = 0;
pthread_mutex_t mutex; 

void* sum_array(void* arg) {
    int thread_part = *(int*)arg;
    int start = thread_part * (ARRAY_SIZE / NUM_THREADS);
    int end = (thread_part + 1) * (ARRAY_SIZE / NUM_THREADS);

    int local_sum = 0;
    for (int i = start; i < end; i++) {
        local_sum += array[i];
    }

    pthread_mutex_lock(&mutex);
    sum += local_sum;
    pthread_mutex_unlock(&mutex); 

    pthread_exit(0);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }
    pthread_mutex_init(&mutex, NULL);


    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, sum_array, (void*)&thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("La suma total es: %d\n", sum);

    return 0;
}
