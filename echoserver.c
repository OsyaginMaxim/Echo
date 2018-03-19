#include <stdio.h>
#include <string.h>    
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h> 
#include <unistd.h>   
#include <stdlib.h>

int maximum(int* arr, int n){
    int i = 1, tmp = arr[0];
    for(;i < n; i++){
        if(tmp < arr[i])
            tmp = arr[i];
    }
    return tmp;
}

int main(int argc , char *argv[])
{
    int i, iter = 0, socket_d , newSocket , socketLen , readSize, clients[100], portName, max, val, fd[10];
    struct sockaddr_in server , client;
    char message[2000], bye[3] = "bye";
    fd_set rset, wset;
    memset(&server, 0,sizeof(server));

    for(i = 0; i < 10; i++){
        fd[i] = 0;
    }



    socket_d = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_d == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    fcntl(socket_d, F_SETFL, O_NONBLOCK);
    portName = atoi(argv[1]);
    if(portName < 50000 || portName > 55000){
        puts("Port name invalide!");
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portName);

    
    while( bind(socket_d,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("Bind failed! Port %d busy. Port will change.\n",portName );
        if( portName < 54999){
            portName++;
            server.sin_port = htons(portName);
        }else if( portName > 50000){
            portName--;
            server.sin_port = htons(portName);
        }else{
            return 1;
        }
    }
    puts("bind done");
    
    listen(socket_d , 16);
    
    puts("Waiting for incoming connections...");
    socketLen = sizeof(client);
    while(1){
        FD_ZERO(&rset);
        FD_SET(socket_d, &rset);

        //уставнавливаем значения в листе с помощью FD_SET
        for(i = 0; i <= iter; i++){
            FD_SET(fd[i], &rset);
        }
        max = maximum(fd, iter);
        if(socket_d > max){
            max = socket_d;
        }
        if (select(max + 1, &rset, NULL, NULL, NULL) <=0){
            printf("SElect error");
        }
        if (FD_ISSET(socket_d, &rset)){
            newSocket = accept(socket_d, (struct sockaddr *)&client, (socklen_t*)&socketLen);
            if (newSocket < 0){
                printf("accept failed");
                return 1;
            }
            printf("connected iter: %d", iter);
            fcntl(newSocket, F_SETFL, O_NONBLOCK);
            fd[iter] = newSocket;
            iter++;
        }

        for(i = 0; i <= iter; i++){
            if(iter == 0) continue;
            if(FD_ISSET(fd[i], &rset)){
                readSize = recv(fd[i], message, 2000, 0);
                if((readSize <= 0)||(message == bye)){
                    close(fd[i]);
                    fd[i] = 0;
                    printf("Client disconnected");
                    continue;
                }
                send(fd[i], message, 2000, 0);
                bzero(message, 2000);
            }
        }

    }
    close(newSocket);
    close(socket_d);
     
    return 0;
}