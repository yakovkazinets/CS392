#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char *s){
    if (s==NULL){
        return NULL;
    }
    size_t len = strlen(s) +1;
    char *copy = (char *)malloc(len * sizeof(char));
    if(copy ==NULL){
        return NULL;
    }
    /*
    char *t = copy;
    while (*s) {
        *t++ = *s++;
    }
    *t = '\0'
    return copy;
    */
    return (char *)memcpy(copy, s, len);
}

int main(){
    char name[128];
    printf("What is your name?");
    scanf("%s", name);

    printf("Hello, %s!\n", name);
    printf("Your name has %ld characters.\n", strlen(name));

    char *copy = my_strdup(name);
    printf("Hello, %s!\n", copy);
    printf("Your name has %ld characters.\n", strlen(copy));

    free(copy);

    printf("How old are you?");
    int age;
    scanf("%d", &age);
    printf("You are %d year(s) old.\n", age);

    printf("How old will you be in 1 year?");
    scanf("%d", &age);
    printf("You will be %d year(s) old.\n", age);
    
    return 0;
}