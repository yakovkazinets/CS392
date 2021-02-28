/*Yakov Kazinets
I pledge my honor that I have abided by the Stevens Honor System.*/

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

typedef struct arg_struct {
    int start;
    int end;
} thread_args;

int total_count = 0;
pthread_mutex_t lock;

int * sieve(int a, int b){
    int* array[b+1];
    for (int i = 2; i<= b; i++){
        *array[i] = i;
        printf(".");
    }
    int i = 2;
    while ((i*i) <= b){
        if (array[i] != 0){
            for(int j = 2; j<b; j++){
                if (*array[i] * j > b){
                    break;
                } else {
                    *array[*array[i] * j] = 0;
                }
            }
            i++;
        }
    }
    for (int i = 2; i<= b; i++){
        if (array[i] !=0){
            printf("%ls, ", array[i]);
        }
    }
    printf("\n");
    return *array;
}

int main(int argc, char *argv[]){
    if (argc < 3){
        fprintf(stderr, "Usage: ./mtsieve -s <starting value> -e <ending value> -t <num threads>\n");
        return EXIT_FAILURE;
    }
    //int opt;
    //long int e1 = 0, s1 = 0, t1 = 0;
    int s = 2;
    int e = 20;
    int t = 4;
    printf("%d\n", t);
    /*while ((opt = getopt(argc, argv, "est")) != -1){
        switch (opt){
            case 'e':
                e1 = optarg;
                printf("%d", e1);
                break;
            case 's':
                s1 = optopt;
                printf("%d", s1);
            case 't':
                t1 = optopt;
                printf("%d", t1);
                break;
            case '?':
                if (optopt == 'e' || optopt == 's' || optopt == 't') {         
                    fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);     
                } else if (isprint(optopt)) {         
                    fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);     
                } else {         
                    fprintf(stderr, "Error: Unknown option character '\\x%x'.\n", optopt);     
                }
                return EXIT_FAILURE; 
        }
    }*/
    if (t < 1){
        fprintf(stderr, "Error: Number of threads cannot be less than 1.\n");
        return EXIT_FAILURE;
    }
    if (s > e){
        fprintf(stderr, "Error: Ending value must be >= starting value.\n");
        return EXIT_FAILURE;
    }
    int *a;
    a = sieve(s, e);
    

    return EXIT_SUCCESS;
}


