#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TOTAL_WRITES 6400
#define TOTAL_READS 25400

pthread_mutex_t mutex;
pthread_mutex_t write_mutex;
pthread_mutex_t read_mutex;

int readers_count = 0;
int reads_count = 0;
int writes_count = 0;

typedef struct {
    int id;
    int operations;
} thread_args;

void simulate_work(void) {
    for (int i = 0; i < 10000; i++);
}

int get_operation(int total, int count, int id) {
    return total / count + (id < total % count);
}

void* reader(void* arg){
    thread_args* args = (thread_args*)arg;
    for (int i = 0; i < args->operations; i++) {
        pthread_mutex_lock(&read_mutex);
        pthread_mutex_lock(&mutex);
        readers_count++;
        if (readers_count == 1) {
            pthread_mutex_lock(&write_mutex);
        }
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&read_mutex);
        simulate_work();
        reads_count++;
        pthread_mutex_lock(&mutex);
        readers_count--;
        if (readers_count == 0) {
            pthread_mutex_unlock(&write_mutex);
    }
        pthread_mutex_unlock(&mutex);


    }
    return NULL;

}

void* writer(void* arg){
    thread_args* args = (thread_args*)arg;
    for (int i = 0; i < args->operations; i++) {
        pthread_mutex_lock(&read_mutex);
        pthread_mutex_lock(&write_mutex);
        simulate_work();
        writes_count++;
        pthread_mutex_unlock(&write_mutex);
        pthread_mutex_unlock(&read_mutex);

    }
    return NULL;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        return EXIT_FAILURE;
    }

    int writers_num = atoi(argv[1]);
    int readers_num = atoi(argv[2]);

    if (writers_num <= 0 || readers_num <= 0) {
        return EXIT_FAILURE;
    }

    pthread_t* writers = malloc(writers_num * sizeof(pthread_t));
    pthread_t* readers = malloc(readers_num * sizeof(pthread_t));
    thread_args* writer_args = malloc(writers_num * sizeof(thread_args));
    thread_args* reader_args = malloc(readers_num * sizeof(thread_args));

    if (!writers || !readers || !writer_args || !reader_args) {
        free(writers);
        free(readers);
        free(writer_args);
        free(reader_args);
        return EXIT_FAILURE;
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);
    pthread_mutex_init(&read_mutex, NULL);

    for (int i = 0; i < writers_num; i++) {
        writer_args[i].id = i;
        writer_args[i].operations =
            get_operation(TOTAL_WRITES, writers_num, i);

        pthread_create(&writers[i], NULL, writer, &writer_args[i]);
    }

    for (int i = 0; i < readers_num; i++) {
        reader_args[i].id = i;
        reader_args[i].operations =
            get_operation(TOTAL_READS, readers_num, i);

        pthread_create(&readers[i], NULL, reader, &reader_args[i]);
    }

    for (int i = 0; i < writers_num; i++) {
        pthread_join(writers[i], NULL);
    }
    for (int i = 0; i < readers_num; i++) {
        pthread_join(readers[i], NULL);
    }

    printf("%d\n", reads_count);
    printf("%d\n", writes_count);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_mutex);
    pthread_mutex_destroy(&read_mutex);
    free(writers);
    free(readers);
    free(writer_args);
    free(reader_args);

    return EXIT_SUCCESS;
}