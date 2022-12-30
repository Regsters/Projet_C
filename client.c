#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>


void checkHostName(int hostname){
    if (hostname == -1){
        perror("gethostname");
        exit(1);
    }
}

void checkHostEntry(struct hostent * hostentry){
    if (hostentry == NULL){
        perror("gethostbyname");
        exit(1);
    }
}
void checkIPbuffer(char *IPbuffer){
    if (NULL == IPbuffer){
        perror("inet_ntoa");
        exit(1);
    }
}

int main (void){
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;

    hostname = gethostname(hostbuffer,sizeof(hostbuffer));
    checkHostName(hostname);

    host_entry = gethostbyname(hostbuffer);
    checkHostEntry(host_entry);

    IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
    printf("%s", IPbuffer);



    int sockid;
    int server_port = 8888;
    char *server_ip = "192.168.0.143";

    sockid = socket(AF_INET, SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    char *msg = IPbuffer;

    connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));

    send(sockid,(const char *)msg,strlen(msg),0);

    close(sockid);
}
