#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define IPADDRESS "127.0.0.1" // define server ip address
#define SERV_PORT 8765
#define MAXSIZE 4096

int main(int argc, char *argv[])
{
    int socket_server;
    struct sockaddr_in server_address;
    
    // create socket
    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    
    // construct server address
    memset(&server_address, 0, sizeof server_address);
    server_address.sin_port = htons(SERV_PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, IPADDRESS, &server_address.sin_addr);
    
    // connet to server
    connect(socket_server, (struct sockaddr *)&server_address, sizeof(server_address));
    
    
    for (;;) {
        char buffer[MAXSIZE] = {0};
        scanf("%s", buffer);
        printf("Sent :%s\n", buffer);
        write(socket_server, buffer, strlen(buffer));
        
        memset(buffer, 0, sizeof buffer);
        
        read(socket_server, buffer, MAXSIZE);
        printf("Echo from server: %s\n", buffer);
        
        // Press q to exit.
        if (strcmp(buffer, "q") == 0) {
            printf("Client closed\n");
            close(socket_server);
            break;
        }
    }
    return 0;
}
