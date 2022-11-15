#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(){
    struct sockaddr_in address_sock;
    address_sock.sin_family = AF_INET;
    address_sock.sin_port = htons(4242);
    inet_aton ("127.0.0.1", &address_sock.sin_addr);

    int descr = socket(PF_INET,SOCK_STREAM, 0);
    int r=connect(descr, (struct socketaddr *));
    
}