#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int fd[2];
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

//основная функция(запускает дфс + читает ответ)
int run(int graphlength);
//обходит граф, запуская ото всюду процессы
void dfs(int vertex, int graphlength);



int run(int graphlength)
{
    //Создадим трубу
    pipe(fd);
    int count = 0;
    //запишем то, сколько у нас уже есть процессы
    write(fd[1], &count, sizeof(int));
    //запускаем с первой вершины
    dfs(0,graphlength);
        
    
    //читаем ответ
    read(fd[0], &count, sizeof(int));
    return count;
}
    void dfs(int vertex, int graphlength){
        
        int i;
        pid_t whereami=-1;
        for (i=0; i<graphlength; i++){
            if (graph[i][0] == vertex){
                printf("Process %i, from vertex %i, to %i, parent pid %i \n",getpid(),vertex,graph[i][1],getppid());
                //запускаем процесс из каждой вершины
                whereami = fork();
            }
            if (whereami == 0){
                break;
            }
        }
        if (whereami == 0){
            //рекурентно запускаем обход дальше
            dfs(graph[i][1],graphlength);
        }
        
        // увеличиваем число, идущее в ответ
        int count;
        //читаем из трубы
        read(fd[0], &count, sizeof(int));
        count++;
        //пишем в трубу
        write(fd[1], &count, sizeof(int));
        
        // Вершина-процесс отключается
        printf(" i am a new, my name is %i, and our new count = %i\n", getpid(),count);
        exit(0);
    }


int main()
{
    //длина графа, нужна для работы функций
    int graphlength=sizeof(graph)/(2*sizeof(int));
    //первый процесс
    pid_t initialpid=getpid();
    //ответ
    int answer = run(graphlength);
    //когда останется только первый процесс, то можем выводить ответ
    printf("Vertices-process count %i\n", answer);
    return 0;
}
