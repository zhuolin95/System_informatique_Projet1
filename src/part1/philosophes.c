#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define cycles 1000000

int N;
pthread_mutex_t *chopsticks;

void* philosopher(void* arg){
    int id = *(int*)arg;
    int left = id;
    int right = (id+1)%N;

    for(int i=0; i<cycles; i++){
        if(left<right){
            pthread_mutex_lock(&chopsticks[left]);
            pthread_mutex_lock(&chopsticks[right]);
        }else{
            pthread_mutex_lock(&chopsticks[right]);
            pthread_mutex_lock(&chopsticks[left]);
        }

        if(left<right){
            pthread_mutex_unlock(&chopsticks[right]);
            pthread_mutex_unlock(&chopsticks[left]);
        }else{
            pthread_mutex_unlock(&chopsticks[left]);
            pthread_mutex_unlock(&chopsticks[right]);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <number_of_philosophers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    N = atoi(argv[1]);
    if (N<=1){
        return EXIT_SUCCESS;
    }

    pthread_t *threads = malloc(N*sizeof(pthread_t));
    int *ids = malloc(N*sizeof(int));
    chopsticks = malloc(N*sizeof(pthread_mutex_t));

    for(int i=0; i<N; i++){
        pthread_mutex_init(&chopsticks[i], NULL);
    }

    for(int i=0; i<N; i++){
        ids[i]=i;
        pthread_create(&threads[i],NULL,philosopher,&ids[i]);
    }

    for(int i=0; i<N; i++){
        pthread_join(threads[i],NULL);
    }

    for(int i=0; i<N; i++){
        pthread_mutex_destroy(&chopsticks[i]);
    }
    free(threads);
    free(ids);
    free(chopsticks);

    return EXIT_SUCCESS;
}