#include "../base/Queue.h"

void* func1(void *arg);
void* func2(void *arg);

int main(int argc, char *argv[]){

    SQueue *queue = new SQueue(10);
    pthread_t p1;
    pthread_t p2;

    pthread_create(&p1, NULL, func1, (void*)queue);
    pthread_create(&p2, NULL, func2, (void*)queue);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    delete queue;

    return 0;
}

void* func1(void *arg){
 
    SQueue *queue = (SQueue*) (arg);
    int num = 0;
    SBucket *bucket = new SBucket();
    while (num < 20) {
        bucket->size = num;
        printf("-------->%d\n", bucket->size);
        queue->Push(&bucket);
        num++;
        sleep(2);
    }
    delete bucket;
    return NULL;
}

void* func2(void *arg){

    SQueue *queue = (SQueue*) (arg);
    int num = 0;
    SBucket *bucket = new SBucket();
    while (num != 19) {
        queue->Pop(&bucket);
        printf("%d<--------\n", bucket->size);
        sleep(1);
    }
    delete bucket;
    return NULL;
}
