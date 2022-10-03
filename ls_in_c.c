#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int MAX_NUMBRE_FILE = 100;



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
		stat_file(entity->d_name);
		const char* ext = strrchr(entity->d_name, '.');
		if(!ext){
		
		}
		else{
			printf("extension is %s\n", ext +1);
		}
		/* problème gestion d_type, ne donne que 8 et 4 et pas le reste*/
		if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0){
			char path[100] = {0};    /*Peut-être une alocation dynamique si possible*/
			strcat(path, name);
			strcat(path, "/");
			strcat(path, entity->d_name);
			ls(path);
		}entity = readdir(dir);
	}
	closedir(dir);
	return 0;
}

void main(){
	ls(".");
	

}


