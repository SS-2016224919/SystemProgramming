#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <dirent.h>

#define BUFFER_SIZE 512

int main(int ac, char *av[]){
	if(ac != 3){
		printf("./2016224919_PE4 source target\n");
		return 0;
	}
	if(strcmp(av[1], av[2])==0){
		printf("'%s' and '%s' area the same file\n", av[1], av[2]);
		return 0;
	}
	
	char buffer[BUFFER_SIZE];
	FILE* fin;
	FILE* fout;
	DIR* fout_dir;
	int rlength;

	fin = fopen(av[1], "rb"); // open original file as read only binary mode
	if(fin == NULL){
		printf("fopen() failed, %s not found \n", av[1]);
		return 0;
	}

	fout_dir = opendir(av[2]);
	if(fout_dir == NULL){ 
		fout = fopen(av[2], "wb"); // open target file as write binary mode
		if(fout == NULL){
			printf("fopen() filed, %s not fount \n", av[2]);
			return 0;
		}
	}else{ // target is directory
		int fullPathLength = strlen(av[2])+strlen(av[1]);
		char* fullPath;
		fullPath = (char *)malloc(strlen+1);
		strcpy(fullPath, av[2]);
		strcat(fullPath, "/");
		strcat(fullPath, av[1]);
		fout = fopen(fullPath, "wb"); // open target file as write binary mode
		if(fout == NULL){
			printf("fopen() filed, %s not fount \n", fullPath);
			return 0;
		}		
	}
	// copy original file to target file
	while ((rlength = fread(buffer, 1, BUFFER_SIZE, fin)) > 0){
		if(rlength == -1){
			printf("fread() failed");
			return 0;
		}
		fwrite(buffer, 1, rlength, fout);
	}
	fclose(fin);
	fclose(fout);
	//remove original file
	if (unlink(av[1]) != 0){
		printf("file (%s) delete fail. \n", av[1]);
	}
	return 0;
}
