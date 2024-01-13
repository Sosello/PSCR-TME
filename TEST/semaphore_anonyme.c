#include <pthread.h>
#include <semaphore.h>

sem_t mutex; //

void *my_thread(void *arg){
    while(1){
        sem_wait (&mutex); // P( )
        sem_post(&mutex); // V ( )
    }
}

int main(){
    pthread_t thread1_id, thread2_id;
    sem_init(&mutex, 0, 1); // initialize mutex
    pthread_create(&thread1_id, NULL, &my_thread, NULL);
    pthread_create(&thread2_id, NULL, &my_thread, NULL);
    pthread_join(thread1_id, NULL);
    pthread_join(thread2_id, NULL);
    sem_destroy(&mutex);
}