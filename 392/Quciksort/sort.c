/*******************************************************************************
 * Name        : sort.c
 * Author      : Yakov Kazinets
 * Date        : 2/20/2020
 * Description : Uses quicksort to sort a file of either ints, doubles, or
 *               strings.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

typedef enum{
    STRING,
    INT,
    DOUBLE
} elem_t;

/**
 * Basic structure of sort.c:
 *
 * Parses args with getopt.
 * Opens input file for reading.
 * Allocates space in a char** for at least MAX_ELEMENTS strings to be stored,
 * where MAX_ELEMENTS is 1024.
 * Reads in the file
 * - For each line, allocates space in each index of the char** to store the
 *   line.
 * Closes the file, after reading in all the lines.
 * Calls quicksort based on type (int, double, string) supplied on the command
 * line.
 * Frees all data.
 * Ensures there are no memory leaks with valgrind. 
 */
void printhelper(){
    printf("Usage: ./sort [-i|-d] [filename]\n");
	printf("-i: Specifies the file contains ints.\n");
	printf("-d: Specifies the file contains doubles.\n");
	printf("filename: The file to sort.\n");
	printf("No flags defaults to sorting strings.\n");

}
void doublehelper(double *array, const int len){
    int i = 0;
    while (i<len){
        printf("%f\n", *(array + i));
        i++;
    }
}
void inthelper(int *array, const int len){
    int i = 0;
    while (i<len){
        printf("%d\n", *(array + i));
        i++;
    }
}
void stringhelper(char **array, const int len){
    int i = 0;
    while (i<len){
        printf("%s\n", *(array + i));
        i++;
    }
}
int main(int argc, char **argv) {
    int opt;
    //case 1
    if(argc < 2 || argc > 3) {
        printhelper(argv[0]);
        return EXIT_FAILURE;
    }
    int x = 0;
    while ((opt = getopt(argc, argv, ":if:id")) != -1){
    	switch(opt){
    		case 'i':
    			x = 1;
    			break;
    		case 'd':
    			x = 2;
    			break;
    		case '?':
    			printf("Error: Unknown option '-%s' received.\n", argv[1]);
    			printhelper();
    			return EXIT_FAILURE;
            default:
                break;
    	}
    }
    char buf[MAX_ELEMENTS];
    FILE *source;
    int losingmymind=0;
    int len=0;
    if(argc==2){
    	source=fopen(argv[1], "r");
    	if (source==NULL) {
	    	fprintf(stderr, "Error: Cannot open file '%s'.\n", argv[1]);
	    	return EXIT_FAILURE;
    	}
        char **temporary=(char **)malloc(MAX_ELEMENTS * sizeof(char*)); 
        while (fgets(buf, MAX_STRLEN + 2, source)) {
            char *piza = strchr(buf, '\n');
            if (piza==NULL){
                buf[MAX_STRLEN]='\0';
            }
            else {
                *piza='\0';
            }
            temporary[losingmymind]=(char *)malloc(MAX_STRLEN * sizeof(char));
            strcpy(*(temporary+losingmymind), buf);
            losingmymind++;
            len++;
        }
        fclose(source);
        quicksort(temporary, len, sizeof(char *), str_cmp);
        stringhelper(temporary, len);
        for (int i =0; i<=len; i++) {
            free(temporary[i]);
        }
        free(temporary);
    }
    else if (argc==3) {
    	source = fopen(argv[2], "r");
    	if (source==NULL){
	    	fprintf(stderr, "Error: Cannot open file '%s'.\n", argv[2]);
	    	return EXIT_FAILURE;
	    }
        if (x==1){
            int *temporary = malloc(MAX_ELEMENTS * sizeof(int));
            while (fgets(buf, MAX_STRLEN + 2, source)) {
                char *piza = strchr(buf, '\n');
                if (piza==NULL){
                    buf[MAX_STRLEN]='\0';
                }
                else{
                    *piza ='\0';
                }
                int num=atoi(buf);
                *(temporary+losingmymind)=num;
                losingmymind++;
                len++;
            }
            fclose(source);
            quicksort(temporary, len, sizeof(int), int_cmp);
            inthelper(temporary, len);
            free(temporary);
        }
        else{
            double *temporary=malloc(MAX_ELEMENTS * sizeof(double));
            while (fgets(buf, MAX_STRLEN + 2, source)) {
                char *piza=strchr(buf,'\n');
                if (piza==NULL) {
                    buf[MAX_STRLEN]='\0';
                }
                else {
                    *piza='\0';
                }
                double num=atof(buf);
                *(temporary+losingmymind)=num;
                losingmymind++;
                len++;
            }
            fclose(source);
            quicksort(temporary, len, sizeof(double), dbl_cmp);
            doublehelper(temporary, len);
            free(temporary);
        }
    }
    return EXIT_SUCCESS;
}
//PLZ NO SEG FAULT!