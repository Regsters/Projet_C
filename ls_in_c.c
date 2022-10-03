#include <stdio.h>
#include <dirent.h>
#include <string.h>

int ls(const char* name){
	DIR* dir = opendir(name);
	printf("Reading files in: %s\n", name);
	if (dir == NULL){
		return 1;
		}
	struct dirent* entity;
	entity = readdir(dir);
	while(entity != NULL ){
		printf("%hhd %s\n", entity->d_type , entity->d_name);
		if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0){
			char path[100] = {0};
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


