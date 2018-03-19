#include <stdio.h> 
#include <string.h>   
#include <sys/socket.h>    
#include <arpa/inet.h> 
#include <unistd.h>
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[2000] , server_reply[2000], bye[100] = "bye";
	int portName = atoi(argv[1]);     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(portName);
 
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
    
    while(1)
    {
        
        printf("Enter message : ");
        scanf("%s" , message);
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        if (!strcmp(message, bye)){
            if( recv(sock , server_reply , 2000 , 0) < 0)
            {
                puts("recv failed");
                break;
            }
         
            puts("Server reply :");
            puts(server_reply);

            break;
        }
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);
        bzero(message, 2000);
        bzero(server_reply, 2000);
    }
     
    close(sock);
    return 0;
}