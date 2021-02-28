#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

volatile sig_atomic_t got_signal = 0;

void catch_signal(int sig) {
    got_signal = 1;
    printf("\n");
}

bool is_prime(unsigned long n) {
    for (int div = 2, end = (int)sqrt(n); div <= end; div++) {
        if (n % div == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]){
    if (argc !=2){
        fprintf(stderr, "Usage: %s <seconds>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int sleep_secs = atoi(argv[1]);
    if (sleep_secs<1){
        fprintf(stderr, "Error: Invalid number of seconds '%s' for max snooze time.\n", argv[1]);
        return EXIT_FAILURE;
    }

    struct sigaction action;

    memset(&action, 0, sizeof(struct sigaction)); 
    action.sa_handler = catch_signal;
    action.sa_flags = SA_RESTART; // Restart syscalls if possible

    if (sigaction(SIGINT, &action, NULL) == -1){
        perror("sigaction");
        return EXIT_FAILURE;
    }

    unsigned int secs_count = 0;
    while (true){
        if(got_signal){
            printf("I only got to nap for %d seconds.\n", secs_count);
            break;
        }
        if(!got_signal){
            secs_count++;
            printf("I have been napping for %d seconds.\n", secs_count);
        }
        sleep(1);
    }

    if (secs_count > sleep_secs){
        printf("You let me nap for %d seconds, but I only wanted to nap for a max of %d seconds.\n", secs_count, sleep_secs);
    } else {
        printf("You let me nap for %d seconds, thanks for not letting me oversleep!.\n", secs_count);
    }
    EXIT_SUCCESS;
    
}