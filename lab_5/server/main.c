/*
    Lab 5 - Basic Socket Programming in C
    CSCI 484 - Computer Networks
    By James Halladay
    Date: 4/11/2022

    Here we are going to set up a server using the socket API.

*/


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char **argv) {

    int status, sock;
    struct addrinfo hints, *servinfo, *res, *p;
    char ipstr[INET6_ADDRSTRLEN];

    struct sockaddr_in serv_addr;

    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_port = htons(8080);
    // serv_addr.sin_addr.s_addr = inet_addr("176.9.4.102");

    // sock = socket(AF_INET, SOCK_STREAM, 0);

    // connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));




    memset(&hints, 0, sizeof hints);
    printf("Hello world\n");

    if( argc != 2 ) {
        fprintf(stderr, "usage: showip hostname\n");
        return 1;
    }

    hints.ai_family =  AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(argv[1], NULL, &hints, &res);

    printf(argv[1]);
    printf("\n");

    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }


    printf("IP Addresses for %s:\n\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next ){
        void *addr;
        char *ipver;

        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p-> ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf(" %s: %s\n", ipver, ipstr);
    }


    freeaddrinfo(res);


    return 0;
}