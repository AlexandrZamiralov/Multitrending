#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

//функция, печатающая матрицу
void prt_mtr(int *m, int rows, int cols) {
    int i;
    for (i = 0; i < rows * cols; ++i) {
        //перенос, когда номер элемента кратен количеству столбцов
        if (i % cols == 0) {
            printf("\n");
        }
        printf("%i ", m[i]);
    }
    printf("\n");
}

int main() {
    
    int rows = 3;
    int cols = 3;
    int *first;
    int *second;
    char *cmd;
    int *result;
    //два процесса работают с матрицами, а оставшийся с символами. выделим им память
    int shmem1 = shmget(0, rows * cols * sizeof(int), IPC_CREAT | 0666);
    int shmem2 = shmget(0, rows * cols * sizeof(int), IPC_CREAT | 0666);
    int shmem4 = shmget(0, rows * cols * sizeof(int), IPC_CREAT | 0666);
    int shmem3 = shmget(0, sizeof(char), IPC_CREAT | 0666);
    if (shmem1 == -1 || shmem2 == -1 || shmem3 == -1|| shmem4 == -1) {
    fprintf(stderr, "Error while allocating shared memory!\n");
        return 1;
    }
    //матрицы и символ
    first = (int *) shmat(shmem1, 0, 0);
    second = (int *) shmat(shmem2, 0, 0);
    result = (int *) shmat(shmem4, 0, 0);
    cmd = (char *) shmat(shmem3, 0, 0);
    if (first == (int *) -1 || second == (int *) -1 || cmd == (char *) -1|| result == (int*) -1) {
        fprintf(stderr, "Error while assigning shared memory!\n");
        return 1;
    }
    //сначала все процессы неработают
    pid_t crt = -1;
    pid_t sum = -1;
    pid_t prt = -1;
    //запустим процесс создания
    crt = fork();
    //чтобы что-то проссумировать, надо что-то иметь
    if (crt > 0) {
        sum = fork();
    }
    //чтобы напечатать ответ, надо проссумировать
    if (crt > 0 && sum > 0) {
        prt = fork();
    }
    //установим индекатор нажатия кнопки на 'c'
    cmd[0] = 'c';
    //бесконечный цикл
    while (1) {
        //если все работает, то продолжаем
        if (crt > 0 && sum > 0 && prt > 0) {
            char op;
            scanf("%c", &op);
            //если кто-то нажал на 'x', то остановим программу
            if (op == 'x') {
                cmd[0] = 'x';
                printf("Process %i instruct to end.\n", getpid());
                return 0;
            }
        }
        //если запущен криэйт, и индекатор с. то генерируем
        if (crt == 0) {
            if (cmd[0] == 'c') {
                printf("Process %i: creating two %ix%i matrices.\n", getpid(), rows, cols);
                int i;
                for (i = 0; i < rows * cols; ++i) {
                    first[i] = (int) (random() % 9 - 0);
                    second[i] = (int) (random() % 9 - 0);
                }
                //в штатном режиме переходим на следующий шаг
                if (cmd[0] != 'x') {
                    cmd[0] = 'p';
                    sleep(0.5);
                }
                //при вызове выхода, заканчиваем работу
            } else if (cmd[0] == 'x') {
                printf("Process %i go to end.\n", getpid());
                return 0;
            }
        }
        //если запущен сум, и криэйт передал эстафету
        if (sum == 0) {
            if (cmd[0] == 'p') {
                //начинаем считать
                printf("Process of summation %i has began.\n", getpid());
                int i;
                for (i = 0; i < rows * cols; ++i) {
                   result[i]= first[i] + second[i];
                }
                //в штатном режиме переходим на следующий шаг
                if (cmd[0] != 'x') {
                    cmd[0] = 's';
                    sleep(0.5);
                }
                //при вызове выхода, заканчиваем работу
            } else if (cmd[0] == 'x') {
                printf("Process %i go to end.\n", getpid());
                return 0;
            }
        }
        //если принт запущен, и сум передал эстафету
        if (prt == 0) {
            if (cmd[0] == 's') {
                //печатаем условие и  результат
                printf("Process %i printing result.\n", getpid());
                prt_mtr(first, rows, cols);
                printf("+");
                prt_mtr(second, rows, cols);
                printf("=");
                prt_mtr(result, rows, cols);
                //в штатном режиме, передаем права опять криэйту и повторяем круг
                if (cmd[0] != 'x') {
                    cmd[0] = 'c';
                    sleep(4);
                }
                //при вызове выхода, заканчиваем работу
            } else if (cmd[0] == 'x') {
                printf("Process %i go to end.\n", getpid());
                return 0;
            }
        }
    }
}
