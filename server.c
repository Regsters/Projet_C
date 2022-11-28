#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>


int main(){
    char server_message [256] = "You have research the server";
    //initialisation du socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //initialisation de l'addresse du serveur
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    //verification de la connection
    listen(server_socket, 1);

    int client_socket;
    //acceptation de la connection
    client_socket = accept(server_socket, NULL, NULL);
    //envoie du message
    send(client_socket, server_message, sizeof(server_message), 0);
    //fermeture du socket

    close(server_socket);

    return 0;
}