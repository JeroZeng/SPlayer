#include "../base/queue.h"

void* func1(void *arg);
void* func2(void *arg);

int main(int argc, char *argv[]){

    SQueue *queue = new SQueue();
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
    while (num < 20) {
        SData *data = new SData();
        data->num = num;
        queue->Push(data);
        printf("-------->%d\n", num);
        num++;
        sleep(1);
    }
    return NULL;
}

void* func2(void *arg){

    SQueue *queue = (SQueue*) (arg);
    int num = 0;
    SData *data = NULL;
    while (num != 19) {
        data = queue->Pop();
        if (data != NULL) {
            num = data->num;
            printf("%d<--------\n", num);
            delete data;
        } else {
            printf("+++++++++++++\n");
        }
        sleep(2);
    }
    return NULL;
}
