/*******************************************************************************
 * Name        : spfind.c
 * Author      : Tae Hwan Kim and Yakov Kazinets
 * Date        : 3/31/2020
 * Description : Sorted Permission Finder
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
 
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


void print_usage() {
	printf("Usage: ./spfind -d <directory> -p <permissions string> [-h]\n");
}


int main(int argc, char *argv[]) {
	if (argc == 1) {
		print_usage();
		return EXIT_FAILURE;
	}
	
	int p_to_sort[2], sort_to_parent[2];
	pipe(p_to_sort);
	pipe(sort_to_parent);
	
	pid_t pid[2];
	if ((pid[0] = fork()) == 0) {  //pfind
		close(p_to_sort[0]);
		dup2(p_to_sort[1], STDOUT_FILENO);
		
		//close other file descriptors
		
		close(sort_to_parent[0]);
		close(sort_to_parent[1]);
		
		if (execv("pfind", argv) == -1) {
            fprintf(stderr, "Error: pfind failed. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
		
	}
	
	if ((pid[1] = fork()) == 0) {  //sort
		close(p_to_sort[1]);
		dup2(p_to_sort[0], STDIN_FILENO);
		close(sort_to_parent[0]);
		dup2(sort_to_parent[1], STDOUT_FILENO);
		
		
		
		if (execlp("sort", "sort", NULL) == -1) {
            fprintf(stderr, "Error: sort failed. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
		
	}
	
	close(sort_to_parent[1]);
	dup2(sort_to_parent[0], STDIN_FILENO);
	//close all other descriptors
	close(p_to_sort[0]);
	close(p_to_sort[1]);
	
	char buffer[4096];
	int num = 0;
	while (1) {	//while (true)
		ssize_t count = read(sort_to_parent[0], buffer, sizeof(buffer)-1);
		//printf("Count val is: %ld\n", count);
		for(int x = 0; x < count; x++) {
			if(buffer[x] == '\n') {
				num++;
			}
		}
		if (count == -1) {
			if (errno == EINTR) {
				//num++;
				continue;
			} else {
				perror("read()");
				exit(EXIT_FAILURE);
			}
		} else if (count == 0) {
			break;
		} else {
			
			write(STDOUT_FILENO, buffer, count);
			
		}
		
		printf("Total matches: %d\n", num);
	}
	
	//buffer[num]='\0';
	//char jimbo = "wc -l" + STDIN_FILENO;
	
	
	return EXIT_SUCCESS;
}
	
		