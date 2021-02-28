/*
Authors   : Yakov Kazinets and Tae Kim
Assignment: Minishell 05
Pledge    : I pledge my honor that I have abided by the Stevens Honor System.
Date      : April 15, 2020
*/
#include <errno.h>
#include <limits.h>
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

#define BUFSIZE 4096 
#define PATH_MAX 4096 //max number of letters in a path to be supported
#define MAX_ARGUEMENTS 64

sigjmp_buf jmpbuf;
pid_t p;

sig_atomic_t jimbo;
void printcurrentdir(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) !=NULL){
            printf("[%s%s%s]$ ", BRIGHTBLUE, cwd,DEFAULT);
    }else{
          fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
          exit(EXIT_FAILURE);
    }
}

void catch_signal(int sig){
    int check;
    if(jimbo == 0){
        printf("\n");
        
    }
    if((check = waitpid(p, NULL, WNOHANG)) == -1){
        write(STDOUT_FILENO, NULL, 1);
  }
    siglongjmp(jmpbuf, 1);
    jimbo = 1;
}

int cd(char *path){
    if (strcmp(path, "~") == 0){
        struct passwd *pw;
        if ((pw = getpwuid(getuid())) == NULL){
            fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
        if (chdir(pw->pw_dir) == -1){
            fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", path , strerror(errno));
            return EXIT_FAILURE;
        }
    }else{
        if (chdir(path) == -1){
            fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", path , strerror(errno));
            return EXIT_FAILURE;
        }
    }
    return 0;
}

void parse(char *line, char **argv){
    while (*line != '\0') {       // if not the end of line 
        while (*line == ' ' || *line == '\t' || *line == '\n'){
            *line++ = '\0';     // replace white spaces with 0 
                        
        }
        *argv++ = line;  // save the argument position
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n'){
            line++;
        }                  // skip the argument until ... 
    }
    *argv = '\0';                 // mark the end of argument list 
}

int  execute(char **argv){
    pid_t  pid;
    int    status;

    if ((pid = fork()) < 0) {
            fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
    }
    else if (pid == 0) {                   // for the child process:
        if (execvp(*argv, argv) < 0) {     // execute the command  
            fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        jimbo = 0; 
    }
    else {    
          
        p =pid;                             // for the parent:      
        while (wait(&status) != pid);       // wait for completion  
    }
    return 0;
}

int main(){
    //signal handler for SIGINT ^C
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = catch_signal;
    action.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &action, NULL)==-1){
        fprintf(stderr, "Error: Cannot register signal handler. %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    sigsetjmp(jmpbuf, 1);

    char buf[BUFSIZE];
    char *args[MAX_ARGUEMENTS + 1];

    // Main Loop
    while (true){
        printcurrentdir();
        fflush(stdout);
        ssize_t  bytes_read = read(STDIN_FILENO, buf, BUFSIZE -1);
        if(bytes_read > 0){
            buf[bytes_read - 1] = '\0';
        }
        if (bytes_read == 1){
            continue;
        }                  //copied from in class code not sure if the bytes_read ==1 is needed but kept just in case
        if (strncmp(buf, "exit", 4) ==0){ //checking the exit
            break;
        } 
        //parser
        parse(buf, args);
        if (strcmp(*args,"cd") == 0){
            if (args[2] != NULL){
                fprintf(stderr, "Error: Too many arguments to cd.\n");
                continue;
            }
            else if (args[1] == NULL && cd("~") != 0){
                continue;
            }else if (cd(args[1]) != 0){
                continue;
            }
        }else if (execute(args) != 0){
                continue;
        }

    }
    return EXIT_SUCCESS;
}