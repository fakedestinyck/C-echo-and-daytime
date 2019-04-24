#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#define PORT 13 // define port
#define MAXSIZE 4096 // define max buffer size
#define BACKLOG 10 // define max pending connections queue


int
main(int argc, char const *argv[])
{
    int socket_server;
    struct sockaddr_in socket_address;
    struct sockaddr_in their_address;
    socklen_t server_in_size;
    int client;
    int yes = 1;
    time_t now;
    // create a server
    socket_server = socket(AF_INET, SOCK_STREAM, 0); // AF_INET Internet network address
    if (socket_server == -1) {
        printf("Failed to create a server\n");
        return -1;
    }
    
    // construct address
    memset(&socket_address, 0, sizeof socket_address);
    socket_address.sin_port = htons(PORT);
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    
    
    // bind socket
    if (bind(socket_server, (struct sockaddr *)&socket_address, sizeof their_address) < 0) {
        printf("Error when binding\n");
        return -1;
    }
    // listen to socket
    if (listen(socket_server, BACKLOG) == -1) {
        printf("Error when listening\n");
        return -1;
    }
    
    // fork, echo
    if (!fork()) {
        // drop privilege
        printf("Before privilege separation: %d\n",geteuid());
        printf("%d\n",seteuid(501));
        printf("After privilege separation: %d\n",geteuid());
        for (;;) {
            // construct address to write client address
            // after accepting the socket
            memset(&their_address, 0, sizeof their_address);
            
            // wait until receive requests from client
            server_in_size = sizeof their_address;
            client = accept(socket_server, (struct sockaddr *)&their_address, &server_in_size);
            if (client == -1) {
                printf("Error when accepting\n");
                return -1;
            }
            printf("success\n");
            now = time(NULL);
            char buffer[MAXSIZE] = {0};
            snprintf(buffer, sizeof buffer, "%.24s\r\n", ctime(&now));
            write(client, buffer, strlen(buffer));
            close(client);
            client = -1;
        }
    }
    close(socket_server);
    socket_server = -1;
    return 0;
}

