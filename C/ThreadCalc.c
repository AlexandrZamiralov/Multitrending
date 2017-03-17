#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <memory.h>
#include <fcntl.h>
#include <pthread.h>

int graph[][2] = {
    {0, 1},
    {0, 2},
    {1, 3},
    {1, 4},
    {2, 5},
    {4, 6},
    {4, 7},
    {5, 8},
    {6, 9}
};
//аргументы для тредов из дфс
struct arg_struct {
    int* count;
    int vertex;
    int graphlength;
};

//так же, как  и в 3 задачи, только вместо процессов потоки
void* dfs(void *arguments){
    struct arg_struct *args = arguments;
    int i,k=0,threads=0,vertex=args->vertex;
    
    for (i=0;i<args->graphlength;i++){
        if (graph[i][0] == vertex){
            threads++;
        }
    }
    
    pthread_t newthread[threads];
    pthread_t* pnt=&newthread[0];
    //рекурсивно передаем треду задачу запустить треды дальше
    for (i=0;i<args->graphlength;i++){
        if (graph[i][0] == vertex){
            (*(args->count))++;
            printf("New thread from vertex %i to %i\n",args->vertex,graph[i][1]);
            pthread_create(pnt+k, NULL, dfs, (void*) (args+graph[i][1]-vertex));
            pthread_join(*(pnt+k),NULL);
            k++;
        }
    }
    //выходим из треда, чтобы остался в конце 1
    pthread_exit(NULL);
}


int main() {
    //выделяем общую память, для работы с графом
    int shmid = shmget(0, sizeof(int), IPC_CREAT | 0666);
    int* count = (int*) shmat(shmid, NULL, 0);
    int i;
    int graphlength=sizeof(graph)/(2*sizeof(int));
    shmid = shmget(0, (graphlength+2)*sizeof(struct arg_struct), IPC_CREAT | 0666);
    struct arg_struct* arguments = (struct arg_struct*) shmat(shmid,NULL,0);
    
    pthread_t zerothread;

    struct arg_struct zeroargs;
    *count = 1;
    //cоздаем аргументы для каждого треда
    for(i=0;i<graphlength+1;i++){
        (arguments+i)->count = count;
        (arguments+i)->vertex = i;
        (arguments+i)->graphlength=graphlength;
    }
    
    
    pthread_create(&zerothread, NULL, dfs, (void*) arguments);
    //когда останется нулевой тред, то напечатаем ответ
    pthread_join(zerothread,NULL);
    printf("Count = %i\n",*count);
    
    pthread_exit(NULL);
    
}
