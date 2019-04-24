#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
//#define IPADDRESS "127.0.0.1" // define server ip address
#define SERV_PORT 7
#define MAXSIZE 4096
#define MAXTIMEOUT 5

int main(int argc, char *argv[])
{
    int socket_server;
    struct sockaddr_in server_address;
    char buffer[MAXSIZE] = {0};
    int error = -1;
    int len;
    len = sizeof(int);
    struct timeval tm;
    fd_set set;
    unsigned long ul = 1;
    int ret = 0;
    
    // check if host is provided
    if (argc != 2) {
        printf("Usage: echo <host>\n");
        return -1;
    }
    // include ipv6 address
    char ipaddress[40] ={0};
    
    // use strlcpy to safely copy host to ipaddress
    strlcpy(ipaddress, argv[1], 40);
    
    // create socket
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        printf("Error when creating socket\n");
        return -1;
    }
    
    // set to unblock module
    ioctl(socket_server, FIONBIO, &ul);
    
    // construct server address
    memset(&server_address, 0, sizeof server_address);
    server_address.sin_port = htons(SERV_PORT);
    // AF_INET Internet network address
    server_address.sin_family = AF_INET;
    
    // if the address is neither ipv4 nor ipv6, exit.
    if (inet_pton(AF_INET, ipaddress, &server_address.sin_addr) < 1) {
        if (inet_pton(AF_INET6, ipaddress, &server_address.sin_addr) < 1) {
            printf("Invalid hostname\n");
            return -1;
        }
        server_address.sin_family = AF_INET6;
    }
    printf("Trying to connect to the server...\n");

    
    // try to connet to the server
    if (connect(socket_server, (struct sockaddr *)&server_address, sizeof server_address) < 0) {
        tm.tv_sec = MAXTIMEOUT;
        tm.tv_usec = 0;
        FD_ZERO(&set);
        FD_SET(socket_server, &set);
        if (select(socket_server+1, NULL, &set, NULL, &tm) > 0) {
            getsockopt(socket_server, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            
            if(error == 0) ret = 1;
            else ret = 0;
        } else {
            ret = 0;
        }
    } else {
        ret = 1;
    }
    ul = 0;
    
    // set to block module
    ioctl(socket_server, FIONBIO, &ul);
    if (!ret) {
        close( socket_server );
        
        fprintf(stderr , "Cannot connect to the server!\n");
        
        return -1;
    } else {
        printf("Successfully connected to the server\n");
    }
    
    for (;;) {
        scanf("%s", buffer);
        printf("Sent: %s\n", buffer);
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
