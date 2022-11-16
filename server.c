#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void fin (int i);
void appli(int no_client_socket);
char buffer[512];
int my_socket;

void main(void){
    int client_socket;
    struct sockaddr_in my_address, client_address;
    int my_address_len, lg;

    bzero(&my_address,sizeof(my_address));
    my_address.sin_port = htons(30000);
    my_address.sin_addr.s_addr = htonl(INADDR_ANY);

    /*creation of the socket*/
    if ((my_socket = socket(AF_INET, SOCK_STREAM, 0))== -1){
        printf("test\n");
        exit(0);
    }
    signal(SIGINT,fin);
    bind(my_socket,(struct sockaddr *) &my_address,sizeof(my_address));
    listen(my_socket,5);

    my_address_len = sizeof(client_address);

    while(1){
        client_socket = accept(my_socket,(struct sockaddr *)&client_address, &my_address_len);
        if (fork()==0){
            close(my_socket);
            
            lg = read(client_socket,buffer,512);
            printf("The server receved : %s\n", buffer);
            sprintf(buffer,"%s of the server", buffer);
            write(client_socket, buffer,512);
            shutdown(client_socket,2);
            close(client_socket);
            exit(0);
        }
    }
    shutdown(my_socket,2);
    close(my_socket);
}

void appli(int no_client_socket){
    printf("here is the server");

}

void end(int i){
    shutdown(my_socket, 2);
    close(my_socket);
}