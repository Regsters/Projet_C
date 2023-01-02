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
    
    char line[200];
    char *victim_ip;
    char key[160];
    
    FILE * fp = fopen("cle.txt", "r");
    if(fp == NULL){
    	printf("Erreur lors de l'ouverture");
    	return 1;
    }
    if (fgets(line, 200, fp) != NULL){
    	strcpy(key,line);
    }
    if (fgets(line, 200, fp) != NULL){
    	strcpy(victim_ip, line);
    }
    fclose(fp);
    //récupération de la clé et de l'ip sur le fichier
    printf("key : %s, ip : %s", key, victim_ip);
    
    int server_port = 8888;
    int sockid;
    sockid = socket(AF_INET, SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(victim_ip);
    
    
    if(connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
    	printf("Erreur lors de la connection");
    	exit(1);
    }
    send(sockid,(const char *)key,strlen(key),0);
    close(sockid);
    
}
