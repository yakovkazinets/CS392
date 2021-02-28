/*
Yakov Kazinets
I pledge my honor that I have abided by the Stevens Honor System.
Permission Finder Assignment 3*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>
#include <dirent.h>

int findpaths(char *dir,int permint){
	DIR *curr; //the current directory being used!
	char pathstr[PATH_MAX];
    char filepath[PATH_MAX];
	struct dirent *file_place;
    struct stat stat1;
	size_t pathlength;
    if (realpath(dir, pathstr) == NULL){	
        fprintf(stderr, "Error: Cannot get full path of file '%s'. %s\n", dir, strerror(errno));
        return EXIT_FAILURE;
    }
    if ((curr=opendir(pathstr)) == NULL){ 
        fprintf(stderr, "Error: Cannot open directory '%s'. %s\n", pathstr, strerror(errno));
        return EXIT_FAILURE;
    }
    filepath[0] = '\0';
    if (strcmp(pathstr, "/")){
        strncpy(filepath, pathstr, PATH_MAX);
    }
    pathlength = strlen(filepath) + 1;
    filepath[pathlength-1]='/';
	filepath[pathlength]='\0';
    while ((file_place = readdir(curr)) != NULL){
        if (strcmp(file_place->d_name, ".") == 0 || strcmp(file_place->d_name, "..")==0){
            continue;
        }
        strncpy(filepath+pathlength, file_place->d_name, PATH_MAX-pathlength);
        if (lstat(filepath, &stat1)<0){
            fprintf(stderr, "Error: Cannot stat file '%s'. %s\n", filepath, strerror(errno));
            continue;
        }
        if((stat1.st_mode-(stat1.st_mode & S_IFMT))==permint){
				printf("%s\n", filepath);
		} 
        if (file_place->d_type == DT_DIR){
			findpaths(filepath, permint);
        }
    }
    closedir(curr);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv){
    int optint;
	bool doptbool = false;
    bool poptbool = false;
	char *dir; 
	char *permissions;
	// Will handle the input with getopts and checks if the flags given are correct, if not it exits with failure!
	while ((optint = getopt(argc,argv, ":d:p:h")) != -1){
		switch(optint) {
			case 'd':
				doptbool = true;
				dir = optarg;
				break;
			case 'p':
				poptbool = true;
				permissions = optarg;
				break;
			case 'h':
				puts("Usage: ./pfind -d <directory> -p <permissions string> [-h]");
				return EXIT_SUCCESS;
			case '?':
				fprintf(stderr, "Error: Unknown option '-%c' recieved.\n", optopt);
				return EXIT_FAILURE;
		}
	}
    //checks if -d was used at all, if not, exit with failure
	if(doptbool == false) {
		printf("Error: Required argument -d <directory> not found.\n");
		return EXIT_FAILURE;
	}
    //checks if -p was used at all, if not, exit with failure
	if(poptbool == false) {
		printf("Error: Required argument -p <permissions string> not found.\n");
		return EXIT_FAILURE;
	}
	FILE *my_file = fopen(dir, "r");
	if(my_file == NULL){ // File open error
		fprintf(stderr,"Error: Cannot stat '%s'. No such file or directory.\n", dir);
		return EXIT_FAILURE;
	}
	//Check the permission string!
	if(strlen(permissions) != 9){ //String length
		fprintf(stderr,"Error: Permissions string '%s' is invalid.\n", permissions);
		return EXIT_FAILURE;
	}else{ 
		for(int i=0;i<9;i+=3){
			//As per class examples: Check 3 positions each in a for loop.
			if(permissions[i] !='r'&& permissions[i] != '-'){
				fprintf(stderr,"Error: Permissions string '%s' is invalid.\n", permissions);
				return EXIT_FAILURE;
			}
			if(permissions[i + 1] !='w'&& permissions[i + 1] != '-'){
				fprintf(stderr,"Error: Permissions string '%s' is invalid.\n", permissions);
				return EXIT_FAILURE;
			}
			if(permissions[i + 2] !='x'&& permissions[i + 2] != '-'){
				fprintf(stderr,"Error: Permissions string '%s' is invalid.\n",permissions);
				return EXIT_FAILURE;
			}
		}
	}
	//the bitshifting
	int int1 = 0;
	int shifterbit = 1;
	int int2 = 0;
	for(int i=0;i<9;i++){
	    int2 = 8 - i; // Need to access from right to left
		if(permissions[int2] != '-'){
			int1 += shifterbit;
		}
		shifterbit = shifterbit << 1;
	}
    //the very first recursive call!
	findpaths(dir,int1);
    //closing the file at the end as it is no longer needed to be open 
	fclose(my_file);
	return EXIT_SUCCESS; //if we reach this point everything went very well!
}


