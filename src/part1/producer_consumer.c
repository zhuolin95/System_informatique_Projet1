#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define BUFFER_SIZE 8
#define TOTAL_ITEMS 131072


pthread_mutex_t mutex;
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
sem_t empty_slots;
sem_t occupied_slots;
int produced_count = 0;
int consumed_count = 0;
int *consumed_items;

typedef struct{
    int id;
    int operations;
} thread_args;

void simulate_work(void) {
    for (int i = 0; i < 10000; i++) {
    }
}


void* producer(void* arg) {
    thread_args* args = (thread_args*)arg;
    for (int i = 0; i < args->operations;i++){
        simulate_work();
        sem_wait(&empty_slots);
        pthread_mutex_lock(&mutex);
        buffer[in] = args->id;
        in = (in + 1) % BUFFER_SIZE;
        produced_count++;
        pthread_mutex_unlock(&mutex);
        sem_post(&occupied_slots);
    }

    return NULL;
}

void* consumer(void* arg) {
    thread_args* args = (thread_args*)arg;
    for (int i = 0; i < args->operations; i++){
        sem_wait(&occupied_slots);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        consumed_count++;
        consumed_items[item]++;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_slots);
        simulate_work();
    }
    return NULL;
}


int get_operation(int total, int count, int id){
    return total / count + (id < total % count);
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        return EXIT_FAILURE;
    
    }

    int producers_num = atoi(argv[1]);
    int consumers_num = atoi(argv[2]);

    if (producers_num <= 0 || consumers_num <= 0) {
        return EXIT_FAILURE;
    }

    consumed_items = calloc(producers_num, sizeof(int));

    pthread_t* producers = malloc(producers_num * sizeof(pthread_t));
    pthread_t* consumers = malloc(consumers_num * sizeof(pthread_t));
    thread_args* producer_thread_args = malloc(producers_num * sizeof(thread_args));
    thread_args* consumer_thread_args = malloc(consumers_num * sizeof(thread_args));

    if (!producers || !consumers || !producer_thread_args || !consumer_thread_args) {
        free(producers);
        free(consumers);
        free(producer_thread_args);
        free(consumer_thread_args);
        return EXIT_FAILURE;
    }

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&occupied_slots, 0, 0);

    for (int i = 0; i < producers_num; i++) {
        producer_thread_args[i].id = i;
        producer_thread_args[i].operations = get_operation(TOTAL_ITEMS, producers_num, i);
        pthread_create(&producers[i], NULL, producer, &producer_thread_args[i]);
    }
    for (int i = 0; i < consumers_num; i++) {
        consumer_thread_args[i].id = i;
        consumer_thread_args[i].operations = get_operation(TOTAL_ITEMS, consumers_num, i);
        pthread_create(&consumers[i], NULL, consumer, &consumer_thread_args[i]);
    }

    for (int i = 0; i < producers_num; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < consumers_num; i++) {
        pthread_join(consumers[i], NULL);
    }

    printf("%d\n", produced_count);
    printf("%d\n", consumed_count);

    for (int i = 0; i < producers_num; i++) {
        int expected = get_operation(TOTAL_ITEMS, producers_num, i);

        printf("%d\n", i);
        printf("%d\n", expected);
        printf("%d\n", consumed_items[i]);
        
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&occupied_slots);
    free(producers);
    free(consumers);
    free(producer_thread_args);
    free(consumer_thread_args);

    return EXIT_SUCCESS;
}
