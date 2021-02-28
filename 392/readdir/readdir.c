#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define BUFSIZE 128

int main (){
    DIR *dir = opendir(".");
    if (dir == NULL){
        fprintf(stderr, "Error: cannot open current directory.\n");
        exit(EXIT_FAILURE);
    }
    char buf[PATH_MAX]; // max number of chars in any path /*PATH_MAX includes the \0, so +1 is not required*/
    char timebuf[BUFSIZE];
    struct stat sb;
    
    struct dirent *de;
    while ((de = readdir(dir)) != NULL){        //de->d_name the file within this folder, could also be a directory
        char *res = realpath(de->d_name, buf);  //takes a file name and buffer! if result comes out as null could not do the path for whatever reason
        if (!res){                              //stat is the function, but statstruct is the struct 
            fprintf(stderr, "Error: realpath() failed. %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (!stat(buf, &sb)){
            strftime(timebuf, BUFSIZE, "%b %d %Y %H:%M", localtime(&sb.st_mtime)); // b is month, d is day, Y is 4 number year
            printf("%s [%s]\n", buf, timebuf);
        } else {
            printf("%s\n", buf);
        }
        
    }

    closedir(dir);
    exit(EXIT_SUCCESS);
    
}