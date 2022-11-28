#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    int network_socket;
    //initialisation du socket
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    //message d'erreur de connection
    if (connection_status == -1){
        printf("There was an error in the connection \n");
    }

    char server_rep[256];
    //reception du message
    recv(network_socket, &server_rep, sizeof (server_rep), 0);

    //affichage du message
    printf("The server sent the data: %s\n", server_rep);

    //fermeture du socket
    close(network_socket);
    return 0;
}