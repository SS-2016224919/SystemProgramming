#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STR_MAX 1024

main(int ac, char *av[]) {
	if (ac == 1)
		do_ls(".");
	if (av[1][0] == '-') {
		switch (av[1][1]) {
		case 'R':
			do_ls_all(".", 1);
			break;
		}
	} else
		while (--ac) {
			printf("%s:\n", *++av);
			do_ls(*av);
		}
}

do_ls(char dirname[]) {
	DIR *dir_ptr;
	struct dirent *direntp;

	if ((dir_ptr = opendir(dirname)) == NULL) {
		fprintf(stderr, "error");
	} else {
		while ((direntp = readdir(dir_ptr)) != NULL) {
			printf("%s\n", direntp->d_name);
		}
		closedir(dir_ptr);
	}
}

do_ls_all(const char *dpath, int depth) {
	DIR *dir_ptr;
	struct dirent *direntp;
	if (!(dir_ptr = opendir(dpath))){
		return;
	}
	if (!(direntp = readdir(dir_ptr))){
		return;
        }
	for (; direntp != NULL; direntp = readdir(dir_ptr)) {
		if (direntp->d_type == DT_DIR) {
			char path[STR_MAX];
			int len = snprintf(path, sizeof(path) - 1, "%s/%s", dpath,
					direntp->d_name);
			path[len] = 0;
			if (strcmp(direntp->d_name, ".") == 0
					|| strcmp(direntp->d_name, "..") == 0) {
				continue;
			}
			if (!((strlen(direntp->d_name) > 1) && (direntp->d_name[0] == '.'))) {
				printf("\nDirectory name : %s\n", path);
			}
/*
                        if( (depth > 1) && (strlen(direntp->d_name) > 1) && (direntp->d_name[0] == '.') ){
                           printf("len: %zd  name: %s", strlen(direntp->d_name), direntp->d_name);
                           return;
                        }
*/
			do_ls_all(path, depth + 1);
		} else {
			if (!((strlen(direntp->d_name) > 1) && (direntp->d_name[0] == '.'))) {
				printf(" %s\n", direntp->d_name);
			}
		}
	}
	closedir(dir_ptr);
}

