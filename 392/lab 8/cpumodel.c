#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Tae Hwan Kim and Yakov Kazinets
//I pledge my honor that I have abided by the Stevens Honor System. 

bool starts_with(const char *str, const char *prefix) {
    /* TODO:
       Return true if the string starts with prefix, false otherwise.
       Note that prefix might be longer than the string itself.
     */
	 if (strlen(prefix) > strlen(str)) {
		 return false;
	 }
	 else {
		 return strncmp(prefix, str, strlen(prefix)) == 0;
	 }

}

int main(int argc, char *argv[]) {
    /* TODO:
       Open "cat /proc/cpuinfo" for reading.
       If it fails, print the string "Error: popen() failed. %s.\n", where
       %s is strerror(errno) and return EXIT_FAILURE.
     */
	  FILE *pipe;
      //FILE *f;
      int status;
      //unsigned long bytes_read;
      char buf[256];
      if ((pipe = popen("cat /proc/cpuinfo", "r")) == NULL){
        fprintf(stderr, "Error: popen() failed. %s.\n", strerror(errno));
        return EXIT_FAILURE;
      }

    /* TODO:
       Allocate an array of 256 characters on the stack.
       Use fgets to read line by line.
       If the line begins with "model name", print everything that comes after
       ": ".
       For example, with the line:
       model name      : AMD Ryzen 9 3900X 12-Core Processor
       print
       AMD Ryzen 9 3900X 12-Core Processor
       including the new line character.
       After you've printed it once, break the loop.
     */

		while (fgets(buf, 255, pipe)) {
			if(starts_with(fgets(buf, 255, pipe) , "model name")) {
				buf[256] = '\0';
			    printf("%s", buf+13);
				break;
			}
		}


    /* TODO:
       Close the file descriptor and check the status.
       If closing the descriptor fails, print the string
       "Error: pclose() failed. %s.\n", where %s is strerror(errno) and return
       EXIT_FAILURE.
     */
	 if ((status = pclose(pipe)) < 0) {
      fprintf(stderr, "Error: pclose() failed. %s.\n", strerror(errno));
      return EXIT_FAILURE;
    }

	
	//return EXIT_SUCCESS;
    return !(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS);
}
