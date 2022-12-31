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
    
    
}
