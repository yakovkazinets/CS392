#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "my.h"

void quitProg();

void mybzero(s1, n)
	void *s1;
	unsigned n;
{
	register char *t = s1;

	while (n != 0) 
	{
		*t++ = 0;
		n--;
	}
}

void error(char *msg)
{
	my_str(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, quitProg);
	int sockfd, portno, n, bytesRead, i;
	fd_set active_fd_set, read_fd_set;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
    char* username;

	if (argc < 3)
	{
		my_str("Not enough arguments");
		exit(0);
	}
	portno = my_atoi(argv[2]);
    
    //Get Client's Username
	my_str("Enter a Username: ");
	mybzero(buffer,512);
	bytesRead = read(0,buffer,512);
	buffer[bytesRead-1] = '\0';
	my_strncpy(username, buffer, 512);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{ 
		error("ERROR opening socket");
	}
	server = gethostbyname(argv[1]);
	if (server == NULL) 
	{
		my_str("ERROR, no such host\n");
		exit(0);
	}
	mybzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	my_strncpy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	{
		my_str("ERROR connecting");	
	}
	
	n = write(sockfd,username,my_strlen(username));
	if (n < 0)
	{
		my_str("ERROR writing to socket");
	}
	my_str("Welcome, ");
	my_str(buffer);
	my_str("!\n");
	mybzero(buffer,256);
	//Send messages
	FD_ZERO (&active_fd_set);
	FD_SET (sockfd, &active_fd_set);
	FD_SET (0, &active_fd_set);
   

	while(1)
	{
		/* Block until input arrives on one or more active sockets. */
		read_fd_set = active_fd_set;
		if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
		{
			exit (0);
		}
		/* Service all the sockets with input pending. */
			if (FD_ISSET (sockfd, &read_fd_set))
			{
				bytesRead = read(i,buffer,256);
				if (bytesRead < 0)
				{
					my_str("ERROR Reading!");
					continue;
				}
				buffer[bytesRead-1] = '\0';
				//Read message from server, print to client console
				my_str(buffer);
				mybzero(buffer,256);
            }
			else if(FD_ISSET (STDIN_FILENO, &read_fd_set))
			{
				//Read message that client input, write to server
				n = write(sockfd,buffer,my_strlen(buffer));
				if (n < 0)
				{
					my_str("ERROR writing to socket");
				}
				mybzero(buffer,256);
			}
			
		
	}

	return 0;
}

void quitProg()
{
	my_str("\nServer, plz don't freak out!\n");
	exit(0);
}