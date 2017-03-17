#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
    int clientSocket, portNum, nBytes; // дескрипторы сокетов
    char buffer[1024]; // буфeр для приема
    struct sockaddr_in serverAddr; // структура с адресом
    socklen_t addr_size;
    
    //cоздаем сокет Домен сокета, тип сокета,протокол передачи данных
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    //port tcp
    portNum = 7891;
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    //локал хост 127.0.0.1
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    addr_size = sizeof serverAddr;
    //конектимся к серверу
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    //бесконечный цикл работы
    while(1){
        printf("Type a sentence to send to server:\n");
        //читаем ввод
        fgets(buffer,1024,stdin);
        //наглядно выводим  его
        printf("You typed: %s",buffer);
        //количество передаваемых байтов выделяем
        nBytes = strlen(buffer) + 1;
        //отправляем в сокет
        send(clientSocket,buffer,nBytes,0);
        //читаем с обновленного сокета
        recv(clientSocket, buffer, 1024, 0);
        //выводим, обработанный сервером, текст
        printf("Received from server: %s\n\n",buffer);   
    }
    
    return 0;
}
