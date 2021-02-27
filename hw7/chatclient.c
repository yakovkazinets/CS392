/*Yakov Kazinets and Tae Kim.
I pledge my honor that I have abided by the Stevens Honor System.
 Thank for a wonderful semester*/

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "util.h"

int client_socket = -1;

char username[MAX_NAME_LEN + 1];
char bufcomingin[BUFLEN +1];
char bufcomingout[MAX_MSG_LEN +1];
//the first is for establishing the connection while the other is used while in the server
int handle_stdin(int *client_socket, char *bufcomingin) {
    memset(bufcomingin, 0, BUFLEN+1);
    int num_bytes_received;
    
    if((num_bytes_received = recv(*client_socket, bufcomingin, BUFLEN, 0)) <0){
        fprintf(stderr, "Error: Failed to receive message from server. %s.\n", strerror(errno));
        return num_bytes_received;
    }else if(num_bytes_received ==0){
        fprintf(stderr, "All connections are busy. Try again later.\n");
        return num_bytes_received;
    }else{
        bufcomingin[num_bytes_received] = '\0';
        return num_bytes_received;
    }
}
//----------------------------------------------------------------
int handle_stdin_in_server(int *client_socket, char *bufcomingin) {
    memset(bufcomingin, 0, BUFLEN+1);
    int num_bytes_received;
    
    if((num_bytes_received = recv(*client_socket, bufcomingin, BUFLEN, 0)) <0){
        fprintf(stderr, "Warning: Failed to receive incoming message. %s.\n", strerror(errno));
        return num_bytes_received;
    }else if(num_bytes_received ==0){
        fprintf(stderr, "\nConnection to server has been lost.\n");
        return num_bytes_received;
    }else{
        bufcomingin[num_bytes_received] = '\0';
        return num_bytes_received;
    }
}
//----------------------------------------------------------------
//the first is for establishing the connection while the other is used while in the server
int handle_client_socket(int *client_socket, char *bufcomingout, size_t sizeint) {
    int num_bytes_send;
    if((num_bytes_send = send(*client_socket, bufcomingout, sizeint, 0)) <0){
        fprintf(stderr, "Error: Failed to send username to server. %s.\n", strerror(errno));
    }
    return num_bytes_send;
}
//----------------------------------------------------------------
int handle_client_socket_in_server(int *client_socket, char *bufcomingout, size_t sizeint) {
    int num_bytes_send;
    if((num_bytes_send = send(*client_socket, bufcomingout, sizeint, 0)) <0){
        fprintf(stderr, "Warning: Failed to receive incoming message from server. %s.\n", strerror(errno));
    }
    return num_bytes_send;
}

int main(int argc, char *argv[]) {
    //COMMAND LINE ARGUEMENTS--------------------------------------
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int client_socket = EXIT_SUCCESS;
    int retval = EXIT_SUCCESS;
    struct sockaddr_in server_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int address;
    int portnumber;
    char *serveraddress = argv[1];

    if(strcmp("localhost", serveraddress) ==0){
        serveraddress = "127.0.0.1";
    }

    memset(&server_addr, 0, addrlen);
    server_addr.sin_family = AF_INET;
    address = inet_pton(AF_INET, serveraddress, &server_addr.sin_addr);

    if (address <= 0){
        fprintf(stderr, "Error: Invalid IP address '%s'.\n", serveraddress);
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    if (is_integer(argv[2]) == false){
        fprintf(stderr, "Error: Invalid input '%s' received for port number.\n", argv[2]);
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    if(parse_int(argv[2], &portnumber, "port number") == false){
        //Error message made in util.h
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    if(65535 < portnumber || 1024> portnumber){
        fprintf(stderr, "Error: Port must be in range [%d, %d].\n", 1024, 65535);
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    server_addr.sin_port = htons(portnumber);
    //PROMPTING FOR A USERNAME---------------------------------------

    int whileint = NO_INPUT;
    int whilebreak = OK;
    while(whileint != whilebreak){
        printf("Please enter a username:\n");
        fflush(stdout);
        whileint = get_string(username, MAX_NAME_LEN);
        //checks if the username given was too long
        if (whileint == TOO_LONG){
            fprintf(stderr, "Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
            //No exit failure just keep repeating to the user to give a better username
        }

    }
    printf("Hello, %s. Let's try to connect to the server.\n", username);

    //ESTABLISHING CONNECTION----------------------------------------

    /*After obtaining the user name, which may actually be the same as someone already logged into
    the system, your program should create a TCP socket. If that fails, error out with the message
    where %s is the strerror of errno. Throughout the rest of this document, the %s in any error
    message ending with "%s.\n" can be assumed to be for the strerror of errno.*/

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0))<0){
        fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    /*Then your program should attempt to connect to the server. If it fails, error out with the message:
    "Error: Failed to connect to server. %s.\n"*/
    if(connect(client_socket, (struct sockaddr *) &server_addr, addrlen) <0){
        fprintf(stderr, "Error: Failed to connect to server. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    /*The program should then try to receive the welcome message from the server. If it fails, error
    out with the message:
    "Error: Failed to receive message from server. %s.\n"
    Interestingly, if the number of bytes received is 0, that means the server closed the connection on
    the client. In that case, error out with the message:
    "All connections are busy. Try again later.\n"
    All handled in handle_stdin, the error messages*/
    
    if (handle_stdin(&client_socket, bufcomingin) <=0){
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    /*Next, print a new line, the welcome message received from the server, and two more new lines.*/
    printf("\n");
    printf("%s\n", bufcomingin);
    printf("\n");
    /*Finally, send the username to the server. If it fails, error out with the message:*/
    
    strcpy(bufcomingout, username);
    if(handle_client_socket(&client_socket, bufcomingout, strlen(username)) <0){
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    //HANDLING ACTIVITY ON FILE DESCRIPTORS (SOCKETS)----------------------------- 
    //if we got here it means nothing went wrong with the input, or we messed up error handling so far

    printf("[%s]: ", username);
    fflush(stdout);
    /*Using fd_set and select (man 2 select), your program should now loop forever, prompting
    the user for input and determining if there is activity on one of the two file descriptors.
    Specifically, both the socket and STDIN_FILENO should be added to the fd_set. If there is
    activity on
    */
    fd_set fdsetter;
    bool whilebool = true;
    while (whilebool){
        //fd setting
        FD_ZERO(&fdsetter);
        FD_SET(STDIN_FILENO, &fdsetter);
        FD_SET(client_socket, &fdsetter);
        //checking if select worked and if it didn't it tells the user and exits with FAILURE
        if(select(client_socket +1, &fdsetter, NULL,NULL,NULL) <0 && errno !=EINTR){
            fprintf(stderr, "Error: Failed in use of select(). %s\n", strerror(errno));
            retval = EXIT_FAILURE;
            goto EXIT;
        }
        
        /*1. STDIN_FILENO -----------------------------------------------------
        Get the string from standard input and store it in outbuf. Use get_string() supplied in
        util.h to help you with this task. If it’s too long, print to standard output:
        "Sorry, limit your message to %d characters.\n"
        where %d is MAX_MSG_LEN.
        Otherwise, send the message to the server, regardless of its contents. Then, if the message is
        equal to "bye", print "Goodbye.\n" and shut down the client.*/
        
       if(whilebool && FD_ISSET(STDIN_FILENO, &fdsetter)){
           switch (get_string(bufcomingout, MAX_MSG_LEN)){
               case OK: //There was an input and that input was not too long
                    if(handle_client_socket_in_server(&client_socket, bufcomingout, strlen(bufcomingout)) >-1){
                        if(strcmp("bye", bufcomingout) ==0){
                            printf("Goodbye.\n");
                            retval = EXIT_SUCCESS;
                            goto EXIT;
                        }else{
                            printf("[%s]: ", username);
                            fflush(stdout);
                        }
                    }
                    break;
               case TOO_LONG: //There was an input and that input was too long
                    printf("Sorry, limit your message to %d characters.\n", MAX_MSG_LEN);
                    break;
               case NO_INPUT: //There was no input
                    break;
               default: //Just incase none of the above happened which should not happen
                    break;
           }
       }

       /*2. CLIENT SOCKET -----------------------------------------------------
        Receive data from the socket and store it in inbuf. If the number of bytes received is -1, warn
        the user with the message: "Warning: Failed to receive incoming message. %s.\n"
        If the number of bytes received is 0, the server abruptly broke the connection with the client,
        as in the server crashed or perhaps the network failed. Error out by telling the user:
        "\nConnection to server has been lost.\n"
        Null-terminate the string.
        Compare it to "bye" and if it’s equal, print "\nServer initiated shutdown.\n" and
        exit the program.
        Otherwise, print the message from the server*/
        if(whilebool && FD_ISSET(client_socket, &fdsetter)){
            int num_bytes_received;
            if((num_bytes_received = handle_stdin_in_server(&client_socket, bufcomingin)) ==0){
                retval = EXIT_FAILURE;
                goto EXIT;
            }else if(num_bytes_received >=1){
                if (strcmp("bye", bufcomingin) ==0){
                    printf("\nServer initiated shutdown.\n");
                    retval = EXIT_SUCCESS;
                    goto EXIT;
                }else{
                    printf("\n");
                    printf("%s\n", bufcomingin);
                    printf("[%s]: ", username);
                    fflush(stdout);
                }
            }

        }

    }
    



EXIT:
    if(fcntl(client_socket, F_GETFD)> -1){
        close(client_socket);
    }
    return retval;
}