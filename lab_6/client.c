
#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h>    
#include <string.h>     
#include <netdb.h>      
#include <getopt.h>  
#include <fcntl.h>

int main(int argc, char **argv){
    struct sockaddr_in serv_addr;      
    off_t fileSize, offset;
    char buffer[1024] = {0};
    char user_input[1024] = {0};
    int sock;  

    serv_addr.sin_family = AF_INET;     
    serv_addr.sin_port = htons(5000);  
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    sock = socket(AF_INET, SOCK_STREAM, 0);     // creates a socket
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, argv[1], strlen(argv[1]), 0);
    printf("Starting client...\n");

    if(argc > 1) {
        if(strcmp(argv[1], "2") == 0) {
            printf("File transfer mode:\n");

            int fileDescriptor = open("recieved.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

            recv(sock, buffer, 1024, 0);
            fileSize = atoi(buffer);
            printf("%ld\n", fileSize);
            
            while (fileSize > 0) {
                memset(buffer, 0 ,strlen(buffer));
                offset = recv(sock, buffer, 1024, 0);
                write(fileDescriptor, buffer, offset);
                fileSize = fileSize - offset;
            }
            close(fileDescriptor);
            printf("Done\n");
        } else if(strcmp(argv[1], "1") == 0) {
            printf("Echo mode:\n");
                // memset(buffer, 0, strlen(buffer));
                // recv(sock, buffer, 1024, 0);
                // memset(buffer, 0, strlen(buffer));
            while(1) {
                memset(buffer, 0, strlen(buffer));
                recv(sock, buffer, 1024, 0);
                printf("%s\n", buffer);
                user_input[0] = '\0';
                printf("Enter a string: ");
                scanf("%s", user_input);
                // fgets(user_input, 1024, stdin);
                send(sock, user_input, strlen(user_input), 0);
            }

        } else {
            printf("Sending %s\n", argv[1]);
            send(sock, argv[1], strlen(argv[1]), 0);
            recv(sock, buffer, 1024, 0);  
            printf("%s\n", buffer); 
        }
    }

    return 0;
}