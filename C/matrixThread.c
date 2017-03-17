#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

//размер матрицы
const static int n = 3;

static int m = 1;
static int l = 0;
static int first[n][n];
static int second[n][n];
static int result[n][n];

//создать семафор
static pthread_mutex_t mutex;

//функция создания матриц
void *crt(void *p) {
    int k = 1;
    //бесконечный цикл
    while (1) {
        if (k == m && (k - 1) == l) {
            //заходим в семафор и закрываем дверь
            pthread_mutex_lock(&mutex);
            l++;
            //создаем две матрицы
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    first[i][j] = rand() % 9 - 0;
                    second[i][j] = rand() % 9 - 0;
                }
            }
            k++;
            //после работы открываем дверь и уходим
            pthread_mutex_unlock(&mutex);
            sleep(5);
        }
    }
}
//функция сумирования
void *sum(void *p) {
    int k = 1;
    while (1) {
        if (k == m && k == l) {
            //заходим в семафор и закрываем дверь
            pthread_mutex_lock(&mutex);
            //записываем суммы в результирующую матрицуу
            for (int i = 0; i < n ; ++i) {
                for (int j = 0; j < n ; ++j) {
                    result[i][j]=first[i][j]+second[i][j];
                }
            }
            k++;
            //уходим, открыв дверь
            pthread_mutex_unlock(&mutex);
            sleep(5);
        }
    }
}
//функция печатанья, все то же самое, как и в предыдущих двух
void *prt(void *p) {
    int k = 1;
    while (1) {
        if (k == m && k == l) {
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < n ; ++i) {
                for (int j = 0; j < n ; ++j) {
                    printf("%i ", first[i][j]);
                }
                printf("\n");
            }
            printf("+ \n");
            for (int i = 0; i < n ; ++i) {
                for (int j = 0; j < n ; ++j) {
                    printf("%i ", second[i][j]);
                }
                printf("\n");
            }
            printf("= \n");
            for (int i = 0; i < n ; ++i) {
                for (int j = 0; j < n ; ++j) {
                    printf("%i ", result[i][j]);
                }
                printf("\n");
            }
            printf("\n\n");
            k++;
            m++;
            pthread_mutex_unlock(&mutex);
            sleep(5);
        }
    }
}

int main() {
    //создаем три потока без ничего
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;
    //даем поток задания, запускаем без аргументов
    pthread_create(&thread_1, NULL, &crt, NULL);
    pthread_create(&thread_2, NULL, &sum, NULL);
    pthread_create(&thread_3, NULL, &prt, NULL);
    //ждем пока закончится поток 1, вечно
    pthread_join(thread_1, NULL);
    
    return 0;
}
