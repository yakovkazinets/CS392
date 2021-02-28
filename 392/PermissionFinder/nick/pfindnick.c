/*
Yakov Kazinets
I pledge my honor that I have abided by the Stevens Honor System.
*/


#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int recurse(char *directory,int perm){
	// The part of the program that produces the expected output by recursing.

	DIR *dir;
	char path[PATH_MAX];
	struct dirent *entry;
    struct stat sb;
	char filepath[PATH_MAX];
	size_t pathlen;
	
    if (realpath(directory, path) == NULL) {	//Copies entire path into "path"
        fprintf(stderr, "Error: Cannot get full path of file '%s'. %s\n", directory, strerror(errno));
        return EXIT_FAILURE;
    }

    if ((dir = opendir(path)) == NULL) { //Opens directory into dir
        fprintf(stderr, "Error: Cannot open directory '%s'. %s\n", path, strerror(errno));
        return EXIT_FAILURE;
    }

    filepath[0] = '\0';
    if (strcmp(path, "/")) {
        strncpy(filepath, path, PATH_MAX);
    }
	
    pathlen = strlen(filepath) + 1;
	
    filepath[pathlen - 1] = '/';
    
	filepath[pathlen] = '\0';

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
		
        strncpy(filepath + pathlen, entry->d_name, PATH_MAX - pathlen);
		
        if (lstat(filepath, &sb) < 0) {
            fprintf(stderr, "Error: Cannot stat file '%s'. %s\n", filepath, strerror(errno));
            continue;
        }
        if (entry->d_type == DT_DIR) { //If directory.
			
			if((sb.st_mode - (sb.st_mode & S_IFMT)) == perm){ //compare permission strings
				printf("%s\n", filepath);
			}
			recurse(filepath,perm);
			
        } else { //if file
			
			if((sb.st_mode - (sb.st_mode & S_IFMT)) == perm){ //compare permission strings
				printf("%s\n", filepath);
			}
        }
    }

    closedir(dir);
    return EXIT_SUCCESS;
}


int main(int argc, char **argv){
	
	int opt;
	bool d_opt = false, p_opt = false;
	char *directory; 
	char *permstr;
	
	
	// Handle input with getopt

	while ((opt = getopt(argc,argv, ":d:p:h")) != -1){
		switch(opt) {
			case 'd':
				d_opt = true;
				directory = optarg;
				break;
			case 'p':
				p_opt = true;
				permstr = optarg;
				break;
			case 'h':
				puts("Usage: ./pfind -d <directory> -p <permissions string> [-h]");
				return EXIT_SUCCESS;
			case '?':
				fprintf(stderr, "Error: Unknown option '-%c' recieved.\n", optopt);
				return EXIT_FAILURE;
		}
	}
	
	if(d_opt == false) {
		printf("Error: Required argument -d <directory> not found.\n");
		return EXIT_FAILURE;
	}
	if(p_opt == false) {
		printf("Error: Required argument -p <permissions string> not found.\n");
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(directory, "r");
	if(fp == NULL){ // File open error
		fprintf(stderr,"Error: Cannot stat '%s'. No such file or directory.\n", directory);
		return EXIT_FAILURE;
	}

	// Validate permstring
	if(strlen(permstr) != 9){ //String length
		fprintf(stderr,"Error: Permissions string '%s' is invalid.\n", permstr);
		return EXIT_FAILURE;
	}
	else{ 
		for(int i = 0; i < 9; i += 3){
			//As per class examples: Check 3 positions each in a for loop.
			if(permstr[i] != 'r' && permstr[i] != '-')
			{
				fprintf(stderr,"Error: Permissions string '%s' is invalid.\n", permstr);
				return EXIT_FAILURE;
			}
			
			if(permstr[i + 1] != 'w' && permstr[i + 1] != '-')
			{
				fprintf(stderr,"Error: Permissions string '%s' is invalid.\n", permstr);
				return EXIT_FAILURE;
			}
			
			if(permstr[i + 2] != 'x' && permstr[i + 2] != '-')
			{
				fprintf(stderr,"Error: Permissions string '%s' is invalid.\n",permstr);
				return EXIT_FAILURE;
			}
		}
	}
	
	//Use bitshifting to get an integer value for the permissions

	int perm_int = 0;
	int shift = 1;
	int ind = 0;
	for(int i = 0; i <= 8; i++){
		ind = 8 - i; // Need to access from right to left
		if(permstr[ind] != '-'){
			perm_int += shift;
		}
		shift = shift << 1;
	}
	
	recurse(directory,perm_int); 
	
	fclose(fp);
	return EXIT_SUCCESS;
}


