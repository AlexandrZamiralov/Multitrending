#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(){
    int welcomeSocket, newSocket, portNum, clientLen, nBytes; // дескрипторы сокетов
    char buffer[1024]; // буфeр для приема
    struct sockaddr_in serverAddr; // структура с адресом
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    int i;
    //cоздаем сокет TCP Домен сокета, тип сокета,протокол передачи данных
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    //port tcp
    portNum = 7891;
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    //локал хост 127.0.0.1
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    //связывает сокет с адрессом
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    //можем принимать 5 соединений одновременно
    if(listen(welcomeSocket,5)==0)
        printf("Listening\n");
    else
        printf("Error\n");
    
    addr_size = sizeof serverStorage;
    
    //чтобы продолжить принимать запросы от клиентов создаем вечный цикл
    while(1){
        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
        //создаем для каждого запроса дочерний процесс
        if(!fork()){
            nBytes = 1;
            //пока дочерний процесс жив
            while(nBytes!=0){
                //получаем сообщение из сокета
                nBytes = recv(newSocket,buffer,1024,0);
                
                for (i=0;i<nBytes-1;i++){
                    //повышаем регистр
                    buffer[i] = toupper(buffer[i]);
                }
                //передаем данные в сокет
                send(newSocket,buffer,nBytes,0);
            }
            //закрываем сокет
            close(newSocket);
            exit(0);
        }
        //если родительский процесс, то закрываем розетку и идем дальше ждать запроса
        else{
            close(newSocket);
        }
    }
    
    return 0;
}
