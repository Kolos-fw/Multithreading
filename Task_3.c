#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>

typedef struct {
    long *data;  
    int start;       
    int end;         
    long result;     
} Task;

void *worker(void *arg){
    Task *t = (Task *)arg;
    long sum = 0;
    for (int i = t->start; i < t->end; i++) {
        sum += t->data[i];
    }
    
    t->result = sum; 
    return NULL; 
}

int main(){
    long data[1000];
    long single_sum = 0;
    for (int i = 0; i < 1000; i++){
        data[i] = i;
        single_sum += data[i];
    }
    
    pthread_t threads[4];

    Task tasks[4];
    for (int i = 0; i < 4; i++){
        tasks[i] = (Task){ .start = i * 250, .data = data,
        .end = (i + 1) * 250};
        pthread_create(&threads[i], NULL, worker, &tasks[i]);
    }

    for (int i = 0; i < 4; i++){ pthread_join(threads[i], NULL); }
    long parallel_sum = tasks[0].result + tasks[1].result +
    tasks[2].result + tasks[3].result;

    printf("Однопоточная сумма: %ld\n", single_sum);
    printf("Многопоточная сумма: %ld\n", parallel_sum);

    return 0;
}
