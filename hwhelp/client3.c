#define _POSIX_SOURCE
#include "my.h"
//#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
//#include <strings.h>
#include <signal.h>
#define h_addr h_addr_list[0]

int main(int argc, char *argv[]){
	if(argc !=3){
    	my_str("Input wrong try again using format: ./client <hostname> <port>\n");
    	exit(0);
    }
    int usernameTest=0;
    char readBuffer[2048];
    char writeBuffer[1024];
    char message[2048];
    int n;
    int k;
    int test;
    char username[1024];
    memset(username,'\0', 1024);
    pid_t pid;
    //creat the sockBOIIIIIIIIIII
    int sockBOIIIII;
    sockBOIIIII= socket(AF_INET, SOCK_STREAM, 0);
    if (sockBOIIIII<0){
    	my_str("Socket Error\n");
    	exit(0);
    }
    my_str("Please enter a username\n");
    while(1){
  	k=read(0, username, 1024);
  	if(k!=0)
  		break;
  	}
 	if(username[k-1]=='\n')
       	username[k-1]='\0';
  	usernameTest++;
  	my_str("Username set to: ");
	my_str(username);
  	my_str(" \n");
    //sockaddr junk
    struct sockaddr_in socketAddress;
    memset(&socketAddress, '\0', sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(my_atoi(argv[2]));
    socketAddress.sin_addr.s_addr = inet_addr(argv[1]);
    if(connect(sockBOIIIII,(struct sockaddr *) &socketAddress, (sizeof(socketAddress)))<0){
    	my_str("Connecting error\n");
    	exit(0);
    }
    int testBOI=write(sockBOIIIII,username,my_strlen(username));
    if(testBOI<0){
    	my_str("Write error");
    }
    if((pid = fork())<0){
    	my_str("forking error teehee\n");
    }
    else if (pid==0){
	    while(1){
    		memset(writeBuffer,'\0', 2048);
    		memset(message,'\0', 2048);

  			n = read(0, writeBuffer, 1024);
			if (n<0){
           		my_str("Read error from client\n");
	       	}
        	else{
        		if(my_strncmp(writeBuffer, "/nick", 5)==0){
        			if(writeBuffer[n-1]=='\n')
       					writeBuffer[n-1]='\0';
        		}
       			my_strcpy(message, writeBuffer);
	   			if(my_strncmp(writeBuffer,"/exit",5)==0){
        			my_str("Exitting from server\n");
        			kill(0, SIGKILL);
					kill(1, SIGKILL);
       				exit(0);
       			}
       		    else{
       			test=write(sockBOIIIII,message,my_strlen(message));
       			if(test<0){
       				my_str("Write error");
    			}
      			}		
	   		}		
    	}
	}
	else{
		while(1){
			memset(readBuffer,'\0', 2048);
			if(recv(sockBOIIIII, readBuffer, 2048, 0)<0){
    			my_str("Error receiving dataz\n");
    		}
	    	else{
    			my_str(readBuffer);
    		}
		}
	}
    if(close(sockBOIIIII)<0){
  		my_str("Socket closing error\n");
  		exit(0);
  	}

  	return 0;
}