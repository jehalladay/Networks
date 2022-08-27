//What is happening is this program creates a socket, and binds to that socket 
//then continues to listen

//WATCH THE LECTURE ON 04/11/2022 YOU'RE AT TIME 21:53
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h>     
#include <string.h>    
#include <netdb.h>      
#include <getopt.h>   
#include <errno.h>
#include <getopt.h>
#include <arpa/inet.h>

enum Mode {
    ECHO,
    TRANSFER,
    DONE,
    NONE
};


int main(int argc, char **argv){
    struct addrinfo hints, *res;
    struct sockaddr_storage remoteAddress;
    int serverSocket;
    char buffer[1024] = {0};
    enum Mode option = NONE;

    // 1) Start listening on a specific port and accept connections

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&hints, 0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "5000", &hints, &res);      
    bind(serverSocket, res->ai_addr, res->ai_addrlen);      //bind takes in a socket, and specific address, 

    listen(serverSocket, 1);    

    // 2) The first data sent to the server after it accepts the connection will be the "mode" the server 
        // should enter. The two modes would be, echo a string back to the client, the second mode would 
        // be transfer a file to the client. To determine the mode, the server should receive the first 
        // data and use 1 for echo and 2 for file transfer (look at strcmp).

    int addrlen = sizeof(struct sockaddr_storage);
    int incomingSocket = accept(serverSocket, (struct sockaddr*)&remoteAddress, &addrlen);    //We need to pass an instance of the struct that represent the remote address
                                                                                            // The accept needs to have the clients IP address so that you can talk back

    char mode = 0;

    char close[8] = "close";
    // buffer = "close";
    // if(strcmp(buffer, close) == 0) {
    //     printf("comparison with buffer successful");
    // } else {
    //     printf(buffer);
    //     printf("comparison with buffer unsuccessful");
    // }
    int recv_size = 1;


    send(incomingSocket, "Enter 1 for echo or 2 for transfer\n", 36, 0);
    while(recv_size != 0) {
        while(option == NONE){
            recv_size = recv(incomingSocket, &mode, 1, 0);
            if(mode == '1'){
                printf("Echo mode\n");
                send(incomingSocket, "Echo mode\n", 11, 0);
                option = ECHO;
            } else if(mode == '2'){
                printf("Transfer mode\n");
                send(incomingSocket, "Transfer mode\n", 15, 0);
                option = TRANSFER;
            } else if (recv_size == 0) {
                printf("Connection closed\n");
                break;
            } else {
                printf("Invalid mode\n");
            }
        }

        // 3) When in echo mode, if the server receives the string "close" then 
            // it should reply back with "goodbye" and then close the client connection (look at strcmp).

        while(option == ECHO) {
            memset(buffer, '\0', 1024);
            recv_size = recv(incomingSocket, &buffer, 1024, 0);
            if(recv_size == 0) {
                printf("Client disconnected\n");
                break;
            }
            // close = "close\0";

            if(strcmp(buffer, close) == 0) {
                printf("Entered closing branch\n");
                printf("Goodbye\n");
                send(incomingSocket, "Goodbye\n", 8, 0);
                option = NONE;
                break;
            } else {
                printf("Debug:");
                printf(buffer);
                printf(close);
                printf(":Debug-End:\n");
            }

            printf("%s\n", buffer);
            send(incomingSocket, ">> ", 3, 0);
            send(incomingSocket, buffer, strlen(buffer), 0);
        }

        while(option == TRANSFER) {
            memset(buffer, '\0', 1024);
            recv_size = recv(incomingSocket, buffer, 1024, 0);
            if(recv_size == 0) {
                printf("Client disconnected\n");
                break;
            }
            // now we will send the file, 1024 bytes at a time

            FILE *fp = fopen("test.txt", "r");
            if(fp == NULL) {
                printf("File not found\n");
                send(incomingSocket, "File not found\n", 16, 0);
                break;
            }
            while(!feof(fp)) {
                memset(buffer, '\0', 1024);
                fread(buffer, 1, 1024, fp);
                send(incomingSocket, buffer, strlen(buffer), 0);
            }

            option = DONE;
            break;

        }

        if(option == DONE) {
            printf("Goodbye\n");
            send(incomingSocket, "\nGoodbye\n", 9, 0);
            break;
        }
    }

    
    
    
    return 0;
}