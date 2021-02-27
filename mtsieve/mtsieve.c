/*Yakov Kazinets and Tae Kim
I pledge my honor that I have abided by the Stevens Honor System.*/

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
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

void *sieve(void *ptr){
    thread_args *jimbo = (thread_args *) ptr;
    int a = jimbo->start;
    int b2 = jimbo->end;
    int *low_primes;
    double b = (double) b2;
    double low_primes_limit= ceil(sqrt(b));
    low_primes = malloc(sizeof(int) * b2);
    for(int i = 2; i< low_primes_limit; i++){
        low_primes[i] = 1;
    }
    for (int i = 2; i< low_primes_limit; i++){
        if (low_primes[i]){
            for (int j = i; i * j < low_primes_limit; j++){
                low_primes[i * j] = 0;
            }
        }
    }

    int high_primes_limit = b2-a +1;
    bool *high_primes = (bool *)malloc((high_primes_limit) * sizeof(bool));
    for (int i = 0; i<high_primes_limit; i++){
        high_primes[i] = true;
    }
    for (int p = 0; p<b2; p++){
        if(low_primes[p]){
            int i = ceil((double)a/p) * p -a;
            if (a <= p){
                i +=p;
            }
            while(i<high_primes_limit){
                high_primes[i] = false;
                i+=p;
            }
        }
    }
    free(low_primes);
    
    int counterof3 = 0;
    for (int i = 0; i<high_primes_limit; i++){
        if(high_primes[i]){
            int counter = 0;
            int temp = i+a;
            while(temp > 0 && counter < 2){
                if (temp % 10 == 3){
                    counter++;
                }
                temp = temp /10;
            }
            if (counter > 1){
                counterof3++;
            }
        }
    }
    int retval;
	if ((retval = pthread_mutex_lock(&lock)) != 0) {
		 fprintf(stderr, "Warning: Cannot lock mutex. %s.\n",strerror(retval));
	}
    total_count += counterof3;
    if ((retval = pthread_mutex_unlock(&lock)) != 0) {
			 fprintf(stderr, "Warning: Cannot unlock mutex. %s.\n",strerror(retval));
    }
    free(high_primes);
    pthread_exit(NULL);
    

}



int main(int argc, char *argv[]){
    if (argc == 1){
        fprintf(stderr, "Usage: ./mtsieve -s <starting value> -e <ending value> -t <num threads>\n");
        return EXIT_FAILURE;
    }
    /*###########################################*/
    int opt;
    int s_flag = 0, e_flag = 0, t_flag = 0;
    int a =0, b=0, t=0;
    char *ptr1;
    char *ptr2;
    char *ptr3;
    /*###########################################*/
    opterr = 0;
    while ((opt = getopt(argc,argv, "s:e:t:")) != -1){
        switch(opt) {
            case 's':
                s_flag = 1;
                for(int i = 0; i < strlen(optarg); i++){
                    if(!isdigit(optarg[i])){
                        fprintf(stderr, "Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, opt);
                        return EXIT_FAILURE;
                    }
                }
                if(!(strtol(optarg, &ptr1, 10) <= INT_MAX)){
                    fprintf(stderr,"Error: Integer overflow for parameter '-%c'.\n",opt);
                    return EXIT_FAILURE;
                } else {
                    a = atoi(optarg);
                }
                opterr=0;
                break;
            case 'e':
                e_flag = 1;
                for(int i = 0; i < strlen(optarg); i++){
                    if(!isdigit(optarg[i])){
                        fprintf(stderr, "Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, opt);
                        return EXIT_FAILURE;
                    }
                }
                if(!(strtol(optarg, &ptr2, 10) <= INT_MAX)){
                    fprintf(stderr,"Error: Integer overflow for parameter '-%c'.\n",opt);
                    return EXIT_FAILURE;
                } else {
                    b = atoi(optarg);
                }
                break;
            case 't':
                t_flag = 1;
                for(int i = 0; i < strlen(optarg); i++){
                    if(!isdigit(optarg[i])){
                        fprintf(stderr, "Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, opt);
                        return EXIT_FAILURE;
                    }
                }
                if(!(strtol(optarg, &ptr3, 10) <= INT_MAX)){
                    fprintf(stderr,"Error: Integer overflow for parameter '-%c'.\n",opt);
                    return EXIT_FAILURE;
                } else {
                    t = atoi(optarg);
                }
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
    }
    
    /*###########################################*/

    if (argv[optind] != NULL){
        fprintf(stderr, "Error: Non-option argument '%s' supplied.\n", argv[optind] );
        return EXIT_FAILURE;
    }
    if (!s_flag){
        fprintf(stderr, "Error: Required argument <starting value> is missing.\n");
        return EXIT_FAILURE;
    }
    if (!(a >= 2)){
        fprintf(stderr, "Error: Starting value must be >= 2.\n");
        return EXIT_FAILURE;
    }
    if (!e_flag){
        fprintf(stderr, "Error: Required argument <ending value> is missing.\n" );
        return EXIT_FAILURE;
    }
    if (!(b >= 2)){
        fprintf(stderr, "Error: Ending value must be >= 2.\n");
        return EXIT_FAILURE;
    }
    if (a > b){
        fprintf(stderr, "Error: Ending value must be >= starting value.\n" );
        return EXIT_FAILURE;
    }
    if (!t_flag){
        fprintf(stderr, "Error: Required argument <num threads> is missing.\n" );
        return EXIT_FAILURE;
    }
    if (t < 1){
        fprintf(stderr, "Error: Number of threads cannot be less than 1.\n" );
        return EXIT_FAILURE;
    }
    if (t > 2*get_nprocs()){
        fprintf(stderr, "Error: Number of threads cannot exceed twice the number of processors(%d).\n", get_nprocs() );
        return EXIT_FAILURE;
    }
    if ((b-a) + 1 < t){
        t = (b-a) + 1;
    }

    printf("Finding all prime numbers between %d and %d.\n", a, b);
    if (t == 1){
        printf("%d segment:\n", t);
    }else{
        printf("%d segments:\n", t);
    } 
    /**Threading!**/
    int thread_length = (b-a + 1)/t;
    int thread_length_remainder = (b-a + 1) % t;
    int temp_start = a;
    int temp_end =a+thread_length -1;
    int retval;
    if ((retval = pthread_mutex_init(&lock, NULL)) != 0) {
        fprintf(stderr, "Error: Cannot create mutex. %s.\n", strerror(retval));
        return EXIT_FAILURE;
    }
    
    pthread_t threads[t];
    thread_args targs[t];

    for(int i =0; i<t; i++){
        if (thread_length_remainder > 0){
            targs[i].start= temp_start;
            targs[i].end = temp_end + 1;
            thread_length_remainder--;
        } else {
            targs[i].start= temp_start;
            targs[i].end = temp_end;
        }
        printf("   [%d, %d]\n", targs[i].start, targs[i].end);
        temp_start = targs[i].end+1;
        temp_end = temp_start+thread_length - 1;
        if((pthread_create(&threads[i], NULL, sieve, (void *)(&targs[i]))) !=0){
            fprintf(stderr, "Error: Cannot create thread %d. %s.\n", i + 1, strerror(errno));
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < t; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Warning: Thread %d did not join properly.\n", i + 1);
        }
    }

    if ((retval = pthread_mutex_destroy(&lock)) != 0) {
        fprintf(stderr, "Error: Cannot destroy mutex. %s.\n", strerror(retval));
    }
    /*********************************************/
    printf("Total primes between %d and %d with two or more '3' digits: %d\n", a, b, total_count);
    return EXIT_SUCCESS;
}

