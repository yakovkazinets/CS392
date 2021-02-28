/*
Yakov Kazinets and Tae Kim
Assignment: Minishell 05
Pledge    : I pledge my honor that I have abided by the Stevens Honor System.
Date      : April 12, 2020
*/
#include <errno.h>
#include <pthread.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BRIGHTBLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"

#define BUFSIZE 128

sigjmp_buf jmpbuf;

void catch_signal(int sig){
    write(STDOUT_FILENO, "\n", 1);
    siglongjmp(jmpbuf, 1);
}

int main(int argc, char *argv[]) {
    //SIGINT signal
    struct sigaction action;

    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = catch_signal;
    action.sa_flags = SA_RESTART;
    
    if (sigaction(SIGINT, &action, NULL)==-1){
        perror("sigaction");
        return EXIT_FAILURE;
    }
    
    char buf[BUFSIZE];
    sigsetjmp(jmpbuf, 1);
    
    while (true){
        if (strncmp(buf, "exit", 4) ==0){
            break;
        }   
        else if (argc > 2){
            fprintf(stderr, "Error: Too many arguements to cd.\n");
            return EXIT_FAILURE;
        }
        else if (strcmp(argv[0], "cd") == 0){ //commands relating to cd
            if (argc == 1 || strcmp(argv[1], "~") ==0){ //if only cd is sent to minishell or cd~

            }
            else { //if a command or directory along with cd is sent to minishell

            }
        } else if(strcmp(argv[0], "exit") == 0){ //if exit command is called break and exit success
            break;
        } else { //if we get a command that is not cd or exit
            fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));

        }
    }
    return EXIT_SUCCESS;
}

