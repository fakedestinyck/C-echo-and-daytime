#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define IPADDRESS "127.0.0.1" // define server ip address
#define SERV_PORT 13
#define MAXSIZE 4096

int main(int argc, char *argv[])
{
    int socket_server;
    struct sockaddr_in server_address;
    int n;
    char buffer[MAXSIZE] = {0};
    
    // create socket
    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    
    // construct server address
    memset(&server_address, 0, sizeof server_address);
    server_address.sin_port = htons(SERV_PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, IPADDRESS, &server_address.sin_addr);
    
    // connet to server
    connect(socket_server, (struct sockaddr *)&server_address, sizeof server_address);
    
    
    for (;;) {
        if ((n=read(socket_server,buffer, sizeof buffer)) <= 0) {
            break;
        }
        buffer[n] = 0;
        if (fputs(buffer, stdout) == EOF) {
            printf("Error when fputting to stdout\n");
        }
    }
    if (n<0) {
        printf("Error when getting returned message\n");
    }
    return 0;
}
