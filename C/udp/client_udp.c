#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
    int clientSocket, portNum, nBytes; // дескрипторы сокетов
    char buffer[1024]; // буфeр для приема
    struct sockaddr_in serverAddr; // структура с адресом
    socklen_t addr_size;
    
    //создаем UDP сервер
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    
    //настраиваем на подключение к локалхосту
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    
    addr_size = sizeof serverAddr;
    //бесконечный цикл
    while(1){
        printf("Type a sentence to send to server:\n");
        //считываем
        fgets(buffer,1024,stdin);
        //печатаем для проверки
        printf("You typed: %s",buffer);
        
        nBytes = strlen(buffer) + 1;
        
        //отправляем сообщение серверу
        sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
        
        //считываем сообщение от сервера
        nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL);
        //печатаем ответ
        printf("Received from server: %s\n",buffer);
        
    }
    
    return 0;
}
