#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXLINE 5000

int open_clientfd(char *hostname, char *port){
	int clientfd;
	struct addrinfo hints, *listp, *p;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_flags |= AI_ADDRCONFIG;
	getaddrinfo(hostname, port, &hints, &listp);

	for (p=listp; p; p = p->ai_next) {
		if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) <0){
			continue;
		}
		if(connect(clientfd, p->ai_addr, p->ai_addrlen) != -1){
			break;
		}
		close(clientfd);
	}

	freeaddrinfo(listp);
	if(!p){
		return -1;
	}
	else{
		return clientfd;
	}
}

int main(int argc, char **argv){
	int clientfd;
	char *host, *port, buf[MAXLINE];

	host = argv[1];
	port = argv[2];

	clientfd = open_clientfd(host, port);

	while (fgets(buf, MAXLINE, stdin) != NULL){
		write(clientfd, buf, strlen(buf));
		memset(buf, 0, MAXLINE);
		read(clientfd, buf, MAXLINE);
		fputs(buf, stdout);
		fputs("\n", stdout);
		memset(buf, 0, MAXLINE);
	}
	close(clientfd);
	exit(0);
}