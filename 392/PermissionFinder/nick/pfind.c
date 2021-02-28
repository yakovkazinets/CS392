/**
* Author: Nicholas Szegheo
* Date:	3/8/2020
* Pledge: I pledge my honor that I have abided by the Stevens Honor System.
* Description: CS392 Assignment 3: Permission Finder.
**/

#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int recurse(char *dir_string, int perm_int){
	//The part of the program that produces the expected output by recursing.

	DIR *current_dir;
	char path_string[PATH_MAX];
	struct dirent *entry;
    struct stat sb;
	char file_path[PATH_MAX];
	size_t path_len;

	
    if (realpath(dir_string, path_string) == NULL) { //path_string initialized here
        fprintf(stderr, "Error: Cannot get full path of file '%s'. %s.\n", dir_string, strerror(errno));
        return EXIT_FAILURE;
    }

    if ((current_dir = opendir(path_string)) == NULL) {
        fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n", path_string, strerror(errno));
        return EXIT_FAILURE;
    }

	//Initialize as empty
    file_path[0] = '\0';

    if (strcmp(path_string, "/")) {
        strncpy(file_path, path_string, PATH_MAX);
    }
	
    path_len = strlen(file_path) + 1;
    file_path[path_len - 1] = '/';
	file_path[path_len] = '\0';

    while ((entry = readdir(current_dir)) != NULL) {
		//Reading files
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			//Skip current directory and parent
            continue;
        }
		
        strncpy( file_path + path_len, entry->d_name, PATH_MAX - path_len );
		
        if (lstat(file_path, &sb) < 0) { //Stat file
            fprintf(stderr, "Error: Cannot stat file '%s'. %s.\n", file_path, strerror(errno));
            continue;
        }

		if((sb.st_mode - (sb.st_mode & S_IFMT)) == perm_int){ //Check permissions
				printf("%s\n", file_path);
		
		} 
        if (entry->d_type == DT_DIR) { //Directory case - Recursive call
			recurse(file_path,perm_int);
        }
    }
	//Cleanup
    closedir(current_dir);
    return EXIT_SUCCESS;
}



int main(int argc, char **argv){
	
	int opt;
	bool d_opt = false, p_opt = false;
	char *directory; 
	char *permstr;
	
	
	//Handle input with getopt

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
				printf("Usage: %s -d <directory> -p <permissions string> [-h]\n", argv[0]);
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
	if(fp == NULL){ //File open error
		fprintf(stderr,"Error: Cannot stat '%s'. No such file or directory.\n", directory);
		return EXIT_FAILURE;
	}

	// Validate permstring
	if(strlen(permstr) != 9){ //String length check
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
		ind = 8 - i; //Need to access from right to left
		if(permstr[ind] != '-'){
			perm_int += shift;
		}
		shift = shift << 1;
	}
	
	//Initial Recursive call
	recurse(directory,perm_int); 
	
	//Cleanup
	fclose(fp);
	return EXIT_SUCCESS;
}


