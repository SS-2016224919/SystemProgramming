#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>

#define STR_MAX 1024


void do_ls(char[]);
void do_ls_all_recursive(char[], int);
void dostat(char *);
void show_file_info( char *, struct stat *);
void mode_to_letters( int , char [] );
char *uid_to_name( uid_t );
char *gid_to_name( gid_t );

main(int ac, char *av[]) {
	if (ac == 1)
		do_ls(".");
	if (av[1][0] == '-') {
		switch (av[1][1]) {
		case 'R':
			do_ls_all_recursive(".", 1);
			break;
		}
	} else
		while (--ac) {
			printf("%s:\n", *++av);
			do_ls(*av);
		}
}

void do_ls(char dirname[]) {
	DIR *dir_ptr;
	struct dirent *direntp;

	if ((dir_ptr = opendir(dirname)) == NULL) {
		fprintf(stderr, "open error %s\n", dirname);
	} else {
		while ((direntp = readdir(dir_ptr)) != NULL) {
                	dostat( direntp->d_name );
		}
		closedir(dir_ptr);
	}
}

void do_ls_all_recursive(char dirname[], int depth) {
	DIR *dir_ptr;
	struct dirent *direntp;
	if ((dir_ptr = opendir(dirname)) == NULL ){
		fprintf(stderr, "open error %s\n", dirname);
	}else{
  		if( (direntp = readdir(dir_ptr)) == NULL ){
			fprintf(stderr, "read error \n");
		}else{
			for ( ; direntp != NULL ; direntp = readdir(dir_ptr) ){
				if (direntp->d_type == DT_DIR){
					char path[STR_MAX];
					int len = snprintf(path, sizeof(path) - 1, "%s/%s", dirname,
							direntp->d_name);
					path[len] = 0;
					if (strcmp(direntp->d_name, ".") == 0
							|| strcmp(direntp->d_name, "..") == 0){
						continue;
					}
					if (!((strlen(direntp->d_name) > 1) && (direntp->d_name[0] == '.'))){
						printf("\nDirectory name : %s\n", path);
					}
/*
       	                 if( (depth > 1) && (strlen(direntp->d_name) > 1) && (direntp->d_name[0] == '.') ){
       	                    printf("len: %zd  name: %s", strlen(direntp->d_name), direntp->d_name);
       	                    return;
       	                 }
*/
					do_ls_all_recursive(path, depth + 1);
				} else {
					if (!((strlen(direntp->d_name) > 1) && (direntp->d_name[0] == '.'))) {
						dostat( direntp->d_name );
					}
				}
			}
		}
		closedir(dir_ptr);
	}
}


void dostat( char *filename )
{
	struct stat info;

	if ( stat(filename, &info) == -1 )	
		perror( filename );		
	else					
		show_file_info( filename, &info );
}

void show_file_info( char *filename, struct stat *info_p ){
	char	*uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void	mode_to_letters();
        char    modestr[11];

	mode_to_letters( info_p->st_mode, modestr );

	printf( "%s"    , modestr );
	printf( "%4d "  , (int) info_p->st_nlink);	
	printf( "%-8s " , uid_to_name(info_p->st_uid) );
	printf( "%-8s " , gid_to_name(info_p->st_gid) );
	printf( "%8ld " , (long)info_p->st_size);
	printf( "%.12s ", 4+ctime(&info_p->st_mtime));
	printf( "%s\n"  , filename );

}
void mode_to_letters(int mode, char str[] ){
    strcpy( str, "----------" );

    if ( S_ISDIR(mode) )  str[0] = 'd';
    if ( S_ISCHR(mode) )  str[0] = 'c';
    if ( S_ISBLK(mode) )  str[0] = 'b';

    if ( mode & S_IRUSR ) str[1] = 'r';
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( mode & S_IXUSR ) str[3] = 'x';

    if ( mode & S_IRGRP ) str[4] = 'r';
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( mode & S_IXGRP ) str[6] = 'x';

    if ( mode & S_IROTH ) str[7] = 'r';
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( mode & S_IXOTH ) str[9] = 'x';
}

char *uid_to_name( uid_t uid ){
	struct	passwd *getpwuid(), *pw_ptr;
	static  char numstr[10];

	if ( ( pw_ptr = getpwuid( uid ) ) == NULL ){
		sprintf(numstr,"%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name ;
}

char *gid_to_name( gid_t gid ){
	struct group *getgrgid(), *grp_ptr;
	static  char numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
		sprintf(numstr,"%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}
