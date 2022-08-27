

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

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("176.9.4.102");

    sock = socket(AF_INET, SOCK_STREAM, 0);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));


    send(sock, "Hello world", strlen("Hello world"), 0);
    read(sock, buffer, 16);
    printf("%s\n", buffer);


    return 0;
}


